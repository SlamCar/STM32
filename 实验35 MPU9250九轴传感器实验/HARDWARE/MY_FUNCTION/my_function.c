#include "my_function.h"
#include "sys.h" 
#include "delay.h"
#include "math.h"
#include "usart.h"
#include "sdram.h"
#include "mpu.h"
#include "includes.h"
#include "ltdc.h"

/////////////////////////////////////////////////////////////////////////////////
//将abc分开一位一位地放入数组中
//
/////////////////////////////////////////////////////////////////////////////////
u16 do_mat(u16 abc,u16 *save,u8 start,u8 end)
{
	
    u8 i;
	u16 divide_num;
	
	for(i=start;i<end+1;i++)
	{
		divide_num=pow(10,(end-i));
	    save[i]=abc/divide_num%10;
		
//		printf("%d  \r\n",divide_num);
	}
//	for(i=0;i<end+1;i++)
//	{
////	  printf("%d  ",save[i]);
//	}
//	printf("\r\n");
	

}







////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//crc校验
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////








unsigned int cal_crc(u16 *ptr, unsigned char len) 
{
	OS_ERR err;
	u8 i;
	u16 crc;
	u8 da;
	u16 crc_ta[256]={ /* CRC ??? */
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};
	
   crc=0;
   

//    for(i=len;i>0;i--)
	while(len--!=0) 
	{
		da=(u8) (crc/256); /* ? 8 ?????????? CRC ?? 8 ? */
		crc<<=8; /* ?? 8 ?,??? CRC ?? 8 ???
		8
		2 */
		 crc^=crc_ta[da^*ptr]; /* ? 8 ????????????? CRC ,?????? CRC */
		ptr++;

	}
	
// return(crc);

}










////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//My_KEY
//
//
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//void My_Key_Init(void)
//{

//    GPIO_InitTypeDef GPIO_InitStructure;
// 
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE,ENABLE);//使能PORTA,PORTE时钟

//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15|GPIO_Pin_10;//KEY0-KEY2
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
// 	GPIO_Init(GPIOE, &GPIO_InitStructure);//初始化GPIOE1,2,3,4
//	
//}


//u8 My_Key_Scan(u8 mode)
//{
//	static u8 key_up=1;//按键按松开标志
//	if(mode)key_up=1;  //支持连按		  
//	if(key_up&&(KEY4==0||KEY5==0||KEY7==0||KEY8==0||KEY9==0||KEY10==0))
//	{
//		delay_ms(10);//去抖动 
//		key_up=0;
//             if(KEY4==0)return KEY4_PRES;
//		else if(KEY5==0)return KEY5_PRES;
//		else if(KEY7==0)return KEY7_PRES;
//	    else if(KEY8==0)return KEY8_PRES;
//	    else if(KEY9==0)return KEY9_PRES;
//		else if(KEY10==0)return KEY10_PRES;
//	}else if(KEY4==1&&KEY5==1&&KEY7==1&&KEY8==1&&KEY9==1&&KEY10==1)key_up=1; 	    
// 	return 0;// 无按键按下

//}




//extern u8 tmp_buf[33];	

//u8 key;
//u8 pwm=123;

//void KEY_Run(void)
//{
// 
// 
//   key=My_Key_Scan(0);	//得到键值
//	
//	   	if(key)
//		{						   
//			switch(key)
//			{				 
//				case KEY4_PRES:	//控制LED0翻转
//                                 pwm+=5;
//					break;
//				case KEY5_PRES:	//控制LED1翻转	 
//                                 pwm-=5;
//					break;
//				case KEY7_PRES:	//同时控制LED0,LED1翻转 
//                                 do_mat(pwm,tmp_buf,0,2);
//				
//					break;
//				case KEY8_PRES:	//同时控制LED0,LED1翻转 

//					break;
//				case KEY9_PRES:	//同时控制LED0,LED1翻转 

//					break;
//				case KEY10_PRES:	//同时控制LED0,LED1翻转 

//					break;
//			}
//		}
//}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//easytrace
//实时跟踪识别物体
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define min3v(v1, v2, v3)   ((v1)>(v2)? ((v2)>(v3)?(v3):(v2)):((v1)>(v3)?(v3):(v1)))//取最大
#define max3v(v1, v2, v3)   ((v1)<(v2)? ((v2)<(v3)?(v3):(v2)):((v1)<(v3)?(v3):(v1)))//取最小值

typedef struct
	  {
    unsigned char  red;             // [0,255]
    unsigned char  green;           // [0,255]
    unsigned char  blue;            // [0,255]
    }COLOR_RGB;//RGB格式颜色

typedef struct
	  {
    unsigned char hue;              // [0,240]
    unsigned char saturation;       // [0,240]
    unsigned char luminance;        // [0,240]
    }COLOR_HSL;//HSL格式颜色

typedef struct
	  {
    unsigned int X_Start;              
    unsigned int X_End;
	  unsigned int Y_Start;              
    unsigned int Y_End;
    }SEARCH_AREA;//区域

