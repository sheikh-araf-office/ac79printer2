/* coap -- simple implementation of the Constrained Application Protocol (CoAP)
 *         as defined in draft-ietf-core-coap
 *
 * Copyright (C) 2010--2013 Olaf Bergmann <bergmann@tzi.org>
 *
 * This file is part of the CoAP library libcoap. Please see
 * README for terms of use.
 */
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "printf.h"
//#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <errno.h>
#include "libcoap/coap_config.h"
#include "libcoap/resource.h"
#include "libcoap/coap.h"

#include "system/includes.h"
#include "wifi/wifi_connect.h"

#include "app_config.h"

#ifdef USE_COAP_SERVER_TEST
#define perror printf
#define COAP_RESOURCE_CHECK_TIME 2

#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

/* temporary storage for dynamic resource representations */
static int quit = 0;

/* changeable clock base (see handle_put_time()) */
static time_t my_clock_base = 0;

struct coap_resource_t *time_resource = NULL;
#define WITHOUT_ASYNC
#ifndef WITHOUT_ASYNC
/* This variable is used to mimic long-running tasks that require
 * asynchronous responses. */
static coap_async_state_t *async = NULL;
#endif /* WITHOUT_ASYNC */

/* SIGINT handler: set quit to 1 for graceful termination */
static void
handle_sigint(int signum)
{
    quit = 1;
}

#define COAP_INDEX "This is a test server made with libcoap (see http://libcoap.sf.net)\n" \
   	      "Copyright (C) 2010--2016 Olaf Bergmann <bergmann@tzi.org> TEST_OK!\n\n"

static void
hnd_post_index(coap_context_t  *ctx, struct coap_resource_t *resource,
               coap_address_t *peer, coap_pdu_t *request, str *token,
               coap_pdu_t *response)
{
    printf("hnd_post_index received : (%s)\n", request->data);
}

static void
hnd_get_index(coap_context_t  *ctx, struct coap_resource_t *resource,
              coap_address_t *peer, coap_pdu_t *request, str *token,
              coap_pdu_t *response)
{
    unsigned char buf[3];

    response->hdr->code = COAP_RESPONSE_CODE(205);

    coap_add_option(response, COAP_OPTION_CONTENT_TYPE,
                    coap_encode_var_bytes(buf, COAP_MEDIATYPE_TEXT_PLAIN), buf);

    coap_add_option(response, COAP_OPTION_MAXAGE,
                    coap_encode_var_bytes(buf, 0x2ffff), buf);

    coap_add_data(response, strlen(COAP_INDEX), (unsigned char *)COAP_INDEX);
}

static void
hnd_get_time(coap_context_t  *ctx, struct coap_resource_t *resource,
             coap_address_t *peer, coap_pdu_t *request, str *token,
             coap_pdu_t *response)
{
    coap_opt_iterator_t opt_iter;
    coap_opt_t *option;
    unsigned char buf[40];
    size_t len;
    time_t now;
    coap_tick_t t;
    coap_subscription_t *subscription;

    /* FIXME: return time, e.g. in human-readable by default and ticks
     * when query ?ticks is given. */

    /* if my_clock_base was deleted, we pretend to have no such resource */
    response->hdr->code =
        my_clock_base ? COAP_RESPONSE_CODE(205) : COAP_RESPONSE_CODE(404);

    if (request != NULL &&
        coap_check_option(request, COAP_OPTION_OBSERVE, &opt_iter)) {
#ifndef WITHOUT_OBSERVE
        subscription = coap_add_observer(resource, peer, token);
        if (subscription) {
            subscription->non = request->hdr->type == COAP_MESSAGE_NON;
            coap_add_option(response, COAP_OPTION_OBSERVE, 0, NULL);
        }
#endif
    }
    if (resource->dirty == 1)
        coap_add_option(response, COAP_OPTION_OBSERVE,
                        coap_encode_var_bytes(buf, ctx->observe), buf);


    if (my_clock_base)
        coap_add_option(response, COAP_OPTION_CONTENT_FORMAT,
                        coap_encode_var_bytes(buf, COAP_MEDIATYPE_TEXT_PLAIN), buf);

    coap_add_option(response, COAP_OPTION_MAXAGE,
                    coap_encode_var_bytes(buf, 0x01), buf);

    if (my_clock_base) {

        /* calculate current time */
        coap_ticks(&t);
        now = my_clock_base + (t / COAP_TICKS_PER_SECOND);

        if (request != NULL
            && (option = coap_check_option(request, COAP_OPTION_URI_QUERY, &opt_iter))
            && memcmp(COAP_OPT_VALUE(option), "ticks",
                      min(5, COAP_OPT_LENGTH(option))) == 0) {
            /* output ticks */
            len = snprintf((char *)buf,
                           min(sizeof(buf), response->max_size - response->length),
                           "%u", (unsigned int)now);
            coap_add_data(response, len, buf);

        } else {			/* output human-readable time */
            struct tm *tmp;
            struct tm result;
            tmp = gmtime_r(&now, &result);
            len = strftime((char *)buf,
                           min(sizeof(buf), response->max_size - response->length),
                           "%b %d %H:%M:%S", tmp);
            coap_add_data(response, len, buf);
        }
    }
}

