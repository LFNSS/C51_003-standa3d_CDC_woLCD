#include <usb.h>

#define FIFO_0_SIZE 32

bit zlp;		  // Флаг нулевого пакета. Устанавливается
				  // (при необходимости) при приеме SETUP пакета
bit set_addr_flg; // Флаг устанавливается при получении запроса SET_ADDRESS
bit ep1_ready;	  // Флаг устанавливается, когда готовы данные для передачи 1-ой конечной точкой
				  // сбрасывается по окончании передачи
bit ep2_ready;	  // Флаг устанавливается когда принятые  USB контроллером данные записаны в приемный буфер
				  // RX_BUF[32]

unsigned char EP2Nbytes;

unsigned char idata addr; // Адрес, присвоенный HOST контроллером данному USB устройству

unsigned char xdata ENDP0_RXBUF[32]; // приемный буфер 0-ой конечной точки
unsigned char xdata RX_BUF[32];		 // буфер, в который помещаются принятые EP2 данные
unsigned int data data_to_transfer;	 // число байт, подлежащих передаче. Используется в процедурах
									 //  EP0_USB, usb_get_descriptor
char code *pbuffer;					 // Указатель на данные (дескрипторы). Используется в процедурах
									 //  EP0_USB, usb_get_descriptor
unsigned int idata data_block_size;	 // размер блока данных, который HOST контроллер ждет в ответ на SETUP пакет

void RESET_USB(void);
void EP0_USB(void);
void EP1_USB(void);
void EP2_USB(void);
void WRFIFO(char code *(data *(buf_ptr_ptr)), unsigned int data *nb);
void usb_get_status_device(void);
void usb_get_status_interface(void);
void usb_get_status_endpoint(void);
void usb_set_address(void);
void usb_get_descriptor(void);
void usb_get_configuration(void);
void usb_set_configuration(void);
void usb_hid_set_idle(void);
void usb_clear_feature_endpnt(void);
void STALL(void);
void delay(unsigned char tau);

void usb_init(void)
{

	PLLDIV = 0x10; /* Установка делителя ФАПЧ для кварца 24MHz*/
	PLLCON = 0x02; /* Запуск ФАПЧ Pll_enable(); */
	while (!(PLLCON & MSK_PLOCK))
		; /* Ждем захват ФАПЧ*/

	zlp = 0; // Флаг нулевого пакета. Устанавливается
			 // (при необходимости) при приеме SETUP пакета
	set_addr_flg = 0;
	ep1_ready = 1;
	ep2_ready = 0;
	addr = 0;

	USBCON = MSK_USBE; /* Включаем USB контроллер*/
	USBINT = 0;		   // см. doc4373
	USBADDR = MSK_FEN; /* разрешаем функцию (по умолчанию разрешена)*/

	USBIEN = MSK_EEORINT; /* разрешено прерывание по окончании сброса */
						  /* шины USB (по умолчанию разрешено)*/

	IEN1 = IEN1 | MSK_EUSB; /* разрешение прерывания от USB контроллера*/

	USBCON = USBCON | MSK_DETACH; /* Отключаемся/подключаемся к шине USB */
	delay(5);					  /* для начала нумерации */
	USBCON = USBCON & (~MSK_DETACH);
}

void USB(void) interrupt IRQ_USB using 2
{ // прерывания остаются только от 0 и 3 EP
	// Если RESET USB, то
	if (USBINT & MSK_EORINT)
	{
		RESET_USB();
		USBINT = 0;
	}
	// Если прерывание от 0-й конечной точки, то
	if (UEPINT & MSK_EP0INTE)
	{
		EP0_USB();
		UEPINT = UEPINT & (~MSK_EP0INTE);
	}

	if (UEPINT & MSK_EP1INTE)
	{
		EP1_USB();
		UEPINT = UEPINT & (~MSK_EP1INTE);
	}
	if (UEPINT & MSK_EP2INTE)
		if (!ep2_ready)
		{
			EP2_USB();
			UEPINT = UEPINT & (~MSK_EP2INTE);
		}
}

#pragma NOAREGS

