//**********************************************************************
//�ļ���:filter.c
//�����б�
//1   �ֶ��ƻ��ȶ�״̬  stable_manual_break
//2   �˲���ʼ��        filter_init
//3   ����              filter_sort
//4   ȡƽ��ֵ          stable_filter_average
//5   ȡƽ��ֵ1         stable_filter_average1
//6   ȡƽ��ֵ2         stable_filter_average2
//7   ȡƽ��ֵ3         stable_filter_average3
//8   �˲�����          filter(void)
//9   ����Զ�����      auto_zer0
//10  �غ��Զ�����      stable_auto_zer0_A
//10a �غ��Զ�����      stable_auto_zer0_B
//11  �ظ����Զ�����    repeat_adjust
//12  �����˲��������  Get_Filter_Data
//
//**********************************************************************
#include "filter.h"
#include "ad_acquire.h"
#include "stdlib.h"
#include "global.h"

 static uint32   stable_times;                 //�ȶ���ʱ��
   
 static uint32   Buf_Total1[AD_AVER_CNT],Buf_Total2[AD_AVER_CNT],Buf_Total3[AD_AVER_CNT];
 
 static uint32   AD_Buf_Raw1[AD_AVER_CNT1],ad_buf_index1,flag_ad_buf_full1,fast_filter_flag;
 static uint32   AD_Buf_Raw2[AD_AVER_CNT2],ad_buf_index2,flag_ad_buf_full2;
  
 static uint32  filter_data_final;
 static uint32  raw_data,Average_AD_A,Average_AD_B;
        
 uint32 flag_auto_track_enable,auto_track_cnt;  //����������¶��������Զ����ٺ��غɸ���
 uint32 flag_load_track_enable,load_track_cnt; 
/////////////////////////////////////////////////////////////
//1 �˺�����У׼��ʱ����ã���Ϊ�ƻ�ƽ��
/////////////////////////////////////////////////////////////
void  stable_manual_break(void)
{
 stable_times  = 0;
 stable_flag = FALSE; 
}   
//////////////////////////////////
//2 �˲���ʼ��
//////////////////////////////////////     
void  filter_init(void)
{
 uint32 i;
 //for(i=0;i<AD_SORT_BUF_CNT;i++)
 // Buf_Sort1[i] = 0;
 for(i=0;i<AD_AVER_CNT;i++)
   {
    Buf_Total1[i] = 0;
    Buf_Total2[i] = 0;
    Buf_Total3[i] = 0;
   } 
   
 for(i=0;i<AD_AVER_CNT1;i++)
  AD_Buf_Raw1[i] = 0;
 for(i=0;i<AD_AVER_CNT2;i++)
  AD_Buf_Raw2[i] = 0;

  ad_buf_index1     = 0;
  flag_ad_buf_full1 = 0;
  ad_buf_index2     = 0;
  flag_ad_buf_full2 = 0;

}

//********************************************************
uint32  get_buf_data(void)
        { //��ǰ������������ȡ���������Сֵ���ƽ��ֵ
           //���û����  ����ȡ �������ݵ�ƽ��ֵ
           uint32  buf[AD_AVER_CNT1];
           uint32  sum;
           uint8   pass,i;
           if(1 == flag_ad_buf_full1)
            {
                for(i=0;i<AD_AVER_CNT1;i++)
                 buf[i] = AD_Buf_Raw1[i];
                
                for(pass = 0;pass < AD_AVER_CNT1 - 1 ;pass++)
	              {
		            for( i = 0;i < (AD_AVER_CNT1 - 1 - pass);i++)
		             {
			           if(buf[i] > buf[i+1])
			             {
			               sum = buf[i];
		   	               buf[i] = buf[i+1];
			               buf[i+1] = sum ;
			             }
		             }
	              }
	         sum = 0;  
	         for(i=4;i<12;i++)   
	         sum += buf[i];
	         return(sum/8);
	        }
           else
            {
              sum = 0;
              for(i=0;i<ad_buf_index1;i++)
               sum += AD_Buf_Raw1[i];         
 	          return(sum/ad_buf_index1); 
            }
         }     
             