static void
hnd_put_time(coap_context_t  *ctx, struct coap_resource_t *resource,
             coap_address_t *peer, coap_pdu_t *request, str *token,
             coap_pdu_t *response)
{
    coap_tick_t t;
    size_t size;
    unsigned char *data;

    /* FIXME: re-set my_clock_base to clock_offset if my_clock_base == 0
     * and request is empty. When not empty, set to value in request payload
     * (insist on query ?ticks). Return Created or Ok.
     */

    /* if my_clock_base was deleted, we pretend to have no such resource */
    response->hdr->code =
        my_clock_base ? COAP_RESPONSE_CODE(204) : COAP_RESPONSE_CODE(201);

    resource->dirty = 1;

    coap_get_data(request, &size, &data);

    if (size == 0) {	/* re-init */
        my_clock_base = clock_offset;
    } else {
        my_clock_base = 0;
        coap_ticks(&t);
        while (size--) {
            my_clock_base = my_clock_base * 10 + *data++;
        }
        my_clock_base -= t / COAP_TICKS_PER_SECOND;
    }
}

static void
hnd_delete_time(coap_context_t  *ctx, struct coap_resource_t *resource,
                coap_address_t *peer, coap_pdu_t *request, str *token,
                coap_pdu_t *response)
{
    my_clock_base = 0;		/* mark clock as "deleted" */

    /* type = request->hdr->type == COAP_MESSAGE_CON  */
    /*   ? COAP_MESSAGE_ACK : COAP_MESSAGE_NON; */
}

#ifndef WITHOUT_ASYNC
static void
hnd_get_async(coap_context_t  *ctx, struct coap_resource_t *resource,
              coap_address_t *peer, coap_pdu_t *request, str *token,
              coap_pdu_t *response)
{
    coap_opt_iterator_t opt_iter;
    coap_opt_t *option;
    unsigned long delay = 5;
    size_t size;

    if (async) {
        if (async->id != request->hdr->id) {
            coap_opt_filter_t f;
            coap_option_filter_clear(f);
            response->hdr->code = COAP_RESPONSE_CODE(503);
        }
        return;
    }

    option = coap_check_option(request, COAP_OPTION_URI_QUERY, &opt_iter);
    if (option) {
        unsigned char *p = COAP_OPT_VALUE(option);

        delay = 0;
        for (size = COAP_OPT_LENGTH(option); size; --size, ++p) {
            delay = delay * 10 + (*p - '0');
        }
    }

    async = coap_register_async(ctx, peer, request,
                                COAP_ASYNC_SEPARATE | COAP_ASYNC_CONFIRM,
                                (void *)(COAP_TICKS_PER_SECOND * delay));
}

