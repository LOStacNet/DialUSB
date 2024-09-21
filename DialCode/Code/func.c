#include "common.h"

bit num_lock = 0;
bit caps_lock = 0;
UINT8 HIDKey[8]={0,0,0,0,0,0,0,0};//按键数据数组

/**
 * @brief 普通按键
 * HID_KEY_CTRL = HID_L_ALT;
 * HID_KEY_MAIN = HID_KEYBOARD_A;
 * drv_usb_keyboard(HIDKey);
 * @param p
**/
void drv_usb_keyboard(uchar *p) {
    char idata temp[9] = {1, 0, 0, 0, 0, 0, 0, 0, 0};
    uchar i;
    for (i = 0; i < 8; i++) {
        temp[i + 1] = p[i];
    }
    drv_usb_write_ep2(temp, 9);
}

/**
 * @brief 普通单按键点击的实现,无输入验证
 * example-drv_usb_keyboard_click(KEY_Left_Control|KEY_Left_Alt,HID_KEYBPAD_DOT)任务管理器快捷键
 * @param status Modifier keys status
 * @param key the normal key
**/
void drv_usb_keyboard_click(uchar status,uchar key) {
    HIDKey[0]=status;//控制键
    HIDKey[2]=key;//按键
	
		FLAG = 0;
		drv_usb_keyboard(HIDKey);
		while (FLAG == 0);//等待发送完毕
		HIDKey[0]=0;//全部归零
    HIDKey[2]=0;
		drv_usb_keyboard(HIDKey);
		while (FLAG == 0);
}

/**
 * @brief 旋钮
 *
 * @param wheel
**/
void drv_usb_dial(int wheel) {
    char buf_tx[4] = {
        3,
        0,
        0,
        0,
    };

    buf_tx[1] = *((char *)&wheel + 1);
    buf_tx[2] = *((char *)&wheel + 0);

    drv_usb_write_ep2(buf_tx, 3);
}

/**
 * @brief 单击多媒体按键
 *
 **/
extern UINT8 FLAG;  // 上传完成标志
void drv_usb_mul_click(UINT16 dat) {
    FLAG = 0;
    drv_usb_mul(dat);
    while (FLAG == 0);
    drv_usb_mul(0);
    while (FLAG == 0);
}


/**
 * @brief 多媒体按键
 *
 * @param dat
**/
void drv_usb_mul(UINT16 dat) {
    char idata temp[4] = {2, 0, 0, 0};

    temp[1]=dat & 0xFF;
    temp[2]=(dat >> 8) & 0xFF;
		
    drv_usb_write_ep2(temp, 4);
}

/**
 * @brief 获取数字锁、大写锁定、fn
 *
 * @return uchar
**/
uchar get_numlock(void) { return !!(num_lock); }
uchar get_capslock(void) { return !!(caps_lock); }
uchar get_fn_lock(void) { return !(num_lock == caps_lock); }
