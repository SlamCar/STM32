#ifndef __MY_FUNCTION_H
#define __MY_FUNCTION_H
#include "sys.h"




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//my——key
//包含按键初始化
//以及按键运行函数
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





#define KEY4 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_11)
#define KEY5 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_12)
#define KEY7 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_13)
#define KEY8 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14)
#define KEY9 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15)
#define KEY10 GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_10)


#define KEY4_PRES 5
#define KEY5_PRES 6
#define KEY7_PRES 7
#define KEY8_PRES 8
#define KEY9_PRES 9
#define KEY10_PRES 10

void My_Key_Init(void);

u8 My_Key_Scan(u8);

void KEY_Run(void);
u16 do_mat(u16 abc,u16 *save,u8 start,u8 end);


unsigned int cal_crc(u16 *ptr, unsigned char len);



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//easytrace
//实时跟踪识别物体
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#define IMG_X 0	  //图片x坐标
#define IMG_Y 0	  //图片y坐标
#define IMG_W 320 //图片宽度
#define IMG_H 240 //图片高度

#define ALLOW_FAIL_PER 2 //容错率，每1<<ALLOW_FAIL_PER个点允许出现一个错误点，容错率越大越容易识别，但错误率越大
#define ITERATE_NUM    8 //迭代次数，迭代次数越多识别越精确，但计算量越大

typedef struct{
    unsigned char  H_MIN;//目标最小色调
    unsigned char  H_MAX;//目标最大色调	
    
	unsigned char  S_MIN;//目标最小饱和度  
    unsigned char  S_MAX;//目标最大饱和度
	
	unsigned char  L_MIN;//目标最小亮度  
    unsigned char  L_MAX;//目标最大亮度
	
	unsigned int  WIDTH_MIN;//目标最小宽度
	unsigned int  HIGHT_MIN;//目标最小高度

	unsigned int  WIDTH_MAX;//目标最大宽度
	unsigned int  HIGHT_MAX;//目标最大高度

}TARGET_CONDI;//判定为的目标条件

typedef struct{
	unsigned int x;//目标的x坐标
	unsigned int y;//目标的y坐标
	unsigned int w;//目标的宽度
	unsigned int h;//目标的高度
}RESULT;//识别结果

//唯一的API，用户将识别条件写入Condition指向的结构体中，该函数将返回目标的x，y坐标和长宽
//返回1识别成功，返回1识别失败
int Trace(const TARGET_CONDI *Condition,RESULT *Resu);

void read_color(void);






#endif




