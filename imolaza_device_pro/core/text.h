

#ifndef __TEXT__H__
#define __TEXT__H__

extern const char *M_FLASH_KEY_DEVICE_TIME_TZ;
extern const char *M_FLASH_KEY_DEVICE_TIME;
extern const char *M_FLASH_KEY_RTC_TIME;
extern const char *M_FLASH_KEY_DEVICE_RUNNING_MODE;
/** http 代理服务器地址 */
extern const char *M_FLASH_KEY_DEVICE_HTTP_PROXY_ADDR;
/** 设备地址 */
extern const char *M_FLASH_KEY_DEVICE_LOCAL_INFO;
/** wifi */
extern const char *M_FLASH_KEY_DEVICE_WIFI_SSID;
extern const char *M_FLASH_KEY_DEVICE_WIFI_PSW;

// const char *M_FLASH_KEY_DEVICE_NEW_WIFI_PSW = "wifi_info";
/** 连接方式 */
extern const char *M_FLASH_KEY_DEVICE_CONNECT_OPTION;
/** SN */
extern const char *M_FLASH_KEY_DEVICE_SN;
/** Key */
extern const char *M_FLASH_KEY_DEVICE_EN_KEY;
// 泵模式标记
extern const char *M_FLASH_SOLEDOLD_MODE_IS_OPEN;
// 泵模式时间
extern const char *M_FLASH_SOLEDOLD_MODE_TIME;
/** Hammer */
extern const char *M_FLASH_KEY_DEVICE_WATER_HAMMER;
/** 重启原因 */
extern const char *RESTART_REUSE;

/////////////////////////////      flash          ///////////////////////////////////
/** IP 地址信息 */
extern const char *STATIC_IP_IN_FLAG;
/** IP  地址 */
extern const char *STATIC_IP_IN_IPADDR;
/** 位数 */
extern const char *STATIC_IP_IN_IPMASK;
/** 网关 */
extern const char *STATIC_IP_IN_IPGW;

/** 配网相关字段 */
/** JSON ssid 字段*/
extern const char *CONFIG_NET_OR_BIND_TEXT_SSID;
/** JSON psw 字段*/
extern const char *CONFIG_NET_OR_BIND_TEXT_PSW;
/** JSON type 字段*/
extern const char *CONFIG_NET_OR_BIND_TEXT_TYPE;
/** JSON token 字段*/
extern const char *CONFIG_NET_OR_BIND_TEXT_TOKEN;
/** JSON binding 字段*/
extern const char *CONFIG_NET_OR_BIND_TEXT_BINDING;

/** OTA_URL */
extern const char *OTA_URL;

/** OTA_COMPLETE */
extern const char *OTA_COMPLETE;

/** OTA_UPDATA_FAIL_COUNT */
const char *OTA_UPDATA_FAIL_COUNT;

#endif /* __TEXT__H__ */
