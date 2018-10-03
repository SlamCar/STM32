#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "ltdc.h"
#include "sdram.h"
#include "includes.h"
#include "my_function.h"
#include "heart_beat.h"
#include "crc.h"
#include "re_an.h"
#include "feed_back.h"
#include "data.h"
#include "dma.h"
#include "mpu.h"
#include "mpu9250.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 



//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С	
#define START_STK_SIZE 		128
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);

//�������ȼ�
#define HEART_BEAT_TASK_PRIO		5
//�����ջ��С	
#define HEART_BEAT_STK_SIZE 		128
//������ƿ�
OS_TCB HEART_BEAT_TaskTCB;
//�����ջ	
CPU_STK HEART_BEAT_TASK_STK[HEART_BEAT_STK_SIZE];

void heart_beat_task(void *p_arg);

//�������ȼ�
#define RECEIVE_TASK_PRIO		4
//�����ջ��С	
#define RECEIVE_STK_SIZE 		128
//������ƿ�
OS_TCB RECEIVE_TaskTCB;
//�����ջ	
CPU_STK RECEIVE_TASK_STK[RECEIVE_STK_SIZE];
void receive_task(void *p_arg);

//�������ȼ�
#define MPU9250_TASK_PRIO		4
//�����ջ��С	
#define MPU9250_STK_SIZE 		256
//������ƿ�
OS_TCB MPU9250_TaskTCB;
//�����ջ	
CPU_STK MPU9250_TASK_STK[MPU9250_STK_SIZE];
void mpu9250_task(void *p_arg);

//�������ȼ�
#define FEED_BACK_TASK_PRIO		4
//�����ջ��С	
#define FEED_BACK_STK_SIZE 		128
//������ƿ�
OS_TCB FEED_BACK_TaskTCB;
//�����ջ	
CPU_STK FEED_BACK_TASK_STK[FEED_BACK_STK_SIZE];
void feed_back_task(void *p_arg);



////////////////////////////////////////////////////////
OS_TMR 	tmr1;		//��ʱ��1
OS_TMR	tmr2;		//��ʱ��2
void tmr1_callback(void *p_tmr, void *p_arg); 	//��ʱ��1�ص�����
void tmr2_callback(void *p_tmr, void *p_arg);	//��ʱ��2�ص�����



////////////////////////////////////////////////////////

void usart1_send_char(u8 c);
void usart1_niming_report(u8 fun,u8*data,u8 len);
void mpu9250_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz);
void usart1_report_imu(short roll,short pitch,short yaw,short csb,int prs);


////////////////////////////////////////////////////////
//LCDˢ��ʱʹ�õ���ɫ
int lcd_discolor[14]={	WHITE, RED,   BLUE,  BRED,      
						GRED,  GBLUE, BLACK,   MAGENTA,       	 
						GREEN, CYAN,  YELLOW,BROWN, 			
						BRRED, GRAY };


u16 heart_beat[57]={0x039c,0x6010,0x000e};
u16 feed_back[57];

extern struct heart_times h_times;
extern struct _data_package data_package;

#define SEND_BUF_SIZE 7800	//�������ݳ���,��õ���sizeof(TEXT_TO_SEND)+2��������.

u8 SendBuff[SEND_BUF_SIZE]; //�������ݻ�����
const u8 TEXT_TO_SEND[]={"ALIENTEK Apollo STM32F7 DMA ����ʵ��"}; 



