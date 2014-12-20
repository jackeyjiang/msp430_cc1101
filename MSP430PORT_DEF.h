

/*
SI     P1.4
SCLK   P1.5
SO     P1.2
GD2    P1.3
CSN    P1.0
GD0    P1.1
*/
#include <msp430G2553.h> 

#define 	GDO0_BIT    BIT0
#define 	GDO2_BIT	BIT1
#define     MISO_BIT	BIT2
#define 	MOSI_BIT	BIT3
#define	    SCK_BIT	    BIT2
#define  	CSN_BIT		BIT0

#define 	GDO0_DIR    P2DIR
#define 	GDO2_DIR	P2DIR
#define     MISO_DIR	P2DIR
#define 	MOSI_DIR	P3DIR
#define	    SCK_DIR	    P3DIR
#define  	CSN_DIR		P3DIR


#define 	GDO0_POUT   P2OUT
#define 	GDO2_POUT	P2OUT
#define     MISO_POUT	P2OUT
#define 	MOSI_POUT	P3OUT
#define	    SCK_POUT	P3OUT
#define  	CSN_POUT	P3OUT

#define 	GDO0_PIN    P2IN
#define 	GDO2_PIN	P2IN
#define     MISO_PIN	P2IN
#define 	MOSI_PIN	P3IN
#define	    SCK_PIN	    P3IN
#define  	CSN_PIN		P3IN


#define 	GDO0_IN         GDO0_PIN&GDO0_BIT 
#define 	GDO2_IN	        GDO2_PIN&GDO2_BIT
#define     MISO_IN	        MISO_PIN&MISO_BIT
#define 	MOSI_IN	        MOSI_PIN&MOSI_BIT
#define	    SCK_IN	        SCK_PIN &SCK_BIT
#define  	CSN_IN		    CSN_PIN &CSN_BIT


#define 	GDO0_UP         GDO0_POUT|=GDO2_BIT 
#define 	GDO2_UP	        GDO2_POUT|=GDO2_BIT
#define     MISO_UP  	    MISO_POUT|=MISO_BIT
#define 	MOSI_UP	        MOSI_POUT|=MOSI_BIT
#define	    SCK_UP	        SCK_POUT|=SCK_BIT
#define  	CSN_UP		    CSN_POUT|=CSN_BIT

#define 	GDO0_DN         GDO0_POUT&=~GDO2_BIT 
#define 	GDO2_DN	        GDO2_POUT&=~GDO2_BIT
#define     MISO_DN  	    MISO_POUT&=~MISO_BIT
#define 	MOSI_DN	        MOSI_POUT&=~MOSI_BIT
#define	    SCK_DN	        SCK_POUT &=~SCK_BIT
#define  	CSN_DN		    CSN_POUT &=~CSN_BIT




#define 	GDO0_DIR_OUT        GDO0_DIR|=GDO0_BIT 
#define 	GDO2_DIR_OUT	    GDO2_DIR|=GDO2_BIT 
#define     MISO_DIR_OUT	    MISO_DIR|=MISO_BIT 
#define 	MOSI_DIR_OUT	    MOSI_DIR|=MOSI_BIT 
#define	    SCK_DIR_OUT	        SCK_DIR |=SCK_BIT  
#define  	CSN_DIR_OUT         CSN_DIR |=CSN_BIT 


#define 	GDO0_DIR_IN         GDO0_DIR&=~GDO0_BIT  
#define 	GDO2_DIR_IN	        GDO2_DIR&=~GDO2_BIT 
#define     MISO_DIR_IN	        MISO_DIR&=~MISO_BIT 
#define 	MOSI_DIR_IN	        MOSI_DIR&=~MOSI_BIT 
#define	    SCK_DIR_IN	        SCK_DIR &=~SCK_BIT 
#define  	CSN_DIR_IN          CSN_DIR &=~CSN_BIT 




#define 	KEY1_BIT    BIT6
#define 	KEY2_BIT	BIT7
#define 	KEY3_BIT	BIT3
#define     LED1_BIT	BIT3
#define 	LED2_BIT	BIT4
#define     MOTOD1_BIT  BIT5
#define     MOTOD2_BIT  BIT4

#define 	KEY1_DIR    P2DIR
#define 	KEY2_DIR	P2DIR
#define 	KEY3_DIR	P1DIR
#define     LED1_DIR	P2DIR
#define 	LED2_DIR	P3DIR
#define     MOTOD1_DIR  P2DIR
#define     MOTOD2_DIR  P2DIR