void RESET_USB(void)
{

	UEPNUM = 6; // Запрещаем конечные точки с 1 по 6
	UEPCONX = UEPCONX & (~MSK_EPEN);
	UEPNUM = 5;
	UEPCONX = UEPCONX & (~MSK_EPEN);
	UEPNUM = 4;
	UEPCONX = UEPCONX & (~MSK_EPEN);
	UEPNUM = 3;
	UEPCONX = UEPCONX & (~MSK_EPEN);
	UEPNUM = 2;
	UEPCONX = UEPCONX & (~MSK_EPEN);
	UEPNUM = 1;
	UEPCONX = UEPCONX & (~MSK_EPEN);

	addr = 0;

	UEPNUM = 0;			 // конфигурируем 0-ю конечную точку
	UEPCONX = MSK_EPEN;	 // разрешена 0-я конечная точка типа Control
	UEPRST = MSK_EP0RST; // сброс FIFO 0-й конечной точки
	UEPRST = UEPRST & (~MSK_EP0RST);
	UEPIEN = MSK_EP0INTE; // разрешение прерывания от 0-й конечной точки
}

void EP1_USB(void)
{
	//char i;

	UEPNUM = 1;
	if (UEPSTAX & MSK_TXCMPL)
	{ // Если передан блок данных, то
		UEPSTAX = UEPSTAX & (~MSK_TXCMPL);
		ep1_ready = 0;
		// Debug
		//for(i=0;i<15;++i) UEPDATX = i;  // только для отладки. Переделать!!!
		//EPSTAX = UEPSTAX|MSK_TXRDY;

		// End
	}
}

void EP2_USB(void)
{
	unsigned char i;
	char idata ddd;
	//	unsigned char i,EP2Nbytes;					// Если принятые данные будут обрабатываться вне этой процедуры, то
	// EP2Nbytes должна быть глобальной переменной!

	UEPNUM = 2;

	if (UEPSTAX & MSK_STALLED)
	{ // Ошибка, в т.ч. CRC
		UEPSTAX = UEPSTAX & (~MSK_STALLED);
		if (UEPSTAX & MSK_RXOUTB0)
		{ // если данные были приняты
			EP2Nbytes = UBYCTLX;
			for (i = 0; i < EP2Nbytes; ++i)
				ddd = UEPDATX; // очищаем приемный буфер (на всякий случай)
			UEPSTAX = UEPSTAX & (~MSK_RXOUTB0);
		}
	}

	if (UEPSTAX & MSK_RXOUTB0)
	{ // Если принят блок данных, то
		EP2Nbytes = UBYCTLX;
		if (EP2Nbytes == OUTPUT_REPORT_COUNT)
		{
			for (i = 0; i < EP2Nbytes; ++i)
				RX_BUF[i] = UEPDATX;

			UEPIEN = UEPIEN & (~MSK_EP2INTE); // запрет прерывания от 2-й конечной точки
											  // этот запрет не помогает от вызова обработчика 2-й конечной точки,
											  // помогает флаг ep2_ready - пока данные не будут обработаны
											  // командным процессором и флаг не будет сброшен, обработчик не будет вызван

			ep2_ready = 1; // Данные получены
		}
		else
			for (i = 0; i < EP2Nbytes; ++i)
				ddd = UEPDATX; // очищаем приемный буфер (на всякий случай)

		UEPSTAX = UEPSTAX & (~MSK_RXOUTB0); // Установка бита готовности к приему новых данных на точку EP2
											// Надо ли его устанавливать, или лучше подождать пока принятые данные
											// будут использованы? - Надо 07.12.08 (при этом сбрасывается запрос
											// в регистре UEPINT, а порча содержимого RX_BUF раньше времени
											// заблокирована флагом ep2_ready = 1)
	}

	if (UEPSTAX & MSK_RXOUTB1)
		UEPSTAX = UEPSTAX & (~MSK_RXOUTB1);
}