//读取RBG格式颜色，唯一需要移植的函数
extern unsigned short LCD_ReadPoint(unsigned short x,unsigned short y);//读某点颜色

static void ReadColor(unsigned int x,unsigned int y,COLOR_RGB *Rgb)
	{
		unsigned short C16;

		C16 = LCD_ReadPoint(x,y);     //读某点颜色

		Rgb->red   =	 (unsigned char)((C16&0xf800)>>8);
		Rgb->green =	 (unsigned char)((C16&0x07e0)>>3);
		Rgb->blue  =   (unsigned char)((C16&0x001f)<<3);
	}


u16 k;
//RGB转HSL
static void RGBtoHSL(const COLOR_RGB *Rgb, COLOR_HSL *Hsl)
	{
			int h,s,l,maxVal,minVal,difVal;
			int r  = Rgb->red;
			int g  = Rgb->green;
			int b  = Rgb->blue;
		
				maxVal = max3v(r, g, b);
				minVal = min3v(r, g, b);
				
				difVal = maxVal-minVal;
		
		//计算亮度
			l = (maxVal+minVal)*240/255/2;
		
		if(maxVal == minVal)//若r=g=b
			{
				h = 0; 
				s = 0;
			}
		else
		{
			//计算色调
			if(maxVal==r)
			{
				if(g>=b)
					h = 40*(g-b)/(difVal);
				else
					h = 40*(g-b)/(difVal) + 240;
			}
			else if(maxVal==g)
					h = 40*(b-r)/(difVal) + 80;
			else if(maxVal==b)
					h = 40*(r-g)/(difVal) + 160;
			//计算饱和度
			if(l == 0)
					s = (difVal)*240/(511 - (maxVal+minVal));
			else if(l<=120)
					s = (difVal)*240/(maxVal+minVal);
			else
					s = (difVal)*240/(511 - (maxVal+minVal));
		}
			Hsl->hue =        (unsigned char)(((h>240)? 240 : ((h<0)?0:h)));//色度
			Hsl->saturation = (unsigned char)(((s>240)? 240 : ((s<0)?0:s)));//饱和度
			Hsl->luminance =  (unsigned char)(((l>240)? 240 : ((l<0)?0:l)));//亮度
//            k++;
//		    if(k>1000)
//			{
//			  printf("%d  %d  %d    ",Hsl->hue,Hsl->saturation,Hsl->luminance);
//				k=0;
//			}
			
	}

//匹配颜色
static int ColorMatch(const COLOR_HSL *Hsl,const TARGET_CONDI *Condition)//HSL格式颜色、判定为的目标条件
	{
		if( 
					Hsl->hue		>	Condition->H_MIN &&
					Hsl->hue		<	Condition->H_MAX &&
					Hsl->saturation	>	Condition->S_MIN &&
					Hsl->saturation	<   Condition->S_MAX &&
					Hsl->luminance	>	Condition->L_MIN &&
					Hsl->luminance	<   Condition->L_MAX 
			)          //hue为色调，saturation为饱和度 ，luminance为亮度
				return 1;
		else
				return 0;
	}

//搜索腐蚀中心
static int SearchCentre(unsigned int *x,unsigned int *y,const TARGET_CONDI *Condition,const SEARCH_AREA *Area)
//TARGET_CONDI判定为的目标条件、SEARCH_AREA区域
	{
			unsigned int SpaceX,SpaceY,i,j,k,FailCount=0;
			COLOR_RGB Rgb;
			COLOR_HSL Hsl;
			
			SpaceX = Condition->WIDTH_MIN/3;   //目标最小宽度
			SpaceY = Condition->HIGHT_MIN/3;   //目标最小高度
		
//			SpaceX = Condition->WIDTH_MIN;   //目标最小宽度
//			SpaceY = Condition->HIGHT_MIN;   //目标最小高度

				for(i=Area->Y_Start;i<Area->Y_End;i+=SpaceY)
				{
					for(j=Area->X_Start;j<Area->X_End;j+=SpaceX)
					{
						FailCount=0;
						for(k=0;k<SpaceX+SpaceY;k++)
						{
							if(k<SpaceX)
								ReadColor(j+k,i+SpaceY/2,&Rgb);
							else
								ReadColor(j+SpaceX/2,i+(k-SpaceX),&Rgb);
							  RGBtoHSL(&Rgb,&Hsl);
							
							if(!ColorMatch(&Hsl,Condition))
								FailCount++;
							if(FailCount>((SpaceX+SpaceY)>>ALLOW_FAIL_PER))
								break;
						}
						if(k==SpaceX+SpaceY)
						{
							*x = j+SpaceX/2;
							*y = i+SpaceY/2;
							return 1;
						}
					}
				}
		return 0;
	}

