

#ifndef _USB_H_
#define _USB_H_

/*_____ D E C L A R A T I O N S _________________________________*/

typedef unsigned char Uchar;
typedef float Float16;

typedef unsigned char Uint8;
typedef unsigned int Uint16;
typedef unsigned long int Uint32;

typedef char Int8;
typedef int Int16;
typedef long int Int32;

typedef unsigned char Bool;
typedef unsigned char Byte;
typedef unsigned int Word;
typedef unsigned long int DWord;

typedef union {
  Uint32 dw; // l changed in dw (double word) because l is used for signed long...
  Uint16 w[2];
  Uint8 b[4];
} Union32;

typedef union {
  Uint16 w;
  Uint8 b[2];
} Union16;

/*_____E N D   O F   D E C L A R A T I O N S ____________________*/

// Константы, составляющие дескриптор устройства

#define USB_SPECIFICATION 0x1001
//#define DEVICE_CLASS          0x00
#define DEVICE_CLASS 0x02
#define DEVICE_SUB_CLASS 0
#define DEVICE_PROTOCOL 0
#define EP_CONTROL_LENGTH 32
#define VENDOR_ID 0xEB03  // Atmel = 03EBh
#define PRODUCT_ID 0x0920 //0x0920
#define RELEASE_NUMBER 0x0000
#define MAN_INDEX 0x01
#define PRD_INDEX 0x02
#define SRN_INDEX 0x03

#define LANG_ID 0x00
#define LANGUAGE_ID 0x0904

//#define MAN_INDEX             0x01
//#define PRD_INDEX             0x02
//#define SRN_INDEX             0x03

// Константы для дескриптора интерфейса

/* CONFIGURATION DESCRIPTOR */
#define CONF_LENGTH 0x4300 // 4300
#define NB_INTERFACE 2
#define CONF_NB 1
#define CONF_INDEX 0
#define CONF_ATTRIBUTES USB_CONFIG_BUSPOWERED
#define MAX_POWER 50 /* 100 mA */
                     /* INTERFACE 0 DESCRIPTOR */
#define INTERFACE0_NB 0
#define ALTERNATE0 0
#define NB_ENDPOINT0 1        // 1 -> 2
#define INTERFACE0_CLASS 0x02 /* CDC ACM Comm */
#define INTERFACE0_SUB_CLASS 0x02
#define INTERFACE0_PROTOCOL 0x01
#define INTERFACE0_INDEX 0

/* INTERFACE 1 DESCRIPTOR */
#define INTERFACE1_NB 1
#define ALTERNATE1 0
#define NB_ENDPOINT1 2
#define INTERFACE1_CLASS 0x0A /* CDC ACM Data */
#define INTERFACE1_SUB_CLASS 0
#define INTERFACE1_PROTOCOL 0
#define INTERFACE1_INDEX 0

/* ENDPOINT 3 DESCRIPTOR */
#define ENDPOINT_NB_3 0x83
#define EP_ATTRIBUTES_3 0x03 // interrupt
#define EP_SIZE_3 ((Uint16)32) << 8
#define EP_INTERVAL_3 0x02

/* ENDPOINT 1 DESCRIPTOR */
#define ENDPOINT_NB_1 0x81
#define EP_ATTRIBUTES_1 0x02 // bulk
#define TX_EP_SIZE 32
#define EP_SIZE_1 ((Uint16)TX_EP_SIZE) << 8
#define EP_INTERVAL_1 0x00

/* ENDPOINT 2 DESCRIPTOR */
#define ENDPOINT_NB_2 0x02
#define EP_ATTRIBUTES_2 0x02 // bulk
#define RX_EP_SIZE 32
#define EP_SIZE_2 ((Uint16)RX_EP_SIZE) << 8
#define EP_INTERVAL_2 0x00

//#define INTERFACE_CLASS       0x03   // HID
//#define INTERFACE_SUB_CLASS   0xFF
//#define INTERFACE_PROTOCOL    0xFF

/*_____ U S B   D E V I C E   D E S C R I P T O R _______________________*/