int main(void)
{
    OS_ERR err;
	CPU_SR_ALLOC();
    
    Write_Through();                //͸д
	MPU_Memory_Protection();		//������ش洢����
    Cache_Enable();                 //��L1-Cache
    HAL_Init();				        //��ʼ��HAL��
    Stm32_Clock_Init(432,25,2,9);   //����ʱ��,216Mhz 
    delay_init(216);                //��ʱ��ʼ��
	uart_init(115200);		        //���ڳ�ʼ��
    LED_Init();                     //��ʼ��LED
    KEY_Init();                     //��ʼ������
    SDRAM_Init();                   //��ʼ��SDRAM
	LCD_Init();			            //��ʼ��LCD
	MPU9250_Init();             	//��ʼ��MPU9250
	MYDMA_Config(DMA2_Stream7,DMA_CHANNEL_4);//��ʼ��DMA
	

	
	POINT_COLOR=RED;
	LCD_ShowString(30,50,200,16,16,"Apollo STM32F4/F7"); 
	LCD_ShowString(30,70,200,16,16,"MPU9250 TEST");	
	LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
	LCD_ShowString(30,110,200,16,16,"2016/7/19");	 		
 	 
    while(mpu_dmp_init())         
    {   
		LCD_ShowString(30,130,200,16,16,"MPU9250 Error");
		delay_ms(200);
		LCD_Fill(30,130,239,130+16,WHITE);
 		delay_ms(200);
		LED0_Toggle;//DS0��˸ 
    }
	
    LCD_ShowString(30,130,200,16,16,"MPU9250 OK");
	LCD_ShowString(30,150,200,16,16,"KEY0:UPLOAD ON/OFF");
    POINT_COLOR=BLUE;     //��������Ϊ��ɫ
    LCD_ShowString(30,170,200,16,16,"UPLOAD ON ");	 
 	LCD_ShowString(30,200,200,16,16," Temp:    . C");	
 	LCD_ShowString(30,220,200,16,16,"Pitch:    . C");	
 	LCD_ShowString(30,240,200,16,16," Roll:    . C");	 
 	LCD_ShowString(30,260,200,16,16," Yaw :    . C");
	
	
	
	OSInit(&err);		    	        //��ʼ��UCOSIII
	OS_CRITICAL_ENTER();	            //�����ٽ���	
	
	//������ʼ����
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, //����ѡ��,Ϊ�˱���������������񶼱��渡��Ĵ�����ֵ
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ 
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);      //����UCOSIII
    while(1)
    {
	} 
}

//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	  //ʹ��ʱ��Ƭ��ת���ȹ���,����Ĭ�ϵ�ʱ��Ƭ����
	OSSchedRoundRobinCfg(DEF_ENABLED,10,&err);  
#endif	
	
	//������ʱ��1
	OSTmrCreate((OS_TMR		*)&tmr1,		//��ʱ��1
                (CPU_CHAR	*)"tmr1",		//��ʱ������
                (OS_TICK	 )20,			//20*10=200ms
                (OS_TICK	 )100,          //100*10=1000ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //����ģʽ
                (OS_TMR_CALLBACK_PTR)tmr1_callback,//��ʱ��1�ص�����
                (void	    *)0,			//����Ϊ0
                (OS_ERR	    *)&err);		//���صĴ�����					
	//������ʱ��2
	OSTmrCreate((OS_TMR		*)&tmr2,		
                (CPU_CHAR	*)"tmr2",		
                (OS_TICK	 )200,			//200*10=2000ms	
                (OS_TICK	 )0,   					
                (OS_OPT		 )OS_OPT_TMR_ONE_SHOT, 	//���ζ�ʱ��
                (OS_TMR_CALLBACK_PTR)tmr2_callback,	//��ʱ��2�ص�����
                (void	    *)0,			
                (OS_ERR	    *)&err);	
	OS_CRITICAL_ENTER();	//�����ٽ���
	//����TASK1����
	OSTaskCreate((OS_TCB 	* )&HEART_BEAT_TaskTCB,		
				 (CPU_CHAR	* )"Heat_Beat task", 		
                 (OS_TASK_PTR )heart_beat_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )HEART_BEAT_TASK_PRIO,     
                 (CPU_STK   * )&HEART_BEAT_TASK_STK[0],	
                 (CPU_STK_SIZE)HEART_BEAT_STK_SIZE/10,	
                 (CPU_STK_SIZE)HEART_BEAT_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,  					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP,
                 (OS_ERR 	* )&err);		
    //����TASK2����
	OSTaskCreate((OS_TCB 	* )&RECEIVE_TaskTCB,		
				 (CPU_CHAR	* )"receive task", 		
                 (OS_TASK_PTR )receive_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )RECEIVE_TASK_PRIO,     	
                 (CPU_STK   * )&RECEIVE_TASK_STK[0],	
                 (CPU_STK_SIZE)RECEIVE_STK_SIZE/10,	
                 (CPU_STK_SIZE)RECEIVE_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
                 (OS_ERR 	* )&err);	
