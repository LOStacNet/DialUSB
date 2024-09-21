/**
 * @author: Pomin
 * @date: 2024-07-15
 * @github: https://github.com/POMIN-163
 * @description: dial + 3 mul key + 2 USB EN
 **/
#include "common.h"

UINT8X key_state[3 + 1] = {
    1,
    1,
    1,
    1,
};  // ����״̬ (������ + EC11 ��һ��)
UINT8X back_state[3 + 1] = {
    1,
    1,
    1,
    1,
};                   // ������һ�ε�״̬
long key_msg = 0;    // ������Ϣ

#define EC11_B_BACK_STA back_state[3]
#define EC11_B_NOW_STA  key_state[3]
#define BOOT_ADDR  0x3800
typedef void( *pTaskFn)( void );
pTaskFn jmp_bootloader = BOOT_ADDR;

void init(void);
void key_act(void);
void key_scan(void);

/* ����ɨ�� */
void key_scan(void) {
    // 0 - 2 �� 3 ������, 3 �� EC11 B ��
    UINT8X i;
    EC11_B_BACK_STA = EC11_B_NOW_STA;
    EC11_B_NOW_STA = IO_EC11_B;

    /* EC11 ��Ӧ ������/�½���ȫ���� �����������ĸо� */
    if (!EC11_B_NOW_STA && EC11_B_BACK_STA) {
        if (IO_EC11_A) {
            key_msg = 22;  // ˳ʱ��
        } else {
            key_msg = 21;  // ��ʱ��
        }
    }
    /* ��ͨ����ɨ�� */
    for (i = 0; i < 3; i++) {
        back_state[i] = key_state[i];
    }
    key_state[0] = IO_KEY0;
    key_state[1] = IO_KEY1;
    key_state[2] = IO_KEY2;

    for (i = 0; i < 3; i++) {
        if (key_state[i] && !back_state[i]) {
            key_msg = i + 1;
        }
    }
}

/* ������������ */
void key_act(void) {
    switch (key_msg) {
        case 1:
						drv_usb_keyboard_click(KEY_Left_GUI,HID_KEYBOARD_L);//����
            break;
        case 2:
            drv_usb_keyboard_click(KEY_Left_Control|KEY_Left_Alt,HID_KEYBPAD_DOT);
            break;
        case 3:
						drv_usb_mul_click(HID_CONSUMER_VOLUME_MUTE);
            break;
        case 4:
            break;
        case 21:
            drv_usb_dial(-200);
            break;
        case 22:
            drv_usb_dial(200);
            break;
        default:
            break;
    }
}


void init(void) {
    UINT8X i;
    CfgFsys();       // CH552ʱ��ѡ��24M����
    mDelaymS(100);   // �޸���Ƶ�ȴ��ڲ������ȶ�,�ؼ�
    drv_usb_init();  // USB�豸ģʽ��ʼ��
    EA = 1;          // ����Ƭ���ж�
    UEP2_T_LEN = 0;  // ��ն˵�2���ͳ���
    FLAG = 0;        // ���USB�жϴ�����ɱ�־
    IO_KEY0 = 1;     // Ӳ���Ѿ���/����, �˴���ʼ��1/0����
    IO_KEY1 = 1;
    IO_KEY2 = 1;

    IO_USB_EN1 = 1;
    IO_USB_EN2 = 1;

    IO_EC11_A = 1;
    IO_EC11_B = 1;
    IO_EC11_D = 1;
    if (!IO_EC11_D)
    {
        jmp_bootloader();
    }
    // ���ô��� ���� L16 warning
    get_numlock();
    get_capslock();
    get_fn_lock();
}

static void TASK_CYCLE_1(void) {
    static uint idata sw_num = 0;
    if (IO_EC11_D == 0) {
        sw_num++;
        if (sw_num == 5) {
            drv_usb_dial(1);
        }
    } else {
        if (sw_num >= 5) drv_usb_dial(0);
        sw_num = 0;
    }
}

static void TASK_CYCLE_2(void) {
    key_scan();
    if (key_msg) {
        key_act();
        key_msg = 0;
    }
}


/* һЩα���̲߳��� (����Ϊ�����ʱ��) */
typedef struct {
    void (*task)(void);
    unsigned int cycle_length;
    unsigned int last_run;
} task_s;

static task_s systemTasks[] = {
    {TASK_CYCLE_1, 1, 0},
    {TASK_CYCLE_2, 1, 0},
};

#define task_num sizeof(systemTasks) / sizeof(task_s)
volatile unsigned int data systime_ms;
UINT8X scheduler_index;

void main(void) {
    init();

    while (1) {
        mDelaymS(1);
        systime_ms++;
        for (scheduler_index = 0; scheduler_index < task_num;scheduler_index++) {
            if (systime_ms - systemTasks[scheduler_index].last_run >systemTasks[scheduler_index].cycle_length) 
						{
                systemTasks[scheduler_index].last_run = systime_ms;
                systemTasks[scheduler_index].task();
            }
        }
    }
}
