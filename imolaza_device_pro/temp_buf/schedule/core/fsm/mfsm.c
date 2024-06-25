/*
    计划运行状态机

    将计划运行的各种事件通知整理到应用层     状态具体有

        计划就绪态：在请求了浇水时间发现需要执行之后的计划是就绪态         A
        计划运行态：计划运行中的状态，包含开始、切换。                    B
        计划浸泡态：计划经过计算之后需要浸泡的状态、                      C
        计划停止态：计划运行完成之后，将要结束的状态                      D
        计划空闲态：每个计划运行中必须经历的状态，主要是完成计划运行的清理工作，和计算下一次运行时间    E

        A 的下一个状态一定是   B
        B 的下一个状态是  C   或者  D
        C 的下一个转台一定是   B
        D 的下一个状态一定是   E
        E 的下一个状态时  A(正常)   B(恢复)   C(恢复)
    事件具体有
        计划进入就绪事件
        计划进入运行事件
        计划进入浸泡事件
        计划进入将要停止事件
        计划进入空闲状态事件

*/
// extern struct schedule_run_notice_mem;

// #include "tool.h"
// #include "bsp_hw.h"
// #include " wifi/conn_eth.h"
// #include " fun/fun.h"
// #include " fun/text.h"
#include "core/fset_lib.h"

typedef enum
{
    m_schedule_fsm_prepare = 0,
    m_schedule_fsm_start,
    m_schedule_fsm_running,
    m_schedule_fsm_sack,
    m_schedule_fsm_pause,
    m_schedule_fsm_stop,
    m_schedule_fsm_idle
} m_fsm_status;

struct m_schedule_cbc_fsm_tel
{
    m_fsm_events m_trig_current_event;

    m_fsm_status m_trig_current_state;

    // m_fsm_event_handle_fun acti_fun_handle;

    m_fsm_status trig_next_state;

    bool action;

    int sub_event_id;

    uint64_t  action_time;

    void *pdata;
};

// #define Transform(num) (num == 
//m_schedule__prepare ? 0 : ( num == m_schedule__start ? 1 : (num == m_schedule__sack ? 2 : (num == m_schedule__running ? 3 : (num ==  m_schedule__pause ? 4 : (num == m_schedule__stop ? 5 : 6))))))

// int (*trig_acti_fun_handle[5])(struct schedule_run_notice_mem *mem) = {0};

/**
 * @brief function
 *
 */
m_fsm_event_handle_fun m_event_handle_fun;

/**
     m_schedule__prepare = 0, // 29-24
    m_schedule__running,  =1   // 24-x
    m_schedule__start,    =2   // x-254
    m_schedule__will_stop,=3    // x-50
    m_schedule__idle ,    =4
    m_schedule__sack      =5

    M_FSM_EVENT_PREPARE_RUN = 10,
    M_FSM_EVENT_WILL_START,
    M_FSM_EVENT_RUNNING,
    M_FSM_EVENT_SACKING,
    M_FSM_EVENT_PAUSE,
    M_FSM_EVENT_STOP,

 */
struct m_schedule_cbc_fsm_tel m_schedule_cbc_fsm_tel_table[] =
    {
        //{到来的事件，当前的状态，将要要执行的函数，下一个状态}
        {M_FSM_EVENT_PREPARE_RUN, m_schedule_fsm_prepare, m_schedule_fsm_running, false, NULL},

        {M_FSM_EVENT_WILL_START, m_schedule_fsm_start, m_schedule_fsm_sack, false, NULL},

        {M_FSM_EVENT_RUNNING, m_schedule_fsm_running, m_schedule_fsm_sack, false, NULL},

        {M_FSM_EVENT_SACKING, m_schedule_fsm_sack, m_schedule_fsm_running, false, NULL},

        {M_FSM_EVENT_PAUSE, m_schedule_fsm_pause, m_schedule_fsm_prepare, false, NULL},

        {M_FSM_EVENT_STOP, m_schedule_fsm_stop, m_schedule_fsm_prepare, false, NULL},
        
        {M_FSM_EVENT_WATER_HAMMER, m_schedule_fsm_running, m_schedule_fsm_running, false, NULL},

        {M_FSM_EVENT_HANGUP, m_schedule_fsm_running, m_schedule_fsm_running, false, NULL},

        {M_FSM_EVENT_HANGUP_RECOVER, m_schedule_fsm_running, m_schedule_fsm_running, false, NULL},
        // 更新状态
        {M_FSM_EVENT_PRE, m_schedule_fsm_running, m_schedule_fsm_running, false, NULL},

        {M_FSM_EVENT_NEXT, m_schedule_fsm_running, m_schedule_fsm_running, false, NULL},
};

