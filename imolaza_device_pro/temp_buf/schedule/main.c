

#include "core/notify/notify.h"
#include "TestOut.h"
#include "core/schedule/mschedule.h"
#include "core/fset_lib.h"
#include <stdio.h>
#include "main.h"

#define TEST_CODE 0

m_watering_schedule wm_sch;
char t_var_str_a[100] = {0}; /** cahr []*/
char t_var_str_b[20] = {0};  /** cahr []*/
uint64_t t_var_u64_a = 0;    /** 计划id*/
uint64_t t_var_u64_b = 0;    /** 计划id*/
uint64_t t_var_u64_c = 0;    /** 计划id*/
int t_var_int_a = 0;         /** 计划id*/
uint8_t cu_smart;            /** 智能计划使用参数*/

uint64_t times = 0;

/**
 * @brief 添加一个计划
 *
 * @param in_info  计划
 * @param in_enable 计划启用还是禁用
 */
stat_m m_ext_schedule_add(m_watering_schedule *in_info, bool in_enable)
{
    stat_m stat = fail_r;

    if (in_enable)
    {
        DEBUG_TEST(DB_I,"schedule enable add .. ");
        /** 1、初始化*/
        {
            stat = m_callable_schedule_virtual_init(in_info);
        }

        /** 2、保存到运行链表*/
        {
            if (stat != fail_r)
                m_callable_schedule_list_add(in_info);
            else
                DEBUG_E("m_callable_schedule_virtual_init FAIL !!");
        }

        /** 3、保存到flash*/
        {
            if (stat != fail_r)
                m_splan_add_item(&in_info->sch_info);
            else
                DEBUG_E("m_callable_schedule_list_add FAIL !!");
            if (stat == succ_r)
                m_callable_add_notice_schedule_run_node(in_info->sch_info.cu_id, in_info->WSTime, m_schedule__idle, in_info);
            else
            {
                DEBUG_TEST(DB_W,"Plan is abnormal or special, Please check !");
                stat = succ_r;
            };
        }
    }
    // else
    // {
    //     stat = m_callable_schedule_virtual_init(in_info);
    //     stat = m_splan_add_item(&in_info->sch_info);
    //     if (stat == succ_r)
    //         m_callable_schedule_flag_disable(in_info->sch_info.cu_id);
    //     else
    //         /** Fail */;
    // }
    // printf("start add .. ");
    return stat;
}

void Test_add_schedule(char *msg)
{
    memset(&wm_sch, 0, sizeof(m_watering_schedule));
    m_insi_foramt_value("%lld", &wm_sch.sch_info.cu_id, msg);
    m_insi_foramt_value("%hhd", &wm_sch.sch_info.cu_smart, msg);
    m_insi_foramt_str(wm_sch.sch_info.cu_sack, msg);
    m_insi_foramt_value("%lld", &wm_sch.sch_info.available_time.tr_start_time, msg);
    m_insi_foramt_value("%lld", &wm_sch.sch_info.available_time.tr_end_time, msg);
    m_insi_foramt_str(wm_sch.sch_info.cu_temp_param, msg);
    m_insi_foramt_str(wm_sch.sch_info.cu_zone, msg);
    m_insi_foramt_value("%hhd", &wm_sch.sch_info.cu_run_interval_type, msg);
    m_insi_foramt_str(wm_sch.sch_info.cu_week_list, msg);
    m_insi_foramt_value("%lld", &t_var_u64_a, msg);
    if (m_ext_schedule_add(&wm_sch, t_var_u64_a) == succ_r)
        DEBUG_TEST(DB_W,"Success !");
    else
        DEBUG_TEST(DB_W,"Fail !");
}
void Test_time(uint32_t ins)
{
    // for (int i = 0; i <= ins; i++)
    {
        // times = i * 30;
        m_callable_query_notice_schedule_run_node(ins);
    }
}


int runnningEvenTestInput(enum mschedule_event mEventId , uint64_t tims)
{
    stat_m stat = fail_r;
    ScheduleEventTriggerEncry( mEventId ,tims);
    return stat;
}

void m_schedule_init()
{
    // Py_Initialize();
    m_callable_schedule_initial();
}
#include<time.h>
time_t seconds;
int _main()
{
  	seconds = time(NULL);    
    // printf("Start..\n");
    // char schedule[200] = "35,2,1&2,1010220800,1700225199,100 00:01,1#2;2#2,109,null,1]";
    DEBUG_OUT_TO_FILE("---------------------Main-Start----------------------%lld-" ,seconds);
    // printf("kkl");

    /** 初始化*/
    m_callable_schedule_initial();

    uint32_t arr[][2] = {{1, 120}, {2, 120}};

    /** 准备数据*/
    // Test_add_schedule(schedule);
    StartSchedule(35, 240, 0, 0 , 1 , 1 , 1 , 30, arr ,NULL);

    /** 时间推进*/
    // mTaskCreate(Test_time, "time",1024 ,NULL, 0, NULL);
    // while(1);
    Test_time(10 * 2);

    DEBUG_OUT_TO_FILE("---------------------Main-Run-End---------------------%lld--\n\n" ,seconds);
    return 0;
}