static void
check_async(coap_context_t  *ctx, coap_tick_t now)
{
    coap_pdu_t *response;
    coap_async_state_t *tmp;

    size_t size = sizeof(coap_hdr_t) + 13;

    if (!async || now < async->created + (unsigned long)async->appdata) {
        return;
    }

    response = coap_pdu_init(async->flags & COAP_ASYNC_CONFIRM
                             ? COAP_MESSAGE_CON
                             : COAP_MESSAGE_NON,
                             COAP_RESPONSE_CODE(205), 0, size);
    if (!response) {
        debug("check_async: insufficient memory, we'll try later\n");
        async->appdata =
            (void *)((unsigned long)async->appdata + 15 * COAP_TICKS_PER_SECOND);
        return;
    }

    response->hdr->id = coap_new_message_id(ctx);

    if (async->tokenlen) {
        coap_add_token(response, async->tokenlen, async->token);
    }

    coap_add_data(response, 4, (unsigned char *)"done");

    if (coap_send(ctx, &async->peer, response) == COAP_INVALID_TID) {
        debug("check_async: cannot send response for message %d\n",
              response->hdr->id);
    }
    coap_delete_pdu(response);
    coap_remove_async(ctx, async->id, &tmp);
    coap_free_async(async);
    async = NULL;
}
#endif /* WITHOUT_ASYNC */

static void
init_resources(coap_context_t *ctx)
{
    coap_resource_t *r;

    r = coap_resource_init(NULL, 0, 0);

    //注册get资源
    coap_register_handler(r, COAP_REQUEST_GET, hnd_get_index);

    coap_add_attr(r, (unsigned char *)"ct", 2, (unsigned char *)"0", 1, 0);
    coap_add_attr(r, (unsigned char *)"title", 5, (unsigned char *)"\"General Info\"", 14, 0);
    coap_add_resource(ctx, r);

    /* store clock base to use in /time */
    my_clock_base = clock_offset;

    r = coap_resource_init((unsigned char *)"time", 4, 0);
    coap_register_handler(r, COAP_REQUEST_GET, hnd_get_time);
    coap_register_handler(r, COAP_REQUEST_PUT, hnd_put_time);
    coap_register_handler(r, COAP_REQUEST_DELETE, hnd_delete_time);

    coap_add_attr(r, (unsigned char *)"ct", 2, (unsigned char *)"0", 1, 0);
    coap_add_attr(r, (unsigned char *)"title", 5, (unsigned char *)"\"Internal Clock\"", 16, 0);
    coap_add_attr(r, (unsigned char *)"rt", 2, (unsigned char *)"\"Ticks\"", 7, 0);
    r->observable = 1;
    coap_add_attr(r, (unsigned char *)"if", 2, (unsigned char *)"\"clock\"", 7, 0);

    coap_add_resource(ctx, r);
    time_resource = r;

    //注册post资源
    r = coap_resource_init((unsigned char *)"/post/test", strlen("/post/test"), 0);
    coap_register_handler(r, COAP_REQUEST_POST, hnd_post_index);
    coap_add_attr(r, (unsigned char *)"ct", 2, (unsigned char *)"0", 1, 0);
    coap_add_resource(ctx, r);

#ifndef WITHOUT_ASYNC
    r = coap_resource_init((unsigned char *)"async", 5, 0);
    coap_register_handler(r, COAP_REQUEST_GET, hnd_get_async);

    coap_add_attr(r, (unsigned char *)"ct", 2, (unsigned char *)"0", 1, 0);
    coap_add_resource(ctx, r);
#endif /* WITHOUT_ASYNC */
}

static void
usage(const char *program, const char *version)
{
    const char *p;

    p = strrchr(program, '/');
    if (p) {
        program = ++p;
    }

    printf("%s v%s -- a small CoAP implementation\n"
           "(c) 2010,2011 Olaf Bergmann <bergmann@tzi.org>\n\n"
           "usage: %s [-A address] [-p port]\n\n"
           "\t-A address\tinterface address to bind to\n"
           "\t-p port\t\tlisten on specified port\n"
           "\t-v num\t\tverbosity level (default: 3)\n",
           program, version, program);
}

