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



//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		128
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);

//任务优先级
#define HEART_BEAT_TASK_PRIO		5
//任务堆栈大小	
#define HEART_BEAT_STK_SIZE 		128
//任务控制块
OS_TCB HEART_BEAT_TaskTCB;
//任务堆栈	
CPU_STK HEART_BEAT_TASK_STK[HEART_BEAT_STK_SIZE];

void heart_beat_task(void *p_arg);

//任务优先级
#define RECEIVE_TASK_PRIO		4
//任务堆栈大小	
#define RECEIVE_STK_SIZE 		128
//任务控制块
OS_TCB RECEIVE_TaskTCB;
//任务堆栈	
CPU_STK RECEIVE_TASK_STK[RECEIVE_STK_SIZE];
void receive_task(void *p_arg);

//任务优先级
#define MPU9250_TASK_PRIO		4
//任务堆栈大小	
#define MPU9250_STK_SIZE 		256
//任务控制块
OS_TCB MPU9250_TaskTCB;
//任务堆栈	
CPU_STK MPU9250_TASK_STK[MPU9250_STK_SIZE];
void mpu9250_task(void *p_arg);

//任务优先级
#define FEED_BACK_TASK_PRIO		4
//任务堆栈大小	
#define FEED_BACK_STK_SIZE 		128
//任务控制块
OS_TCB FEED_BACK_TaskTCB;
//任务堆栈	
CPU_STK FEED_BACK_TASK_STK[FEED_BACK_STK_SIZE];
void feed_back_task(void *p_arg);



////////////////////////////////////////////////////////
OS_TMR 	tmr1;		//定时器1
OS_TMR	tmr2;		//定时器2
void tmr1_callback(void *p_tmr, void *p_arg); 	//定时器1回调函数
void tmr2_callback(void *p_tmr, void *p_arg);	//定时器2回调函数



////////////////////////////////////////////////////////

void usart1_send_char(u8 c);
void usart1_niming_report(u8 fun,u8*data,u8 len);
void mpu9250_send_data(short aacx,short aacy,short aacz,short gyrox,short gyroy,short gyroz);
void usart1_report_imu(short roll,short pitch,short yaw,short csb,int prs);


////////////////////////////////////////////////////////
//LCD刷屏时使用的颜色
int lcd_discolor[14]={	WHITE, RED,   BLUE,  BRED,      
						GRED,  GBLUE, BLACK,   MAGENTA,       	 
						GREEN, CYAN,  YELLOW,BROWN, 			
						BRRED, GRAY };


u16 heart_beat[57]={0x039c,0x6010,0x000e};
u16 feed_back[57];

extern struct heart_times h_times;
extern struct _data_package data_package;

#define SEND_BUF_SIZE 7800	//发送数据长度,最好等于sizeof(TEXT_TO_SEND)+2的整数倍.

u8 SendBuff[SEND_BUF_SIZE]; //发送数据缓冲区
const u8 TEXT_TO_SEND[]={"ALIENTEK Apollo STM32F7 DMA 串口实验"}; 



int main(void)
{
    OS_ERR err;
	CPU_SR_ALLOC();
    
    Write_Through();                //透写
	MPU_Memory_Protection();		//保护相关存储区域
    Cache_Enable();                 //打开L1-Cache
    HAL_Init();				        //初始化HAL库
    Stm32_Clock_Init(432,25,2,9);   //设置时钟,216Mhz 
    delay_init(216);                //延时初始化
	uart_init(115200);		        //串口初始化
    LED_Init();                     //初始化LED
    KEY_Init();                     //初始化按键
    SDRAM_Init();                   //初始化SDRAM
	LCD_Init();			            //初始化LCD
	MPU9250_Init();             	//初始化MPU9250
	MYDMA_Config(DMA2_Stream7,DMA_CHANNEL_4);//初始化DMA
	

	
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
		LED0_Toggle;//DS0闪烁 
    }
	
    LCD_ShowString(30,130,200,16,16,"MPU9250 OK");
	LCD_ShowString(30,150,200,16,16,"KEY0:UPLOAD ON/OFF");
    POINT_COLOR=BLUE;     //设置字体为蓝色
    LCD_ShowString(30,170,200,16,16,"UPLOAD ON ");	 
 	LCD_ShowString(30,200,200,16,16," Temp:    . C");	
 	LCD_ShowString(30,220,200,16,16,"Pitch:    . C");	
 	LCD_ShowString(30,240,200,16,16," Roll:    . C");	 
 	LCD_ShowString(30,260,200,16,16," Yaw :    . C");
	
	
	
	OSInit(&err);		    	        //初始化UCOSIII
	OS_CRITICAL_ENTER();	            //进入临界区	
	
	//创建开始任务
	OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR|OS_OPT_TASK_SAVE_FP, //任务选项,为了保险起见，所有任务都保存浮点寄存器的值
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值 
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);      //开启UCOSIII
    while(1)
    {
	} 
}

//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	CPU_SR_ALLOC();
	p_arg = p_arg;
	
	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif
	
#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	  //使能时间片轮转调度功能,设置默认的时间片长度
	OSSchedRoundRobinCfg(DEF_ENABLED,10,&err);  
