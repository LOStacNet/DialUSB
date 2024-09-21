#include "common.h"

bit num_lock = 0;
bit caps_lock = 0;
UINT8 HIDKey[8]={0,0,0,0,0,0,0,0};//������������

/**
 * @brief ��ͨ����
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
 * @brief ��ͨ�����������ʵ��,��������֤
 * example-drv_usb_keyboard_click(KEY_Left_Control|KEY_Left_Alt,HID_KEYBPAD_DOT)�����������ݼ�
 * @param status Modifier keys status
 * @param key the normal key
**/
void drv_usb_keyboard_click(uchar status,uchar key) {
    HIDKey[0]=status;//���Ƽ�
    HIDKey[2]=key;//����
	
		FLAG = 0;
		drv_usb_keyboard(HIDKey);
		while (FLAG == 0);//�ȴ��������
		HIDKey[0]=0;//ȫ������
    HIDKey[2]=0;
		drv_usb_keyboard(HIDKey);
		while (FLAG == 0);
}

/**
 * @brief ��ť
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
 * @brief ������ý�尴��
 *
 **/
extern UINT8 FLAG;  // �ϴ���ɱ�־
void drv_usb_mul_click(UINT16 dat) {
    FLAG = 0;
    drv_usb_mul(dat);
    while (FLAG == 0);
    drv_usb_mul(0);
    while (FLAG == 0);
}


/**
 * @brief ��ý�尴��
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
 * @brief ��ȡ����������д������fn
 *
 * @return uchar
**/
uchar get_numlock(void) { return !!(num_lock); }
uchar get_capslock(void) { return !!(caps_lock); }
uchar get_fn_lock(void) { return !(num_lock == caps_lock); }
