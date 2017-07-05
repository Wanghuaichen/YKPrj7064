//**********************************************************************
//�ļ���:global.h
//**********************************************************************
#ifndef _GLOBAL_H_
#define _GLOBAL_H_

#include "type_def.h"

#define SOFT_VERSION             1.0 

//��У׼�汾�������������
//�޸������˵���Ŀ �����������޸�Ϊ��У����
#define INNER_CAL_VERSION          0 //�Ƿ���У׼�汾

#define DOUBLE_DIVISION            0  //�Ƿ�˫���̰汾
//#define DOUBLE_DIVISION          0

#define  QIAN_DIVISION             0
#define   WAN_DIVISION             1
#define WAN10_DIVISION             2

#define TFTLCD     1
#define LCDTYPE    TFTLCD   

////////////////////////////ADD BY WJG 2012.06.01
#define MONOLCD    0
#define TFTLCD     1
#define LCDTYPE    TFTLCD  

/////////////////////////////ADD BY WJG 2015.05.15
//�����¶ȷ�ʽ

#define USE_DS18B20    0  

 
//////////////////////////////////////////ADDATA
#define   EPM7064S    1
#define     CS5530    0

#define   AD_IC    EPM7064S
////////////////////////////////////////////
#define  USB_ENABLE                 1

////////////////////////////////////////////
#define   INNER_CODE_STEP_MAX      50
#define   INNER_CODE_STEP_MIN       5
#define   INNER_CODE_STEP_DEF      18

#define   USER_ZER0_CODE_MIN      100000
#define   USER_ZER0_CODE_MAX      10000000

#define   INNER_CODE_STEP10       (2000000*15)/13    //Ĭ��10��У׼�Ĳ���������
///////////////////////////////////////////////////////
//�����ļ��ֹ���ģʽ
///////////////////////////////////////////////////////

#define MACHINE_NORMAL_MODE        0    //��������ģʽ
#define MACHINE_SERVICE_MODE       1    //ά��ģʽ
#define MACHINE_DEMARC_MODE        2    //�궨ģʽ
#define MACHINE_SCREENCAL_MODE     3    //��ĻУ׼״̬

///////////////////////////////////////////////////////
//�������������ļ��ֹ���״̬
//////////////////////////////////////////////////////
#define STAT_NULL               0x00
#define STAT_CALING             0x01
#define STAT_WEIGH              0x02
#define STAT_PCS                0x03  
#define STAT_100                0x04

#define STAT_CHECK              0x05
#define STAT_DIFFER             0x06
#define STAT_TOTAL              0x07
#define STAT_ANIMAL             0x08
#define STAT_DENSITY            0x09
#define STAT_PEAK               0x0A
#define STAT_TARGET             0x0B
//ADD
#define STAT_CALING_INNER       0x0C

///////////////////////////////////////////////////////
//2��У׼ ����У׼
//////////////////////////////////////////////////////
#define CAL_MODE_OUT            0x00
#define CAL_MODE_INNER          0x01

///////////////////////////////////////////////////////
//���ص�λ
//////////////////////////////////////////////////////

#define UNIT_NULL               0x00
#define UNIT_G                  0x01
#define UNIT_MG                 0x02
#define UNIT_LB                 0x03  
#define UNIT_OZ                 0x04

///////////////////////////////////////////////////////
//�����ļ���״̬
//////////////////////////////////////////////////////

#define KEY_PRESS               0x01  //����
#define KEY_RELEASE             0x02  //�ɿ�
#define KEY_DO_START            0x04  //��ʼ����˰���
#define KEY_DO_OVER             0x08  //�������


///////////////////////////////////////////////////////
//�����������������̵ĸ�ֵ����
///////////////////////////////////////////////////////
#define   DATA_WEIGHT_PER_PCS             0
#define   DATA_BAIFENBI_LIMIT             1
#define   DATA_ANIMAL_TIME                2
#define   DATA_PASSWORD                   3
#define   DATA_USER_ID                    4

///////////////////////////////////////////////////////
//���߷��书���б�
///////////////////////////////////////////////////////
#define   FUNCTION_NULL             0
#define   FUNCTION_QP               1
#define   FUNCTION_ZER0             2
#define   FUNCTION_CAL              3
#define   FUNCTION_IDLE             4
#define   FUNCTION_INCAL            5
#define   FUNCTION_END              6