#endif	
	
	//创建定时器1
	OSTmrCreate((OS_TMR		*)&tmr1,		//定时器1
                (CPU_CHAR	*)"tmr1",		//定时器名字
                (OS_TICK	 )20,			//20*10=200ms
                (OS_TICK	 )100,          //100*10=1000ms
                (OS_OPT		 )OS_OPT_TMR_PERIODIC, //周期模式
                (OS_TMR_CALLBACK_PTR)tmr1_callback,//定时器1回调函数
                (void	    *)0,			//参数为0
                (OS_ERR	    *)&err);		//返回的错误码					
	//创建定时器2
	OSTmrCreate((OS_TMR		*)&tmr2,		
                (CPU_CHAR	*)"tmr2",		
                (OS_TICK	 )200,			//200*10=2000ms	
                (OS_TICK	 )0,   					
                (OS_OPT		 )OS_OPT_TMR_ONE_SHOT, 	//单次定时器
                (OS_TMR_CALLBACK_PTR)tmr2_callback,	//定时器2回调函数
                (void	    *)0,			
                (OS_ERR	    *)&err);	
	OS_CRITICAL_ENTER();	//进入临界区
	//创建TASK1任务
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
    //创建TASK2任务
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
//创建TASK3任务
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
//创建TASK4任务
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
	OS_CRITICAL_EXIT();	//退出临界区
	OSTaskDel((OS_TCB*)0,&err);	//删除start_task任务自身
}

//任务1的任务函数
void heart_beat_task(void *p_arg)
{

	
	OS_ERR err;
	
	OSTmrStart(&tmr1,&err);	//开启定时器1
	
	heart_beat[5]=h_times.day=2;
	heart_beat[4]=h_times.month=10;
	heart_beat[3]=h_times.year=2018;
	
	
	while(1)
	{	
		heart_run();
		
		OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);   //延时10ms
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
			OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);   //延时10ms

		
	}

}



void mpu9250_task(void *p_arg)
{
     OS_ERR err;
	
//	OSTaskResume((OS_TCB*)0,&err);
	

	
	while(1)
	{
	      
		mpu9250_run();
	    OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);   //延时10ms
	}
}


void feed_back_task(void *p_arg)
{
    OS_ERR err;
	
	while(1)
	{
	      
		feed_back_send();
	    OSTimeDlyHMSM(0,0,0,10,OS_OPT_TIME_PERIODIC,&err);   //延时10ms
	}
}


//定时器1的回调函数
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

//定时器2的回调函数
void tmr2_callback(void *p_tmr,void *p_arg)
{
	static u8 tmr2_num = 0;
	tmr2_num++;		//定时器2执行次数加1
	LCD_ShowxNum(182,111,tmr2_num,3,16,0x80);  //显示定时器1执行次数
	LCD_Fill(126,131,233,313,lcd_discolor[tmr2_num%14]); //填充区域
	LED1_Toggle;
	printf("定时器2运行结束\r\n");
}





//串口1发送1个字符 
//c:要发送的字符
void usart1_send_char(u8 c)
{
	while((USART1->ISR&0X40)==0);	//循环发送,直到发送完毕   
    USART1->TDR=c;  
} 
//传送数据给匿名四轴地面站(V4版本)
//fun:功能字. 0X01~0X1C
//data:数据缓存区,最多28字节!!
//len:data区有效数据个数
void usart1_niming_report(u8 fun,u8*data,u8 len)
{
	u8 send_buf[32];
	u8 i;
	if(len>28)return;	//最多28字节数据 
	send_buf[len+3]=0;	//校验数置零
	send_buf[0]=0XAA;	//帧头
	send_buf[1]=0XAA;	//帧头
	send_buf[2]=fun;	//功能字
	send_buf[3]=len;	//数据长度
	for(i=0;i<len;i++)send_buf[4+i]=data[i];			//复制数据
	for(i=0;i<len+4;i++)send_buf[len+4]+=send_buf[i];	//计算校验和	
	for(i=0;i<len+5;i++)usart1_send_char(send_buf[i]);	//发送数据到串口1 
}
//发送加速度传感器数据+陀螺仪数据(传感器帧)
//aacx,aacy,aacz:x,y,z三个方向上面的加速度值
//gyrox,gyroy,gyroz:x,y,z三个方向上面的陀螺仪值 
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
	tbuf[12]=0;//因为开启MPL后,无法直接读取磁力计数据,所以这里直接屏蔽掉.用0替代.
	tbuf[13]=0;
	tbuf[14]=0;
	tbuf[15]=0;
	tbuf[16]=0;
	tbuf[17]=0;
	usart1_niming_report(0X02,tbuf,18);//传感器帧,0X02
}	
//通过串口1上报结算后的姿态数据给电脑(状态帧)
//roll:横滚角.单位0.01度。 -18000 -> 18000 对应 -180.00  ->  180.00度
//pitch:俯仰角.单位 0.01度。-9000 - 9000 对应 -90.00 -> 90.00 度
//yaw:航向角.单位为0.1度 0 -> 3600  对应 0 -> 360.0度
//csb:超声波高度,单位:cm
//prs:气压计高度,单位:mm
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
	usart1_niming_report(0X01,tbuf,12);//状态帧,0X01
}  

