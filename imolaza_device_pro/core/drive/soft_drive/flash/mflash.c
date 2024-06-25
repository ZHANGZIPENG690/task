

#include "mflash.h"




extern stat_m m_remove_schedule_run_notice_node_all(void);

/**
 * @brief Initialize the data store
 */
stat_m m_callable_drive_flash_init(void)
{
    stat_m stat = fail_r;
    stat = m_ext_flash_init();
    return stat;
}

/**
 * @brief Use the key to remove the all
 * @todo Unit Input ...
 *
 * @return m_stat
 */
stat_m m_callable_drive_flash_remove_all(void)
{
    stat_m stat = fail_r;
    stat = m_ext_flash_remove_all();
    return stat;
}
/**
 * @brief Use the key to remove the factory
 * @todo Unit Input ...
 *
 * @return m_stat
 */
stat_m m_callable_drive_flash_factory_reset(void)
{
    stat_m stat = fail_r;
    stat = m_ext_flash_factory_reset();
    // m_remove_schedule_run_notice_node_all();
    return stat;
}
/**
 * @brief Use the key to remove the stored value
 * @todo Unit Input ...
 *
 * @param in_key The key to remove
 * @return m_stat
 */
stat_m m_callable_drive_flash_remove(const char *in_key)
{
    stat_m stat = fail_r;
    stat = m_ext_flash_remove(in_key);
    return stat;
}
char s[3] = "?";
/**
 * @brief Add a new value to the store, corresponding to the key passed in
 * @todo Unit Input ...
 *
 * @param in_type The type of value added
 * @param in_key The key of value added
 * @param in_data  The alue of value added
 * @return m_stat
 */
stat_m m_callable_drive_flash_write(enum m_paramete_enum in_type, const char *in_key, void *in_data)
{
    stat_m stat = fail_r;


    DEBUG_TEST(DB_W,"%s -> %s", in_key,  (in_type == M_TYPE_Str  ? (char *)in_data : s ));

    stat = m_ext_flash_write(in_type, in_key, in_data);
    return stat;
}

/**
 * @brief The value is obtained in the storage from the existing key
 * @todo Unit Out ...
 *
 * @param in_type The type to read
 * @param in_key The key to read
 * @param out_resp The value returned
 * @return m_stat
 */
stat_m m_callable_drive_flash_read(enum m_paramete_enum in_type, const char *in_key, void *out_resp)
{
    stat_m stat = fail_r;
    stat = m_ext_flash_read(in_type, in_key, out_resp);
    return stat;
}
/**
 * @brief 操作一个存储标记
 *
 * @param in_flag 句柄
 * @param in_status 设置/获取/删除
 * @return stat_m
 */
stat_m m_callable_flash_rescue_message_flag(const char *in_flag, enum m_stroage_rescue_message in_status)
{
    stat_m state = fail_r;
    uint8_t xx = 0;
    switch (in_status)
    {
    case M_GLO_STROAGE_RESCUE_MESSAGE_SET:
        DEBUG_TEST( DB_I,"---->%d (%s)", in_status, in_flag);
        xx = 1;
        state = m_callable_drive_flash_write(M_TYPE_U8, in_flag, &xx);
        break;
    case M_GLO_STROAGE_RESCUE_MESSAGE_GET:
        if ((state = m_callable_drive_flash_read(M_TYPE_U8, in_flag, &xx)) == succ_r)
            DEBUG_TEST( DB_I,"(%s)rescue_message_flag %d fined", in_flag, xx);
        break;
    case M_GLO_STROAGE_RESCUE_MESSAGE_DEL:
        state = m_callable_drive_flash_remove(in_flag);
        break;

    default:
        break;
    }
    return state;
}
