

#include "instance_fsm.h"

#if UNIT_TEST
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

#endif

struct base_fsm instance_fsm[6] =
    {
        {M_BASE_STATUS_IDLE, m_ext_instance_fsm__idle, NULL, M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE, 0},
        {M_BASE_STATUS_PREPARE, m_ext_instance_fsm__prepare, NULL, M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE, 0},
        {M_BASE_STATUS_RUNNING, m_ext_instance_fsm__run, NULL, M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE, 0},
        {M_BASE_STATUS_SACK, m_ext_instance_fsm__sack, NULL, M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE, 0},
        {M_BASE_STATUS_PAUSE, m_ext_instance_fsm__pause, NULL, M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE, 0},
        {M_BASE_STATUS_STOP, m_ext_instance_fsm__stop, NULL, M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE, 0},
};

enum base_state cur_state = M_BASE_STATUS_IDLE;
enum base_state pre_state = M_BASE_STATUS_IDLE;
enum base_state new_state = M_BASE_STATUS_IDLE;
bool is_update = false;
/** 正在运行的 标记下*/
uint64_t id_buf = 0;

void m_static_inst_fsm_seek(void);

stat_m m_static_instance_running_fsm_monitor(void)
{
    int index = new_state;
    // m_static_inst_fsm_seek();
    if (is_update)
    {
        // if (instance_fsm[i].current_state == new_state)
        {
            is_update = false;

            // m_static_time_link_notece_node_get_pre_ste(instance_fsm[new_state].data, &cur_state);
            cur_state = pre_state;

            if ((cur_state == M_BASE_STATUS_IDLE) && instance_fsm[new_state].current_state == M_BASE_STATUS_IDLE)
            {
                cur_state = M_STATIC_FSM_CHANGE_STATE_CAUSE_NATURE, cur_state = M_BASE_STATUS_PREPARE;
                index = 2;
            }
            if (instance_fsm[new_state].current_state == M_BASE_STATUS_PREPARE || cur_state == M_BASE_STATUS_STOP)
                cur_state = M_BASE_STATUS_IDLE;

            instance_fsm[index].base_fsm_fun(instance_fsm[new_state].change_cause,
                                             cur_state,
                                             instance_fsm[new_state].data,
                                             instance_fsm[new_state].time_s);
        }
    }
    return succ_r;
}

void m_static_inst_fsm_seek(void)
{

    for (int i = 0; i < 5; i++)
    {
        // if (instance_fsm[i].current_state == M_BASE_STATUS_RUNNING || instance_fsm[i].current_state == M_BASE_STATUS_SACK)
        {
            printf("%d  - %d\n", i, instance_fsm[i].current_state);
        }
    }
}

void m_static_inst_fsm_clear(void)
{
    for (int i = 0; i < 5; i++)
    {
        if (instance_fsm[i].current_state == M_BASE_STATUS_RUNNING || instance_fsm[i].current_state == M_BASE_STATUS_SACK)
        {
            // instance_fsm[i].current_state = M_BASE_STATUS_IDLE;
            pre_state = 0;
            pre_state = M_BASE_STATUS_IDLE;
            break;
        }
    }
}

stat_m m_static_instance_running_fsm_init(void)
{

    return succ_r;
}

stat_m m_static_instance_running_fsm_post(uint8_t event_fag, enum fsm_change_cause cause, enum base_state ste, void *data, uint64_t time_s)
{
    while (is_update)
        mDelay_ms(50);
    is_update = true;
    new_state = ste - M_PRIVATE_BASE_STATUS_FSM;
    instance_fsm[new_state].change_cause = cause;
    instance_fsm[new_state].data = data;
    instance_fsm[new_state].time_s = time_s;
    pre_state = event_fag;
#if UNIT_TEST
    m_static_instance_running_fsm_monitor();
#endif
    // printf("%d \n ", ste);
    return succ_r;
}

// #if UNIT_TEST
//     pthread_mutex_lock(&mutex);
// #endif