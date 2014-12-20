//
//CC1100.h
//
// Copyright (c) NewMSG. All rights reserved. 2007
// purpose : Test CC1100
#include "CC1100.h"
#include "../MSP430PORT_DEF.h"


#define 	WRITE_BURST     	    0x40						//����д��
#define 	READ_SINGLE     	    0x80						//��
#define 	READ_BURST      	    0xC0						//������
#define 	BYTES_IN_RXFIFO         0x7F  						//���ջ���������Ч�ֽ���
#define 	CRC_OK                  0x80 						//CRCУ��ͨ��λ��־

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
//��������delay(unsigned int s)
//���룺ʱ��
//�������
//������������ͨ͢ʱ,�ڲ���
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
//��������SpisendByte(INT8U dat)
//���룺���͵�����
//�������
//����������SPI����һ���ֽ�
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
//��������void RESET_CC1100(void)
//���룺��
//�������
//������������λCC1100
//*****************************************************************************************
void RESET_CC1100(void) 
{
	CSN_DN; 
	while (MISO_IN);  
    SpiTxRxByte(CCxxx0_SRES); 		//д�븴λ����
	while (MISO_IN); 
    CSN_UP; 
}

//*****************************************************************************************
//��������void POWER_UP_RESET_CC1100(void) 
//���룺��
//�������
//�����������ϵ縴λCC1100
//*****************************************************************************************
void POWER_UP_RESET_CC1100(void) 
{
	CSN_UP; 
	halWait(1); 
	CSN_DN; 
	halWait(1); 
	CSN_UP; 
	halWait(41); 
	RESET_CC1100();   		//��λCC1100
}

//*****************************************************************************************
//��������void halSpiWriteReg(INT8U addr, INT8U value)
//���룺��ַ��������
//�������
//����������SPIд�Ĵ���
//*****************************************************************************************
void halSpiWriteReg(INT8U addr, INT8U value) 
{
    CSN_DN;
    while (MISO_IN);
    SpiTxRxByte(addr);		//д��ַ
    SpiTxRxByte(value);		//д������
    CSN_UP;
}

//*****************************************************************************************
//��������void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count)
//���룺��ַ��д�뻺������д�����
//�������
//����������SPI����д���üĴ���
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
//��������void halSpiStrobe(INT8U strobe)
//���룺����
//�������
//����������SPIд����
//*****************************************************************************************
void halSpiStrobe(INT8U strobe) 
{
    CSN_DN;
    while (MISO_IN);
    SpiTxRxByte(strobe);		//д������
    CSN_UP;
}

//*****************************************************************************************
//��������INT8U halSpiReadReg(INT8U addr)
//���룺��ַ
//������üĴ�����������
//����������SPI���Ĵ���
//*****************************************************************************************
INT8U halSpiReadReg(INT8U addr) 
{
	INT8U temp, value;
    temp = addr|READ_SINGLE;//���Ĵ�������
	CSN_DN;
	while (MISO_IN);
	SpiTxRxByte(temp);
	value = SpiTxRxByte(0);
	CSN_UP;
	return value;
}


