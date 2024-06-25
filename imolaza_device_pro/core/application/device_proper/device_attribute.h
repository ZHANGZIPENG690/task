
/**
 * @file device_attribute.h
 * @brief xcv
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @note You should need a global device property struct
 * @date 2023-07-10
 */

#ifndef __DEVICE_ATTRIBUTE__H__
#define __DEVICE_ATTRIBUTE__H__

#include "core/fset_lib.h"
#include "core/text.h"
#include "../../drive/soft_drive/flash/mflash.h"

/**
 * @brief Device hardware-related parameters, Device information
 */
struct hardware_info
{
    /**  Device Mac Addr  */
    char mac[15];
    /**  Device Mac Identification  */
    char macsuffix[5];

    /**  Control chip model  */
    int out_drive_chip;
    /**  production order direct labor cost summary A001 A002 */
    uint32_t hearware_version;
    /**  production order direct labor cost summary  */
    uint32_t generations;
    /**  Device Type (4/6/8/12/16)  */
    uint8_t type;
};

/**
 * @brief Device adv info
 */
struct baseinfo
{
    /**  Local cache address exists / updated */
    bool local;
    /**  Debug Mode */
    bool debug;
    /**  Time Zone */
    int timezone;
    /**  Wifi sign  */
    int wifi_sign;
    /**  The number of cache schedule */
    uint16_t schedule_sum;
    /**  Light brightness level */
    uint32_t led_level;
    /**  Longitude  */
    double lon;
    /**  Latitude */
    double lat;
    /**  水锤开启 */
    int water_hammer;
    /**  泵模式 */
    int pump_mode;
    /**  泵模式时间 */
    int  pump_mode_time;
    /** http代理服务器地址 */
    char http_prosy[50];
    /** http代理服务器地址 更新需要 */
    char http_prosy_update;
    /**  Firmware Version  */
    char app_version[32];
    /** 上一次的 wifi 信息 */
    struct connect_option net_info;
    /** 上一次的服务器连接信息 */
    struct connect_option server_info;
};

struct device_attribute
{
    /** 设备存储在flash */
    uint64_t timestamp;
    /** rtc 中的时间 */
    uint64_t rtc_time;

    /**   SN/KEY Code*/
    SN_KEY_CODE encrypt_code;

    /**   hardware Code*/
    struct hardware_info hardware;
    /** vase info --> local ... */
    struct baseinfo profile;

    /** 设备运行模式 比如 直接连接 配网 蓝牙GATT*/
    enum running_mode device_running_mode;
};

/**
 * @brief Loads device properties from flash，He calls the set function for various properties
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_init(void);

/**
 * @brief The device restarts
 *
 */
stat_m m_callable_device_attribute_restart(void);
/**
 * @brief Device reset
 *
 */
stat_m m_callable_device_attribute_reset(void);

/**
 * @brief
 *
 * @param lon Longitude,
 * @param lat latitude
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_local(double lon, double lat,int tz); /*  设备地址设置  */

/**
 * @brief Device light settings
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_light(int);
/**
 * @brief Device sensor configuration settings
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_sensor(int);

/**
 * @brief Device channel configuration settings
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_hammer(int);
/**
 * @brief Device water hammer configuration get
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_hammer(void);
/**
 * @brief Equipment pump configuration settings
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_soledold(int a , int in_time);
/**
 * @brief Equipment pump configuration get
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_soledold(int *out_mode, int *out_time);
/**
 * @brief dormant mode settings
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_dormant(uint64_t start_time, uint64_t end_time);

/**
 * @brief
 *
 * @param lon Longitude,
 * @param lat latitude
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_local(double *x, double *y); /*  设备地址获取  */
/**
 * @brief Device light acquisition
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_light(int);
/**
 * @brief Device sensor configuration acquisition
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_sensor(int);



/**
 * @brief
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_device_type(int *);

/**
 * @brief 获取设备运行模式
 *
 * @param out_rmode 运行模式输出
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_running_mode(int  *out_rmode);
/**
 * @brief 设置直连模式
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_running_set_mode(int mode,int connection_option);
/**
 * @brief 确定保存设备运行模式，保证下次设备灯光和连接方式
 * 
 * @param mode 模式
 * @return stat_m 
 */