struct usb_st_device_descriptor
{
  Uchar bLength;            /* Size of this descriptor in bytes */
  Uchar bDescriptorType;    /* DEVICE descriptor type */
  Uint16 bscUSB;            /* Binay Coded Decimal Spec. release */
  Uchar bDeviceClass;       /* Class code assigned by the USB */
  Uchar bDeviceSubClass;    /* Sub-class code assigned by the USB */
  Uchar bDeviceProtocol;    /* Protocol code assigned by the USB */
  Uchar bMaxPacketSize0;    /* Max packet size for EP0 */
  Uint16 idVendor;          /* Vendor ID. ATMEL = 0x03EB */
  Uint16 idProduct;         /* Product ID assigned by the manufacturer */
  Uint16 bcdDevice;         /* Device release number */
  Uchar iManufacturer;      /* Index of manu. string descriptor */
  Uchar iProduct;           /* Index of prod. string descriptor */
  Uchar iSerialNumber;      /* Index of S.N.  string descriptor */
  Uchar bNumConfigurations; /* Number of possible configurations */
};

code struct usb_st_device_descriptor DEVICE_DESCRIPTOR =
    {
        sizeof(DEVICE_DESCRIPTOR),
        DEVICE,
        USB_SPECIFICATION,
        DEVICE_CLASS,
        DEVICE_SUB_CLASS,
        DEVICE_PROTOCOL,
        EP_CONTROL_LENGTH,
        VENDOR_ID,
        PRODUCT_ID,
        RELEASE_NUMBER,
        MAN_INDEX,
        PRD_INDEX,
        SRN_INDEX,
        0x01};

struct usb_st_configuration_descriptor
{
  Uchar bLength;             /* size of this descriptor in bytes */
  Uchar bDescriptorType;     /* CONFIGURATION descriptor type */
  Uint16 wTotalLength;       /* total length of data returned */
  Uchar bNumInterfaces;      /* number of interfaces for this conf. */
  Uchar bConfigurationValue; /* value for SetConfiguration resquest */
  Uchar iConfiguration;      /* index of string descriptor */
  Uchar bmAttibutes;         /* Configuration characteristics */
  Uchar MaxPower;            /* maximum power consumption */
};

struct usb_st_interface_descriptor
{
  Uchar bLength;            /* size of this descriptor in bytes */
  Uchar bDescriptorType;    /* INTERFACE descriptor type */
  Uchar bInterfaceNumber;   /* Number of interface */
  Uchar bAlternateSetting;  /* value to select alternate setting */
  Uchar bNumEndpoints;      /* Number of EP except EP 0 */
  Uchar bInterfaceClass;    /* Class code assigned by the USB */
  Uchar bInterfaceSubClass; /* Sub-class code assigned by the USB */
  Uchar bInterfaceProtocol; /* Protocol code assigned by the USB */
  Uchar iInterface;         /* Index of string descriptor */
};

struct usb_st_hid_descriptor
{
  Uchar bLength;            /* Size of this descriptor in bytes */
  Uchar bDescriptorType;    /* HID descriptor type */
  Uint16 bscHID;            /* Binay Coded Decimal Spec. release */
  Uchar bCountryCode;       /* Hardware target country */
  Uchar bNumDescriptors;    /* Number of HID class descriptors to follow */
  Uchar bRDescriptorType;   /* Report descriptor type */
  Uint16 wDescriptorLength; /* Total length of Report descriptor */
};

struct usb_st_endpoint_descriptor
{
  Uchar bLength;          /* Size of this descriptor in bytes */
  Uchar bDescriptorType;  /* ENDPOINT descriptor type */
  Uchar bEndpointAddress; /* Address of the endpoint */
  Uchar bmAttributes;     /* Endpoint's attributes */
  Uint16 wMaxPacketSize;  /* Maximum packet size for this EP */
  Uchar bInterval;        /* Interval for polling EP in ms */
};

#define SIZE_OF_REPORT 0x22
#define INPUT_REPORT_COUNT 0x0F
#define OUTPUT_REPORT_COUNT 0x0F
//#define SIZE_OF_REPORT  0x17