///////////////////����У׼�� ��Ŀ
#define   CAL_POINT_NUM            11

////////////����Ԥ��ʱ�� ��
#define     PREHOT_TIME           1800
//�������ʱ�� s
#define    ENTER_SLEEP_TIME       300

//�û�У׼��������
#define   USER_CAL_ZER0            1
#define   USER_CAL_ZER0_ADD        2
#define   USER_CAL_FULL            3
#define   USER_CAL_OVER_PRE        4  //�ȴ�ж�غ���ȶ�״̬
#define   USER_CAL_OVER            5
//

//#define   WEIGHT_TIP_VALUE        200.0090
//#define   WEIGHT_ALM_VALUE        205.0090
///���㷶Χ 10%  20%
#define   START_ZER0_MAX           5  //�����̵����֮һ  
#define    USER_ZER0_MAX           4  //�����̵İٷ�֮��

//����ת��ϵ��
#define    G_TO_MG                1000
#define    G_TO_LB          0.00220462
#define    G_TO_OZ         0.035273962



//�����������ȶ�����ٴ�����ȥƤ����
#define   POWER_ON_STABLE_TIMES    2 



//�����������
#define    PCS_ADJUST_COEF        0.5  //�����ĵ�������
#define    WPS_MAX                100  //(������ֵ)
#define    WPS_MIN              0.001  //�����صķ�Χ
#define    PCS_MAX              99999  //�����ظ���
//�ֶ���������ķ�Χ
#define    PCS_IN_MIN              10 
#define    PCS_IN_MAX            9999  //(������ֵ)



//100 �������
#define    MAX_100                200 //������ֵ
#define    MIN_100              0.001
#define    TARGET_100_MAX     9999.99

//100 �������
#define    CHECK_MAX                200 //������ֵ
#define    CHECK_MIN              0.001
//�ֶ��������ްٷֱȵķ�Χ
#define    CHECK_LIMIT2_MIN        0.01 //������ֵ
#define    CHECK_LIMIT2_MAX          20

//Ŀ����� ������Χ
#define    TOTAL_MAX_TIMES        999
//Ŀ����� ������Χ
//��Ŀ����ط�Χ
#define    TARGET_MAX             200
#define    TARGET_MIN           0.001
//����������Ŀ��ֵ�����������Χ 1%
#define    TARGET_OK             0.01

//���������Ӧʱ�����÷�Χ(��)
#define    ANIMAL_TIME_MAX       20


//�ܶȳ���ʱ���ˮ�����÷�Χ
#define    DENSITY_TEMP_MAX       45
#define    DENSITY_TEMP_MIN       10

//////////////////////////////////////
//�����¶Ȳ��ܴ���39.5
//�²�仯ҪС��2.5     ���򱨾�
#define   ENV_TEMP_WARING        50
#define   TEMP_CHANGE_WARING      6
//////////////////////////////////////
//�����¶ȳ��� 5-50 �Ⱥ� ������ʾ�û�
#define   TEMP_SENSOR_NC_VALUE    999.99


#define   TEMP_UNIT_OC           0x01
#define   TEMP_UNIT_OF           0x80



//У���ֽ�
#define   REC_CHECK_DATA1       0x55
#define   REC_CHECK_DATA2       0xaa

//���ȵ����ȼ�
#define   BRIGHT_CLASS_TOTAL      20
#define   DEFAULT_BRIGHTNESS       8
#define   SLEEP_BRIGHTNESS         1

//���������ȼ�
#define   SPEAK_CLASS_TOTAL       16
#define   DEFAULT_SPEAKER_VOLUME   8

//����ɼ��׶Σ��������봰�ڵ�ID
#define   PASSWORD_INPUT_ID  0x12345678
#define   PASSWORD_DEFAULT         9999
#define   USER_ID_DEFAULT           001

//#define   MODEL_ID_DEFAULT   0x00010002 //200.0000
#define   MODEL_ID_DEFAULT   0xC90100C8 //200.0000