stat_m m_callable_device_attribute_running_save_mode(int mode);
/**
 * @brief 设置设备的运行模式
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_running_mode_set_wifi_info(char *wifi_name, char *wifi_psw);

/**
 * @brief 重置设备
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_all_clear(void);

/**
 * @brief 设备mac地址设置
 *
 * @param lon Longitude,
 * @param lat latitude
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_mac_addr(char *in_mac_addr);
/**
 * @brief 设备mac地址获取
 *
 * @param lon Longitude,
 * @param lat latitude
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_mac_addr(char *out_mac_addr);
/**
 * @brief 设备mac地址设置
 *
 * @param lon Longitude,
 * @param lat latitude
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_mac_suffix_addr(char *in_mac_addr);
/**
 * @brief 设备mac地址获取
 *
 * @param lon Longitude,
 * @param lat latitude
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_mac_suffix_addr(char *out_mac_addr);
/**
 * @brief 设置当前的硬件芯片输出的类型
 *
 * @param in_out_chip
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_out_chip(int in_out_chip);
/**
 * @brief 获取当前的硬件芯片输出的类型
 *
 * @param out_out_chip 类型
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_out_chip(int *out_out_chip);
/**
 * @brief 设置应用程序版本信息
 *
 * @param in_app_version 输入
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_app_version(char *in_app_version);
/**
 * @brief 获取应用程序版本信息
 *
 * @param out_app_version 输出
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_app_version(char *out_app_version);
stat_m m_callable_device_attribute_get_app_version_ext(char *out_app_version);


/**
 * @brief 获取当前的sn吗
 *
 * @param out_app_version 输出
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_sn(char *out_sn_version);
/**
 * @brief 获取当前连接的wifi ssid
 *
 * @param out_app_version 输出
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_current_connect_net_name(char *net_name);
/**
 * @brief 判断当前WiFi是否和传入一致
 * 
 * @param net_name 
 * @return stat_m 
 */
stat_m m_callable_device_attribute_get_current_is_net_name_same(char *net_name);
/**
 * @brief 获取当前的硬件版本  A001
 *
 * @param out_out_chip 类型
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_hearward_version(uint32_t *out_hear_ware);


/**
 * @brief 获取当前的时区
 *
 * @param b 得到的时区
 * @return stat_m
 */
extern stat_m m_callable_device_attribute_get_device_timezone(int *tz);


/**
 * @brief 设置设备的mac地址
 * 
 * @param in_mac 
 * @return stat_m 
 */
stat_m m_callable_device_attribute_set_mac(char *in_mac);
/**
 * @brief 获取设备的mac地址
 * 
 * @param out_mac 
 * @return stat_m 
 */
stat_m m_callable_device_attribute_get_mac(char *out_mac);



/**
 * @brief Equipment pump configuration settings
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_sign_strage(int a);
/**
 * @brief Equipment pump configuration settings
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_set_schedule_num(int a);
/**
 * @brief Equipment pump configuration settings
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_sign_strage(int *a);
/**
 * @brief Equipment pump configuration settings
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_schedule_num(int *a);
/**
 * @brief 改变设备的计划数量
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_change_schedule_num(int in_u_d);
/**
 * @brief 获取地址信息
 *
 * @param lon Longitude,
 * @param lat latitude
 * @return stat_m
 */
stat_m m_callable_device_attribute_get_local(double *lon, double *lat);
/**
 * @brief 获取设备rtc的RTC时间
 * 
 * @return uint64_t 
 */
uint64_t m_callable_device_attribute_get_rtc_time(void);



/**
 * @brief 确定保存设备代理服务器地址，保证下次连接正常
 *
 * @param mode 模式
 * @return stat_m
 */
stat_m m_callable_device_attribute_http_proxy_save_addr(void);
/**
 * @brief 设置设备的代理服务器地址
 *
 * @return stat_m
 */
stat_m m_callable_device_attribute_http_proxy_set_addr(char *in_http_proxy_addr);
/**
 * @brief 获取设备的代理服务器地址
 *
 * @param out_rmode
 * @return stat_m
 */
stat_m m_callable_device_attribute_http_proxy_get_addr(char *out_addr);

#endif /** __DEVICE_ATTRIBUTE__H__ */
