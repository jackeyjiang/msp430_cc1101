//
//CC1100.h
//
// Copyright (c) NewMSG. All rights reserved. 2007
// purpose : Test CC1100
#include "CC1100.h"
#include "../MSP430PORT_DEF.h"


#define 	WRITE_BURST     	    0x40						//连续写入
#define 	READ_SINGLE     	    0x80						//读
#define 	READ_BURST      	    0xC0						//连续读
#define 	BYTES_IN_RXFIFO         0x7F  						//接收缓冲区的有效字节数
#define 	CRC_OK                  0x80 						//CRC校验通过位标志

 // 100k E
 const RF_SETTINGS rfSettings = {
    0x00,
    0x08,   // FSCTRL1   Frequency synthesizer control.
    0x00,   // FSCTRL0   Frequency synthesizer control.
    0x10,   // FREQ2     Frequency control word, high byte.
    0xA7,   // FREQ1     Frequency control word, middle byte.
    0x62,   // FREQ0     Frequency control word, low byte.
    0x5B,   // MDMCFG4   Modem configuration.
    0xF8,   // MDMCFG3   Modem configuration.
    0x03,   // MDMCFG2   Modem configuration.
    0x22,   // MDMCFG1   Modem configuration.
    0xF8,   // MDMCFG0   Modem configuration.

    0x00,   // CHANNR    Channel number.
    0x47,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
    0xB6,   // FREND1    Front end RX configuration.
    0x10,   // FREND0    Front end RX configuration.
    0x18,   // MCSM0     Main Radio Control State Machine configuration.
    0x1D,   // FOCCFG    Frequency Offset Compensation Configuration.
    0x1C,   // BSCFG     Bit synchronization Configuration.
    0xC7,   // AGCCTRL2  AGC control.
    0x00,   // AGCCTRL1  AGC control.
    0xB2,   // AGCCTRL0  AGC control.

    0xEA,   // FSCAL3    Frequency synthesizer calibration.
    0x2A,   // FSCAL2    Frequency synthesizer calibration.
    0x00,   // FSCAL1    Frequency synthesizer calibration.
    0x11,   // FSCAL0    Frequency synthesizer calibration.
    0x59,   // FSTEST    Frequency synthesizer calibration.
    0x81,   // TEST2     Various test settings.
    0x35,   // TEST1     Various test settings.
    0x09,   // TEST0     Various test settings.
    0x0B,   // IOCFG2    GDO2 output pin configuration.
    0x06,   // IOCFG0D   GDO0 output pin configuration. Refer to SmartRF?Studio User Manual for detailed pseudo register explanation.

    0x04,   // PKTCTRL1  Packet automation control.
    0x05,   // PKTCTRL0  Packet automation control.
    0x00,   // ADDR      Device address.
    0x0c    // PKTLEN    Packet length.
};

INT8U leng=8;
INT8U PaTabel[8] = {0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0};

//*****************************************************************************************
//函数名：delay(unsigned int s)
//输入：时间
//输出：无
//功能描述：普通廷时,内部用
//*****************************************************************************************		
void delay(unsigned int s)
{
	unsigned int i;
	for(i=0; i<s; i++);
	for(i=0; i<s; i++);
}


void halWait(INT16U timeout) {
    do {
      for(INT8U i=0;i<20;i++)  _NOP(); 
    } while (--timeout);
}


void SpiInit(void)
{
	MOSI_DIR_OUT;
    MISO_DIR_IN;
	SCK_DIR_OUT;
	CSN_DIR_OUT;
	GDO0_DIR_IN;
	GDO2_DIR_IN;
        
    CSN_DN;
    SCK_DN;
    CSN_UP;
}

	

//*****************************************************************************************
//函数名：SpisendByte(INT8U dat)
//输入：发送的数据
//输出：无
//功能描述：SPI发送一个字节
//*****************************************************************************************
INT8U SpiTxRxByte(INT8U dat)
{
	INT8U i,temp;
	temp = 0;
	
	SCK_DN;
	for(i=0; i<8; i++)
	{
		if(dat & 0x80)
		{
			MOSI_UP	;
		}
		else MOSI_DN;
		dat <<= 1;

		SCK_UP; 
		temp <<= 1;
		if(MISO_IN)temp++; 
		SCK_DN;
	}
	return temp;
        
        
}

