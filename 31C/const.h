#ifndef _CONST_H_
#define _CONST_H_

#define WAIT_1mkS \
    _nop_();      \
    _nop_();      \
    _nop_();      \
    _nop_() // Задержка на 1 мксек
#define WAIT_5mkS \
    WAIT_1mkS;    \
    WAIT_1mkS;    \
    WAIT_1mkS;    \
    WAIT_1mkS;    \
    WAIT_1mkS // Задержка на 5 мксек
#define WAIT_25mkS \
    WAIT_5mkS;     \
    WAIT_5mkS;     \
    WAIT_5mkS;     \
    WAIT_5mkS;     \
    WAIT_5mkS // Задержка на 25 мксек
#define WAIT_100mkS \
    WAIT_25mkS;     \
    WAIT_25mkS;     \
    WAIT_25mkS;     \
    WAIT_25mkS // Задержка на 100 мксек
#define WAIT_500mkS \
    WAIT_100mkS;    \
    WAIT_100mkS;    \
    WAIT_100mkS;    \
    WAIT_100mkS;    \
    WAIT_100mkS // Задержка на 500 мксек

/* Типы конечных точек  */
#define CONTROL 0x00
#define ISOCHRONOUS 0x01
#define BULK 0x02
#define INTERRUPT 0x03

#define BULK_IN 0x86
#define BULK_OUT 0x82
#define INTERRUPT_IN 0x87
#define INTERRUPT_OUT 0x83
#define ISOCHRONOUS_IN 0x85
#define ISOCHRONOUS_OUT 0x81

/* для вычисления номера конечной точки */
#define EP_DIRECT_OUT 0x00
#define EP_DIRECT_IN 0x80
/* для вычисления конфигурации точки */
#define EP_CONFIG_OUT 0x00
#define EP_CONFIG_IN 0x04

/* делители частоты кварца */
#define PLL_3MHz 0xF0
#define PLL_4MHz 0xC0
#define PLL_6MHz 0x70
#define PLL_8MHz 0x50
#define PLL_12MHz 0x30
#define PLL_16MHz 0x20
#define PLL_18MHz 0x72
#define PLL_20MHz 0xB4
#define PLL_24MHz 0x10
#define PLL_32MHz 0x21
#define PLL_40MHz 0xB9

/* коды запросов */
#define GET_STATUS_DEVICE 0x8000
#define GET_STATUS_INTERF 0x8100
#define GET_STATUS_ENDPNT 0x8200
#define CLEAR_FEATURE_DEVICE 0x0001
#define CLEAR_FEATURE_INTERF 0x0101
#define CLEAR_FEATURE_ENDPNT 0x0201
#define SET_FEATURE_DEVICE 0x0003
#define SET_FEATURE_INTERF 0x0103
#define SET_FEATURE_ENDPNT 0x0203
#define SET_ADDRESS 0x0005
#define GET_DESCRIPTOR_DEVICE 0x8006
#define GET_DESCRIPTOR_INTERF 0x8106
#define GET_DESCRIPTOR_ENDPNT 0x8206
#define SET_DESCRIPTOR 0x0007
#define GET_CONFIGURATION 0x8008
#define SET_CONFIGURATION 0x0009
#define GET_INTERFACE 0x810A
#define SET_INTERFACE 0x010B
#define SYNCH_FRAME 0x820C

#define SET_IDLE 0x210A
#define SET_IDLE_1 0xA10A
#define GET_REPORT 0xA101

/* Типы дескрипторов */
#define DEVICE 0x01
#define CONFIGURATION 0x02
#define STRING 0x03
#define INTERFACE 0x04
#define ENDPOINT 0x05
#define HID 0x21
#define REPORT 0x22

/* Аттрибуты устройства */
#define USB_CONFIG_BUSPOWERED 0x80
#define USB_CONFIG_SELFPOWERED 0x40
#define USB_CONFIG_REMOTEWAKEUP 0x20

/* Номера выполнимых команд */
#define STOP 1
#define INIT_STM 2
#define INIT_X 3
#define INIT_Y 4
#define INIT_Z 5
#define SET_REGIME 6
#define RESET_CNTX 7
#define RESET_CNTY 8
#define RESET_CNTZ 9
#define MOVE_ABS 10
#define MOVE_REL 11
#define SHT_TTL 12
#define DROP_FLAGS 13
#define SEND_ACK 14
#define ERONEOUS_COMMAND 0xFF
#define COMMAND_EXECUTED 0x80

/* Конфигурация платы */
#define DIRX P0_0 // направление вращения
#define DIRY P0_2
#define DIRZ P0_4
#define ResetX P0_7
#define ResetY P0_7
#define ResetZ P0_7
#define StepX P0_1
#define StepY P0_3
#define StepZ P0_5
#define TTL1 P3_0 // сигнал ТТЛ
#define TTL2 P3_1 // сигнал ТТЛ 2 (Read Out)

#define GET_LSWSTAT       \
    P4_0 = 0;             \
    P1_7 = 0;             \
    P4_1 = 1;             \
    P1_6 = 1;             \
    WAIT_1mkS;            \
    SW_STATR = P1 & 0x3F; \
    P4_0 = 1;             \
    P1_7 = 1;             \
    WAIT_5mkS
#define GET_RSWSTAT       \
    P4_0 = 1;             \
    P1_7 = 1;             \
    P4_1 = 0;             \
    P1_6 = 0;             \
    WAIT_1mkS;            \
    SW_STATL = P1 & 0x3F; \
    P4_1 = 1;             \
    P1_6 = 1;             \
    WAIT_5mkS
#define GET_ROSTAT flagRo = P3_1;

#endif /* _CONST_H_ */