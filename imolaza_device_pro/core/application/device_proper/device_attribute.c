

#include "device_attribute.h"
#include <string.h>
static struct device_attribute _device;

/** 如果 0 老设备    1 新设备  */
extern int OLD_DEVICE_PEI;

stat_m m_static_device_sn_key_load(void);

stat_m m_static_device_attribute_load_local(void);

stat_m m_static_device_attribute_load_pump_mode(void);
/**
 * @brief Device water hammer configuration load
 *
 * @return stat_m
 */
stat_m m_static_device_attribute_load_hammer(void);
/**
 * @brief Loads device properties from flash，He calls the set function for various properties
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_init(void)
{
    stat_m stat = fail_r;

    /** 0、加载 KEY and SN
     *
     */
    m_static_device_sn_key_load();

    m_callable_midllle_rtc_init();

    m_static_device_attribute_load_local();
    m_callable_device_attribute_load_timezone_info();

    /** 1、 加载 flash 存的时间
     *         如果有存储时间 那就会按照 步骤3 直接连接WIFI/Bluetool模式
     *         没有那就是配网模式
     */
    _device.timestamp = 0;
    stat = m_callable_drive_flash_read(M_TYPE_U64, M_FLASH_KEY_DEVICE_TIME, (char *)&_device.timestamp);
    DEBUG_TEST(DB_I, "%d Flash Timestamp : %lld", stat, _device.timestamp);

    /** 默认无运行模式 */
    _device.device_running_mode = M_DEVICE_RUNNING_MODE_NONE;

    /** 2、 加载 rtc 存的时间
     *              这个时间只有在离线模式下 才有作用
     */

    // stat = m_callable_drive_flash_read(M_TYPE_U64, M_FLASH_KEY_RTC_TIME, (char *)&_device.rtc_time);
    stat = m_callable_midlle_rtc_get_time(&_device.rtc_time);
    DEBUG_TEST(DB_I, "RTC Time : %lld - Tz %d ", _device.rtc_time, _device.profile.timezone);
    if (_device.rtc_time > 0 && _device.rtc_time < 4917127147)
    {
        if (_device.profile.timezone == 0)
        {
            _device.timestamp = _device.rtc_time;
            _device.rtc_time = 0;
        }
        else
        {
            _device.timestamp = _device.rtc_time;
            m_callable_timer_manage_time_direct_set(_device.timestamp - _device.profile.timezone * 3600, _device.profile.timezone);
        }
    }

    /** 3、 加载 设备运行模式 （离线模式和在线运行模式）
     *          找到了  先确定是不是离线模式
     *          没找到  按照在线模式运行
     */

    // enum running_mode mode = _device.device_running_mode;
    stat = m_callable_drive_flash_read(M_TYPE_U32, M_FLASH_KEY_DEVICE_RUNNING_MODE, (int *)&_device.device_running_mode);

    /** 根据运行模式 分配 数据发送方式 */
    if (stat == succ_r)
    {
        DEBUG_TEST(DB_W, "M_DEVICE_RUNNING_MODE  %d .... ", _device.device_running_mode);
        /** 离线连接模式 - 蓝牙发送数据 */
        if (_device.device_running_mode == M_DEVICE_RUNNING_MODE_OFFLINE_DIRECT)
        {
            _device.profile.net_info.option = M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL;
        }
        /** 离线绑定模式 - 蓝牙发送数据 */
        else if (_device.device_running_mode == M_DEVICE_RUNNING_MODE_OFFLINE_CONFIG)
        {
            _device.profile.net_info.option = M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL;
        }
        /** 在线配网和连接模式 - TCP发送数据 */
        else if (_device.device_running_mode == M_DEVICE_RUNNING_MODE_CONFIG || _device.device_running_mode == M_DEVICE_RUNNING_MODE_DERECT)
        {
            _device.profile.net_info.option = M_NETWORK_MANAGE_CONNECT_MODE_TCP;
        }
    }
    else
    {
        DEBUG_TEST(DB_W, "M_DEVICE_RUNNING_MODE  %d .... %lld", _device.device_running_mode, _device.timestamp);
        if (_device.timestamp != 0 && m_callable_drive_flash_read(M_TYPE_U64, M_FLASH_KEY_DEVICE_TIME, (char *)&_device.timestamp) == succ_r)
        {
            _device.device_running_mode = M_DEVICE_RUNNING_MODE_DERECT;
            _device.profile.net_info.option = M_NETWORK_MANAGE_CONNECT_MODE_TCP;
            m_callable_device_attribute_running_save_mode(M_DEVICE_RUNNING_MODE_DERECT);
        }
    }

    /** 4、 加载 设备WIFI信息
     *      值得注意的是 如果上面的设备模式确定 但是找不到wifi信息  仍然会被确定为配置模式或者离线模式
     */

    if (_device.device_running_mode != M_DEVICE_RUNNING_MODE_OFFLINE_CONFIG && _device.device_running_mode != M_DEVICE_RUNNING_MODE_OFFLINE_DIRECT &&
        _device.device_running_mode != M_DEVICE_RUNNING_MODE_CONFIG)
    {
        stat = m_callable_drive_flash_read(M_TYPE_Str, M_FLASH_KEY_DEVICE_WIFI_SSID, (char *)_device.profile.net_info.name);
        if (stat == succ_r)
        {
            stat = m_callable_drive_flash_read(M_TYPE_Str, M_FLASH_KEY_DEVICE_WIFI_PSW, (char *)_device.profile.net_info.token);
            // 这里需要注意 如果没有密码会怎么办    后面可以测试下
            m_callable_network_set_net_info(_device.profile.net_info.name, _device.profile.net_info.token);
        }
        // _device.device_running_mode = M_DEVICE_RUNNING_MODE_DERECT;
    }

    // /** 4、 加载 连接方式的信息
    //  *      举例： tcp mqtt 蓝牙
    //  */

    // mode = _device.profile.net_info.option;
    // if (_device.device_running_mode != M_DEVICE_RUNNING_MODE_OFFLINE_CONFIG && _device.device_running_mode != M_DEVICE_RUNNING_MODE_OFFLINE_DIRECT)
    // {
    //     if (_device.hardware.hearware_version == DEVICE_HEARWARE_A003 || _device.hardware.hearware_version == DEVICE_HEARWARE_A002 ||
    //         _device.hardware.hearware_version == DEVICE_HEARWARE_A001 ||
    //         _device.hardware.hearware_version == DEVICE_HEARWARE_B12 ||
    //         _device.hardware.hearware_version == DEVICE_HEARWARE_C11 ||
    //         _device.hardware.hearware_version == DEVICE_HEARWARE_B11)
    //     {
    //         _device.profile.net_info.option = M_NETWORK_MANAGE_CONNECT_MODE_TCP;
    //         // _device.profile.net_info.option = M_NETWORK_MANAGE_CONNECT_MODE_MQTT;
    //     }

    //     // stat = m_callable_drive_flash_read(M_TYPE_Int, DEVICE_CONNECT_OPTION,  (int *)&_device.profile.wifi_info.option);
    //     // _device.profile.net_info.option = M_NETWORK_MANAGE_CONNECT_MODE_TCP;
    // }
    // else
    // {
    //     DEBUG_TEST(DB_W, "BLUETOOL");
    //     _device.profile.net_info.option = M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL;
    //     // m_callable_offline_bluetooth_running_mode_setting(M_DEVICE_RUNNING_MODE_CONFIG);
    // }
    // if (strcmp(_device.profile.app_version, KEP_VERSION_TCP_WOLFSSL) >= 0)
    //     _device.profile.net_info.option = M_NETWORK_MANAGE_CONNECT_MODE_TCP_WOLFSSL;
    // if (strcmp(_device.profile.app_version, KEP_VERSION_TCP_WOLFSSL) >= 0)
    // if (strcmp(_device.profile.app_version, KEP_VERSION_TCP_WOLFSSL) >= 0)
    //     _device.profile.net_info.option = M_NETWORK_MANAGE_CONNECT_MODE_TCP_MBEDTLS;