//从腐蚀中心向外腐蚀，得到新的腐蚀中心
static int Corrode(unsigned int oldx,unsigned int oldy,const TARGET_CONDI *Condition,RESULT *Resu)
{
	
	unsigned int Xmin,Xmax,Ymin,Ymax,i,FailCount=0;
	COLOR_RGB Rgb;
	COLOR_HSL Hsl;
	
	for(i=oldx;i>IMG_X;i--)
		{
				ReadColor(i,oldy,&Rgb);
				RGBtoHSL(&Rgb,&Hsl);
				if(!ColorMatch(&Hsl,Condition))
					FailCount++;
				if(FailCount>(((Condition->WIDTH_MIN+Condition->WIDTH_MAX)>>2)>>ALLOW_FAIL_PER))
					break;	
		}
	Xmin=i;
	FailCount=0;
	
	for(i=oldx;i<IMG_X+IMG_W;i++)
		{
				ReadColor(i,oldy,&Rgb);
				RGBtoHSL(&Rgb,&Hsl);
				if(!ColorMatch(&Hsl,Condition))
					FailCount++;
				if(FailCount>(((Condition->WIDTH_MIN+Condition->WIDTH_MAX)>>2)>>ALLOW_FAIL_PER))
					break;	
		}
	Xmax=i;
	FailCount=0;
	
	for(i=oldy;i>IMG_Y;i--)
		{
				ReadColor(oldx,i,&Rgb);
				RGBtoHSL(&Rgb,&Hsl);
				if(!ColorMatch(&Hsl,Condition))
					FailCount++;
				if(FailCount>(((Condition->HIGHT_MIN+Condition->HIGHT_MAX)>>2)>>ALLOW_FAIL_PER))
					break;	
		}
	Ymin=i;
	FailCount=0;
	
	for(i=oldy;i<IMG_Y+IMG_H;i++)
		{
				ReadColor(oldx,i,&Rgb);
				RGBtoHSL(&Rgb,&Hsl);
				if(!ColorMatch(&Hsl,Condition))
					FailCount++;
				if(FailCount>(((Condition->HIGHT_MIN+Condition->HIGHT_MAX)>>2)>>ALLOW_FAIL_PER))
					break;	
		}
	Ymax=i;
	FailCount=0;
	
	Resu->x	= (Xmin+Xmax)/2;
	Resu->y	= (Ymin+Ymax)/2;
	Resu->w	= Xmax-Xmin;
	Resu->h	= Ymax-Ymin;

	if(
			 ((Xmax-Xmin)>(Condition->WIDTH_MIN)) && ((Ymax-Ymin)>(Condition->HIGHT_MIN)) &&\
			 ((Xmax-Xmin)<(Condition->WIDTH_MAX)) && ((Ymax-Ymin)<(Condition->HIGHT_MAX))
	   )
		  return 1;	
	else
		  return 0;	
}


void read_color(void)
{
	COLOR_RGB Rgb;
	COLOR_HSL Hsl;
    ReadColor(5,5,&Rgb);
	RGBtoHSL(&Rgb,&Hsl);

}

//唯一的API，用户将识别条件写入Condition指向的结构体中，该函数将返回目标的x，y坐标和长宽
//返回1识别成功，返回1识别失败
int Trace(const TARGET_CONDI *Condition,RESULT *Resu)  //TARGET_CONDI识别条件  RESULT识别结果
{
	unsigned int i;
	static unsigned int x0,y0,flag=0;  //静态变量
	static SEARCH_AREA Area={IMG_X,IMG_X+IMG_W,IMG_Y,IMG_Y+IMG_H};  //x坐标 y坐标  w宽度  h高度
	RESULT Result;	//RESULT识别结果
	

	if(flag==0)    //已经定义
		{
			if(SearchCentre(&x0,&y0,Condition,&Area))  //搜索腐蚀中心   &是取地址运算符
				 flag=1;
			else
				{
						Area.X_Start= IMG_X	       ;
						Area.X_End  = IMG_X+IMG_W  ;
						Area.Y_Start= IMG_Y		     ;
						Area.Y_End  = IMG_Y+IMG_H  ;

						if(SearchCentre(&x0,&y0,Condition,&Area))	
						{
							flag=0;
							return 0;
						}	
				}
		}
		
	Result.x = x0;
	Result.y = y0;
	
	for(i=0;i<ITERATE_NUM;i++)  //进行迭代计算
		 Corrode(Result.x,Result.y,Condition,&Result);
		
	if(Corrode(Result.x,Result.y,Condition,&Result))//从腐蚀中心向外腐蚀
		{
			x0=Result.x;
			y0=Result.y;
			Resu->x=Result.x;
			Resu->y=Result.y;
			Resu->w=Result.w;
			Resu->h=Result.h;
			flag=1;

			Area.X_Start= Result.x - ((Result.w)>>1);
			Area.X_End  = Result.x + ((Result.w)>>1);
			Area.Y_Start= Result.y - ((Result.h)>>1);
			Area.Y_End  = Result.y + ((Result.h)>>1);


			return 1;
		}
	else
		{
			flag=0;
			return 0;
		}

}













