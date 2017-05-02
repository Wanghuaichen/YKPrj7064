//**********************************************************************
//�ļ���:alarm_pro.h
//**********************************************************************
#ifndef _ALARM_PRO_H
#define _ALARM_PRO_H

#include "type_def.h"
//������Ϣ�ṹ��
typedef struct alm_struct    //������Ϣ��¼
{
    const uint16 alm_id;    //������ϢID
    const uint8* ch_mess;    //������Ϣ
    const uint8* en_mess;
    uint8 flag_log;            //�Ѿ��Ǽǵı�־=TRUE��ʾ�����ڵ��Ѿ����ڱ�������
    struct alm_struct* next_record;    //��һ����¼
}Alm_Struct;
//��ʾ��Ϣ�ṹ��
typedef struct tips_struct
{
    const uint16 tips_id;        //��ʾ��ϢID    
    const uint8* ch_tips;        //������ʾ��Ϣ
    const uint8* en_tips;        //Ӣ����ʾ��Ϣ
    struct tips_struct* next_tips;    //��һ����ʾ��Ϣ
}Tips_Struct;

//�洢��¼�ṹ��
typedef struct recorder_struct   //�洢��¼ ��ռ��14���ֽڣ���2���ֽ�Ԥ��
{
    const uint16 rec_flag;       //��Ч��¼��־0x55aa
    const uint8  rec_year;       //��¼��ʱ��
    const uint8  rec_month;
    const uint8  rec_day;
    const uint8  rec_hour;    
    const uint8  rec_min;
    const uint8  rec_sec;
    const uint8  rec_mode;     //��¼��ģʽ
    union {                    //ʹ��������
          float  rec_float;    //���ݿ����Ǹ�������Ҳ����������
          uint32 rec_uint32;
          }rec_data;
    const uint8  rec_check;    //У���ֽ� ��FLAG���11���ֽ��ۼӺ�      
}recorder_Struct;


//���ֱ������
#define ALM_LEV_WARN          0x00
#define ALM_LEV_TIPS          0x01

//�����౨�����(ԽС���ȼ�Խ��)
#define NONE_ALM                 0
//��ʾ�౨�����(ԽС���ȼ�Խ��)
#define FIRST_WARN_ALARM             1 
#define ALARM_WEIGH_OUT_WARN         1   //��������
    
#define ALARM_WPS_OUT_WARN           2   //�����ز��ڷ�Χ    
#define ALARM_PCS_OUT_WARN           3   //�������ڷ�Χ
#define ALARM_CHECK_OUT_WARN         4   //���ز��ڷ���
#define ALARM_100_OUT_WARN           5   //�ٷֱȻ������ڷ�Χ
#define ALARM_100_TARGET_WARN        6   //�ٷֱ����ݳ���Χ
#define ALARM_DS18B20_WARN           7
#define ALARM_TEMP_OUT_WARN          8   //�¶ȳ���ĳ����ֵ���磺45��
#define ALARM_TEMP_CHANGE_OUT_WARN   9   //�¶ȱ仯�ϴ󣬱���仯��3��
#define ALARM_LOAD_TIME_OUT_WARN    10   //���س�ʱ


#define LAST_WARN_ALARM              50  //

#define FIRST_TIPS_ALARM             61
#define TIPS_NOT_STABLE_WARN         61   //���ȶ� ���ڱ�������ʱ��
#define TIPS_EE_BAD_WARN             62   //EEPROM ����
#define TIPS_SAVE_OK_WARN            63   //SAVE OK
#define TIPS_CLEAR_OK_WARN           64
#define TIPS_FIRST_PAGE_WARN         65
#define TIPS_LAST_PAGE_WARN          66
#define TIPS_NO_VALID_WARN           67
#define TIPS_BAD_ENV_WARN            68
#define TIPS_SAMPLE_WPS_WARN         69
#define TIPS_SAMPLE_BENCH_WARN       70
#define TIPS_INPUT_LIMIT_WARN        71
#define TIPS_CAL_STOP_WARN           72

#define LAST_TIPS_ALARM              99

//��ʾ����Ϣ
//��ʾ�౨�� 
extern Alm_Struct warn_weigh_out;
extern Alm_Struct warn_wps_out;
extern Alm_Struct warn_pcs_out;
extern Alm_Struct warn_check_out;
extern Alm_Struct warn_100_out;
extern Alm_Struct warn_100_data_out;

extern Alm_Struct warn_DS18B20_err;
extern Alm_Struct warn_temp_out;
extern Alm_Struct warn_temp_c_out;
extern Alm_Struct warn_loadtime_out;


extern Alm_Struct tips_save_fault;
extern Alm_Struct tips_EE_bad;
extern Alm_Struct tips_save_ok;
extern Alm_Struct tips_clear_ok;
extern Alm_Struct tips_first_page;
extern Alm_Struct tips_last_page;
extern Alm_Struct tips_invalid_data;
extern Alm_Struct tips_bad_envir;
extern Alm_Struct tips_sample_wps;
extern Alm_Struct tips_sample_bench;
extern Alm_Struct tips_input_limit;
extern Alm_Struct tips_cal_stop;


extern void Log_Alm(Alm_Struct* alm,uint8 level);
extern void Del_Alm(Alm_Struct* alm,uint8 level);
extern void Put_Alm_Mess(void);
extern void Set_Clr_Alarm_Sound(void);
#endif