//����TASK3����
	OSTaskCreate((OS_TCB 	* )&MPU9250_TaskTCB,		
				 (CPU_CHAR	* )"mpu9250 task", 		
                 (OS_TASK_PTR )mpu9250_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )MPU9250_TASK_PRIO,     	
                 (CPU_STK   * )&MPU9250_TASK_STK[0],	
                 (CPU_STK_SIZE)MPU9250_STK_SIZE/10,	
                 (CPU_STK_SIZE)MPU9250_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
                 (OS_ERR 	* )&err);	
//����TASK4����
	OSTaskCreate((OS_TCB 	* )&FEED_BACK_TaskTCB,		
				 (CPU_CHAR	* )"feed_back task", 		
                 (OS_TASK_PTR )feed_back_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )FEED_BACK_TASK_PRIO,     	
                 (CPU_STK   * )&FEED_BACK_TASK_STK[0],	
                 (CPU_STK_SIZE)FEED_BACK_STK_SIZE/10,	
                 (CPU_STK_SIZE)FEED_BACK_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, 
                 (OS_ERR 	* )&err);			 
	OS_CRITICAL_EXIT();	//�˳��ٽ���
	OSTaskDel((OS_TCB*)0,&err);	//ɾ��start_task��������
}

//����1��������
void heart_beat_task(void *p_arg)
{

	
	OS_ERR err;
	
	OSTmrStart(&tmr1,&err);	//������ʱ��1
	
	heart_beat[5]=h_times.day=2;
	heart_beat[4]=h_times.month=10;
	heart_beat[3]=h_times.year=2018;
	
	
	while(1)
	{	
		heart_run();
		
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);   //��ʱ10ms
	}
}


void receive_task(void *p_arg)
{
    OS_ERR err;
    	 
	data_package.head=0x039c;
 
	
	
	while(1)
	{
				   
			re_an();  //receive and analyze
            data_write();   
			OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);   //��ʱ10ms

		
	}

}



void mpu9250_task(void *p_arg)
{
     OS_ERR err;
	
//	OSTaskResume((OS_TCB*)0,&err);
	

	
	while(1)
	{
	      
		mpu9250_run();
	    OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);   //��ʱ10ms
	}
}


void feed_back_task(void *p_arg)
{
    OS_ERR err;
	
	while(1)
	{
	      
		feed_back_send();
	    OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);   //��ʱ10ms
	}
}


//��ʱ��1�Ļص�����
void tmr1_callback(void *p_tmr, void *p_arg)
{
	
	h_times.second++;
	
	heart_beat[8]=h_times.second;
	
	
	if(h_times.second==30) 
	{	
        heart_beat[28]=CRC16(heart_beat,56);
	    HAL_UART_Transmit(&UART1_Handler,(uint8_t*)heart_beat,58,1000);
	}
	
	
//	HAL_UART_Transmit(&UART1_Handler,(uint8_t*)heart_beat,58,1000);
	
	
//	for(i=0;i<58;i++) 
//	{
//       printf("%d",heart_beat[i]);
//	}
//    printf("\r\n");
//	HAL_UART_Transmit(&UART1_Handler,(uint8_t*)heart_beat,58,1000);
}

//��ʱ��2�Ļص�����
void tmr2_callback(void *p_tmr,void *p_arg)
{
	static u8 tmr2_num = 0;
	tmr2_num++;		//��ʱ��2ִ�д�����1
	LCD_ShowxNum(182,111,tmr2_num,3,16,0x80);  //��ʾ��ʱ��1ִ�д���
	LCD_Fill(126,131,233,313,lcd_discolor[tmr2_num%14]); //�������
	LED1_Toggle;
	printf("��ʱ��2���н���\r\n");
}





