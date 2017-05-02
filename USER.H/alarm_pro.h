//**********************************************************************
//文件名:alarm_pro.h
//**********************************************************************
#ifndef _ALARM_PRO_H
#define _ALARM_PRO_H

#include "type_def.h"
//报警信息结构体
typedef struct alm_struct    //报警信息记录
{
    const uint16 alm_id;    //报警信息ID
    const uint8* ch_mess;    //报警信息
    const uint8* en_mess;
    uint8 flag_log;            //已经登记的标志=TRUE表示报警节点已经处在报警链上
    struct alm_struct* next_record;    //下一条记录
}Alm_Struct;
//提示信息结构体
typedef struct tips_struct
{
    const uint16 tips_id;        //提示信息ID    
    const uint8* ch_tips;        //中文提示信息
    const uint8* en_tips;        //英文提示信息
    struct tips_struct* next_tips;    //下一条提示信息
}Tips_Struct;

//存储记录结构体
typedef struct recorder_struct   //存储记录 共占用14个字节，另2个字节预留
{
    const uint16 rec_flag;       //有效记录标志0x55aa
    const uint8  rec_year;       //记录的时间
    const uint8  rec_month;
    const uint8  rec_day;
    const uint8  rec_hour;    
    const uint8  rec_min;
    const uint8  rec_sec;
    const uint8  rec_mode;     //记录的模式
    union {                    //使用联合体
          float  rec_float;    //数据可以是浮点数，也可以是整数
          uint32 rec_uint32;
          }rec_data;
    const uint8  rec_check;    //校验字节 除FLAG外的11个字节累加和      
}recorder_Struct;


//两种报警类别
#define ALM_LEV_WARN          0x00
#define ALM_LEV_TIPS          0x01

//故障类报警序号(越小优先级越高)
#define NONE_ALM                 0
//警示类报警序号(越小优先级越高)
#define FIRST_WARN_ALARM             1 
#define ALARM_WEIGH_OUT_WARN         1   //重量超载
    
#define ALARM_WPS_OUT_WARN           2   //单件重不在范围    
#define ALARM_PCS_OUT_WARN           3   //件数不在范围
#define ALARM_CHECK_OUT_WARN         4   //检重不在反问
#define ALARM_100_OUT_WARN           5   //百分比基数不在范围
#define ALARM_100_TARGET_WARN        6   //百分比数据超范围
#define ALARM_DS18B20_WARN           7
#define ALARM_TEMP_OUT_WARN          8   //温度超过某个数值例如：45度
#define ALARM_TEMP_CHANGE_OUT_WARN   9   //温度变化较大，比如变化了3度
#define ALARM_LOAD_TIME_OUT_WARN    10   //加载超时


#define LAST_WARN_ALARM              50  //

#define FIRST_TIPS_ALARM             61
#define TIPS_NOT_STABLE_WARN         61   //不稳定 用于保存数据时候
#define TIPS_EE_BAD_WARN             62   //EEPROM 出错
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

//提示类信息
//警示类报警 
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