/////////
//********************************************************
uint32  get_buf_data2(void) //
        {
        
           uint32  buf[AD_AVER_CNT];
           uint32  sum;
           uint8   pass,i;
           
            for(i=0;i<AD_AVER_CNT;i++)
              buf[i] = Buf_Total3[i];
                 
            for(pass = 0;pass < AD_AVER_CNT - 1 ;pass++)
	              {
		            for( i = 0;i < (AD_AVER_CNT - 1 - pass);i++)
		             {
			           if(buf[i] > buf[i+1])
			             {
			               sum = buf[i];
		   	               buf[i] = buf[i+1];
			               buf[i+1] = sum ;
			             }
		             }
	              }
	         sum = 0;  
	         for(i=4;i<12;i++)   
	         sum += buf[i];
	         return(sum/8);
	      
         } 
//////////////////////////////////////     
static uint32  filter_Average2(uint32 raw1)
 { 
  //��RAW ���ݷ��ڻ����У���ȡƽ��ֵ
  uint32 i,sum;
   
  AD_Buf_Raw2[ad_buf_index2++] = raw1;
  
  if(AD_AVER_CNT2 == ad_buf_index2)
     {
      flag_ad_buf_full2 = 1;
      ad_buf_index2 = 0;
     }
  ///////////////////////////////////////����
  if(1 == flag_ad_buf_full2)
    {  
     sum = 0;
     for(i=0;i<AD_AVER_CNT2;i++)
     sum+=AD_Buf_Raw2[i];
     return(sum/AD_AVER_CNT2);
    }
  else
    {
     sum = 0;
     for(i=0;i<ad_buf_index2;i++)
     sum+=AD_Buf_Raw2[i];
     return(sum/ad_buf_index2); 
    }   
 }     
/*****************************************************************
*5  ȡƽ��ֵ1
*���г���Ϊ16
/*****************************************************************/  
static uint32  filter_total1(uint32 data1)
  {
  
    static uint32   Buf_data1_count = 0;
	static uint32   Buf_data1_count_H = 0;
	static uint32   Buf_index1 = 0;
	       uint32   data1_tmp;
	  
      //////////////////////////////////////////////ȡ���16������֮��
       Buf_data1_count = Buf_data1_count + data1; //�ۼӱ���������
       if(Buf_data1_count < data1) //���
        {       
         Buf_data1_count_H++;
        }
                                                   
       data1_tmp = Buf_data1_count;
       Buf_data1_count = Buf_data1_count - Buf_Total1[Buf_index1]; //��ȥ�ϴ�����
       if(Buf_data1_count > data1_tmp)
         {       
          Buf_data1_count_H--;
         }
     
       Buf_Total1[Buf_index1++] = data1;    //�����ݱ���
       if(AD_AVER_CNT == Buf_index1)
       Buf_index1 = 0;      
 
     /////////////////////////////��һ�λ����˲�
	 data1_tmp = ((Buf_data1_count_H&0xff)<<24)|((Buf_data1_count&0xffffff00)>>8); //*16����256 �൱�ڳ���16
     return(data1_tmp);
  }    
/*****************************************************************
*5  ȡƽ��ֵ1
*���г���Ϊ16
/*****************************************************************/  
static uint32  filter_total2(uint32 data2)
  {
      static uint32   Buf_data2_count = 0;
      static uint32   Buf_index2 = 0;
	  
      //////////////////////////////////////////////ȡ���16������֮��
      Buf_data2_count = Buf_data2_count + data2; //�ۼӱ���������
                                                 //��ȥ�ϴ�����  
      Buf_data2_count = Buf_data2_count - Buf_Total2[Buf_index2];  
      
      Buf_Total2[Buf_index2++] = data2;    //�����ݱ���
      if(AD_AVER_CNT == Buf_index2)
      Buf_index2 = 0;      
     /////////////////////////////�ڶ��λ����˲� 
     return(Buf_data2_count/AD_AVER_CNT);    
  } 
 /*****************************************************************
*5 ����� ȡ�м�8�����ݵ�ƽ��ֵ
*  ���г���Ϊ16
/*****************************************************************/  
static uint32  filter_total3(uint32 data3)
  {
             uint32   total_tmp;
      static uint32   Buf_index3 = 0;
      static uint32   flag_full  = 0;
	  
	  Buf_Total3[Buf_index3++] = data3;
	  if(AD_AVER_CNT == Buf_index3)
        {
         Buf_index3 = 0;
         flag_full = 1;
        }
      //////////////////////////////////
      if(0==flag_full)
      return(data3);
      else
      return(get_buf_data2());     
  }       
//////////////////////////////////////////////////////  
//8B �µ��˲��㷨 
//�ź�������̣�
//��ʼ�׶Σ���һ����������������½�
//��һ�����������ٻص���Ȼ����Χ��ĳ������������
//
///////////////// 
#define NOT_VALID_VALUE                     8     //6����
#define NOVALID_ACCEP_TIME                  2      //������ĳ������仯2�κ���Ͽ�����Ч����

/////////////////////////////////////////////////////////////////////////////////////////
//�������ݱ仯����������ر��������ݵ�����ֵ
////////////////////////////////////////////////////////////////////////////////////////
static uint32  filter0_proc(void)
{
  static uint32 raw_data1_last,raw_data2_last;
  static uint32 direct_add_cnt,direct_sub_cnt;  //�仯����
  static uint32 filter0_stable_cnt = 0;         //�ȶ�����
         uint32 data_tmp;
  
         uint32 differ_value;
  
  data_tmp = Get_Raw();
  //ʹ����ԭʼ�������жϲ��ȶ���ʹ������˲��������ж� �ȶ�
  if(abs(raw_data1_last-data_tmp) > 50000)
   {
       stable_flag        = 0;
       stable_times       = 0;
	   filter0_stable_cnt = 0;
	   //��Щ���� ��ӿ���ʾ������
	   fast_filter_flag    = 1;	  
	   ad_buf_index1       = 0;
	   flag_ad_buf_full1   = 0;  
	   ad_buf_index2       = 0;
	   flag_ad_buf_full2   = 0;
	   //�ƻ��������ݣ����¿�ʼ���� 
       //���±��������ݴ��������� ������ �� �غɸ��� 	 
       flag_auto_track_enable = 0;  //����������¶��������Զ����ٺ��غɸ���
       auto_track_cnt         = 0;
       flag_load_track_enable = 0;
       load_track_cnt         = 0; 
   }
  
  raw_data1_last = data_tmp;
  
  data_tmp   = filter_total1(data_tmp);      //ֱ�ӵõ���Ҫ�����ԭʼ���� ����16
  data_tmp   = filter_total2(data_tmp);       //��ȡ���16�εĻ���ƽ��ֵ
  raw_data   = filter_total3(data_tmp);       //16�������м�8λ���ݵ�ƽ��ֵ
    
  differ_value = abs(raw_data - raw_data2_last); 
  /////////////////////////////////////////////////////
  if(differ_value > NOT_VALID_VALUE*inner_code_step)   //�仯���ݴ���xx������
    {  
       stable_flag        = 0;
       stable_times       = 0;
	   filter0_stable_cnt = 0;
	   //��Щ���� ��ӿ���ʾ������
	   fast_filter_flag    = 1;	  
	   ad_buf_index1       = 0;
	   flag_ad_buf_full1   = 0;  
	   ad_buf_index2       = 0;
	   flag_ad_buf_full2   = 0;
	   //�ƻ��������ݣ����¿�ʼ���� 
       //���±��������ݴ��������� ������ �� �غɸ��� 	 
       flag_auto_track_enable = 0;  //����������¶��������Զ����ٺ��غɸ���
       auto_track_cnt         = 0;
       flag_load_track_enable = 0;
       load_track_cnt         = 0; 
       
       raw_data2_last = raw_data;
     }
   else
     { //���仯С��һ����ֵʱ��
       filter0_stable_cnt++;
        raw_data2_last = raw_data;
       ///////////////////////////////////���ȶ���ʹ����������ֵ��Ȩ��Ϊ��������  
       if(filter0_stable_cnt < 4)       //�˲������Ե���
            {
             raw_data2_last = raw_data;
            }
       else if(filter0_stable_cnt < 16)
            {
             raw_data2_last = (raw_data*9+raw_data2_last)/10; 
            } 
       else if(filter0_stable_cnt < 24)
            {
             raw_data2_last = (raw_data*8+raw_data2_last*2)/10;    //����
            } 
       
       //�����ȶ�ʱ�� �ñ�־
       if(filter0_stable_cnt > 2)
            {
             direct_add_cnt   = 0;
	         direct_sub_cnt   = 0;    
             fast_filter_flag = 0;  //�˲����濪ʼ������
            }        
      }
     return raw_data2_last;
   } 
     
     