//����1����1���ַ� 
//c:Ҫ���͵��ַ�
void usart1_send_char(u8 c)
{
	while((USART1->ISR&0X40)==0);	//ѭ������,ֱ���������   
    USART1->TDR=c;  
} 
//�������ݸ������������վ(V4�汾)
//fun:������. 0X01~0X1C
//data:���ݻ�����,���28�ֽ�!!
//len:data����Ч���ݸ���
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//���28�ֽ����� 
	send_buf[len+3]=0;	//У��������
	send_buf[0]=0XAA;	//֡ͷ
	send_buf[1]=0XAA;	//֡ͷ
	send_buf[2]=fun;	//������
	send_buf[3]=len;	//���ݳ���
	for(i=0;i<len;i++)send_buf[4+i]=data[i];			//��������
	for(i=0;i<len+4;i++)send_buf[len+4]+=send_buf[i];	//����У���	
	for(i=0;i<len+5;i++)usart1_send_char(send_buf[i]);	//�������ݵ�����1 
}
//���ͼ��ٶȴ���������+����������(������֡)
//aacx,aacy,aacz:x,y,z������������ļ��ٶ�ֵ
//gyrox,gyroy,gyroz:x,y,z�������������������ֵ 
void mpu9250_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz)
{
	u8 tbuf[18]; 
	tbuf[0]=(aacx>>8)&0XFF;
	tbuf[1]=aacx&0XFF;
	tbuf[2]=(aacy>>8)&0XFF;
	tbuf[3]=aacy&0XFF;
	tbuf[4]=(aacz>>8)&0XFF;
	tbuf[5]=aacz&0XFF; 
	tbuf[6]=(gyrox>>8)&0XFF;
	tbuf[7]=gyrox&0XFF;
	tbuf[8]=(gyroy>>8)&0XFF;
	tbuf[9]=gyroy&0XFF;
	tbuf[10]=(gyroz>>8)&0XFF;
	tbuf[11]=gyroz&0XFF;
	tbuf[12]=0;//��Ϊ����MPL��,�޷�ֱ�Ӷ�ȡ����������,��������ֱ�����ε�.��0���.
	tbuf[13]=0;
	tbuf[14]=0;
	tbuf[15]=0;
	tbuf[16]=0;
	tbuf[17]=0;
	usart1_niming_report(0X02,tbuf,18);//������֡,0X02
}	
//ͨ������1�ϱ���������̬���ݸ�����(״̬֡)
//roll:�����.��λ0.01�ȡ� -18000 -> 18000 ��Ӧ -180.00  ->  180.00��
//pitch:������.��λ 0.01�ȡ�-9000 - 9000 ��Ӧ -90.00 -> 90.00 ��
//yaw:�����.��λΪ0.1�� 0 -> 3600  ��Ӧ 0 -> 360.0��
//csb:�������߶�,��λ:cm
//prs:��ѹ�Ƹ߶�,��λ:mm
void usart1_report_imu(short roll,short pitch,short yaw,short csb,int prs)
{
	u8 tbuf[12];   	
	tbuf[0]=(roll>>8)&0XFF;
	tbuf[1]=roll&0XFF;
	tbuf[2]=(pitch>>8)&0XFF;
	tbuf[3]=pitch&0XFF;
	tbuf[4]=(yaw>>8)&0XFF;
	tbuf[5]=yaw&0XFF;
	tbuf[6]=(csb>>8)&0XFF;
	tbuf[7]=csb&0XFF;
	tbuf[8]=(prs>>24)&0XFF;
	tbuf[9]=(prs>>16)&0XFF;
	tbuf[10]=(prs>>8)&0XFF;
	tbuf[11]=prs&0XFF;
	usart1_niming_report(0X01,tbuf,12);//״̬֡,0X01
}  