#if UNIT_TEST
    _device.profile.net_info.option = M_NETWORK_MANAGE_CONNECT_MODE_TCP;
#endif
    // _device.device_running_mode = M_DEVICE_RUNNING_MODE_CONFIG;
    m_callable_drive_flash_read(M_TYPE_Str, M_FLASH_KEY_DEVICE_HTTP_PROXY_ADDR, _device.profile.http_prosy);
    DEBUG_TEST(DB_W, "Running Mode %d  Connect Mode %d Http Proxy: %s", _device.device_running_mode, _device.profile.net_info.option, _device.profile.http_prosy);
    m_static_device_attribute_load_pump_mode();
    m_static_device_attribute_load_hammer();
    m_callable_skip_manage_init();
    m_callable_device_dormant_init();
    m_callable_schedule_stroage_load_all_from_flash();
    m_run_time.sensor_mode = 0;
    // char mac[40] = {0};
    // m_callable_device_attribute_get_mac(mac);

    return stat;
}
/**
 * @brief 设置设备的运行模式
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_running_mode_set_wifi_info(char *wifi_name, char *wifi_psw)
{
    stat_m stat = succ_r;
    strcpy(_device.profile.net_info.name, wifi_name);
    strcpy(_device.profile.net_info.token, wifi_psw);
    stat = m_callable_drive_flash_write(M_TYPE_Str, M_FLASH_KEY_DEVICE_WIFI_SSID, (char *)_device.profile.net_info.name);
    stat = m_callable_drive_flash_write(M_TYPE_Str, M_FLASH_KEY_DEVICE_WIFI_PSW, (char *)_device.profile.net_info.token);
    // 这里需要注意 如果没有密码会怎么办    后面可以测试下

    return stat;
}
/**
 * @brief 确定保存设备运行模式，保证下次设备灯光和连接方式
 *
 * @param mode 模式
 * @return stat_m
 */