//*****************************************************************************************
//��������void halSpiReadBurstReg(INT8U addr, INT8U *buffer, INT8U count)
//���룺��ַ���������ݺ��ݴ�Ļ��������������ø���
//�������
//����������SPI����д���üĴ���
//*****************************************************************************************
void halSpiReadBurstReg(INT8U addr, INT8U *buffer, INT8U count) 
{
    INT8U i,temp;
    temp = addr | READ_BURST;		//д��Ҫ�������üĴ�����ַ�Ͷ�����
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
//��������INT8U halSpiReadReg(INT8U addr)
//���룺��ַ
//�������״̬�Ĵ�����ǰֵ
//����������SPI��״̬�Ĵ���
//*****************************************************************************************
INT8U halSpiReadStatus(INT8U addr) 
{
    INT8U value,temp;
    temp = addr | READ_BURST;		//д��Ҫ����״̬�Ĵ����ĵ�ַͬʱд�������
    CSN_DN;
    while (MISO_IN);
    SpiTxRxByte(temp);
    value = SpiTxRxByte(0);
        
    CSN_UP;
    return value;
}


//*****************************************************************************************
//��������void halRfWriteRfSettings(RF_SETTINGS *pRfSettings)
//���룺��
//�������
//��������������CC1100�ļĴ���
//*****************************************************************************************

void halRfWriteRfSettings(void) 
{

    halSpiWriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL2);//���Ѽӵ�
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
//��������void halRfSendPacket(INT8U *txBuffer, INT8U size)
//���룺���͵Ļ��������������ݸ���
//�������
//����������CC1100����һ������
//*****************************************************************************************

void halRfSendPacket(INT8U *txBuffer, INT8U size) 
{
    halSpiWriteReg(CCxxx0_TXFIFO, size);
    halSpiWriteBurstReg(CCxxx0_TXFIFO, txBuffer, size);	//д��Ҫ���͵�����

    halSpiStrobe(CCxxx0_STX);		//���뷢��ģʽ��������

    // Wait for GDO0 to be set -> sync transmitted
    while (!(GDO0_IN));
    // Wait for GDO0 to be cleared -> end of packet
    while (GDO0_IN);
    halSpiStrobe(CCxxx0_SFTX);
}


void setRxMode(void)
{
    halSpiStrobe(CCxxx0_SRX);		//�������״̬
}

INT8U halRfReceivePacket(INT8U *rxBuffer, INT8U *length) 
{
    INT8U status[2];
    INT8U packetLength;
    INT8U i=(*length)*4;  // �������Ҫ����datarate��length������

    halSpiStrobe(CCxxx0_SRX);		//�������״̬
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
    if ((halSpiReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)) //����ӵ��ֽ�����Ϊ0
    {
        packetLength = halSpiReadReg(CCxxx0_RXFIFO);//������һ���ֽڣ����ֽ�Ϊ��֡���ݳ���
        if (packetLength <= *length) 		//�����Ҫ����Ч���ݳ���С�ڵ��ڽ��յ������ݰ��ĳ���
	    {
            halSpiReadBurstReg(CCxxx0_RXFIFO, rxBuffer, packetLength); //�������н��յ�������
            *length = packetLength;				//�ѽ������ݳ��ȵ��޸�Ϊ��ǰ���ݵĳ���
        
            // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
            halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 	//����CRCУ��λ
	        halSpiStrobe(CCxxx0_SFRX);		//��ϴ���ջ�����
            return (status[1] & CRC_OK);			//���У��ɹ����ؽ��ճɹ�
        }
	    else 
        {
            *length = packetLength;
            halSpiStrobe(CCxxx0_SFRX);		//��ϴ���ջ�����
            return 0;
        }
    } 
    else
    return 0;
}

void setSleepMpde(void)
{
    CSN_UP ;   //CSnΪ��ʱ���빦�ʽ���ģʽ
    halSpiStrobe(CCxxx0_SPWD);		//���빦�ʽ���ģʽ
}

void setWorMode(void)
{
    
}

/*************************************************
�������� �� CC1101_InitWOR
������� �� void
����˵���� ��Ų�WOR���ѹ��ܳ�ʼ�� ����Ϊ0.5����ѵ
������ע�� WOR ��ʼ�� ���������ģʽ
**************************************************/
INT8U CC1101_InitWOR(void)
{
    // halSpiStrobe(CCxxx0_SIDLE); //�������״̬
    halSpiWriteReg(CCxxx0_MCSM2,0x03); //������ѵʱ��
    halSpiWriteReg(CCxxx0_MCSM0,0x38); //RC�Զ�У׼
    halSpiWriteReg(CCxxx0_WOREVT1,0x43); //������ѵ����
    halSpiWriteReg(CCxxx0_WOREVT0,0xb5);

    halSpiWriteReg(CCxxx0_WORCTRL,0x78); //�����Զ�У׼ ����ʱ��
    halSpiStrobe(CCxxx0_SFRX); //��λRX����BUFF
    halSpiStrobe(CCxxx0_SWORRST);
    halSpiStrobe(CCxxx0_SWOR); //����WOR
    return 1;
}

void CC1101_WOR(void)
{
    // halSpiStrobe(CCxxx0_SIDLE); 
    halSpiStrobe(CCxxx0_SFRX); //��λRX����BUFF
    halSpiStrobe(CCxxx0_SWORRST);
    halSpiStrobe(CCxxx0_SWOR); //����WOR
}

void EINT0_IRQHandler(void)       //�жϴ������  GDO0    ����Ϊ06
{
    INT8U i,leng =8;
    INT8U RxBuf[8]={0}; 
    //P3_ISRC = P3_ISRC; 
    if(halRfReceivePacket(RxBuf,&leng) ==0)      //�жϽ����Ƿ���ȷ
    { 
        for(i=0;i<8;i++)
        RxBuf[i] = 0;
    }
    else                           //������ȷ
    {
        //Time_ok = 1;
    }
    CC1101_WOR();      //�ٴν������״̬
    // P3_ISRC = P3_ISRC; //���жϱ�־ 
    // System_Gorun(TEST);
    // System_runfinger(); 
    // EINT0_Off(); 
    //WDT(); 
}