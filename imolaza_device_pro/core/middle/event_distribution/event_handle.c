

#include "core/fset_lib.h"
#include "event_distribution.h"

/** 记录计划运行的开始时间 */
uint64_t temp_u64_schedule_time_count;
int device_running_mode = 0;
/** 离线恢复计划计数 */
int off_recover_schedule = 0;
/** 时间框架 */
static int st_tim_s[6] = {0};
/**
 * @brief RTC 模式不请求 30
 *
 * @param in_flag  设置或者获取的 FLAG
 * @return true
 * @return false
 */
extern bool m_static_recover_instance_rtc_mode_flag_set_get(int in_flag);

char data_m[5];
bool is_addr = true;
/**
 * @brief 时间相关的事件处理
 *
 * @param event_id 30 s 事件   10 s 事件   1 min 事件
 * @return stat_m
 */
stat_m m_static_event_handle(enum event_distribution_id event_id)
{

    stat_m stat = fail_r;
    int resp = 0;
    int device_state;
    m_callable_device_proper_status_get(&device_state);
    m_callable_device_attribute_get_running_mode(&device_running_mode);

    if (device_running_mode == M_DEVICE_RUNNING_MODE_OFFLINE_CONFIG ||
        device_running_mode == M_DEVICE_RUNNING_MODE_NONE ||
        device_running_mode == M_DEVICE_RUNNING_MODE_CONFIG)
        return fail_r;

    uint64_t in_time_s = m_callable_timer_manage_get_device_time();
    switch (event_id)
    {
    case M_EVENT_TIME_6_SEC:
        // DEBUG_TEST( DB_I,6S");//  m_callable_wifi_auto_connect();}
        // if (mDeviceSystemTime < 12000)
        //   {
        //     DEBUG_TEST( DB_I,6S");  m_callable_wifi_auto_connect();}

        m_callable_network_keep_active();
        break;

    case M_EVENT_TIME_30_SEC:
        m_callable_wifi_auto_connect();

        // m_callable_wifi_auto_connect();
        break;
    case M_EVENT_TIME_1_MIN:

    { /** 获取当前时间 */
        resp = m_callable_timer_manage_get_hhmmss();
        m_callable_timer_manage_get_ymd_hms(in_time_s, st_tim_s);
        m_callable_system_printf_heap_free_info(__FUNCTION__, __LINE__);
        DEBUG_TEST(DB_W, "--------> (%d)  %d-%02d-%02d:%02d-%02d-%02d (%d)", resp, st_tim_s[0], st_tim_s[1], st_tim_s[2], resp / 3600, (resp % 3600) / 60, st_tim_s[5], resp % 60);
    }
        // 地址检测
        if (is_addr && (m_callable_device_attribute_get_local(NULL, NULL) == fail_r && device_state == M_DEVICE_GLOBAL_STATUS_IDLE && device_running_mode != M_DEVICE_RUNNING_MODE_CONFIG)) // if()
        {
            is_addr = false;
            resp = M_CMD_NOTIFY_TO_DEVICE_REQUEST_LOCAL_INFO;
            m_callable_local_resp_to_remote(resp, M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, M_TYPE_NULL, NULL, M_TYPE_NULL, NULL, in_time_s, true);
        }

        /** 计划运行，超过运行时间 请求更新天气数据 */
        if (device_state == M_DEVICE_GLOBAL_STATUS_SCHEDULE_RUNNING && temp_u64_schedule_time_count == 0)
        {
            temp_u64_schedule_time_count = in_time_s;
        }
        else if (device_state == M_DEVICE_GLOBAL_STATUS_SCHEDULE_RUNNING &&
                 temp_u64_schedule_time_count > in_time_s &&
                 temp_u64_schedule_time_count - in_time_s / 3600 >= 45)
        {
            m_callable_instance_running_event_input(M_INSTANCE_RUNNING_UPDATE_WEATHER, in_time_s);
        }
        else if (device_state != M_DEVICE_GLOBAL_STATUS_SCHEDULE_RUNNING)
            temp_u64_schedule_time_count = 0;

        /**离线时间计数 */
        off_recover_schedule++;

        // 离线恢复计划   和   6小时一次 同步时间
        if (m_callable_device_attribute_get_rtc_time() >= 1000)
        {
            if (device_running_mode != M_DEVICE_RUNNING_MODE_CONFIG)
            {
                DEBUG_TEST(DB_W, "off_recover_schedule : %d", off_recover_schedule);
                if (off_recover_schedule == 10)
                {
                    m_static_recover_instance_rtc_mode_flag_set_get(true);
                    memcpy(data_m, "[16]", 4);
                    m_callable_network_data_rx(data_m, 4);
                }
                if (off_recover_schedule % (6 * 60) == 0)
                    off_recover_schedule = 360,
                    m_callable_local_resp_to_remote(M_CMD_TWOWAY_SET_DEVICE_TIME, M_TYPE_NULL, NULL, M_TYPE_NULL, NULL, M_TYPE_NULL, NULL, M_TYPE_NULL, NULL, in_time_s, false);
            }
        }
        break;
    case M_EVENT_TIME_45_MIN:
        break;

    default:
        break;
    }
    return stat;
}
