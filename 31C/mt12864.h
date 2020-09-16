
#define A0                  P3_2
#define E                   P3_3
#define E1                  P3_4
#define E2                  P3_5
#define RD_WR               P3_7

#define BUSY                0x80
#define OFF                 0x20
#define RESET               0x10

#define Display_ON          0x3F
#define Display_OFF         0x3E
#define Display_Start_Line  0xC0
#define Set_Page            0xB8
#define Set_Address         0x40

#define CHAR_SIZEX 6
#define CHAR_SIZEY 8

//#define delay_12864			 _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_()
//#define delay_12864			 	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_()
#define delay_12864			 	_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_()

//#define WAIT_12864              A0 = 0; RD_WR = 1; _nop_(); E = 1; _nop_(); _nop_(); while(P2&BUSY); E = 0

// 07.02.2011
//#define WAIT_12864              A0 = 0; RD_WR = 1; _nop_(); _nop_(); E = 1; _nop_(); _nop_(); while(P2&BUSY); E = 0
//#define WAIT_12864              A0 = 0; RD_WR = 1;_nop_();_nop_();_nop_();_nop_(); E = 1; _nop_();_nop_();_nop_();_nop_(); while(P2&BUSY); E = 0
#define WAIT_12864              A0 = 0; RD_WR = 1; delay_12864; E = 1; delay_12864; while(P2&BUSY); E = 0

//For test ONLY!!!
//#define WAIT_12864              A0 = 0; RD_WR = 1; _nop_(); _nop_(); E = 1; _nop_(); _nop_(); /* while(P2&BUSY); */ E = 0

#define SEL_CRYSTAL_12864(n)    if(!n) E1 = 1; else E2 = 1
#define DESEL_CRYSTAL_12864(n)  if(!n) E1 = 0; else E2 = 0
//#define STROBE_12864            _nop_(); E = 1; _nop_(); _nop_(); E = 0
//#define STROBE_12864            _nop_(); _nop_(); _nop_(); E = 1; _nop_(); _nop_(); _nop_(); E = 0; _nop_();
//#define STROBE_12864            _nop_();_nop_();_nop_();_nop_();_nop_();_nop_(); E = 1; _nop_();_nop_();_nop_();_nop_();_nop_();_nop_(); E = 0; _nop_();
#define STROBE_12864            delay_12864; E = 1; delay_12864; E = 0; delay_12864

/* Ïåðåä âûçîâîì WAIT_12864 îáÿçàòåëüíî âûçâàòü SEL_CRYSTAL_12864 */
/* Ïîñëå âûçîâà WAIT_12864 äî ïîñëåäóþùåãî STROBE_12864 äîëæíî ïðîéòè íå ìåíåå 1000ns-140ns-450ns */


unsigned char Xpos,Ypos; 