//*****************************************************************************************
//函数名：void RESET_CC1100(void)
//输入：无
//输出：无
//功能描述：复位CC1100
//*****************************************************************************************
void RESET_CC1100(void) 
{
	CSN_DN; 
	while (MISO_IN);  
    SpiTxRxByte(CCxxx0_SRES); 		//写入复位命令
	while (MISO_IN); 
    CSN_UP; 
}

//*****************************************************************************************
//函数名：void POWER_UP_RESET_CC1100(void) 
//输入：无
//输出：无
//功能描述：上电复位CC1100
//*****************************************************************************************
void POWER_UP_RESET_CC1100(void) 
{
	CSN_UP; 
	halWait(1); 
	CSN_DN; 
	halWait(1); 
	CSN_UP; 
	halWait(41); 
	RESET_CC1100();   		//复位CC1100
}

//*****************************************************************************************
//函数名：void halSpiWriteReg(INT8U addr, INT8U value)
//输入：地址和配置字
//输出：无
//功能描述：SPI写寄存器
//*****************************************************************************************
void halSpiWriteReg(INT8U addr, INT8U value) 
{
    CSN_DN;
    while (MISO_IN);
    SpiTxRxByte(addr);		//写地址
    SpiTxRxByte(value);		//写入配置
    CSN_UP;
}

//*****************************************************************************************
//函数名：void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count)
//输入：地址，写入缓冲区，写入个数
//输出：无
//功能描述：SPI连续写配置寄存器
//*****************************************************************************************
void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count) 
{
    INT8U temp;
    temp = addr | WRITE_BURST;
    CSN_DN;
    while (MISO_IN);
    SpiTxRxByte(temp);
    for (INT8U i = 0; i < count; i++)
    {
        SpiTxRxByte(buffer[i]);
    }
    CSN_UP;
}

//*****************************************************************************************
//函数名：void halSpiStrobe(INT8U strobe)
//输入：命令
//输出：无
//功能描述：SPI写命令
//*****************************************************************************************
void halSpiStrobe(INT8U strobe) 
{
    CSN_DN;
    while (MISO_IN);
    SpiTxRxByte(strobe);		//写入命令
    CSN_UP;
}

//*****************************************************************************************
//函数名：INT8U halSpiReadReg(INT8U addr)
//输入：地址
//输出：该寄存器的配置字
//功能描述：SPI读寄存器
//*****************************************************************************************
INT8U halSpiReadReg(INT8U addr) 
{
	INT8U temp, value;
    temp = addr|READ_SINGLE;//读寄存器命令
	CSN_DN;
	while (MISO_IN);
	SpiTxRxByte(temp);
	value = SpiTxRxByte(0);
	CSN_UP;
	return value;
}


//*****************************************************************************************
//函数名：void halSpiReadBurstReg(INT8U addr, INT8U *buffer, INT8U count)
//输入：地址，读出数据后暂存的缓冲区，读出配置个数
//输出：无
//功能描述：SPI连续写配置寄存器
//*****************************************************************************************
void halSpiReadBurstReg(INT8U addr, INT8U *buffer, INT8U count) 
{
    INT8U i,temp;
    temp = addr | READ_BURST;		//写入要读的配置寄存器地址和读命令
    CSN_DN;
    while (MOSI_IN);
    SpiTxRxByte(temp);   
    for (i = 0; i < count; i++) 
    {
        buffer[i] = SpiTxRxByte(0);
    }
    CSN_UP;
}


//*****************************************************************************************
//函数名：INT8U halSpiReadReg(INT8U addr)
//输入：地址
//输出：该状态寄存器当前值
//功能描述：SPI读状态寄存器
//*****************************************************************************************
INT8U halSpiReadStatus(INT8U addr) 
{
    INT8U value,temp;
    temp = addr | READ_BURST;		//写入要读的状态寄存器的地址同时写入读命令
    CSN_DN;
    while (MISO_IN);
    SpiTxRxByte(temp);
    value = SpiTxRxByte(0);
        
    CSN_UP;
    return value;
}