void test_BBB()
{
    uint8_t out_pc = 0;
    m_chan_managec channel[3] = {
        {
            .enable = 1,
            .id = 5,
            .shouldRunTime = 240,
        },
        {
            .enable = 1,
            .id = 6,
            .shouldRunTime = 240,
        },
        {
            .enable = 1,
            .id = 7,
            .shouldRunTime = 180,
        },
    };

for (size_t i = 0; i < 10; i++)
{
    channel[out_pc].realityRunTime += 60;
    printf("current :%hhd(%d:%d)" , channel[out_pc].id , channel[out_pc].realityRunTime , channel[out_pc].shouldRunTime);
    if(m_callable_zone_quick_run_switch_channel(channel, &out_pc, false, 3, 95000) != succ_r)
    {
        printf("Done ! \n");
        break;
    }
}


}





void the_program_begins()
{
        seconds = time(NULL);    
        DEBUG_OUT_TO_FILE("---------------------Main-Start----------------------%lld-" ,seconds);
}

void the_program_end(){
    DEBUG_OUT_TO_FILE("---------------------Main-Run-End---------------------%lld--\n\n" ,seconds);
}

stat_m m_ext_eigenfunction_should_skip(m_watering_schedule *in_wobj, int in_trigger_timing)
{
    return succ_r;
}
// stat_m m_cmd_finish()
// {

// }

// stat_m m_ext_schedule_switch_action()
// {
//     return 0;
// }

// stat_m m_cmd_finish_dir()
// {

// }

stat_m m_ext_flash_ft_init(void)
{
    stat_m stat = fail_r;

    return stat;
}

stat_m m_ext_flash_remove(const char *in_key)
{
    stat_m stat = fail_r;

    return stat;
}

stat_m m_ext_flash_write(int in_type, const char *in_key, char *in_data, uint64_t *out_resp_u64)
{
    return 0;
}

stat_m m_ext_flash_read(int in_type, const char *in_key, char *out_resp, void *out_resp_u64)
{
    return 0;
}

void print_zone_list(uint16_t sol ,uint64_t in_times)
{
    char abb[30] = {0};
    for (int i = 0; i < 16; i++)
    {
        if(sol & 0x01)
            strcat(abb,"1,");
        else
            strcat(abb,"0,");
        sol >>= 1;
    }
    // strcat(abb,"%lld"  in_times  );
    DEBUG_OUT_TO_FILE_COLOR("\033[0;33m  {%s" , abb);
}
uint16_t ContexSol = 0x0000;
void m_callable_contr_watering(uint8_t zone, bool opf,uint64_t in_times)
{
    zone -= 1;
    if(opf)
        ContexSol = ( 1 << zone );
    else 
        ContexSol &= (~( 1 << zone ));
    print_zone_list(ContexSol,in_times);
    DEBUG_OUT_TO_FILE_COLOR("%lld}\n" , in_times);
}
void m_callable_contr_watering_plus(uint8_t zone, bool opf, uint64_t in_times)
{
    zone -= 1;
    if(opf)
        ContexSol |= ( 1 << zone );
    else 
        ContexSol &= (~( 1 << zone ));
    DEBUG_OUT_TO_FILE_COLOR("\033[m ...\n *" );
    print_zone_list(ContexSol,in_times);
    DEBUG_OUT_TO_FILE_COLOR("%lld}\n" , in_times);
}
stat_m m_callable_flash_flag_rescue_message(char *in_flag, int in_status)
{
    return 0;
}
stat_m m_ext_eigenfunction_should_run(m_watering_schedule *in_obj, int in_trigger_timing)
{
    stat_m stat = fail_r; //         if (item->planinfo.desc_water_cycle_type == CYCLE_DAYS)

    stat = succ_r;
    return stat;
}
stat_m m_callable_spec_scene_net_connext_tx(const char *in_tx_data, int in_tx_len, uint64_t in_spec_cmd, uint64_t in_spec_cmd_flag, uint64_t in_timeout, bool in_tx_queue) {}
uint64_t m_callable_get_device_time_t(void)
{

    return times;
}
int Variables_Interrupt_Animation = 0;
