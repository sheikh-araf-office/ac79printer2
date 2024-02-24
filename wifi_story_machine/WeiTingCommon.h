#ifndef __WEITINGCOMMON_H
#define __WEITINGCOMMON_H

typedef struct _DEVICE_REV
{
  void (*Initial)(void);
  void (*SaveData)(unsigned long  len,unsigned char *BufferReceive);
  unsigned long (*BuferGetEmptySize)(void);
  unsigned char (*CheckBufferEmptySize)(void);
  void (*DataProcesser)(void);
  void (*DataIn)(void);
  void (*DataOut)(unsigned char *Buffer,unsigned int  len);
  void (*RefreshRev)(void);
  volatile unsigned int Pwrite; // 0: w=r
  volatile unsigned int Pread;
  volatile unsigned char Waitting;
  volatile unsigned char IDHandle;
  volatile unsigned long EP;//only for usb
  volatile unsigned char *PBufferRev;
  volatile bool ConnectState;
  volatile unsigned long ReVRecordTotalLen;
}Device_Rev;




#endif