//*****************************************************************************************
//函数名：void halRfWriteRfSettings(RF_SETTINGS *pRfSettings)
//输入：无
//输出：无
//功能描述：配置CC1100的寄存器
//*****************************************************************************************

void halRfWriteRfSettings(void) 
{

    halSpiWriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL2);//自已加的
    // Write register settings
    halSpiWriteReg(CCxxx0_FSCTRL1,  rfSettings.FSCTRL1);
    halSpiWriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL0);
    halSpiWriteReg(CCxxx0_FREQ2,    rfSettings.FREQ2);
    halSpiWriteReg(CCxxx0_FREQ1,    rfSettings.FREQ1);
    halSpiWriteReg(CCxxx0_FREQ0,    rfSettings.FREQ0);
    halSpiWriteReg(CCxxx0_MDMCFG4,  rfSettings.MDMCFG4);
    halSpiWriteReg(CCxxx0_MDMCFG3,  rfSettings.MDMCFG3);
    halSpiWriteReg(CCxxx0_MDMCFG2,  rfSettings.MDMCFG2);
    halSpiWriteReg(CCxxx0_MDMCFG1,  rfSettings.MDMCFG1);
    halSpiWriteReg(CCxxx0_MDMCFG0,  rfSettings.MDMCFG0);
    halSpiWriteReg(CCxxx0_CHANNR,   rfSettings.CHANNR);
    halSpiWriteReg(CCxxx0_DEVIATN,  rfSettings.DEVIATN);
    halSpiWriteReg(CCxxx0_FREND1,   rfSettings.FREND1);
    halSpiWriteReg(CCxxx0_FREND0,   rfSettings.FREND0);
    halSpiWriteReg(CCxxx0_MCSM0 ,   rfSettings.MCSM0 );
    halSpiWriteReg(CCxxx0_FOCCFG,   rfSettings.FOCCFG);
    halSpiWriteReg(CCxxx0_BSCFG,    rfSettings.BSCFG);
    halSpiWriteReg(CCxxx0_AGCCTRL2, rfSettings.AGCCTRL2);
    halSpiWriteReg(CCxxx0_AGCCTRL1, rfSettings.AGCCTRL1);
    halSpiWriteReg(CCxxx0_AGCCTRL0, rfSettings.AGCCTRL0);
    halSpiWriteReg(CCxxx0_FSCAL3,   rfSettings.FSCAL3);
    halSpiWriteReg(CCxxx0_FSCAL2,   rfSettings.FSCAL2);
    halSpiWriteReg(CCxxx0_FSCAL1,   rfSettings.FSCAL1);
    halSpiWriteReg(CCxxx0_FSCAL0,   rfSettings.FSCAL0);
    halSpiWriteReg(CCxxx0_FSTEST,   rfSettings.FSTEST);
    halSpiWriteReg(CCxxx0_TEST2,    rfSettings.TEST2);
    halSpiWriteReg(CCxxx0_TEST1,    rfSettings.TEST1);
    halSpiWriteReg(CCxxx0_TEST0,    rfSettings.TEST0);
    halSpiWriteReg(CCxxx0_IOCFG2,   rfSettings.IOCFG2);
    halSpiWriteReg(CCxxx0_IOCFG0,   rfSettings.IOCFG0);    
    halSpiWriteReg(CCxxx0_PKTCTRL1, rfSettings.PKTCTRL1);
    halSpiWriteReg(CCxxx0_PKTCTRL0, rfSettings.PKTCTRL0);
    halSpiWriteReg(CCxxx0_ADDR,     rfSettings.ADDR);
    halSpiWriteReg(CCxxx0_PKTLEN,   rfSettings.PKTLEN);
}


//*****************************************************************************************
//函数名：void halRfSendPacket(INT8U *txBuffer, INT8U size)
//输入：发送的缓冲区，发送数据个数
//输出：无
//功能描述：CC1100发送一组数据
//*****************************************************************************************

void halRfSendPacket(INT8U *txBuffer, INT8U size) 
{
    halSpiWriteReg(CCxxx0_TXFIFO, size);
    halSpiWriteBurstReg(CCxxx0_TXFIFO, txBuffer, size);	//写入要发送的数据

    halSpiStrobe(CCxxx0_STX);		//进入发送模式发送数据

    // Wait for GDO0 to be set -> sync transmitted
    while (!(GDO0_IN));
    // Wait for GDO0 to be cleared -> end of packet
    while (GDO0_IN);
    halSpiStrobe(CCxxx0_SFTX);
}