stat_m m_callable_schedule_fsm_init(m_fsm_event_handle_fun in_trig_acti_fun_handle)
{
    stat_m stat = fail_r;
    // for (int i = 0; i < 5; i++)
    // {
    //     if (in_trig_acti_fun_handle[i] != NULL)
    //     {
    //         trig_acti_fun_handle[i] = in_trig_acti_fun_handle[i];
    //         m_schedule_cbc_fsm_tel_table[i].acti_fun_handle = trig_acti_fun_handle[i];
    //     }
    //     else
    //     {
    //         stat = fail_r;
    //         break;
    //     }
    // }
    if (in_trig_acti_fun_handle != NULL)
    {
        stat = succ_r;
        m_event_handle_fun = in_trig_acti_fun_handle;
    }

    return stat;
}

void m_schedule_fsm_event_loop(void *args)
{
    while (1)
    {
        for (size_t i = 0; i <= 5; i++)
        {
            if (m_schedule_cbc_fsm_tel_table[i].action)
            {
                DEBUG_TEST(DB_I,"fsm :%d", 555);
                m_schedule_cbc_fsm_tel_table[i].action = false;
                if (m_event_handle_fun != NULL)
                    m_event_handle_fun(m_schedule_cbc_fsm_tel_table[i].m_trig_current_event, m_schedule_cbc_fsm_tel_table[i].sub_event_id, m_schedule_cbc_fsm_tel_table[i].pdata ,m_schedule_cbc_fsm_tel_table[(i)].action_time);
                // m_schedule_cbc_fsm_tel_table[i].trig_acti_fun_handle(m_schedule_cbc_fsm_tel_table[i].pdata);
            }
        }
        mDelay_ms(100);
    }
}
void m_schedule_fsm_enable(void)
{
    // mTaskCreate(m_schedule_fsm_event_loop, "m_fsm_event_loop", 1024 * 3, (void *)7, 2, NULL);
}



int tran(int num)
{
    int result = 0;
   
    return result ;
}
stat_m m_callable_schedule_fsm_post_event(m_fsm_events evt, int in_sub_event_id,  void *pdata , uint64_t in_present_time)
{
    stat_m stat = fail_r;
    long i =0;
    // if (m_schedule_cbc_fsm_tel_table[evt].acti_fun_handle != NULL)、
        // DEBUG_TEST(DB_I,"fsm :%d",tran(evt) );
    if (pdata != NULL)
    {
        // evt = Transform(evt);
        DEBUG_TEST(DB_I,"fsm :%d",(evt) );
        m_schedule_cbc_fsm_tel_table[(evt)].pdata = pdata;
        m_schedule_cbc_fsm_tel_table[(evt)].sub_event_id = in_sub_event_id;
        m_schedule_cbc_fsm_tel_table[(evt)].action = true;
        m_schedule_cbc_fsm_tel_table[(evt)].action_time = in_present_time;
        m_event_handle_fun(m_schedule_cbc_fsm_tel_table[(evt)].m_trig_current_event,m_schedule_cbc_fsm_tel_table[(evt)].sub_event_id, m_schedule_cbc_fsm_tel_table[(evt)].pdata ,m_schedule_cbc_fsm_tel_table[(evt)].action_time);

    }else printf("NULL !!");
    return stat;
}