stat_m m_callable_device_attribute_running_save_mode(int mode)
{
    return m_callable_drive_flash_write(M_TYPE_U32, M_FLASH_KEY_DEVICE_RUNNING_MODE, (int *)&mode);
}
/**
 * @brief 设置设备的运行模式
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_running_set_mode(int mode, int connection_option)
{
    stat_m stat = succ_r;
    _device.device_running_mode = mode;
    DEBUG_TEST(DB_I, "m_callable_device_attribute_running_set_mode (%d,%d)", mode, connection_option);
    _device.profile.net_info.option = connection_option;
    return stat;
}
/**
 * @brief 获取设备的运行模式
 *
 * @param out_rmode
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_running_mode(int *out_rmode)
{
    stat_m stat = fail_r;
    if (_device.device_running_mode != M_DEVICE_RUNNING_MODE_NONE)
    {
        stat = succ_r;
        *out_rmode = _device.device_running_mode;
        // printf("%d %d" , *out_rmode , _device.device_running_mode);
    }
    return stat;
}
/**
 * @brief 确定保存设备代理服务器地址，保证下次连接正常
 *
 * @param mode 模式
 * @return stat_m
 */
stat_m m_callable_device_attribute_http_proxy_save_addr(void)
{
    if (_device.profile.http_prosy_update)
    {
        m_callable_drive_flash_write(M_TYPE_Str, M_FLASH_KEY_DEVICE_HTTP_PROXY_ADDR, _device.profile.http_prosy);
        DEBUG_TEST(DB_I, "m_callable_device_attribute_http_proxy_saveet_addr (%s)", _device.profile.http_prosy);
    }
    _device.profile.http_prosy_update = false;
    return fail_r;
}
/**
 * @brief 设置设备的代理服务器地址
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_http_proxy_set_addr(char *in_http_proxy_addr)
{
    if (in_http_proxy_addr != NULL && strlen(in_http_proxy_addr) > 3)
    {
        memcpy(_device.profile.http_prosy, in_http_proxy_addr, strlen(in_http_proxy_addr));
        _device.profile.http_prosy_update = true;
    }
    DEBUG_TEST(DB_I, "m_callable_device_attribute_http_proxy_set_addr (%s)", _device.profile.http_prosy);
    return succ_r;
}
/**
 * @brief 获取设备的代理服务器地址
 *
 * @param out_rmode
 * @return stat_m
 */
stat_m m_callable_device_attribute_http_proxy_get_addr(char *out_addr)
{
    if (strlen(_device.profile.http_prosy) > 3)
    {
        memset(out_addr, 0, strlen(out_addr));
        memcpy(out_addr, _device.profile.http_prosy, strlen(_device.profile.http_prosy));
    }
    DEBUG_TEST(DB_E, "m_callable_device_attribute_http_proxy_get_addr (%s)", _device.profile.http_prosy);
    return succ_r;
}
/**
 * @brief 获取设备属性 连接模式
 *
 * @param out_rmode 输出的连接模式
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_net_connect_mode(int *out_rmode)
{
    stat_m stat = fail_r;
    stat = succ_r;
    if (out_rmode != NULL)
        *out_rmode = _device.profile.net_info.option;
    return stat;
}
/**
 * @brief The device restarts
 *
 */
stat_m m_callable_device_attribute_restart(void)
{
    stat_m stat = fail_r;
    return stat;
}
/**
 * @brief Device reset
 *
 */
