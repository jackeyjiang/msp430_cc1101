

#include <msp430G2553.h>
#include "MSP430PORT_DEF.h"
#include "grace/grace.h"
#include "cc1101/CC1100.h"
#include "bsp/bsp.h"

#define txmode

/*----------------ID,moto1，pwm1(H),pwm1(L),moto2，pwm3(H),pwm3(L),stop---*/
INT8U  TxBuf[8]= {0x08,0X00,0X00,0X00,0X00,0X00,0X00,0X00};	  //8字节长发送的数据包
INT8U  RxBuf[8]= {0};   //8字节长接收的数据包
INT8U  ActBuf[8]= {0X08,'A','C','K','\r','\n'};  //8字节的回复响应
INT16U PwmWith[21]={  0, 25, 50, 75,100,125,150,175,200,225,
                    250,275,300,325,350,375,400,425,450,475,500};   //21个脉冲宽度
INT8U  PKLEN= 0;        //测试配置是否成功用
INT16U pwm1st_wid= 0;      //脉宽（接收用）
INT16U pwm2nd_wid= 0;      //脉宽（接收用）
INT8U  PwmWidthNum=0;       //脉宽指数I

bool  StopFlag= false;      //暂停标记位
bool  StopFlagOld= false;   //用来记录标记位的变化
bool  Moto1stChange= false;  //电机换向标记
bool  Moto2ndChange= false;  //电机换向标记

/*======== main ========*/
int main(void)
{
    INT8U i=0;
	Grace_init();   //图形化配置选项
    SpiInit();      //cc1101 SPI接口初始化
	POWER_UP_RESET_CC1100(); //上电复位CC1101
	halRfWriteRfSettings();  //配置CC1101的寄存器
    PKLEN= halSpiReadReg(CCxxx0_PKTLEN);  //测试配置是否成功
	halSpiWriteBurstReg(CCxxx0_PATABLE, PaTabel, 8); //配置天线增益
#ifdef txmode  
   setSleepMpde(); //设置CC1101进入低功耗状态
   while(1) //发送端
    {
        __bis_SR_register(LPM0_bits+GIE);  // Enter LPM0
        PWM_set(moto_1st,PwmWith[PwmWidthNum]);   //设置本机的PWM输出 
        TxBuf[1]= Moto1stChange; Moto1stChange=false;
        TxBuf[2]= (PwmWith[PwmWidthNum]>>8)&0x00FF;
        TxBuf[3]=  PwmWith[PwmWidthNum]&0x00FF; 
        TxBuf[4]= Moto2ndChange; Moto2ndChange=false;
        TxBuf[5]= (PwmWith[PwmWidthNum]>>8)&0x00FF;
        TxBuf[6]=  PwmWith[PwmWidthNum]&0x00FF; 
        TxBuf[7]=  StopFlag; StopFlag=false;   //发送一次切换一次
        halRfSendPacket(TxBuf,8);// 传输TxBuf指向的数据
        for(i=1;i<8;i++) TxBuf[i]=0;  //保留TxBuf[0]作为ID号
        setSleepMpde(); //设置CC1101进入低功耗状态    
    }
#else
    setRxMode();//设置CC1101进入接收模式
    while(1) //接收端
    {
  	    if(halRfReceivePacket(RxBuf,&leng))
		{
            //获取LED的状态
            if(RxBuf[0]!=0x08) continue; //如果数据出错，继续读取数据帧
             //获取电机1时候需要转换方向
            if(RxBuf[1]==0X01) MOTOD1_TG; //为‘0x01时改变电机方向’  
            //获取电机1的PWM值,并设置timer输出
            pwm1st_wid=RxBuf[2]<<8;
            pwm1st_wid=RxBuf[3]+pwm1st_wid;
            PWM_set(moto_1st,pwm1st_wid);
             //获取电机2时候需要转换方向
            if(RxBuf[4]==0X01) MOTOD2_TG; //为‘0x01时改变电机方向’  
            //获取电机2的PWM值,并设置timer输出
            pwm2nd_wid=RxBuf[5]<<8;
            pwm2nd_wid=RxBuf[6]+pwm2nd_wid;
            PWM_set(moto_2nd,pwm2nd_wid);
            //获取暂停位
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
            halRfSendPacket(ActBuf,8); //加入回复ACK，这样可以测试双向通信，及距离
            //可以测试一下WOR电磁波唤醒。
		}
	}
#endif
}

