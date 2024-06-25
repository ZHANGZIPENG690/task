

#include "core/fset_lib.h"

// 以前版本的句柄
char mz_able[17][5] = {"PZ01", "PZ02", "PZ03", "PZ04", "PZ05", "PZ06", "PZ07", "PZ08", "PZ09", "PZ10", "PZ11", "PZ12", "PZ13", "PZ14", "PZ15", "PZ16", "PZ00"};

/**
 * @brief 兼容老设备并且等待完成
 *
 * @return stat_m
 */
stat_m m_callable_compatible_old_start_and_wait_done()
{
    stat_m stat = fail_r;
    uint64_t tmep_u64_aaa = 0, tmep_u64_bbb = 0;
    uint8_t temp_u8_aaa = 0;

    /** 跳过 全局 加载 */
    stat = m_callable_drive_flash_read(M_TYPE_U64, "AS", &tmep_u64_aaa);
    stat = m_callable_drive_flash_read(M_TYPE_U64, "AE", &tmep_u64_bbb);
    if (stat == succ_r)
    {
        m_callable_water_delay_set(tmep_u64_aaa, tmep_u64_bbb, 0);
        m_callable_drive_flash_remove("AS");
        m_callable_drive_flash_remove("AE");
    }
    /** 传感器 设置/配置 加载 */
    stat = m_callable_drive_flash_read(M_TYPE_U8, "SS0", &temp_u8_aaa);
    if (stat == succ_r)
    {
        switch (temp_u8_aaa)
        {
        case 11:
            m_callable_sensor_set(1, 1, true);
            break;
        case 21:
            m_callable_sensor_set(2, 1, true);
            break;
        }
        m_callable_drive_flash_remove("SS0");
    }

    /** 区域禁用  加载 */
    for (int i = 1; i <= 16; i++)
    {
        if (m_callable_flash_rescue_message_flag(mz_able[i], M_GLO_STROAGE_RESCUE_MESSAGE_GET) == succ_r)
        {
            m_callable_channel_manage_state_set(false, i, 1752000, 4093470397);
            m_callable_flash_rescue_message_flag(mz_able[i], M_GLO_STROAGE_RESCUE_MESSAGE_DEL);
        }
    }

    /** 休眠模式 加载 */
    

    return stat;
}