///////////////////////////////////////////////////////////////////
//         �˲��㷨
//
//(1) ȡ����ƽ��ֵ             1/16
//(2) ȡ����ƽ��ֵ             1/16
//(3) ȡ������ƽ��ֵ 8��ȡ�м�4��
//(4) ȡƽ��ֵ                  1/8
//
/*////////////////////////////////////////////////////////////////////
void  filter_a(void)
{     
      static uint32 ad_stable_cnt;
      uint32 ad_sample_dat;
      
      ad_sample_dat = filter0_proc();      //������ڿ��ٱ仯�׶�
      ///////////////////////////////////////////////////////////
      if(1==fast_filter_flag)
         {
          stable_flag    = FALSE;
          ad_stable_cnt  = 0;
          Average_AD_A   = ad_sample_dat;  //���ٱ仯�׶�
          Average_AD_B   = Average_AD_A;
          filter_data_final = Average_AD_B;         
 	     }     
 	  else
 	   {
 	     /////////////////////////////////////////////////
 	     if(0==flag_ad_buf_full1) //���ν������滹û�������
 	         {
 	          AD_Buf_Raw1[ad_buf_index1++] = ad_sample_dat;////save 
             }
          else
             {
              ad_stable_cnt++;
           
              // ������� �ٿ�ʼ�ȶ���ʱ�� ����ʱ����ȡ�����ݵ�Ȩ�� 
              if(ad_stable_cnt<4)
                 {
                   if(0 == ad_buf_index1)
 	                 AD_Buf_Raw1[ad_buf_index1++] = (ad_sample_dat*9+AD_Buf_Raw1[AD_AVER_CNT1-1])/10;
 	               else
 	                 AD_Buf_Raw1[ad_buf_index1++] = (ad_sample_dat*9+AD_Buf_Raw1[ad_buf_index1-1])/10;     
                 }
              else //if(ad_stable_cnt<6) 
                 {
                   if(0 == ad_buf_index1)
 	                 AD_Buf_Raw1[ad_buf_index1++] = (ad_sample_dat*8+AD_Buf_Raw1[AD_AVER_CNT1-1]*2)/10;
 	               else
 	                 AD_Buf_Raw1[ad_buf_index1++] = (ad_sample_dat*8+AD_Buf_Raw1[ad_buf_index1-1]*2)/10;  
                 }
             
 	          }
 	      	  ////////////////////////////////////�õ�������  
 	          if(AD_AVER_CNT1 == ad_buf_index1)
 	           {
 	            flag_ad_buf_full1 = 1; 
                ad_buf_index1 = 0;
               }   
 	          Average_AD_A = get_buf_data();   //��ȡ�������м����ݵ�ƽ��ֵ
 	          /////////////////////////////////////////////////////////////////////
 	          ////////////////////////////////////////////////////Ȼ����ȡһ��ƽ��ֵ
 	          Average_AD_B = filter_Average2(Average_AD_A);
 	          ////////////////////////////////////////////////�ж��ȶ��Լ� �����Զ����ٹ��� 
              if(abs(filter_data_final-Average_AD_B) < 2*inner_code_step)
 	           {
 	             stable_times++;
 	             if((FALSE==point10_cal_ok)||(TRUE==point2_cal_start))
 	               {
 	                 if(stable_times > 14)
 	                    stable_flag = TRUE;   
 	               }
 	             else  
 	               {
 	                if(stable_times > 14)
 	                   stable_flag = TRUE;
 	               } 
 	           }
              else
               {
                 stable_times = 0;
               }
               filter_data_final = Average_AD_B;
              //////////////////////////////////////////////////////// 
              if(TRUE == stable_flag)       //�ȶ������Զ����ٹ���
                 flag_auto_track_enable = 1;
              else
                {
                 flag_auto_track_enable = 0;
                 auto_track_cnt         = 0;
                } 
 	          //�غɸ��ٵ�����(1) �ȶ�
 	          //              (2) ��һ������������ 
 	          //              (3) �����û�У׼������
 	          //              (4) ����У׼���������õ�ʱ��
              if((stable_flag==TRUE)&&(TRUE==point10_cal_ok)&&(FALSE==point2_cal_start)) //�ȶ�һ��ʱ���Ҳ����κ�У׼״̬
                flag_load_track_enable = 1;
              else
               {
                flag_load_track_enable = 0;
                load_track_cnt         = 0;
               }              
         }         
 }
*/ 
///////////////////////////////////////////////////////////////////////////////
void  filter(void)
{     
  static uint32 raw_data1_last,raw_data2_last;
  static uint32 direct_cnt1,direct_cnt2;      
  static uint32 filter0_stable_cnt = 0;         //�ȶ�����
         uint32 data_tmp;
  
         uint32 differ_value;
  
  data_tmp = Get_Raw();
  //ʹ����ԭʼ�������жϲ��ȶ���ʹ������˲��������ж� �ȶ�
  if(abs(raw_data1_last-data_tmp) > 50000)
   {
       stable_flag        = 0;
       stable_times       = 0;
       direct_cnt1 = 0;
       direct_cnt2 = 0;
       ///////////////////////////////////
       flag_ad_buf_full2 = 0;
       ad_buf_index2 = 0;
	   //�ƻ��������ݣ����¿�ʼ���� 
       //���±��������ݴ��������� ������ �� �غɸ��� 	 
       flag_auto_track_enable = 0;  //����������¶��������Զ����ٺ��غɸ���
       auto_track_cnt         = 0;
       flag_load_track_enable = 0;
       load_track_cnt         = 0; 
   }
  
  raw_data1_last = data_tmp;
  data_tmp   = filter_total1(raw_data1_last);
  direct_cnt1++;
  if(direct_cnt1>15)
   {
    direct_cnt2++;
    Average_AD_A = data_tmp;
   } 
  else
   {
    direct_cnt2 = 0; 
    //Average_AD_A = raw_data1_last/16;
    Average_AD_A = data_tmp;
   }
  
  data_tmp   = filter_total2(Average_AD_A);  //��ȡ���16�εĻ���ƽ��ֵ
  
  if((direct_cnt2>15)||(abs(filter_data_final-Average_AD_B)<10*inner_code_step))
  Average_AD_B = data_tmp;
  else
  Average_AD_B = Average_AD_A;
  ////////////////////////////////����ƽ��ֵ�˲�
  data_tmp = filter_Average2(Average_AD_B);
  
  ////////////////////////////////////////////////////////////// 
  if(abs(filter_data_final-data_tmp) < inner_code_step)
    {
 	 stable_times++;
 	 if(current_mode==STAT_CALING_INNER)
 	     {
 	      if(stable_times > 30)
 	         stable_flag = TRUE;   
 	     }
 	 else  
 	     {
 	      if(stable_times > 10)
 	         stable_flag = TRUE;
 	     } 
 	}
   else
    {
     stable_times = 0;
    }
   //////////////////////////////////////////
    filter_data_final = data_tmp;
   //////////////////////////////////////////////////////// 
       if(TRUE == stable_flag)       //�ȶ������Զ����ٹ���
            flag_auto_track_enable = 1;
       else
           {
            flag_auto_track_enable = 0;
            auto_track_cnt         = 0;
           } 
    
        ////////////////////////////////////////////////�ж��ȶ��Լ� �����Զ����ٹ��� 
             
 	          //�غɸ��ٵ�����(1) �ȶ�
 	          //              (2) ��һ������������ 
 	          //              (3) �����û�У׼������
 	          //              (4) ����У׼���������õ�ʱ��
              if((stable_flag==TRUE)&&(TRUE==point10_cal_ok)&&(FALSE==point2_cal_start)) //�ȶ�һ��ʱ���Ҳ����κ�У׼״̬
                flag_load_track_enable = 1;
              else
               {
                flag_load_track_enable = 0;
                load_track_cnt         = 0;
               }                       
 } 