unsigned char code CHAR_SET[] = {
		0x00,0x00,0x00,0x00,0x00,0x00,				// 0x00
		0x00,0x00,0x00,0x00,0x00,0x00,				// 0x01
		0x00,0x00,0x00,0x00,0x00,0x00,				// 0x02
		0x00,0x00,0x00,0x00,0x00,0x00,				// 0x03
		0x00,0x00,0x00,0x00,0x00,0x00,				// 0x04
		0x00,0x00,0x00,0x00,0x00,0x00,				// 0x05
		0x00,0x00,0x00,0x00,0x00,0x00,				// 0x06
		0x00,0x00,0x00,0x00,0x00,0x00,				// 0x07
		0x00,0x00,0x00,0x00,0x00,0x00,				// 0x08
		0x00,0x00,0x00,0x00,0x00,0x00,				// 0x09
		0x00,0x00,0x00,0x00,0x00,0x00,				// 0x0A
		0x00,0x00,0x00,0x00,0x00,0x00,				// 0x0B
		0x00,0x00,0x00,0x00,0x00,0x00,				// 0x0C
		0x00,0x00,0x00,0x00,0x00,0x00,				// 0x0D
		0x00,0x00,0x00,0x00,0x00,0x00,				// 0x0E
		0x00,0x00,0x00,0x00,0x00,0x00,				// 0x0F

		0x00,0x7F,0x3E,0x1C,0x08,0x00,				// 0x10
		0x00,0x08,0x1C,0x3E,0x7F,0x00,				// 0x11
		0x06,0x05,0x00,0x06,0x05,0x00,				// 0x12
		0x05,0x06,0x00,0x05,0x06,0x00,				// 0x13
		0x44,0x66,0x77,0x66,0x44,0x00,				// 0x14
		0x11,0x33,0x77,0x33,0x11,0x00,				// 0x15
		0x1C,0x3E,0x3E,0x3E,0x1C,0x00,				// 0x16
		0x10,0x38,0x54,0x10,0x1F,0x00,				// 0x17
		0x04,0x02,0x7F,0x02,0x04,0x00,				// 0x18
		0x10,0x20,0x7F,0x20,0x10,0x00,				// 0x19
		0x08,0x08,0x2A,0x1C,0x08,0x00,				// 0x1A
		0x08,0x1C,0x2A,0x08,0x08,0x00,				// 0x1B
		0x40,0x44,0x4A,0x51,0x40,0x00,				// 0x1C
		0x40,0x51,0x4A,0x44,0x40,0x00,				// 0x1D
		0x20,0x38,0x3E,0x38,0x20,0x00,				// 0x1E
		0x02,0x0E,0x3E,0x0E,0x02,0x00,				// 0x1F

		0x00,0x00,0x00,0x00,0x00,0x00,				// 0x20
		0x00,0x00,0x4F,0x00,0x00,0x00,				// 0x21  !
		0x00,0x07,0x00,0x07,0x00,0x00,				// 0x22  "
		0x14,0x7F,0x14,0x7F,0x14,0x00,				// 0x23  #
		0x24,0x2A,0x7F,0x2A,0x12,0x00,				// 0x24  $
		0x23,0x13,0x08,0x64,0x62,0x00,				// 0x25  %
		0x36,0x49,0x55,0x22,0x50,0x00,				// 0x26  &
		0x00,0x00,0x05,0x03,0x00,0x00,				// 0x27  '
		0x00,0x1C,0x22,0x41,0x00,0x00,				// 0x28  (
		0x00,0x41,0x22,0x1C,0x00,0x00,				// 0x29  )
		0x14,0x08,0x3E,0x08,0x14,0x00,				// 0x2A  *
		0x08,0x08,0x3E,0x08,0x08,0x00,				// 0x2B  +
		0x00,0x00,0x50,0x30,0x00,0x00,				// 0x2C  ,
		0x08,0x08,0x08,0x08,0x08,0x00,				// 0x2D  -
		0x00,0x00,0x60,0x60,0x00,0x00,				// 0x2E  .
		0x20,0x10,0x08,0x04,0x02,0x00,				// 0x2F  /

		0x3E,0x51,0x49,0x45,0x3E,0x00,				// 0x30  0
		0x00,0x42,0x7F,0x40,0x00,0x00,				// 0x31  1
		0x42,0x61,0x51,0x49,0x46,0x00,				// 0x32  2
		0x21,0x41,0x45,0x4B,0x31,0x00,				// 0x33  3
		0x18,0x14,0x12,0x7F,0x10,0x00,				// 0x34  4
		0x27,0x45,0x45,0x45,0x39,0x00,				// 0x35  5
		0x3C,0x4A,0x49,0x49,0x30,0x00,				// 0x36  6
		0x03,0x01,0x71,0x09,0x07,0x00,				// 0x37  7
		0x36,0x49,0x49,0x49,0x36,0x00,				// 0x38  8
		0x06,0x49,0x49,0x29,0x1E,0x00,				// 0x39  9
		0x00,0x36,0x36,0x00,0x00,0x00,				// 0x3A  :
		0x00,0x56,0x36,0x00,0x00,0x00,				// 0x3B  ;
		0x00,0x08,0x14,0x22,0x41,0x00,				// 0x3C  <
		0x14,0x14,0x14,0x14,0x14,0x00,				// 0x3D  =
		0x00,0x41,0x22,0x14,0x08,0x00,				// 0x3E  >
		0x02,0x01,0x51,0x09,0x06,0x00,				// 0x3F  ?

		0x32,0x49,0x79,0x41,0x3E,0x00,				// 0x40  @
		0x7C,0x12,0x11,0x12,0x7C,0x00,				// 0x41  A
		0x7F,0x49,0x49,0x49,0x36,0x00,				// 0x42  B
		0x3E,0x41,0x41,0x41,0x22,0x00,				// 0x43  C
		0x7F,0x41,0x41,0x22,0x1C,0x00,				// 0x44  D
		0x7F,0x49,0x49,0x49,0x41,0x00,				// 0x45  E
		0x7F,0x09,0x09,0x09,0x01,0x00,				// 0x46  F
		0x3E,0x41,0x49,0x49,0x7A,0x00,				// 0x47  G
		0x7F,0x08,0x08,0x08,0x7F,0x00,				// 0x48  H
		0x00,0x41,0x7F,0x41,0x00,0x00,				// 0x49  I
		0x20,0x40,0x41,0x3F,0x01,0x00,				// 0x4A  J
		0x7F,0x08,0x14,0x22,0x41,0x00,				// 0x4B  K
		0x7F,0x40,0x40,0x40,0x40,0x00,				// 0x4C  L
		0x7F,0x02,0x0C,0x02,0x7F,0x00,				// 0x4D  M
		0x7F,0x04,0x08,0x10,0x7F,0x00,				// 0x4E  N
		0x3E,0x41,0x41,0x41,0x3E,0x00,				// 0x4F  O

		0x7F,0x09,0x09,0x09,0x06,0x00,				// 0x50  P
		0x3E,0x41,0x51,0x21,0x5E,0x00,				// 0x51  Q
		0x7F,0x09,0x19,0x29,0x46,0x00,				// 0x52  R
		0x26,0x49,0x49,0x49,0x32,0x00,				// 0x53  S
		0x01,0x01,0x7F,0x01,0x01,0x00,				// 0x54  T
		0x3F,0x40,0x40,0x40,0x3F,0x00,				// 0x55  U
		0x1F,0x20,0x40,0x20,0x1F,0x00,				// 0x56  V
		0x3F,0x40,0x38,0x40,0x3F,0x00,				// 0x57  W
		0x63,0x14,0x08,0x14,0x63,0x00,				// 0x58  X
		0x07,0x08,0x70,0x08,0x07,0x00,				// 0x59  Y
		0x61,0x51,0x49,0x45,0x43,0x00,				// 0x5A  Z
		0x00,0x7F,0x41,0x41,0x00,0x00,				// 0x5B  [
		0x02,0x04,0x08,0x10,0x20,0x00,				// 0x5C  back slash
		0x00,0x41,0x41,0x7F,0x00,0x00,				// 0x5D  ]
		0x04,0x02,0x01,0x02,0x04,0x00,				// 0x5E  ^
		0x40,0x40,0x40,0x40,0x40,0x00,				// 0x5F  _

		0x00,0x01,0x02,0x04,0x00,0x00,				// 0x60  `
		0x20,0x54,0x54,0x54,0x78,0x00,				// 0x61  a
		0x7F,0x48,0x44,0x44,0x38,0x00,				// 0x62  b
		0x38,0x44,0x44,0x44,0x20,0x00,				// 0x63  c
		0x38,0x44,0x44,0x48,0x7F,0x00,				// 0x64  d
		0x38,0x54,0x54,0x54,0x18,0x00,				// 0x65  e
		0x08,0x7E,0x09,0x01,0x02,0x00,				// 0x66  f
		0x08,0x54,0x54,0x54,0x3C,0x00,				// 0x67  g
		0x7F,0x08,0x04,0x04,0x78,0x00,				// 0x68  h
		0x00,0x48,0x7D,0x40,0x00,0x00,				// 0x69  i
		0x20,0x40,0x44,0x3D,0x00,0x00,				// 0x6A  j
		0x00,0x7F,0x10,0x28,0x44,0x00,				// 0x6B  k
		0x00,0x41,0x7F,0x40,0x00,0x00,				// 0x6C  l
		0x7C,0x04,0x78,0x04,0x78,0x00,				// 0x6D  m
		0x7C,0x08,0x04,0x04,0x78,0x00,				// 0x6E  n
		0x38,0x44,0x44,0x44,0x38,0x00,				// 0x6F  o

		0x7C,0x14,0x14,0x14,0x08,0x00,				// 0x70  p
		0x08,0x14,0x14,0x18,0x7C,0x00,				// 0x71  q
		0x7C,0x08,0x04,0x04,0x08,0x00,				// 0x72  r
		0x48,0x54,0x54,0x54,0x20,0x00,				// 0x73  s
		0x04,0x3F,0x44,0x40,0x20,0x00,				// 0x74  t
		0x3C,0x40,0x40,0x20,0x7C,0x00,				// 0x75  u
		0x1C,0x20,0x40,0x20,0x1C,0x00,				// 0x76  v
		0x3C,0x40,0x30,0x40,0x3C,0x00,				// 0x77  w
		0x44,0x28,0x10,0x28,0x44,0x00,				// 0x78  x
		0x0C,0x50,0x50,0x50,0x3C,0x00,				// 0x79  y
		0x44,0x64,0x54,0x4C,0x44,0x00,				// 0x7A  z
		0x00,0x08,0x36,0x41,0x00,0x00,              // 0x7B  {
		0x00,0x00,0x7F,0x00,0x00,0x00,				// 0x7C  |
		0x00,0x41,0x36,0x08,0x00,0x00,				// 0x7D  }
		0x10,0x08,0x08,0x10,0x08,0x00,				// 0x7E  ~
		0x3C,0x22,0x21,0x22,0x3C,0x00				// 0x7F
		
};

