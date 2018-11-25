#include "Data_Base.h"

Feedback_Msg  db_feedbackMsg = {0};
Control_Msg   db_controlMsg =  {0};

ROBOT_Input   g_ROBOT_Input =  {0};
ROBOT_Output  g_ROBOT_Output = {0};

void dataBaseInit()
{
    db_feedbackMsg.now_angle = 0.0;
    db_feedbackMsg.now_speed = 0.0;
    
    db_controlMsg.angle = 0.0;
    db_controlMsg.speed = 0.0;
}

void Update_CmdMsg(const SerialPakage msg)
{
    memcpy(&db_controlMsg, msg.byData_, sizeof(Control_Msg)); 
}

//void Update_FeedbackMsg(const Feedback_Msg msg)
//{
//}

/*
void ROBOT_Input(ROBOT_Input *Input)
{
    //�������ݽ���
    //�������ݿ�  ��������  ����������  �����˲���
    
}

void ROBOT_RunCtr(ROBOT_Input *Input, ROBOT_Output *Output)
{
    //�߼�����  ģ�ͽ���  
}
*/