///////////////////////////////////////////
//9  ����Զ����� ��������������
//////////////////////////////////////////
void   auto_zer0(uint32 weight1)
{
  uint32 x;
  
  auto_track_cnt++;
  if(16 == auto_track_cnt)
     {
        auto_track_cnt = 0;
        //if(data_new < AUTO_0_SETUP * inner_code_step)
        x = abs(weight1-zer0_data);
        if(x < 5*inner_code_step)
          {
           auto_zer0_valid = TRUE;   //�Զ�������Ч  
           zer0_data = weight1;      //�����������Ƿ���Ҫ����    
          } 
       /* else if(x < AUTO_0_SETUP*inner_code_step)
          {
            if(weight1>zer0_data)                             
              zer0_data = zer0_data + inner_code_step;      //ʹ������У׼ǰ������������
            else
              zer0_data = zer0_data - inner_code_step;      //ʹ������У׼ǰ������������
          }*/
     }
}
///////////////////////////////////////////
//10  �غ��ȶ��������Զ�����,
//����֤�ȶ���ı仯��ʩ�Ӹ����
//////////////////////////////////////////

 ///////////////////////////////////////////
//�غɸ��ٹ��� ʵ��
//////////////////////////////////////////

uint32  stable_load_zer0(uint32 weight2)
	{	
     
           uint32   ad_tmp;          // 
    static uint32   lock_ad_last;     
    
    if(weight2 < 100*inner_code_step) 
      {
       lock_ad_last = 0;
       return(weight2);
      }
      
    if(0==load_track_cnt)
       { //������???��? ����?����????��D??��2��
      
         if(abs(weight2-lock_ad_last) > (3 * inner_code_step)) //???��D??��2��3?1y3????��???
           {                                                //?��D?��?��???��?
            lock_ad_last = weight2;
           }
         else
           {
            lock_ad_last = (lock_ad_last*3+weight2*7)/10;        //D?����3??��????��?��
           }  
         load_track_cnt++;
        }
    else
       {   
        ///////////////////////////////////////////////       
        load_track_cnt++;
        if(load_track_cnt > 12)    //??o��?��������??��?��
          {//??o��?����������??��?��?
            load_track_cnt = 1; 
            ad_tmp = abs(weight2-lock_ad_last);   //����????��??����?��??������ 
            
            if(ad_tmp > (2*inner_code_step+inner_code_step/2))
               {  //??��?��??����?��?�䨮��?��???o��?��������?���� ??��y?Y2?����DT?y
                lock_ad_last = weight2;
                //flag_load_track_enable = 0;
                //load_track_cnt = 0;            
               }
            else     
               {    //?a???��????��? ��y?Y��??����??��?����?��?��?��??��??��???��??����?2??��?
                    //��?��??1??��D???��?��?�� �̡�??��?��? ??��?�̡�??1????
                if(weight2 > lock_ad_last) 
                   zer0_data = zer0_data + ad_tmp;
                else
                   zer0_data = zer0_data - ad_tmp;
               }
          }//////////////����???D??OVER      
        } ////////////////
       
       return(lock_ad_last);
	}
	