void gotoxy(unsigned char x, unsigned char y) {
	Xpos = x;
	Ypos = y;
}

void CMD_MT12864(bit ncr, unsigned char c) {
	SEL_CRYSTAL_12864(ncr);
	WAIT_12864;
	A0 = 0;
	RD_WR = 0;
	P2 = c;
	STROBE_12864;
	P2 = 0xFF;
	DESEL_CRYSTAL_12864(ncr);
}

void WR_MT12864(bit ncr, unsigned char pp, unsigned char aa, unsigned char bb) {
	SEL_CRYSTAL_12864(ncr);	

	WAIT_12864;
	RD_WR = 0;
	P2 = Set_Page|(pp&0x07);
	STROBE_12864;
	P2 = 0xFF;

	WAIT_12864;
	RD_WR = 0;
	P2 = Set_Address|(aa&0x3F);
	STROBE_12864;
	P2 = 0xFF;
	WAIT_12864;

	A0 = 1;
	RD_WR = 0;
	P2 = bb;
	STROBE_12864;
	P2 = 0xFF;
	DESEL_CRYSTAL_12864(ncr);
}

void INIT_MT12864(void) {
	unsigned char i,j;
	E  = 0;
	E1 = 0;
	E2 = 0;
	RD_WR = 0;

/* Program Reset */
	CMD_MT12864(0, Display_Start_Line|0x00);  /* Âåðõíþþ ñòðîêó íà 0  */
	CMD_MT12864(1, Display_Start_Line|0x00);  /* Âåðõíþþ ñòðîêó íà 0  */
	CMD_MT12864(0, Display_ON);
	CMD_MT12864(1, Display_ON);
/* Clear Screen */
	for(i=0;i<8;++i)
		for(j=0;j<64;++j) {
			WR_MT12864(0, i, j, 0x00);
			WR_MT12864(1, i, j, 0x00);
		}
	gotoxy(0,0);
}