stat_m m_callable_device_attribute_reset(void)
{
    stat_m stat = fail_r;
    m_callable_drive_flash_remove(M_FLASH_KEY_DEVICE_TIME);
    return stat;
}
/**
 * @brief 重置设备
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_all_clear(void)
{
    // return m_callable_device_attribute_reset();
    DEBUG_TEST(DB_W, "clear device info !");
    return m_callable_drive_flash_remove_all();
}

/**
 * @brief 加载地址
 *
 * @return stat_m
 */
char temp_char_arr[30] = {0};
stat_m m_static_device_attribute_load_local(void)
{
    stat_m stat = fail_r;
    stat = m_callable_drive_flash_read(M_TYPE_Str, M_FLASH_KEY_DEVICE_LOCAL_INFO, temp_char_arr);
    if (stat == succ_r)
    {
        DEBUG_TEST(DB_W, "Already loaded into device address information : %s", temp_char_arr);
        M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(temp_char_arr, "%lf,%lf,%d", &_device.profile.lon, &_device.profile.lat,
                                                  &_device.profile.timezone);
    }
    return stat;
}

#define M_SOLEN_FLAG "MSo"

/**
 * @brief 加载泵模式
 *
 * @return stat_m
 */
stat_m m_static_device_attribute_load_pump_mode(void)
{
    stat_m stat = fail_r;

    stat = m_callable_drive_flash_read(M_TYPE_U32, M_FLASH_SOLEDOLD_MODE_IS_OPEN, (void *)&_device.profile.pump_mode);   // 写入成功
    stat = m_callable_drive_flash_read(M_TYPE_U32, M_FLASH_SOLEDOLD_MODE_TIME, (void *)&_device.profile.pump_mode_time); // 写入成功
    if (stat == succ_r)
        DEBUG_TEST(DB_W, "Already loaded into pump mode: %d - time: %d", _device.profile.pump_mode, _device.profile.pump_mode_time);
    if (m_callable_flash_rescue_message_flag(M_SOLEN_FLAG, M_GLO_STROAGE_RESCUE_MESSAGE_GET) == succ_r)
    {
        DEBUG_TEST(DB_W, "Old : Pump Opened !");
        m_callable_device_attribute_set_soledold(2, 120);
        m_callable_flash_rescue_message_flag(M_SOLEN_FLAG, M_GLO_STROAGE_RESCUE_MESSAGE_DEL);
    }
    return stat;
}
/**
 * @brief 获取设备rtc的RTC时间
 *
 * @return uint64_t
 */
uint64_t m_callable_device_attribute_get_rtc_time(void)
{
    return _device.rtc_time;
}
/**
 * @brief 设备时间设置
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_time(uint64_t loical_time, int tz)
{
    stat_m stat = fail_r;
    uint64_t iou = 0;
    _device.timestamp = loical_time;
    m_callable_device_attribute_set_timezone(tz);
    DEBUG_TEST(DB_W, "m_callable_device_attribute_set_time5555555");
    _device.profile.timezone = tz;
    /** 如果模块没有存储过，那就存，减少读写次数 */
    if (m_callable_drive_flash_read(M_TYPE_U64, M_FLASH_KEY_DEVICE_TIME, &iou) == fail_r)
    {
        DEBUG_TEST(DB_W, "m_callable_device_attribute_set_time66666666");
        stat = m_callable_drive_flash_write(M_TYPE_U64, M_FLASH_KEY_DEVICE_TIME, (char *)&_device.timestamp);
        DEBUG_TEST(DB_W, "m_callable_device_attribute_set_time777777777");
    }

    return stat;
}
/**
 * @brief 加载设备时区
 *
 * @param tz
 * @return stat_m
 */