void setRxMode(void)
{
    halSpiStrobe(CCxxx0_SRX);		//进入接收状态
}

INT8U halRfReceivePacket(INT8U *rxBuffer, INT8U *length) 
{
    INT8U status[2];
    INT8U packetLength;
    INT8U i=(*length)*4;  // 具体多少要根据datarate和length来决定

    halSpiStrobe(CCxxx0_SRX);		//进入接收状态
    //delay(5);
    //while (!GDO1);
    //while (GDO1);
    delay(2);
    while (GDO0_IN)
    {
        delay(2);
        --i;
	    if(i<1)  return 0; 	      
    }	 
    if ((halSpiReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)) //如果接的字节数不为0
    {
        packetLength = halSpiReadReg(CCxxx0_RXFIFO);//读出第一个字节，此字节为该帧数据长度
        if (packetLength <= *length) 		//如果所要的有效数据长度小于等于接收到的数据包的长度
	    {
            halSpiReadBurstReg(CCxxx0_RXFIFO, rxBuffer, packetLength); //读出所有接收到的数据
            *length = packetLength;				//把接收数据长度的修改为当前数据的长度
        
            // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
            halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 	//读出CRC校验位
	        halSpiStrobe(CCxxx0_SFRX);		//清洗接收缓冲区
            return (status[1] & CRC_OK);			//如果校验成功返回接收成功
        }
	    else 
        {
            *length = packetLength;
            halSpiStrobe(CCxxx0_SFRX);		//清洗接收缓冲区
            return 0;
        }
    } 
    else
    return 0;
}

void setSleepMpde(void)
{
    CSN_UP ;   //CSn为高时进入功率降低模式
    halSpiStrobe(CCxxx0_SPWD);		//进入功率降低模式
}

void setWorMode(void)
{
    
}

/*************************************************
函数名称 ： CC1101_InitWOR
函数入参 ： void
函数说明： 电磁波WOR唤醒功能初始化 设置为0.5秒轮训
函数备注： WOR 初始化 并进入掉电模式
**************************************************/
INT8U CC1101_InitWOR(void)
{
    // halSpiStrobe(CCxxx0_SIDLE); //进入空闲状态
    halSpiWriteReg(CCxxx0_MCSM2,0x03); //设置轮训时间
    halSpiWriteReg(CCxxx0_MCSM0,0x38); //RC自动校准
    halSpiWriteReg(CCxxx0_WOREVT1,0x43); //设置轮训周期
    halSpiWriteReg(CCxxx0_WOREVT0,0xb5);

    halSpiWriteReg(CCxxx0_WORCTRL,0x78); //启动自动校准 设置时间
    halSpiStrobe(CCxxx0_SFRX); //复位RX——BUFF
    halSpiStrobe(CCxxx0_SWORRST);
    halSpiStrobe(CCxxx0_SWOR); //启动WOR
    return 1;
}

void CC1101_WOR(void)
{
    // halSpiStrobe(CCxxx0_SIDLE); 
    halSpiStrobe(CCxxx0_SFRX); //复位RX——BUFF
    halSpiStrobe(CCxxx0_SWORRST);
    halSpiStrobe(CCxxx0_SWOR); //启动WOR
}

void EINT0_IRQHandler(void)       //中断处理程序  GDO0    设置为06
{
    INT8U i,leng =8;
    INT8U RxBuf[8]={0}; 
    //P3_ISRC = P3_ISRC; 
    if(halRfReceivePacket(RxBuf,&leng) ==0)      //判断接受是否正确
    { 
        for(i=0;i<8;i++)
        RxBuf[i] = 0;
    }
    else                           //接受正确
    {
        //Time_ok = 1;
    }
    CC1101_WOR();      //再次进入掉电状态
    // P3_ISRC = P3_ISRC; //清中断标志 
    // System_Gorun(TEST);
    // System_runfinger(); 
    // EINT0_Off(); 
    //WDT(); 
}