/*
unsigned char RD_MT12864(bit ncr, unsigned char pp, unsigned char aa) {
	unsigned char c;
	CMD_MT12864(ncr, Set_Page|(pp&0x07));
	CMD_MT12864(ncr, Set_Address|(aa&0x3F));
	SEL_CRYSTAL_12864(ncr);
	WAIT_12864;
	A0 = 1;
	RD_WR = 1;
	E = 1;
	_nop_();
	_nop_();
	c = P2;
	E = 0;
	
	CMD_MT12864(ncr, Set_Page|(pp&0x07));
	CMD_MT12864(ncr, Set_Address|(aa&0x3F));
	SEL_CRYSTAL_12864(ncr);
	WAIT_12864;
	A0 = 1;
	RD_WR = 1;
	E = 1;
	_nop_();
	_nop_();
	c = P2;
	E = 0;
	DESEL_CRYSTAL_12864(ncr);
	return c;
}

*/

void PUTCHAR_12864(unsigned char c) {
	unsigned char i,crs,x0;
	unsigned int i0;
	i0 = (unsigned int)c*6;
	
	if(Xpos<10) {
		crs = 0;
		x0 = 4+Xpos*CHAR_SIZEX;
	}
	else {
		crs = 1;
		x0 = (Xpos-10)*CHAR_SIZEX;
	}
	SEL_CRYSTAL_12864(crs);	
	WAIT_12864;
	RD_WR = 0;
	P2 = Set_Page|Ypos;
	STROBE_12864;
	P2 = 0xFF;

	WAIT_12864;
	RD_WR = 0;
	P2 = Set_Address|x0;
	STROBE_12864;
	P2 = 0xFF;
	
	for(i=0;i<CHAR_SIZEX;++i) {
		WAIT_12864;
		A0 = 1;
		RD_WR = 0;
		P2 = CHAR_SET[i0+(unsigned int)i];
		STROBE_12864;
		P2 = 0xFF;
	}

	DESEL_CRYSTAL_12864(crs);
	++Xpos;
	if(Xpos>19) {
		Xpos = 0;
		Ypos = (++Ypos)&0x07;
	}
}


char putchar (char c)  {

  if (c == '\n')  {
	Ypos = (++Ypos)&0x07;
	Xpos = 0; 
  }

//  PUTCHAR_12864(c);

  return c;
}



