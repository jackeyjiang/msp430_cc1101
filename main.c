

#include <msp430G2553.h>
#include "MSP430PORT_DEF.h"
#include "grace/grace.h"
#include "cc1101/CC1100.h"
#include "bsp/bsp.h"

#define txmode

/*----------------ID,moto1��pwm1(H),pwm1(L),moto2��pwm3(H),pwm3(L),stop---*/
INT8U  TxBuf[8]= {0x08,0X00,0X00,0X00,0X00,0X00,0X00,0X00};	  //8�ֽڳ����͵����ݰ�
INT8U  RxBuf[8]= {0};   //8�ֽڳ����յ����ݰ�
INT8U  ActBuf[8]= {0X08,'A','C','K','\r','\n'};  //8�ֽڵĻظ���Ӧ
INT16U PwmWith[21]={  0, 25, 50, 75,100,125,150,175,200,225,
                    250,275,300,325,350,375,400,425,450,475,500};   //21��������
INT8U  PKLEN= 0;        //���������Ƿ�ɹ���
INT16U pwm1st_wid= 0;      //���������ã�
INT16U pwm2nd_wid= 0;      //���������ã�
INT8U  PwmWidthNum=0;       //����ָ��I

bool  StopFlag= false;      //��ͣ���λ
bool  StopFlagOld= false;   //������¼���λ�ı仯
bool  Moto1stChange= false;  //���������
bool  Moto2ndChange= false;  //���������

/*======== main ========*/
int main(void)
{
    INT8U i=0;
	Grace_init();   //ͼ�λ�����ѡ��
    SpiInit();      //cc1101 SPI�ӿڳ�ʼ��
	POWER_UP_RESET_CC1100(); //�ϵ縴λCC1101
	halRfWriteRfSettings();  //����CC1101�ļĴ���
    PKLEN= halSpiReadReg(CCxxx0_PKTLEN);  //���������Ƿ�ɹ�
	halSpiWriteBurstReg(CCxxx0_PATABLE, PaTabel, 8); //������������
#ifdef txmode  
   setSleepMpde(); //����CC1101����͹���״̬
   while(1) //���Ͷ�
    {
        __bis_SR_register(LPM0_bits+GIE);  // Enter LPM0
        PWM_set(moto_1st,PwmWith[PwmWidthNum]);   //���ñ�����PWM��� 
        TxBuf[1]= Moto1stChange; Moto1stChange=false;
        TxBuf[2]= (PwmWith[PwmWidthNum]>>8)&0x00FF;
        TxBuf[3]=  PwmWith[PwmWidthNum]&0x00FF; 
        TxBuf[4]= Moto2ndChange; Moto2ndChange=false;
        TxBuf[5]= (PwmWith[PwmWidthNum]>>8)&0x00FF;
        TxBuf[6]=  PwmWith[PwmWidthNum]&0x00FF; 
        TxBuf[7]=  StopFlag; StopFlag=false;   //����һ���л�һ��
        halRfSendPacket(TxBuf,8);// ����TxBufָ�������
        for(i=1;i<8;i++) TxBuf[i]=0;  //����TxBuf[0]��ΪID��
        setSleepMpde(); //����CC1101����͹���״̬    
    }
#else
    setRxMode();//����CC1101�������ģʽ
    while(1) //���ն�
    {
  	    if(halRfReceivePacket(RxBuf,&leng))
		{
            //��ȡLED��״̬
            if(RxBuf[0]!=0x08) continue; //������ݳ���������ȡ����֡
             //��ȡ���1ʱ����Ҫת������
            if(RxBuf[1]==0X01) MOTOD1_TG; //Ϊ��0x01ʱ�ı�������  
            //��ȡ���1��PWMֵ,������timer���
            pwm1st_wid=RxBuf[2]<<8;
            pwm1st_wid=RxBuf[3]+pwm1st_wid;
            PWM_set(moto_1st,pwm1st_wid);
             //��ȡ���2ʱ����Ҫת������
            if(RxBuf[4]==0X01) MOTOD2_TG; //Ϊ��0x01ʱ�ı�������  
            //��ȡ���2��PWMֵ,������timer���
            pwm2nd_wid=RxBuf[5]<<8;
            pwm2nd_wid=RxBuf[6]+pwm2nd_wid;
            PWM_set(moto_2nd,pwm2nd_wid);
            //��ȡ��ͣλ
            if(RxBuf[7]==0X01)
            {
              StopFlag=!StopFlag;
              if(StopFlag)
              {
                  PWM_set(moto_1st,0);
                  PWM_set(moto_2nd,0);
                  StopFlagOld= StopFlag;
              }
              else
              {
                  PWM_set(moto_1st,pwm1st_wid);
                  PWM_set(moto_2nd,pwm2nd_wid);
              }   
            }
            for(i=0;i<8;i++) RxBuf[i]=0; 
            halRfSendPacket(ActBuf,8); //����ظ�ACK���������Բ���˫��ͨ�ţ�������
            //���Բ���һ��WOR��Ų����ѡ�
		}
	}
#endif
}