//�ؼ��ṹ��
struct KeyStruct{
    uint32 X_Position;     /* X */
    uint32 Y_Position;     /* Y */
    uint32 Value;          /* value */
};

//�洢��¼�ṹ��
typedef union    
{
   float  rec_float;    //���ݿ����Ǹ�������Ҳ����������
   uint32 rec_uint32;
}un_rec_data;

extern float const machine_fullrange[];

extern un_rec_data  inner_cal_real;

extern uint32   lcd_brightness;      //��ʾ���Աȶȵ�ֵ
extern uint32   key_value; 
extern uint32   Key_Status;
extern uint32   sleepmode;
extern uint32   machine_mode,current_mode,mode_run_flag,current_unit;
extern uint32   stable_flag;
extern uint32   power_on_flag,power_on_clr_cnt,elapsed_time;

extern uint32   Error_Info_Flag,Warning_Info_Flag;
extern uint32   positive_flag;
extern uint32   do_zer0_flag,do_tare_flag;
extern uint32   do_sam100_flag;
extern uint32   asd_var,lnt_var,prt_mod;

extern char*    software_version;
extern struct   KeyStruct Key[20];
extern uint32   clr_data,zer0_data,full_code,new_data,new_data_big,tare_data;
extern uint32   flag_new_data;
extern uint16   line_cal_para, user_cal_para;

extern uint32   point10_cal_start,point10_cal_ok,Point10_Cal_Buf[CAL_POINT_NUM+1]; //��ʵ��

extern uint32   offset_polar;
extern uint32   offset_50g,offset_100g,offset_150g;
extern uint32   expected_100g,expected_50g,expected_25g;

extern float    Point10_k[CAL_POINT_NUM-1];
extern float    percent_10_data;

extern uint32   point2_cal_start,point2_cal_ok,point2_cal_step;
extern uint32   inner_code_step;
extern uint32   auto_zer0_valid;
extern float    weigh_coef1; 

extern float    tare_weight,gross_weight,net_weight;  //�������
extern float    weight_per_pcs;                       //������     
extern uint32   pcs_number;        //����          
extern float    weight_compare_100,number_100;        //�ٷֱȵıȽϻ���

extern float    limit1_weight,limit2_weight;
extern uint32   animal_weight,animal_real_time,animal_time,animal_start,animal_ok;
extern float    peak_weight;
extern uint32   peak_start,peak_end;
extern float    total_weight;
extern uint32   total_times,do_totaladd_flag;
extern float    target_weight;
extern uint32   do_samtarget_flag;
extern float    density_temp,density_value,density_weight1,density_weight2;
extern uint32   density_start;
//////////////////////////////////
extern uint32   wireless_pin1_configuration,wireless_pin2_configuration;
//////////////////////////////////
extern float    temp_float,temp_cmp_point;
extern uint32   temp_cmp_flag;

////////////////////////////�����Զ������������
extern uint32   no_key_time;
/////////////////////////////////
extern uint8    speaker_volume,sound_switch;
extern uint32   which_para,password,model_id,user_id;
//////////////////////////////////
extern uint32   rec_total_num,rec_real_num,rec_total_pages,rec_curre_index;

/////////////////�����ͺſ�ѡ�� �������ı���
extern uint32   system_division,dot_position,inner_code_min,inner_code_max,adc_times;
extern float    additional_num,display_min_value,full_range,weight_alarm;/////////////////////////////////

extern uint8*   const machine_name[3][6]; 
//������У׼����
extern uint32   motor_run_time,motor_err_flag,point2_cal_flag,inner_cal_full,inner_cal_flag;

extern uint32   user_inncal_data;
//////////////////////////////////////////
//add printer function
extern uint8   print_busy,Uart_Printer_Flag;

extern uint8   logo_display_flag,model_display_flag;
extern uint8   repeat_check_flag,load_track_flag,adjust_0000_flag;

extern uint32  debug_data_flag,debug_disp_data1,debug_disp_data2;
/////////////////////////////////////////////////////////
extern void     Set_Relay_Oper(void);
extern uint32   Get_Line_data(uint32 weight1);
extern uint32   motor_run_proc(uint8 target_pos);
#endif