#define 	KEY1_POUT   P2OUT
#define 	KEY2_POUT	P2OUT
#define 	KEY3_POUT	P1OUT
#define     LED1_POUT	P2OUT
#define 	LED2_POUT	P3OUT
#define     MOTOD1_POUT P2OUT
#define     MOTOD2_POUT P2OUT


#define 	KEY1_PIN    P2IN
#define 	KEY2_PIN	P2IN
#define 	KEY3_PIN	P1IN
#define     LED1_PIN	P2IN
#define 	LED2_PIN	P3IN
#define     MOTOD1_PIN  P2IN
#define     MOTOD2_PIN  P2IN


#define 	KEY1_IN         KEY1_PIN&KEY1_BIT 
#define 	KEY2_IN	        KEY2_PIN&KEY2_BIT
#define 	KEY3_IN	        KEY3_PIN&KEY3_BIT
#define     LED1_IN	        LED1_PIN&LED1_BIT
#define 	LED2_IN	        LED2_PIN&LED2_BIT
#define     MOTOD1_IN       MOTOD1_PIN&MOTOD1_BIT
#define     MOTOD2_IN       MOTOD2_PIN&MOTOD2_BIT



#define 	KEY1_UP         KEY1_POUT|=KEY1_BIT 
#define 	KEY2_UP	        KEY2_POUT|=KEY2_BIT
#define 	KEY3_UP	        KEY3_POUT|=KEY3_BIT
#define     LED1_UP  	    LED1_POUT|=LED1_BIT
#define 	LED2_UP	        LED2_POUT|=LED2_BIT
#define     MOTOD1_UP       MOTOD1_POUT|=MOTOD1_BIT
#define     MOTOD2_UP       MOTOD2_POUT|=MOTOD2_BIT

#define 	KEY1_DN         KEY1_POUT&=~KEY1_BIT 
#define 	KEY2_DN	        KEY2_POUT&=~KEY2_BIT
#define 	KEY3_DN	        KEY3_POUT&=~KEY3_BIT
#define     LED1_DN  	    LED1_POUT&=~LED1_BIT
#define 	LED2_DN	        LED2_POUT&=~LED2_BIT
#define     MOTOD1_DN       MOTOD1_POUT&=~MOTOD1_BIT
#define     MOTOD2_DN       MOTOD2_POUT&=~MOTOD2_BIT

#define 	KEY1_TG         KEY1_POUT^=KEY1_BIT 
#define 	KEY2_TG	        KEY2_POUT^=KEY2_BIT
#define 	KEY3_TG	        KEY3_POUT^=KEY3_BIT
#define     LED1_TG  	    LED1_POUT^=LED1_BIT
#define 	LED2_TG	        LED2_POUT^=LED2_BIT
#define     MOTOD1_TG       MOTOD1_POUT^=MOTOD1_BIT
#define     MOTOD2_TG       MOTOD2_POUT^=MOTOD2_BIT


#define 	KEY1_DIR_OUT        KEY1_DIR|=KEY1_BIT 
#define 	KEY2_DIR_OUT	    KEY2_DIR|=KEY2_BIT 
#define 	KEY3_DIR_OUT	    KEY3_DIR|=KEY3_BIT 
#define     LED1_DIR_OUT	    LED1_DIR|=LED1_BIT 
#define 	LED2_DIR_OUT	    LED2_DIR|=LED2_BIT 
#define     MOTOD1_DIR_OUT      MOTOD1_DIR|=MOTOD1_BIT
#define     MOTOD2_DIR_OUT      MOTOD1_DIR|=MOTOD2_BIT



#define 	KEY1_DIR_IN     KEY1_DIR&=~KEY1_BIT  
#define 	KEY2_DIR_IN	    KEY2_DIR&=~KEY2_BIT 
#define 	KEY3_DIR_IN	    KEY3_DIR&=~KEY3_BIT 
#define     LED1_DIR_IN	    LED1_DIR&=~LED1_BIT 
#define 	LED2_DIR_IN	    LED2_DIR&=~LED2_BIT 
#define     MOTOD1_DIR_IN   MOTOD1_DIR&=~MOTOD1_BIT
#define     MOTOD2_DIR_IN   MOTOD1_DIR&=~MOTOD2_BIT
