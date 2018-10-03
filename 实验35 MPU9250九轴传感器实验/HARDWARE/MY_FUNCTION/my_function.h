#ifndef __MY_FUNCTION_H
#define __MY_FUNCTION_H
#include "sys.h"




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//my����key
//����������ʼ��
//�Լ��������к���
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
//ʵʱ����ʶ������
//
//
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#define IMG_X 0	  //ͼƬx����
#define IMG_Y 0	  //ͼƬy����
#define IMG_W 320 //ͼƬ���
#define IMG_H 240 //ͼƬ�߶�

#define ALLOW_FAIL_PER 2 //�ݴ��ʣ�ÿ1<<ALLOW_FAIL_PER�����������һ������㣬�ݴ���Խ��Խ����ʶ�𣬵�������Խ��
#define ITERATE_NUM    8 //������������������Խ��ʶ��Խ��ȷ����������Խ��

typedef struct{
    unsigned char  H_MIN;//Ŀ����Сɫ��
    unsigned char  H_MAX;//Ŀ�����ɫ��	
    
	unsigned char  S_MIN;//Ŀ����С���Ͷ�  
    unsigned char  S_MAX;//Ŀ����󱥺Ͷ�
	
	unsigned char  L_MIN;//Ŀ����С����  
    unsigned char  L_MAX;//Ŀ���������
	
	unsigned int  WIDTH_MIN;//Ŀ����С���
	unsigned int  HIGHT_MIN;//Ŀ����С�߶�

	unsigned int  WIDTH_MAX;//Ŀ�������
	unsigned int  HIGHT_MAX;//Ŀ�����߶�

}TARGET_CONDI;//�ж�Ϊ��Ŀ������

typedef struct{
	unsigned int x;//Ŀ���x����
	unsigned int y;//Ŀ���y����
	unsigned int w;//Ŀ��Ŀ��
	unsigned int h;//Ŀ��ĸ߶�
}RESULT;//ʶ����

//Ψһ��API���û���ʶ������д��Conditionָ��Ľṹ���У��ú���������Ŀ���x��y����ͳ���
//����1ʶ��ɹ�������1ʶ��ʧ��
int Trace(const TARGET_CONDI *Condition,RESULT *Resu);

void read_color(void);






#endif