stat_m m_callable_device_attribute_load_timezone_info()
{
    char temp_char_tz[10] = {0};
    m_callable_drive_flash_read(M_TYPE_Str, M_FLASH_KEY_DEVICE_TIME_TZ, temp_char_tz);
    M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(temp_char_tz, "%d", &_device.profile.timezone);

    return 0;
}
/**
 * @brief 单独设置时区
 *
 * @param tz 时区
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_timezone(int tz)
{
    stat_m stat = fail_r;
    char temp_char_tz[10] = {0};
    if (m_callable_drive_flash_read(M_TYPE_Str, M_FLASH_KEY_DEVICE_TIME_TZ, temp_char_tz) == succ_r)
    {
        if (_device.profile.timezone != tz)
        {
            sprintf(temp_char_tz, "%d", tz);
            stat = m_callable_drive_flash_write(M_TYPE_Str, M_FLASH_KEY_DEVICE_TIME_TZ, temp_char_tz);
        }
    }
    else
    {
        sprintf(temp_char_tz, "%d", tz);
        stat = m_callable_drive_flash_write(M_TYPE_Str, M_FLASH_KEY_DEVICE_TIME_TZ, temp_char_tz);
    }
    return stat;
}
/**
 * @brief
 *
 * @param lon Longitude,
 * @param lat latitude
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_local(double lon, double lat, int tz)
{
    stat_m stat = fail_r;
    char ptr[35] = {0};
    sprintf(ptr, "%lf,%lf,%d", lon, lat, tz);
    _device.profile.lat = lat;
    _device.profile.lon = lon;
    _device.profile.timezone = tz;
    stat = m_callable_drive_flash_write(M_TYPE_Str, M_FLASH_KEY_DEVICE_LOCAL_INFO, (char *)ptr);
    m_callable_device_attribute_config_change_notify_event_callable(
        M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_LOCAL_CHANGE,
        NULL,
        m_callable_timer_manage_get_device_time());

    return stat;
}

/**
 * @brief 获取地址信息
 *
 * @param lon Longitude,
 * @param lat latitude
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_local(double *lon, double *lat)
{
    stat_m stat = succ_r;
    if (_device.profile.lon == 0 || _device.profile.lat == 0)
        stat = fail_r;
    else
    {
        stat = succ_r;
        if (lon != NULL && lat != NULL)
        {
            *lon = _device.profile.lon;
            *lat = _device.profile.lat;
        }
    }
    return stat;
}
/**
 * @brief 设备mac地址设置
 *
 * @param lon Longitude,
 * @param lat latitude
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_mac_addr(char *in_mac_addr)
{
    stat_m stat = fail_r;
    // memset(&_device, 0, sizeof(_device));
    strcpy(_device.hardware.mac, in_mac_addr);
    return stat;
}
/**
 * @brief 设备mac地址获取
 *
 * @param lon Longitude,
 * @param lat latitude
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_mac_addr(char *out_mac_addr)
{
    stat_m stat = fail_r;
    strcpy(out_mac_addr, _device.hardware.mac);
    return stat;
}
/**
 * @brief 设备mac地址设置
 *
 * @param lon Longitude,
 * @param lat latitude
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_mac_suffix_addr(char *in_mac_addr)
{
    stat_m stat = fail_r;
    strcpy(_device.hardware.macsuffix, in_mac_addr);
    return stat;
}
/**
 * @brief 设备mac地址获取
 *
 * @param lon Longitude,
 * @param lat latitude
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_mac_suffix_addr(char *out_mac_addr)
{
    stat_m stat = fail_r;
    strcpy(out_mac_addr, _device.hardware.macsuffix);
    return stat;
}
/**
 * @brief 设置当前的硬件芯片输出的类型
 *
 * @param in_out_chip
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_out_chip(int in_out_chip)
{
    stat_m stat = fail_r;
    _device.hardware.out_drive_chip = in_out_chip;
    return stat;
}
/**
 * @brief 获取当前的硬件芯片输出的类型
 *
 * @param out_out_chip 类型
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_out_chip(int *out_out_chip)
{
    stat_m stat = fail_r;
    *out_out_chip = _device.hardware.out_drive_chip;
    return stat;
}
/**
 * @brief 获取当前的硬件版本  A001
 *
 * @param out_out_chip 类型
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_hearward_version(uint32_t *out_hear_ware)
{
    stat_m stat = fail_r;
    *out_hear_ware = _device.hardware.hearware_version;
    return stat;
}
/**
 * @brief 设置应用程序版本信息
 *
 * @param in_app_version 输入
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_app_version(char *in_app_version)
{
    stat_m stat = fail_r;
    strcpy(_device.profile.app_version, in_app_version);
    return stat;
}
stat_m m_callable_device_attribute_get_app_version_ext(char *out_app_version)
{
    stat_m stat = fail_r;
    strcpy(out_app_version, _device.profile.app_version);
    if (_device.hardware.hearware_version != DEVICE_HEARWARE_A003)
    {
        out_app_version[2] -= '0';
    }
    return stat;
}
/**
 * @brief 获取应用程序版本信息
 *
 * @param out_app_version 输出
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_app_version(char *out_app_version)
{
    stat_m stat = fail_r;
    strcpy(out_app_version, _device.profile.app_version);
    // if (_device.hardware.hearware_version != DEVICE_HEARWARE_A003)
    // {
    //     out_app_version[2] -= 1;
    // }
    return stat;
}
/**
 * @brief 设置设备的mac地址
 *
 * @param in_mac
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_mac(char *in_mac)
{
    stat_m stat = fail_r;
    strcpy(_device.hardware.mac, in_mac);
    stat = m_callable_drive_flash_write(M_TYPE_Str, "re_mac", in_mac); // 写入成功
    return stat;
}
/**
 * @brief 获取设备的mac地址
 *
 * @param out_mac
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_mac(char *out_mac)
{
    stat_m stat = fail_r;
    stat = m_callable_drive_flash_read(M_TYPE_Str, "re_mac", out_mac); // 写入成功
    if (stat != fail_r)
        strcpy(_device.hardware.mac, out_mac);
    return stat;
}
/**
 * @brief 设置当前的sn吗
 *
 * @param out_app_version 输出
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_sn(char *in_sn)
{
    stat_m stat = fail_r;
    strcpy(_device.encrypt_code.sn, in_sn);
    stat = m_callable_drive_flash_write(M_TYPE_Str, M_FLASH_KEY_DEVICE_SN, in_sn); // 写入成功
    m_static_device_sn_key_load();
    return stat;
}
/**
 * @brief 获取当前的sn吗
 *
 * @param out_sn 输出
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_sn(char *out_sn)
{
    stat_m stat = fail_r;
    strcpy(out_sn, _device.encrypt_code.sn);
    return stat;
}
/**
 * @brief 获取当前的sn吗
 *
 * @param out_app_version 输出
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_key(char *out_sn_version)
{
    stat_m stat = fail_r;
    strcpy(out_sn_version, _device.encrypt_code.key);
    return stat;
}
/**
 * @brief 设置当前的key吗
 *
 * @param out_app_version 输出
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_key(char *in_key_version)
{
    stat_m stat = fail_r;
    strcpy(_device.encrypt_code.key, in_key_version);
    stat = m_callable_drive_flash_write(M_TYPE_Str, M_FLASH_KEY_DEVICE_EN_KEY, in_key_version); // 写入成功
    m_static_device_sn_key_load();
    return stat;
}
/**
 * @brief 判断当前WiFi是否和传入一致
 *
 * @param net_name
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_current_is_net_name_same(char *net_name)
{
    DEBUG_E("WiFi Update : %s <-  %s", net_name, _device.profile.net_info.name);
    if (strcmp(net_name, _device.profile.net_info.name) == 0)
        return succ_r;
    return fail_r;
}
/**
 * @brief 设置当前连接的wifi ssid
 *
 * @param out_app_version 输出
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_current_connect_net_name(char *net_name)
{
    stat_m stat = fail_r;
    strcpy(_device.profile.net_info.name, net_name);
    return stat;
}
/**
 * @brief 获取当前连接的wifi ssid
 *
 * @param out_app_version 输出
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_current_connect_net_name(char *net_name)
{
    stat_m stat = fail_r;
    char *rest = NULL;
    strcpy(net_name, _device.profile.net_info.name);
    while ((rest = strstr(net_name, ",")) != NULL)
    {
        m_callable_str_transf_str_resp(net_name);
    }
    return stat;
}

/**
 * @brief Device light settings
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_light(int a)
{
    // stat_m stat = fail_r;
    a = 10;
    return a;
}
/**
 * @brief Device sensor configuration settings
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_sensor(int a)
{
    // stat_m stat = fail_r;
    a = 0;
    return a;
}

/**
 * @brief Device channel configuration settings
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_hammer(int a)
{
    stat_m stat = fail_r; /** hammer */
    if (a)
        stat = m_callable_flash_rescue_message_flag(M_FLASH_KEY_DEVICE_WATER_HAMMER, M_GLO_STROAGE_RESCUE_MESSAGE_SET);
    else
        stat = m_callable_flash_rescue_message_flag(M_FLASH_KEY_DEVICE_WATER_HAMMER, M_GLO_STROAGE_RESCUE_MESSAGE_DEL);
    _device.profile.water_hammer = a;
    DEBUG_TEST(DB_W, "Setting Hammer open/close %d", _device.profile.water_hammer);
    return stat;
}
/**
 * @brief Device water hammer configuration load
 *
 * @return stat_m
 */
