#ifndef _COMMON_H
#define _COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include <string.h>

#include "Public/CH554.H"
#include "Public/DEBUG.H"
#include "keycode.h"

#define uint  unsigned int
#define uchar unsigned char
#define ulong unsigned long

sbit IO_KEY0 = P1 ^ 1;
sbit IO_KEY1 = P3 ^ 1;
sbit IO_KEY2 = P3^ 0;

sbit IO_EC11_D = P1 ^ 5;
sbit IO_EC11_A = P1 ^ 4;
sbit IO_EC11_B = P3 ^ 2;

sbit IO_USB_EN1 = P1 ^ 6;
sbit IO_USB_EN2 = P1 ^ 7;

/* ��ͨ�������� */
extern UINT8 HIDKey[8];
/* ��ý�尴������ */
extern UINT8 HIDKeyMUL[1];
extern UINT8 SetupReq, SetupLen, Ready, Count, FLAG, UsbConfig;

extern bit num_lock;
extern bit caps_lock;

/* usb ��ʼ�� */
void drv_usb_init(void);
/* usb �ϴ����ݵ��˵� 2 */
void drv_usb_write_ep2(char *buf, uchar len);

/* ������dial ���� */
void drv_usb_dial(int wheel);
void drv_usb_keyboard(uchar *p);
void drv_usb_keyboard_click(uchar status,uchar key);
void drv_usb_mul(UINT16 dat);
void drv_usb_mul_click(UINT16 dat);

/* ����״̬ */
uchar get_numlock(void);
uchar get_fn_lock(void);
uchar get_capslock(void);

#ifdef __cplusplus
}
#endif
#endif  // _COMMON_H
