// Standa3d - ver.00.00
// ver. 00.00c
// сделана из stm3d.c ver. 00.00c

// Направление вращения "влево"  - движение к положению 0 мм Проверить!
// Направление вращения "вправо" - движение к положению 7 мм

//#pragma SYMBOLS
//unsigned char code version[]="ver. 00.00d 26.04.2012";    // Где положит линкер неизвестно...

#include <at89c5131.h>
#include <ext_5131.h>
#include <stdio.h>
#include <math.h>
#include <intrins.h>

#include <reg_macro.h>
#include <const.h>

void INIT_TIMER(void);
void sm_init(void);
void delay(unsigned char tau);

unsigned char cntT2;
bit flgT2;

unsigned char idata byteA, byteB;

char xdata char_buff[15];
unsigned char idata char_indx = 0xFF;

#include <usb.c>

void main(void)
{

	Set_x2_mode(); //включить режим x2

	P1 = 0xFF; //в надежде на включение medium pullup

	INIT_TIMER();

	usb_init();
	sm_init();

	for (;;)
	{
	}
}

void INIT_TIMER(void)
{

	//Настройка таймера 2
	T2MOD = 0;
	CP_RL2 = 0; /* автоматическая перезагрузка по переполнению          */
	C_T2 = 0;	/* таймер 2 считает импульсы Fclk_periph/6              */
	EXEN2 = 0;	/* вход T2EX заблокирован                               */

	//RCAP2L =0x06;	/* Firq_T2 = Fclk_periph/(6*(65536-(RCAP2H,RCAP2L)))    */
	//RCAP2H =0xFF;	/* Firq_T2 = 16000 Hz при RCAP2L =0x06, RCAP2H =0xFF    */
	/* Fclk_periph = Fosc = 24 MHz                          */

	RCAP2L = 0x0C; /* Firq_T2 = Fclk_periph/(6*(65536-(RCAP2H,RCAP2L)))    */
	RCAP2H = 0xFE; /* Firq_T2 = 8000 Hz при RCAP2L =0x0C, RCAP2H =0xFE     */
				   /* Fclk_periph = Fosc = 24 MHz                          */

	//RCAP2L =0x18;	/* Firq_T2 = Fclk_periph/(6*(65536-(RCAP2H,RCAP2L)))    */
	//RCAP2H =0xFC;	/* Firq_T2 = 4000 Hz при RCAP2L =0x18, RCAP2H =0xFC     */
	/* Fclk_periph = Fosc = 24 MHz                          */

	cntT2 = 0;
	flgT2 = 0;

	TF2 = 1; /* инициируем перезагрузку TL2,TH2                      */
	TR2 = 1; /* старт таймера 2                                      */
	ET2 = 1; /* разрешены прерывания от таймера 2 по флагу TF2       */

	IPL0 = 0x20; /* прерываниям таймера 2 наивысший приоритет            */
	IPH0 = 0x20;

	Enable_interrupt(); /* разрешены прерывания                                 */
}

void sm_init(void)
{
}

void timer2(void) interrupt IRQ_T2 using 1
{
	unsigned char idata v_UEPNUM;

	TF2 = 0; // сброс флага переполнения таймера 2

	Disable_interrupt();

	// крутим шаговики

	Enable_interrupt();

	// Проверка состояния кнопок и концевых выключателей

	// Проверка состояния уровня Read Out

	if (flgT2)
	{
		--cntT2;
		if (cntT2 == 0)
			flgT2 = 0;
	}

	// Запись ответа в буфер USB контроллера
	if (!ep1_ready)
	{

		byteA = 0x01;
		byteB = 0x02;

		v_UEPNUM = UEPNUM; // Сохраняем содержимое !!!
		Usb_select_ep(1);

		UEPDATX = byteA;
		UEPDATX = byteB;

		ep1_ready = 1;
		Usb_set_tx_ready();
		UEPNUM = v_UEPNUM;
	}
}

void delay(unsigned char tau)
{
	cntT2 = tau;
	flgT2 = 1;
	while (flgT2)
		;
}