code struct
{
  struct usb_st_configuration_descriptor cfg;
  struct usb_st_interface_descriptor ifc0;
  Uchar CS_INTERFACE0[19];
  struct usb_st_endpoint_descriptor ep3;
  struct usb_st_interface_descriptor ifc1;
  struct usb_st_endpoint_descriptor ep1;
  struct usb_st_endpoint_descriptor ep2;
} USB_CONFIGURATION =
    {
        {9, CONFIGURATION, CONF_LENGTH, NB_INTERFACE, CONF_NB,
         CONF_INDEX, CONF_ATTRIBUTES, MAX_POWER},
        {9, INTERFACE, INTERFACE0_NB, ALTERNATE0, NB_ENDPOINT0, INTERFACE0_CLASS,
         INTERFACE0_SUB_CLASS, INTERFACE0_PROTOCOL, INTERFACE0_INDEX},
        {0x05, 0x24, 0x00, 0x10, 0x01, 0x05, 0x24, 0x01, 0x03, 0x01, 0x04, 0x24, 0x02, 0x06, 0x05, 0x24, 0x06, 0x00, 0x01},
        {7, ENDPOINT, ENDPOINT_NB_3, EP_ATTRIBUTES_3, EP_SIZE_3, EP_INTERVAL_3},
        {9, INTERFACE, INTERFACE1_NB, ALTERNATE1, NB_ENDPOINT1, INTERFACE1_CLASS,
         INTERFACE1_SUB_CLASS, INTERFACE1_PROTOCOL, INTERFACE1_INDEX},
        {7, ENDPOINT, ENDPOINT_NB_1, EP_ATTRIBUTES_1, EP_SIZE_1, EP_INTERVAL_1},
        {7, ENDPOINT, ENDPOINT_NB_2, EP_ATTRIBUTES_2, EP_SIZE_2, EP_INTERVAL_2},

};

char code STRING_DESCRIPTOR_LANGID[] =
    {0x04, // bLength
     0x03, // bDescriptorType
     0x09,
     0x04};
char code STRING_DESCRIPTOR_MANNAME[] =
    {0x24, // bLength
     0x03, // bDescriptorType
     'A', 0, 'L', 0, 'M', 0, 'A', 0, 'Z', 0, ' ', 0,
     'E', 0, 'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0,
     'o', 0, 'n', 0, 'i', 0, 'c', 0, 's', 0};
/*
char code STRING_DESCRIPTOR_PRDNAME[]=
                                   {0x26,   // bLength
                                    0x03,   // bDescriptorType
                                    'A',0,'D',0,'U',0,'C',0,'8',0,'3',0,'1',0,
                                    ' ',0,'t',0,'e',0,'s',0,'t',0,' ',0,'b',0,
                                    'o',0,'a',0,'r',0,'d',0};
*/

char code STRING_DESCRIPTOR_PRDNAME[] =
    {0x44, // bLength
     0x03, // bDescriptorType
     'S', 0, 't', 0, 'e', 0, 'p', 0, 'p', 0, 'e', 0, 'r', 0,
     ' ', 0, 'm', 0, 'o', 0, 't', 0, 'o', 0, 'r', 0, ' ', 0,
     'c', 0, 'o', 0, 'n', 0, 't', 0, 'r', 0, 'o', 0, 'l', 0,
     'l', 0, 'e', 0, 'r', 0, ',', 0, ' ', 0, 'd', 0, 'e', 0,
     'v', 0, '.', 0, ' ', 0, '#', 0, '3', 0}; // Изменено 5.02.16

char code STRING_DESCRIPTOR_SRNNUM[] =
    {0x0A, // bLength
     0x03, // bDescriptorType
     '0', 0, '.', 0, '0', 0, '0', 0};
/*
char code HID_REPORT_DESCRIPTOR[]=
                                   {0x06,0x00,0xFF,
                                    0x09,0x01,
                                    0xA1,0x01,
                                    0x19,0x01,
                                    0x29,0x01,
                                    0x15,0x00,
                                    0x26,0xFF,0x00,
                                    0x75,0x08,
                                    0x95,0x07,
                                    0x81,0x02,
                                    0xC0};

*/
#endif /* _USB_H_ */
