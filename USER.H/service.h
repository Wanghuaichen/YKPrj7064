//**********************************************************************
//�ļ���:service.h
//**********************************************************************
#ifndef _SERVICE_H
#define _SERVICE_H

#include "type_def.h"

#define MENU_SERV_MAIN_SEL                   0x00       //���˵�
#define MENU_SERV_MAIN_OP_NUM                   8       //ѡ�����

#define MENU_SERV_SEL_SENSOR_OP        0x00000100       //AD���� ��ȡԭʼ�������˲������ֵ
#define MENU_SERV_SEL_TEMP_OP          0x00000200       //�¶�   ��ȡ�¶���
#define MENU_SERV_SEL_EEPROM_OP        0x00000300       //EEPROM ��д����
#define MENU_SERV_SEL_MODEL_OP         0x00000400       // 
#define MENU_SERV_SEL_MOTOR_OP         0x00000500       //���   ��������ת
#define MENU_SERV_SEL_INTERFACE_OP     0x00000600       //����   �ӿ��źš���ָʾ�Ƶ�
                      //��������ָʾ��
                      //�����ź�ָʾ��
                      //���������ź�
#define MENU_SERV_SEL_BRISPK_OP        0x00000700       //����   ������������ȱ仯
#define MENU_SERV_SEL_SCREEN_OP        0x00000800       //��Ļ   ��ɫ���

extern void  Start_Service_process(void);
#endif