void EP0_USB(void)
{
	unsigned char i, EP0Nbytes;
	unsigned int wRequest;
	UEPNUM = 0;

	if (UEPSTAX & MSK_STALLED)
	{ // Если передан STALL, то
		UEPSTAX = UEPSTAX & (~MSK_STALLED);
		return;
	}

	if (UEPSTAX & MSK_TXCMPL)
	{ // Если передан блок данных, то
		UEPSTAX = UEPSTAX & (~MSK_TXCMPL);
		if (set_addr_flg)
		{
			//			USBCON  = USBCON|MSK_CONFG;  // Изменено 07.12.08
			USBCON = USBCON | MSK_FADDEN; // Изменено 07.12.08 в соотв. с doc4337.pdf Table 92.
			USBADDR = 0x80 | (0x7F & addr);
			set_addr_flg = 0;
			return;
		}
		if (data_to_transfer != 0)
		{
			WRFIFO(&pbuffer, &data_to_transfer);
			UEPSTAX = UEPSTAX | MSK_TXRDY;
		}
		else if (zlp)
		{
			UEPSTAX = UEPSTAX | MSK_TXRDY;
			zlp = 0;
		}
		else
			UEPSTAX = UEPSTAX & (~MSK_DIR);
	}

	if (UEPSTAX & MSK_RXOUTB0)
	{

		EP0Nbytes = UBYCTLX;
		for (i = 0; i < EP0Nbytes; ++i)
			ENDP0_RXBUF[i] = UEPDATX;
		UEPSTAX = UEPSTAX & (~MSK_RXOUTB0);
	}

	if (UEPSTAX & MSK_RXOUTB1)
	{
		UEPSTAX = UEPSTAX & (~MSK_RXOUTB1);
	}

	if (UEPSTAX & MSK_RXSETUP)
	{ // Если принят пакет SETUP, то
		EP0Nbytes = UBYCTLX;
		for (i = 0; i < EP0Nbytes; ++i)
			ENDP0_RXBUF[i] = UEPDATX;
		UEPSTAX = UEPSTAX & (~MSK_RXSETUP);
		((char *)&wRequest)[0] = ENDP0_RXBUF[0];
		((char *)&wRequest)[1] = ENDP0_RXBUF[1];
		((char *)&data_block_size)[0] = ENDP0_RXBUF[7];
		((char *)&data_block_size)[1] = ENDP0_RXBUF[6];
		switch (wRequest)
		{
		case GET_STATUS_DEVICE:
			usb_get_status_device();
			break;
		case GET_STATUS_INTERF:
			usb_get_status_interface();
			break;
		case GET_STATUS_ENDPNT:
			usb_get_status_endpoint();
			break;
		case SET_ADDRESS:
			usb_set_address();
			break;
		case GET_DESCRIPTOR_DEVICE:
		case GET_DESCRIPTOR_INTERF:
		case GET_DESCRIPTOR_ENDPNT:
			usb_get_descriptor();
			break;
		case GET_CONFIGURATION:
			usb_get_configuration();
			break;
		case SET_CONFIGURATION:
			usb_set_configuration();
			break;
		case SET_IDLE:
		case SET_IDLE_1:
			usb_hid_set_idle();
			break;
		case CLEAR_FEATURE_ENDPNT:
			usb_clear_feature_endpnt();
			break;
		default:
			STALL();
			break;
		}
	}
}

void usb_clear_feature_endpnt(void)
{

	UEPSTAX = UEPSTAX | MSK_TXRDY;
}

void usb_hid_set_idle(void)
{
	UEPSTAX = UEPSTAX | MSK_TXRDY;
}

void usb_get_status_device(void)
{
}

void usb_get_status_interface(void)
{
}

void usb_get_status_endpoint(void)
{
}

void usb_set_address(void)
{
	addr = ENDP0_RXBUF[2];
	UEPSTAX = UEPSTAX | MSK_TXRDY;
	set_addr_flg = 1;
}

void usb_get_descriptor(void)
{
	char string_type;
	char descriptor_type;

	string_type = ENDP0_RXBUF[2];
	descriptor_type = ENDP0_RXBUF[3];

	switch (descriptor_type)
	{
	case DEVICE:
		data_to_transfer =
			data_block_size < sizeof(DEVICE_DESCRIPTOR) ? data_block_size : sizeof(DEVICE_DESCRIPTOR);
		pbuffer = (char code *)&DEVICE_DESCRIPTOR;
		break;
	case CONFIGURATION:
		data_to_transfer =
			data_block_size < sizeof(USB_CONFIGURATION) ? data_block_size : sizeof(USB_CONFIGURATION);
		pbuffer = (char code *)&USB_CONFIGURATION;
		break;
	case STRING:
		switch (string_type)
		{
		case LANG_ID:
			data_to_transfer =
				data_block_size < sizeof(STRING_DESCRIPTOR_LANGID) ? data_block_size : sizeof(STRING_DESCRIPTOR_LANGID);
			pbuffer = STRING_DESCRIPTOR_LANGID;
			break;
		case MAN_INDEX:
			data_to_transfer =
				data_block_size < sizeof(STRING_DESCRIPTOR_MANNAME) ? data_block_size : sizeof(STRING_DESCRIPTOR_MANNAME);
			pbuffer = STRING_DESCRIPTOR_MANNAME;
			break;
		case PRD_INDEX:
			data_to_transfer =
				data_block_size < sizeof(STRING_DESCRIPTOR_PRDNAME) ? data_block_size : sizeof(STRING_DESCRIPTOR_PRDNAME);
			pbuffer = STRING_DESCRIPTOR_PRDNAME;
			break;
		case SRN_INDEX:
			data_to_transfer =
				data_block_size < sizeof(STRING_DESCRIPTOR_SRNNUM) ? data_block_size : sizeof(STRING_DESCRIPTOR_SRNNUM);
			pbuffer = STRING_DESCRIPTOR_SRNNUM;
			break;
		default:
			STALL();
			return;
		}
		break;
		//		case HID:
		//			data_to_transfer =
		//				data_block_size<sizeof(USB_CONFIGURATION.hid_descr)?
		//					data_block_size:sizeof(USB_CONFIGURATION.hid_descr);
		//			pbuffer=(char code *)&(USB_CONFIGURATION.hid_descr.bLength);
		//			break;
		//		case REPORT:
		//			data_to_transfer =
		//				data_block_size<sizeof(USB_CONFIGURATION.rep)?
		//					data_block_size:sizeof(USB_CONFIGURATION.rep);
		//			pbuffer=(char code *)&(USB_CONFIGURATION.rep[0]);
		//			break;
	default:
		STALL();
		return;
	}
	if ((data_to_transfer & (FIFO_0_SIZE - 1)) == 0)
		zlp = 1;
	UEPSTAX = UEPSTAX | MSK_DIR;
	WRFIFO(&pbuffer, &data_to_transfer);
	UEPSTAX = UEPSTAX | MSK_TXRDY;
}