stat_m m_static_device_attribute_load_hammer(void)
{
    stat_m stat = fail_r;
    stat = m_callable_flash_rescue_message_flag(M_FLASH_KEY_DEVICE_WATER_HAMMER, M_GLO_STROAGE_RESCUE_MESSAGE_GET);

    if (stat == succ_r)
        _device.profile.water_hammer = 1;
    else
        _device.profile.water_hammer = 0;
    return stat;
}
/**
 * @brief Device water hammer configuration get
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_hammer(void)
{
    stat_m stat = fail_r;
    if (_device.profile.water_hammer)
        stat = succ_r;
    return stat;
}
/**
 * @brief Equipment pump configuration settings
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_soledold(int a, int in_time)
{
    stat_m stat = fail_r;
    DEBUG_TEST(DB_W, "setting pump mode: %d time: %d", a, in_time);
    stat = m_callable_drive_flash_write(M_TYPE_U32, M_FLASH_SOLEDOLD_MODE_IS_OPEN, (void *)&a);    // 写入成功
    stat = m_callable_drive_flash_write(M_TYPE_U32, M_FLASH_SOLEDOLD_MODE_TIME, (void *)&in_time); // 写入成功
    _device.profile.pump_mode = a;
    _device.profile.pump_mode_time = in_time;
    return stat;
}
/**
 * @brief Equipment pump configuration acquisition
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_soledold(int *out_mode, int *out_time)
{
    stat_m stat = fail_r;
    if (out_mode != NULL)
        *out_mode = _device.profile.pump_mode;
    if (out_time != NULL)
        *out_time = _device.profile.pump_mode_time;
    if (_device.profile.pump_mode > 0)
    {
        stat = succ_r;
        DEBUG_TEST(DB_W, "Use Pump !");
    }
    return stat;
}
/**
 * @brief Equipment pump configuration settings
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_sign_strage(int a)
{
    stat_m stat = fail_r;
    _device.profile.wifi_sign = a;
    return stat;
}
/**
 * @brief Equipment pump configuration settings
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_schedule_num(int a)
{
    stat_m stat = fail_r;
    _device.profile.schedule_sum = a;
    return stat;
}
/**
 * @brief Equipment pump configuration settings
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_sign_strage(int *a)
{
    stat_m stat = fail_r;
    *a = _device.profile.wifi_sign;
    return stat;
}
/**
 * @brief 改变设备的计划数量
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_change_schedule_num(int in_u_d)
{
    stat_m stat = fail_r;
    if (in_u_d == true)
        _device.profile.schedule_sum++;
    else if (in_u_d == 9)
        _device.profile.schedule_sum = 0;
    else
        _device.profile.schedule_sum--;
    return stat;
}
/**
 * @brief 获取设备的计划数量
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_schedule_num(int *a)
{
    stat_m stat = fail_r;
    *a = _device.profile.schedule_sum;
    return stat;
}
/**
 * @brief dormant mode settings
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_dormant(uint64_t start_time, uint64_t end_time)
{
    stat_m stat = fail_r;
    start_time = 0;
    end_time = 0;
    start_time = start_time + end_time;
    return stat;
}

/**
 * @brief Device light acquisition
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_light(int a)
{
    stat_m stat = fail_r;
    a = 10;
    return a;
}
/**
 * @brief Device sensor configuration acquisition
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_sensor(int a)
{
    stat_m stat = fail_r;
    a = 10;
    return a;
}

/**
 * @brief 获取当前的时区
 *
 * @param b 得到的时区
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_device_timezone(int *tz)
{
    *tz = _device.profile.timezone;
    return succ_r;
}
/**
 * @brief 获取设备类型
 *
 * @param b 得到的类型 A001
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_device_type(int *b)
{
    stat_m stat = fail_r;
    *b = _device.hardware.type;

    return *b;
}

stat_m m_static_device_sn_key_load(void)
{
    stat_m stat = fail_r;
    printf("oo 1 \n");
    // DEBUG_E("Not Found  Key Key !!   %s", _device.hardware.mac);
    memset(_device.encrypt_code.sn, 0, sizeof(_device.encrypt_code.sn));
    stat = m_callable_drive_flash_read(M_TYPE_Str, M_FLASH_KEY_DEVICE_SN, (void *)_device.encrypt_code.sn);
    if (stat != succ_r)
    {
        DEBUG_E("Not Found  Sn !!");
        // printf("Not Found  Sn !!\n");
    }
    // DEBUG_E("Not Found  Key Key !!   %s", _device.hardware.mac);

    memset(_device.encrypt_code.key, 0, sizeof(_device.encrypt_code.key));
    stat = m_callable_drive_flash_read(M_TYPE_Str, M_FLASH_KEY_DEVICE_EN_KEY, (void *)_device.encrypt_code.key);
    // stat = m_ext_flash_read_str(M_FLASH_KEY_DEVICE_EN_KEY, _device.encrypt_code.key);

    // DEBUG_E("Not Found  Key Key !!   %s", _device.hardware.mac);
    if (stat != succ_r)
    {
        DEBUG_E("Not Found  Key Key !!");
        // printf("Not Found  Key Key !!\n");
    }

    // strcpy(_device.encrypt_code.sn, "26220202A003");

    if (strstr(_device.encrypt_code.sn, STRA001) != NULL)
    {
        DEBUG_TEST(DB_I, "%s", STRA001);
        _device.hardware.hearware_version = DEVICE_HEARWARE_A001;
        _device.profile.net_info.option = M_NETWORK_MANAGE_CONNECT_MODE_TCP;
    }
    else if (strstr(_device.encrypt_code.sn, STRA002) != NULL)
    {
        DEBUG_TEST(DB_I, "%s", STRA002);
        _device.hardware.hearware_version = DEVICE_HEARWARE_A002;
        _device.profile.net_info.option = M_NETWORK_MANAGE_CONNECT_MODE_TCP;
        // OLD_DEVICE_PEI = 0;
    }
    else if (strstr(_device.encrypt_code.sn, STRA003) != NULL)
    {
        DEBUG_TEST(DB_I, "%s", STRA003);
        _device.hardware.hearware_version = DEVICE_HEARWARE_A003;
        // OLD_DEVICE_PEI = 1;
    }
    else
    {
        _device.hardware.hearware_version = ((_device.encrypt_code.sn[8] + 10 - 'A') * 16 * 16) + ((_device.encrypt_code.sn[9] - 48) * 16) + ((_device.encrypt_code.sn[10] - 48));
        DEBUG_TEST(DB_W, "Headware New : %d", _device.hardware.hearware_version);
        // OLD_DEVICE_PEI = 1;
    }

    if ((uint8_t)(_device.encrypt_code.sn[1] - 48) == 1)
        _device.hardware.type = DEVICE_1_ZONE;
    if ((uint8_t)(_device.encrypt_code.sn[1] - 48) == 2)
        _device.hardware.type = DEVICE_12_ZONE;
    if ((uint8_t)(_device.encrypt_code.sn[1] - 48) == 3)
        _device.hardware.type = DEVICE_6_ZONE;
    if ((uint8_t)(_device.encrypt_code.sn[1] - 48) == 4)
        _device.hardware.type = DEVICE_4_ZONE;
    if ((uint8_t)(_device.encrypt_code.sn[1] - 48) == 5)
        _device.hardware.type = DEVICE_18_ZONE;
    if ((uint8_t)(_device.encrypt_code.sn[1] - 48) == 6)
        _device.hardware.type = DEVICE_16_ZONE;
    if ((uint8_t)(_device.encrypt_code.sn[1] - 48) == 7)
        _device.hardware.type = DEVICE_24_ZONE;
    if ((uint8_t)(_device.encrypt_code.sn[1] - 48) == 8)
        _device.hardware.type = DEVICE_8_ZONE;
    if ((uint8_t)(_device.encrypt_code.sn[1] - 48) == 9)
        _device.hardware.type = DEVICE_30_ZONE;
    if ((uint8_t)(_device.encrypt_code.sn[1] - 48) == 0)
        _device.hardware.type = DEVICE_32_ZONE;

    DEBUG_TEST(DB_W, "%d -- %hhd SN: %s", _device.hardware.hearware_version, _device.hardware.type, _device.encrypt_code.sn);

    m_callable_device_attribute_set_out_chip(_device.encrypt_code.sn[0] - 48);

    return stat;
}

/**
 * @brief 把设备通信模式更改为蓝牙模式
 * @param void
 * @param connect_mode_get
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_mode_setting(enum connect_mode connect_mode_get)
{
    stat_m stat = succ_r;
    _device.profile.net_info.option = connect_mode_get;
    return stat;
}

/**
 * @brief 把设备运行模式更改为离线蓝牙模式
 * @param running_mode_get
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_running_mode_setting(enum running_mode running_mode_get)
{
    stat_m stat = succ_r;
    _device.device_running_mode = running_mode_get;
    stat = m_callable_drive_flash_write(M_TYPE_U32, M_FLASH_KEY_DEVICE_RUNNING_MODE, (int *)&_device.device_running_mode);
    return stat;
}

/**
 * @brief 获取设备通信模式
 * @param running_mode_get
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_running_mode_getting(enum running_mode *running_mode_get)
{
    stat_m stat = succ_r;
    stat = m_callable_drive_flash_read(M_TYPE_U32, M_FLASH_KEY_DEVICE_RUNNING_MODE, (int *)&_device.device_running_mode);
    *running_mode_get = _device.device_running_mode;
    return stat;
}