///////////////////////////////////////////////////////////
//
const  float ntc_table[]=
{ //-5     0     5      10     15
 41.033,29.884,25.121,19.805,15.693,
 //20     25    30      35     40
 12.496,10.000,8.0413,6.4972,5.2743,
 //45     50    55      60     65
 4.3013,3.4970,2.8993,2.3959,1.9882,
 //70     75    80      85     90
 1.6567,1.3850,1.1641,0.9814,0.8304,
 //95    100    105
 0.7052,0.5840,0.5140,0.0001
};

const  uint8 ntc_start[]=
{0,//��Ч����
 0 ,5, 10,15,20,25,30,35,
 40,45,50,55,60,65,70,75,
 80,85,90,95,100,105
};

static uint16 ntc_buf[32];

/////////////////////////////////////////////////////////////////////
void filter_init_lpc2478ad(void)
 {
  uint8 i;
  for(i=0;i<32;i++)
     ntc_buf[i] = 0;
    
 }
 
 //////////////////////////////////////////
//********************************************************
void  filter_ad_NTC(uint16 ntc_data)
         { //��ǰ������������ȡ���������Сֵ���ƽ��ֵ
           //���û����  ����ȡ �������ݵ�ƽ��ֵ
           uint32  sum;
           float   x;
           uint8   i;
           static  uint8 index = 0;
           
           ntc_buf[index++] = ntc_data;
           if(32 == index)
           {
            index = 0;
            ////////////////////////////////////////
	        sum = 0;
	        for(i=0;i<32;i++)
	        sum+=ntc_buf[i];  
	        sum = sum/32;
	        /////////////////////////////////���ݴ����ݵ��Ƴ�����ʽ���
	        x = (10*sum)/(1024.00001-sum);
	        ////////////////////////////////���ݴ˵���ֵ������¶�
	        if(x>=ntc_table[0]) //�¶�̫С
	          
	          temp_float = TEMP_SENSOR_NC_VALUE;
	          
	        else if(x<=ntc_table[22]) 
	          
	          temp_float = TEMP_SENSOR_NC_VALUE;
	          
	        else 
	         {
	          i = 1;
	          while(1)
	           {
	            if(x>ntc_table[i])
	               break;
	            else
	             i++;   
	           }
	          //////////////////�����¶� 
	          x =ntc_start[i]-(x-ntc_table[i])/(ntc_table[i-1]-ntc_table[i])*5;
	          
	          if(x>5)
	            temp_float = x - 5;
	         }  
	      }
	        
	 }        
///////////////////////////////////////////////////////////////////
	
	
//////////////////////////////////////////////
//12  �����Զ����ٺ������
//////////////////////////////////////////////
uint32 Get_Raw1_Data(void)
{
return(raw_data);
}
uint32 Get_Ave1_Data(void)
{
 return(Average_AD_A);
}
uint32 Get_Ave2_Data(void)
{
 return(Average_AD_B);
}

uint32 Get_Filter_Data(void)
{
 return(filter_data_final);
}