void WRFIFO(char code *(data *(buf_ptr_ptr)), unsigned int data *nb)
{
	unsigned char i;
	for (i = 0; (i < FIFO_0_SIZE) & (0 < *nb); ++i)
	{
		UEPDATX = *(*buf_ptr_ptr);
		++(*buf_ptr_ptr);
		--(*nb);
	}
}

void usb_get_configuration(void)
{
}

void usb_set_configuration(void)
{ // переписано под CDC 25.08.2020
	//unsigned char i;
	UEPNUM = 1; // конфигурируем 1-ю конечную точку
		// разрешена 1-я конечная точка BULK IN
	UEPCONX = 0x86; // MSK_EPEN|MSK_EPDIR|MSK_EPTYPE1 = 0x86

	UEPRST = MSK_EP1RST; // сброс FIFO 1-й конечной точки
	UEPRST = UEPRST & (~MSK_EP1RST);
	//UEPIEN = UEPIEN|MSK_EP1INTE;		// разрешение прерывания от 1-й конечной точки
	//UEPSTAX = 0;

	ep1_ready = 0;
	//for(i=0;i<15;++i) UEPDATX = i;  // только для отладки. Переделать!!!
	//UEPSTAX = UEPSTAX|MSK_TXRDY;

	UEPNUM = 2;			 // конфигурируем 2-ю конечную точку
						 // разрешена 2-я конечная точка OUT Interrupt
	UEPCONX = 0x82;		 //MSK_EPEN|MSK_EPTYPE1 = 0x82;
	UEPRST = MSK_EP2RST; // сброс FIFO 2-й конечной точки
	UEPRST = UEPRST & (~MSK_EP2RST);
	//UEPIEN = UEPIEN|MSK_EP2INTE;		// разрешение прерывания от 2-й конечной точки
	//UEPSTAX = 0;

	UEPNUM = 3;			 // конфигурируем 3-ю конечную точку
						 // разрешена 3-я конечная точка IN Interrupt
	UEPCONX = 0x87;		 //MSK_EPEN|MSK_EPDIR|MSK_EPTYPE1|MSK_EPTYPE0;
	UEPRST = MSK_EP3RST; // сброс FIFO 3-й конечной точки
	UEPRST = UEPRST & (~MSK_EP3RST);
	//UEPIEN = UEPIEN|MSK_EP3INTE;		// разрешение прерывания от 3-й конечной точки
	//UEPSTAX = 0;

	UEPNUM = 0;
	UEPSTAX = UEPSTAX | MSK_TXRDY;

	USBCON = USBCON | MSK_CONFG; // Изменено 07.12.08 в соотв. с doc4337.pdf Table 92.
								 // (Значение параметра конфигурации не проверяется!!!)
}

void STALL(void)
{
	UEPRST = MSK_EP0RST; // сброс FIFO 0-й конечной точки
	UEPRST = UEPRST & (~MSK_EP0RST);
	UEPSTAX = 0;
	UEPSTAX = MSK_STALLRQ;
}