static coap_context_t *
get_context(const char *node, const char *port)
{
    coap_context_t *ctx = NULL;
    int s;
    struct addrinfo hints;
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Coap uses UDP */
    hints.ai_flags = AI_PASSIVE | AI_NUMERICHOST;

    s = getaddrinfo(node, port, &hints, &result);
    if (s != 0) {
        printf("getaddrinfo: %s\n", strerror(s));
        return NULL;
    }

    /* iterate through results until success */
    for (rp = result; rp != NULL; rp = rp->ai_next) {
        coap_address_t addr;

        if (rp->ai_addrlen <= sizeof(addr.addr)) {
            coap_address_init(&addr);
            addr.size = rp->ai_addrlen;
            memcpy(&addr.addr, rp->ai_addr, rp->ai_addrlen);

            ctx = coap_new_context(&addr);
            if (ctx) {
                /* TODO: output address:port for successful binding */
                goto finish;
            }
        }
    }

    printf("no context available for interface '%s'\n", node);

finish:
    freeaddrinfo(result);
    return ctx;
}

int coap_server_test(void)
{
    coap_context_t  *ctx;
    fd_set readfds;
    struct timeval tv, *timeout;
    int result;
    coap_tick_t now;
    coap_queue_t *nextpdu;


    coap_log_t log_level = LOG_DEBUG;


    coap_set_log_level(log_level);

    ctx = get_context(NULL, COAP_DEFAULT_PORT_STR);
    if (!ctx) {
        return -1;
    }

    //初始化coap服务器资源
    init_resources(ctx);

//  signal(SIGINT, handle_sigint);

    while (!quit) {
        FD_ZERO(&readfds);
        FD_SET(ctx->sockfd, &readfds);

        nextpdu = coap_peek_next(ctx);

        coap_ticks(&now);
        while (nextpdu && nextpdu->t <= now - ctx->sendqueue_basetime) {
            coap_retransmit(ctx, coap_pop_next(ctx));
            nextpdu = coap_peek_next(ctx);
        }

        if (nextpdu && nextpdu->t <= COAP_RESOURCE_CHECK_TIME) {
            /* set timeout if there is a pdu to send before our automatic timeout occurs */
            tv.tv_usec = ((nextpdu->t) % COAP_TICKS_PER_SECOND) * 1000000 / COAP_TICKS_PER_SECOND;
            tv.tv_sec = (nextpdu->t) / COAP_TICKS_PER_SECOND;
            timeout = &tv;
        } else {
            tv.tv_usec = 0;
            tv.tv_sec = COAP_RESOURCE_CHECK_TIME;
            timeout = &tv;
        }
        result = select(FD_SETSIZE, &readfds, 0, 0, timeout);

        if (result < 0) {		/* error */
            if (errno != EINTR) {
                perror("select");
            }
        } else if (result > 0) {	/* read from socket */
            if (FD_ISSET(ctx->sockfd, &readfds)) {
                coap_read(ctx);	/* read received data */
                coap_dispatch(ctx);	/* and dispatch PDUs from receivequeue */
            }
        } else {			/* timeout */
            if (time_resource) {
                time_resource->dirty = 1;
            }
        }

#ifndef WITHOUT_ASYNC
        /* check if we have to send asynchronous responses */
        check_async(ctx, now);
#endif /* WITHOUT_ASYNC */

#ifndef WITHOUT_OBSERVE
        /* check if we have to send observe notifications */
        coap_check_notify(ctx);
#endif /* WITHOUT_OBSERVE */
    }

    coap_free_context(ctx);

    return 0;
}

static void coap_server_start(void *priv)
{
    enum wifi_sta_connect_state state;
    while (1) {
        printf("Connecting to the network...\n");
        state = wifi_get_sta_connect_state();
        if (WIFI_STA_NETWORK_STACK_DHCP_SUCC == state) {
            printf("Network connection is successful!\n");
            break;
        }

        os_time_dly(20);
    }

    coap_server_test();
}

void c_main(void)
{
    if (thread_fork("coap_server_start", 10, 2 * 1024, 0, NULL, coap_server_start, NULL) != OS_NO_ERR) {
        printf("thread fork fail\n");
    }
}

late_initcall(c_main);

#endif//USE_COAP_SERVER_TEST
