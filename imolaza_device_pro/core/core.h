/**
 * @file core.h
 * @brief 状态接口
 * @author 毛威鹏、陈前治
 * @version 1.0.0
 * @date 2023-05-10
 */
// #include </home/ornai/anaconda3/include/python3.10/Python.h>

#ifndef __CORE__H__
#define __CORE__H__
#include <stdint.h>
#include <stdbool.h>
#include "../main/core/config.h"

/** unknow */
#define CONFIG_NEW_CHANNEL_INFO 0

// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_log.h"
// #include "hot_so_fast.h"
#if NEW_BREANCH_15001
#include "tool.h"
// #include "esp32/fun/fun.h"
#endif /** NEW_BREANCH_15001*/
/**  uint8_t */
typedef unsigned char uint8_t;
/**  uint16_t */
typedef unsigned short uint16_t;
/**  uint32_t */
typedef unsigned int uint32_t;
/**  uint64_t */
#ifndef uint64_t
// typedef unsigned int uint64_t;
#endif
/**  bool */
// typedef unsigned char bool;

/** true */
#define true 1
/** false */
#define false 0
/** IM */
#define DEVICE_DEBUG "im"

/** Max limit channel */
#define M_LIMIT_CHANNEL_MAX_NUM 16

#define MAX_SHORT_CURRENT 1400
/** None */
#define None 254
/** one day time sum */
#define NUM_OF_DAY_SEC_SUM 86400

/** malloc */
#define M_MALLOC malloc
/** free */
#define M_FREE free

extern const int M_CONST_SACK_NUM;
extern const int M_CONST_NONE_NUM;
/** IO File */
#include <stdio.h>

#define UNIT_TEST 0
// #define OLD_DEVICE_PEI 1

#define KEP_VERSION_TCP_WOLFSSL "8.5.010"

#define DEVICE_HEARWARE_A001 1
#define DEVICE_HEARWARE_A002 2
#define DEVICE_HEARWARE_A003 3
#define DEVICE_HEARWARE_A004 4
#define DEVICE_HEARWARE_A113 0xa13
#define DEVICE_HEARWARE_B11 0xb11
#define DEVICE_HEARWARE_B12 0xb12
#define DEVICE_HEARWARE_C10 0xc10
#define DEVICE_HEARWARE_C11 0xc11
#define DEVICE_HEARWARE_D11 0xd11

/** Timeout 2 day */
#define M_TIMEOUT_LONG_LIMIT_MIN 2 * 24 * 60
/** Timeout 35 min*/
#define M_TIMEOUT_35_SEC 35
/** Timeout 30 min*/
#define M_TIMEOUT_30_MIN 30
/** Timeout 9 min */
#define M_TIMEOUT_FOREVER 9

/** 检测事件 M_CURRENT_EVENT_30_SEC M_CURRENT_EVENT_INTERRUPT*/
enum current_event_id
{
    /** 30 s */
    M_CURRENT_EVENT_30_SEC,
    /** 短路中断 */
    M_CURRENT_EVENT_INTERRUPT,
    /** 电流过大 */
    M_CURRENT_EVENT_VALUE_OUT,
};

enum m_paramete_enum
{
    M_TYPE_U8 = 0,
    M_TYPE_U16,
    M_TYPE_U32,
    M_TYPE_U64,
    M_TYPE_I16,
    M_TYPE_I64,
    M_TYPE_Int,
    M_TYPE_Float,
    M_TYPE_Array,
    M_TYPE_Str,
    M_TYPE_Str_OK,
    M_TYPE_Str_FAIL,
    M_TYPE_NULL
};

enum key_event
{
    /** 单击 */
    M_KEY_EVENT_SIGN_CLICK = 0x10,
    /** 双击 */
    M_KEY_EVENT_DOUBLE_CLICK,
    /** 长按 */
    M_KEY_EVENT_LONG_PRESS,
};

enum key_id
{
    /** 开始/暂停按键 */
    M_KEY_EVENT_START = 15,
    /** 停止按键 */
    M_KEY_EVENT_STOP,
    /** 左/上一个按键 */
    M_KEY_EVENT_LEFT,
    /** 右/下一个按键 */
    M_KEY_EVENT_RIGHT,
    /** 重置按键 */
    M_KEY_EVENT_RESET,
};

#define M_STRM_STR_3(a, b, c) a b c

enum operate_event
{
    /** 事件：开始 */
    M_OPERATE_EVENT_START = 0x50,
    /** 事件：开始多个 */
    M_OPERATE_EVENT_START_MANY,
    /** 事件：开始选择 */
    M_OPERATE_EVENT_START_SELECT,
    /** 事件：运行中切换 - 切换下一个运行 */
    M_OPERATE_EVENT_RUNNING_SWITCH,
    /** 事件：非运行切换 - 普通通道选择切换 */
    M_OPERATE_EVENT_CHANNEL_SWITCH,
    /** 事件：暂停 */
    M_OPERATE_EVENT_PAUSE,
    /** 事件：停止 */
    M_OPERATE_EVENT_STOP,
    /** 事件：停止选择 */
    M_OPERATE_EVENT_STOP_SELECT,
    /** 事件：停止所有 */
    M_OPERATE_EVENT_STOP_ALL,
};

#define VALUE_IS_INVATE(value) (value <= 0 || value > 99999) ? true : false

/** 设备运行模式 */
enum running_mode
{
    /** 默认无效，必须初始化 ，用来是被有没有初始化 ，或者一些错误信息*/
    M_DEVICE_RUNNING_MODE_NONE,
    /** 配网模式 */
    M_DEVICE_RUNNING_MODE_CONFIG,
    /** 直连模式 */
    M_DEVICE_RUNNING_MODE_DERECT,
    /** 离线绑定模式 */
    M_DEVICE_RUNNING_MODE_OFFLINE_CONFIG,
    /** 离线运行模式 */
    M_DEVICE_RUNNING_MODE_OFFLINE_DIRECT,
};
/**
 * @brief  M_NETWORK_CONNECT_PROGRESS_NOT_INIT M_NETWORK_CONNECT_PROGRESS_NOT_LOGIN
 *
 */
enum connect_progress
{
    /** 未开始初始化 最高时候一般都是wifi或者有不满主连接的状态 */
    M_NETWORK_CONNECT_PROGRESS_CONNECT_READY,
    /** 未开始连接 */
    M_NETWORK_CONNECT_PROGRESS_,
    /** 连接但是未登录 */
    M_NETWORK_CONNECT_PROGRESS_START_LOGIN,
    /** 登陆成功 */
    M_NETWORK_CONNECT_PROGRESS_LOGIN_SUCCESS,
    /** 登陆连接成功 */
    M_NETWORK_CONNECT_PROGRESS_FULL_SUCCESS,
    /** 保持连接，暂时中断 */
    M_NETWORK_CONNECT_PROGRESS_KEEP_CONNECT_INTERRUPT,
};
/**
 * @brief  接收数据的事件 登陆事件搭配模式一起的事件
 * M_EVENT_FT_REVICE_REMOTE_DATA (收到数据) M_EVENT_FT_LOCAL_SERVER_CONNECT
 */
enum event_ft
{
    /** 收到数据 */
    M_EVENT_FT_REVICE_REMOTE_DATA,
    /** 本地连接上服务器 */
    M_EVENT_FT_LOCAL_SERVER_CONNECT,
    /** 服务器断开连接 */
    M_EVENT_FT_LOCAL_SERVER_DIS_CONNECT,
    /** 网络连接成功 */
    M_EVENT_FT_LOCAL_NET_CONNECT,
    /** 网络断开 */
    M_EVENT_FT_LOCAL_NET_DIS_CONNECT,
    /** 登陆 */
    M_EVENT_FT_LOCAL_SERVER_LOGIN,
    /** 登陆失败或丢失 */
    M_EVENT_FT_LOCAL_SERVER_LOSE_LOGIN,
    /** http 请求失败 */
    M_EVENT_FT_LOCAL_HTTP_REQU_FAIL,
};

/** 设备状态概览 M_DEVICE_GLOBAL_STATUS_INIT M_DEVICE_GLOBAL_STATUS_MANUAL_RUNNING*/
enum device_status
{
    /** 初始化状态 */
    M_DEVICE_GLOBAL_STATUS_INIT,
    /** 空闲状态 */
    M_DEVICE_GLOBAL_STATUS_IDLE,
    /** 配网状态 */
    M_DEVICE_GLOBAL_STATUS_CONFIG_NER,
    // /** 离线状态 */
    // M_DEVICE_GLOBAL_STATUS_OFFLINE,
    // /** 在线状态 */
    // M_DEVICE_GLOBAL_STATUS_ONLINE,
    /** 快速运行状态 */
    M_DEVICE_GLOBAL_STATUS_FAST_RUN_RUNNING,
    /** 计划运行状态 */
    M_DEVICE_GLOBAL_STATUS_SCHEDULE_RUNNING,
    /** 手动运行状态 */
    M_DEVICE_GLOBAL_STATUS_MANUAL_RUNNING,
    /** 测试状态 */
    M_DEVICE_GLOBAL_STATUS_TEST_RUNNING,
    /** 更新状态 */
    M_DEVICE_GLOBAL_STATUS_UPDATEING,
    /** 短路状态 泵短路*/
    M_DEVICE_GLOBAL_STATUS_SHORT_M_PUMP,
    /** 短路状态 区域短路 */
    M_DEVICE_GLOBAL_STATUS_SHORT_CHANNEL,
    /** 短路状态 区域短路 */
    M_DEVICE_GLOBAL_STATUS_SHORT_VT_TEST,
};

/**
 * @brief Device authentication key information
 */
struct __SN_KEY_AUTH
{
    /**  SN  */
    char sn[20];
    /**  KEY  */
    char key[35];
};
#define STRA001 "A001"
#define STRA002 "A002"
#define STRA003 "A003"

#define DEVICE_1_ZONE 1
#define DEVICE_4_ZONE 4
#define DEVICE_6_ZONE 6
#define DEVICE_8_ZONE 8
#define DEVICE_12_ZONE 12
#define DEVICE_16_ZONE 16
#define DEVICE_18_ZONE 18
#define DEVICE_24_ZONE 24
#define DEVICE_30_ZONE 30
#define DEVICE_32_ZONE 32

/**
 * @brief 服务器和设备端的连接方式，离线使用也在里面 M_NETWORK_MANAGE_CONNECT_MODE_MQTT M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL  M_NETWORK_MANAGE_CONNECT_MODE_TCP
 *
 */
enum connect_mode
{
    /** WIFI 通过 */
    M_NETWORK_MANAGE_CONNECT_MODE_WIFI,
    /** MQTT通过 */
    M_NETWORK_MANAGE_CONNECT_MODE_MQTT,
    /** 离线使用    通过蓝牙 */
    M_NETWORK_MANAGE_CONNECT_MODE_BLUETOOL,
    /** 传统方式    TCP连接 */
    M_NETWORK_MANAGE_CONNECT_MODE_TCP,
    /** TCP + WolfSSL 通过 */
    M_NETWORK_MANAGE_CONNECT_MODE_TCP_WOLFSSL,
    /** TCP + MbedTLS 通过 */
    M_NETWORK_MANAGE_CONNECT_MODE_TCP_MBEDTLS,
};
/**
 * @brief WIFI信息
 */
struct connect_option
{
    /** TCP  MQTT Bluetool...    */
    enum connect_mode option;
    /**   name */
    char name[32];
    /**  psw  */
    char token[64];
    /**  port */
    int port;
};

/**
 * @brief Device hardware-related parameters, Device information
 */
struct __HARDWARE_INFO
{
    /**  Device Mac Addr  */
    char mac[15];
    /**  Device Mac Identification  */
    char macsuffix[5];
    /**  Firmware Version  */
    char version[32];
    /**  Control chip model  */
    int out_drive_chip;
    /**  production order direct labor cost summary  */
    uint32_t generations;
    /**  Device Type (4/6/8/12/16)  */
    uint8_t type;
};
/**
 * @brief Device state
 */
struct __STATE_INFO
{
    /**  Device online  */
    bool onLine;
    /**  Re-link the server  */
    bool ReConnectServer;
    /**  Link or startup times  */
    uint32_t startup_count;
};
/**
 * @brief Device network server
 */
struct __SERVER_INFO
{
    /**  The server address of the cache */
    char addr[20];
    /**  The server-side port of the cache */
    int port;
    /**  The server-side newword address of the cache */
    char server_address_buffer[50];
    /**  Connect Mode */
    bool DirConnectServer;
};
/**
 * @brief Device adv info
 */
struct __EXTRA_MSG
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
};

/**  None */
typedef struct __SERVER_INFO SERVER_INFO;
/**  None */
typedef struct __SN_KEY_AUTH SN_KEY_CODE;
/**  None */
typedef struct __STATE_INFO current_state;
/**  None */
typedef struct __HARDWARE_INFO physical_information;
/**  None */
typedef struct __EXTRA_MSG extre_info;

/**
 * @brief Device Info
 */
struct __DEVICE_INFO
{
    /**   SN/KEY Code*/
    SN_KEY_CODE encrypt_code;
    /**   mac 、 version  */
    physical_information hardware;
    /**  current state */
    current_state state;
    /**   extre info */
    extre_info extreinfo;
    /** server address for online . warning : only net address ,  Real address to HTTP request to get*/
    SERVER_INFO server;
};

/**  */
/**
 * @brief 传感器通道
 * M_SENSOR_CHANNEL_MANAGE_CHANNEL_1
 */
enum seneor_chann
{
    /** 通道1 */
    M_SENSOR_CHANNEL_MANAGE_CHANNEL_1,
    /** 通道2 */
    M_SENSOR_CHANNEL_MANAGE_CHANNEL_2
};
/**
 * @brief 传感器类型
 * M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH  M_SENSOR_MANAGE_TYPE_NORMAL_DISCONNECT_SWITCH
 */
enum seneor_type
{
    /** 常闭开关型 */
    M_SENSOR_MANAGE_TYPE_NORMAL_CONNECT_SWITCH,
    /** 常开开关型 */
    M_SENSOR_MANAGE_TYPE_NORMAL_DISCONNECT_SWITCH,
    /** 流量 */
    M_SENSOR_MANAGE_TYPE_NORMAL_FLOW_RATE,
    /** */
    M_SENSOR_MANAGE_TYPE_NORMAL_NONE,
};

// 触发状态
enum is_effective
{
    // 两种都需要触发
    M_SENSOR_IS_EFFECTIVE_NONE,
    // 还没触发
    M_SENSOR_IS_EFFECTIVE_NOT_TRIG,
    // 已经触发
    M_SENSOR_IS_EFFECTIVE_ALREADY_TRIG,
};

/**
 * @brief brief
 */
enum schedule_status
{
    /**   invate */
    M_PLAN_INVALID = -1,
    /**   running */
    M_PLAN_RUNNING = 1,
    /**   idle */
    M_PLAN_NOT_RUN = 2, // 设备空闲
    /**   sock */
    M_PLAN_SACK = 3, // 计划运行浸泡
    /**   manual running */
    M_MANUAL_RUNNING = 4,
    /**   fast running */
    M_FAST_RUNNING = 5, // 快速运行
    /**   fast pause */
    M_FAST_PAUSE = 6, // 快速运行暂停
    /**   None */
    M_PLAN_NONE = 254
};
// FAST_SACK 3 //快速运行浸泡

/**
 * @brief brief
 */
typedef enum
{
    /**   error */
    M_ERR = 0,
    /**   success  */
    M_OKK = 1,
    /**   state : normal  */
    m_normal_r,
    /**   state : unnormal  */
    unnormal_r,
    /**   state : general  */
    general_r,
    /**   wifi : not exist  */
    no_exist,
    /**   wifi : ap not exist  */
    ap_no_exist_r,
    /**   wifi : connect time out  */
    ap_time_out_r,
    /**   wifi : existed  */
    existed_r,
    /**   unknow  */
    unknow_r,
    // enum state_schedule
    /**   schedule recover : should run  */
    should_running_r,
    /**   schedule recover : should sack  */
    should_sack_r,
    /**   schedule recover : not should run   */
    not_should_r,
    // enum state_ext
    /**  success   */
    succ_r,
    /**  fail   */
    fail_r,

    need_r,

    /** 月份百分比 */
    mount_count_r,
    /** 全部区域关闭 */
    all_channel_close_r,
    /** 全部区域关闭 */
    all_channel_ignore_r,

    /**  enable   */
    enable_r,
    /**  disable   */
    disable_r,

    /**  new mode    */
    special_newlwan_r

} stat_m;

/**
 * @brief  a
 *
 */
typedef enum
{
    /**  Schedule to be ready for operation    */
    M_FSM_EVENT_PREPARE_RUN = 10,
    /**  Schedule will start    */
    M_FSM_EVENT_WILL_START,
    /**  Schedule running    */
    M_FSM_EVENT_RUNNING,
    /**  Schedule sacking    */
    M_FSM_EVENT_SACKING,
    /**  Schedule RECOVER    */
    M_FSM_EVENT_RECOVER,
    /**  Schedule pause    */
    M_FSM_EVENT_PAUSE,
    /**  Schedule next zone    */
    M_FSM_EVENT_PRE,
    /**  Schedule stop    */
    M_FSM_EVENT_NEXT,
    /**  Schedule pre zone    */
    M_FSM_EVENT_STOP,
    /**  Schedule idle    */
    M_FSM_EVENT_IDLE,
    /**  Schedule water hammer    */
    M_FSM_EVENT_WATER_HAMMER,
    /**  Schedule sack recover    */
    M_FSM_EVENT_SACKING_RECOVER,
    /**  Schedule sack recover    */
    M_FSM_EVENT_HANGUP,
    /**  Schedule sack recover    */
    M_FSM_EVENT_HANGUP_RECOVER,
    // M_FSM_EVENT_UPDATE
    M_FSM_EVENT_SWITCH_PRE,

    M_FSM_EVENT_SWITCH_NEXT,
} m_fsm_events;

/**
 * @brief brief
 */
typedef enum
{
    /**  Schedule notify water hammer     */
    m_schedule__water_hammer = 10, // 10
    /**  Schedule notify state ready    */
    m_schedule__prepare = 30, // 29-24
    /**  Schedule notify state start    */
    m_schedule__start = 20, // x-254
    /**  Schedule notify state sack    */
    m_schedule__sack = 35, // x-254
    /**  Schedule notify state running    */
    m_schedule__running = 40, // 24-x
    /**  Schedule notify state pause    */
    m_schedule__pause = 45, // x-50
    /**  Schedule notify state stop    */
    m_schedule__stop = 46, // x-50
    /**  Schedule notify state idle    */
    m_schedule__idle = 50,
} m_status;

/**
 * @brief brief
 */
typedef stat_m (*m_fsm_event_handle_fun)(m_fsm_events, int, void *, uint64_t);

#define M_PRIVATE_BASE_STATUS_NORMAL 10
#define M_PRIVATE_BASE_STATUS_FSM 30
#define M_PRIVATE_BASE_STATUS_PUMP_SACK 40

enum base_state
{
    M_BASE_STATUS_IDLE = M_PRIVATE_BASE_STATUS_FSM,
    M_BASE_STATUS_PREPARE,
    M_BASE_STATUS_RUNNING,
    M_BASE_STATUS_SACK,
    M_BASE_STATUS_PAUSE,
    M_BASE_STATUS_STOP,
    M_BASE_STATUS_HANG_UP,
};
enum m_funcation
{
    M_M_FUNCATION_NONE = M_PRIVATE_BASE_STATUS_PUMP_SACK,
    M_M_FUNCATION_SACK,
    M_M_FUNCATION_PUMP,
    M_M_FUNCATION_CONFIG_A,
    M_M_FUNCATION_CONFIG_B,
};

#define M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF sscanf
#define M_CALLABLE_DATA_PARSE_FORAMT_PARAM_PRO sscanf

/**
 * @brief schedule notify
 */
stat_m m_static_instance_fsm__prepare(enum base_state ste, void *pdata, uint64_t in_time_s);
/**
 * @brief schedule start
 */
stat_m m_static_instance_fsm__start(enum base_state ste, void *pdata, uint64_t in_time_s);

extern uint64_t mDeviceSystemTime();
/**
 * @brief schedule stop
 */
#define m_schedule_event_stop_callback stat_m m_static_instance_fsm__stop(enum base_state ste, void *pdata, uint64_t in_time_s)
/**
 * @brief schedule water hammer
 */
#define m_schedule_event_water_hammer_callback stat_m m_static_instance_fsm__water_hammer(m_status ste, void *pdata, uint64_t in_time_s)
/**
 * @brief schedule water hangup_callback
 */
#define m_schedule_event_hangup_callback stat_m m_static_instance_fsm__hangup(m_status ste, void *pdata, uint64_t in_time_s)
/**
 * @brief schedule water hangup recover_callback
 */
#define m_schedule_event_hangup_recover_callback stat_m m_static_instance_fsm__hangup_recover(m_status ste, void *pdata, uint64_t in_time_s)
/**  SP  */
/**
 * @brief schedule water next
 */
#define m_schedule_event_switch_next_callback stat_m m_static_instance_fsm__switch_next(m_status ste, void *pdata, uint64_t in_time_s)
/**
 * @brief schedule water  pre
 */
#define m_schedule_event_switch_pre_callback stat_m m_static_instance_fsm__switch_pre(m_status ste, void *pdata, uint64_t in_time_s)
/**
 * @brief schedule water  _start_recove
 */
#define m_schedule_event_switch_start_recove_callback stat_m m_static_instance_fsm__start_recove(m_status ste, void *pdata, uint64_t in_time_s)
/**
 * @brief schedule water  pause
 */
#define m_schedule_event_switch_pause_callback stat_m m_static_instance_fsm__pause(m_status ste, void *pdata, uint64_t in_time_s)

/**
 * @brief brief
 *
 */
struct _io_contr
{
    /** 区域*/
    uint8_t io_contrc[20];
    /** 区域复制*/
    uint8_t io_contrc_cl[20];
    /** 是否更新 */
    bool io_update;
    /** 更新的状态*/
    bool io_update_state;
    /** 更新的状态 */
    void (*m_contr_watering)(uint8_t, bool);
};
/** 区域类型 */
enum channel_type
{
    /** 区域类型是正常  没有浸泡*/
    M_STATIC_CHANNEL_TYPE_NORMAL,
    /** 区域类型是浸泡*/
    M_STATIC_CHANNEL_TYPE_SACK,
    /** 区域类型是浸泡*/
    M_STATIC_CHANNEL_TYPE_PUMP,
    /** 区域类型是节水*/
    M_STATIC_CHANNEL_TYPE_WATERSAVING,
};

enum channel_param_config
{
    // 配置A 的信息
    M_STATIC_CHANNEL_CONFIG_A = M_PRIVATE_BASE_STATUS_PUMP_SACK + 3,
    // 配置B 的信息
    M_STATIC_CHANNEL_CONFIG_B,
};
/**   plan */

/*   time zone */
/**
 * @brief brief
 *
 */
struct time_range
{
    /** start time    */
    uint64_t tr_start_time;
    /**  end time    */
    uint64_t tr_end_time;
};
enum instance_run_type
{
    M_INSTANCE_RUN_TYPE_SCHEDULE,
    M_INSTANCE_RUN_TYPE_FAST_RUN
};
enum instance_sack_type
{
    M_INSTANCE_SACK_TYPE_FOR_CREATE,
    M_INSTANCE_SACK_TYPE_FOR_CONFIG_A,
    M_INSTANCE_SACK_TYPE_FOR_CONFIG_B,
};
/**
 * @brief brief
 *
 */
typedef struct
{
    /** 最大耗费时间 运行和浸泡等等的总时长*/
    uint64_t fn_max_coust_time;
    /** Maximum running Channel*/
    uint8_t fn_max_channel;
    /** Maximum consumption time for not including Soak (最大应该运行时间,不是总时间  不含浸泡)*/
    uint32_t fn_max_should_runtime;
    /** week info */
    uint8_t fn_week[7];
    /** Use Soak Mode*/
    enum m_funcation fn_run_type;
    /** Use Soak Mode*/
    enum instance_sack_type fn_sack_type;
    /** Soak Mode src string*/
    char fn_soak_str[10];
    /** Soak Mode water time*/
    uint16_t fn_soak_water;
    /** Soak Mode sock time*/
    uint16_t fn_soak_cycle;
    /** Pump Mode  */
    uint32_t fn_solenoid_mode_time;
    /** 跳过信息缓存*/
    uint32_t fn_water_skip;
    /** 计算发现需要提前一天执行,True表示是的 */
    bool fn_ahead_one_day_exe;
} m_extrafun;

/**
 * @brief 关于配置改变与其他功能关联的 事件
 *
 */
enum device_function_change_notify_event
{
    /** 时间改变 */
    M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_TIME,
    /** 休眠模式 */
    M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_DORMANT,
    /** 单个计划跳过 */
    M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_A_SKIP,
    /** 取消单个计划跳过 */
    M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_ABORT_A_SKIP,
    /** 全局计划跳过 */
    M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_GLOBAL_SKIP,
    /** 取消全局计划跳过 */
    M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_ABORT_GLOBAL_SKIP,
    /** 天气信息更新 */
    M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_WEATHER_SKIP,
    /** 智能计划更新 */
    M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SMART_CONFIG,
    /** 传感器操作开启 */
    M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SENSOR_OPEN_SKIP,
    /** 传感器操作关闭 */
    M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SENSOR_CLOSE_SKIP,
    /** 区域操作开启 */
    M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_CHANNEL_OPEN,
    /** 区域操作关闭 */
    M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_CHANNEL_CLOSE,
    /** 计划操作开启 */
    M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SCHEDULE_OPEN,
    /** 计划操作关闭 */
    M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SCHEDULE_CLOSE,
    /** 地址修改 */
    M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_LOCAL_CHANGE,
    /** 计划删除 */
    M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SCHEDULE_DELE,
    /** 计划修改 */
    M_DEVICE_FUNCTION_CHANGE_NOTIFY_EVENT_SCHEDULE_CHANGE,
};

/**
 * @brief  一共分为 6 种
 *
    1、是否有休眠模式
    2、是否有waterdelay
    3、是否有计划跳过
    4、是否有传感器跳过
    5、是否有天气跳过
    6、是否有饱和跳过
 */
enum skip_manage_type
{
    /** 休眠模式*/
    M_SKIP_MANAGE_TYPE_DORMANT_SKIP = 200,
    /** water delay */
    M_SKIP_MANAGE_TYPE_WATERDELAY_SKIP,
    /** 单次计划 */
    M_SKIP_MANAGE_TYPE_SIGN_SKIP,
    /** 传感器 */
    M_SKIP_MANAGE_TYPE_SENSOR_SKIP,
    /** 天气 */
    M_SKIP_MANAGE_TYPE_WEATHER_SKIP,
    /** 饱和 */
    M_SKIP_MANAGE_TYPE_SATURATION_SKIP,
    /** 所有区域被关闭 */
    M_SKIP_MANAGE_TYPE_ALL_CHANNEL_DISABLE,
    /** 所有区域被离线调整 0  */
    M_SKIP_MANAGE_TYPE_ALL_CHANNEL_OFFLINE_MONTH_COUNT,
};

/**
 * @brief brief
 *
 */
typedef struct time_range m_time_range;
/**
 * @brief  執行過程中的狀態
 *
 */
enum execution_staus
{
    M_INSTANCE_EXECUTION_STATUS_START,
    M_INSTANCE_EXECUTION_STATUS_RUNNING,
    M_INSTANCE_EXECUTION_STATUS_PAUSE,
    M_INSTANCE_EXECUTION_STATUS_RECOVER,
    M_INSTANCE_EXECUTION_STATUS_HANGUP,
    M_INSTANCE_EXECUTION_STATUS_HANGUP_RECOVER,
    M_INSTANCE_EXECUTION_STATUS_END,
};
/**
 * @brief brief
 *
 */
typedef enum
{
    // at start
    SCHEDULE_RUN_TYPE_AT = 100,
    // befroe time start
    SCHEDULE_RUN_TYPE_BEFROE,
    // sunset start
    SCHEDULE_RUN_TYPE_SUNSET,
    // sunrise start
    SCHEDULE_RUN_TYPE_SUNRISE,
    // spec day
    SCHEDULE_RUN_RHYTHM_SPEC_DAYS,
    // odd day
    SCHEDULE_RUN_RHYTHM_ODD_DAY,
    // even day
    SCHEDULE_RUN_RHYTHM_EVEN_DAY,
    // invertal day
    SCHEDULE_RUN_RHYTHM_INVERTAL_DAY,
    // invertal hour
    SCHEDULE_RUN_RHYTHM_INVERYAL_HOUR,
    // every day
    SCHEDULE_RUN_RHYTHM_EVERY_DAYS, // 保留
    SCHEDULE_RUN_RHYTHM_110,        // 保留
    SCHEDULE_RUN_RHYTHM_111,        // 保留
    SCHEDULE_RUN_RHYTHM_112,        //
                                    // 保留
    SCHEDULE_RUN_RHYTHM_113,        //
                                    // 保留
    SCHEDULE_RUN_RHYTHM_114,        //
                                    // 日落之前结束/结束
    SCHEDULE_RUN_TYPE_SUNSET_PLUS,  // 日出之后开始/结束

    SCHEDULE_RUN_TYPE_SUNRISE_PLUS, // 保留

    SCHEDULE_RUN_RHYTHM_117,      //
                                  // 保留
    SCHEDULE_RUN_RHYTHM_118,      //
                                  // 保留
    SCHEDULE_RUN_RHYTHM_119,      //
                                  // 保留
                                  // 新草坪模式没有时间参数不会添加节点   有了时间参数才会添加时间节点
    SCHEDULE_RUN_RHYTHM_NEW_LAWN, //
} instance_execu_type;
/** 外部参数*/

/**
 * @brief all trig event list
 */
enum mschedule_event
{
    /**< Stop Event: 手动run stops>* **/
    MANUAL_STOP,
    /**< Stop Event : socket run stops>*/
    SOCKET_CMD_STOP,
    /**< Stop Event : schedule change run stops >*/
    SCHEDULE_CHANGE_STOP,
    /**< Stop Event : schedule delete run stops >*/
    SCHEDULE_DELETE_STOP,
    /**< Stop Event : schedule disable run stops >*/
    SCHEDULE_DISABLE_STOP,
    /**< Stop Event : A single skip run stops>*/
    A_SIGNLE_SKIP_STOP,
    /**< Stop Event : global skip run stops>*/
    GLOBAL_SKIP_STOP,
    /**< Stop Event : Rain/Wind skip run stops>*/
    WIND_RAIN_SKIP_STOP,
    /**< Stop Event : Dormant mode run stops>*/
    DORMANT_MODE_STOP,
    /**< Trig Event : sensor trigger>*/
    SENSOR_TRIGGER,
    /**< Trig Event : sensor dis trigger>*/
    SENSOR_DIS_TRIGGER,
    /**< Trig Event : zone enable trigger>*/
    CHANNEL_ENABLE_TRIGGER,
    /**< Trig Event : zone disable trigger>*/
    CHANNEL_DISABLE_TRIGGER,

    /**< Trig Event : instance pause>*/
    RUNNING_PAUSE,
    /**< Trig Event : instanc recover>*/
    RUNNING_RECOVER,

    /**< Trig Event : changing-over " Start " event trigger>*/
    CHANGING_OVER_START_TRIGGER,
    /**< Trig Event : changing-over " Stop " event trigger>*/
    CHANGING_OVER_STOP_TRIGGER,
    /**< Trig Event : changing-over " Left " event trigger>*/
    CHANGING_OVER_LEFT_TRIGGER,
    /**< Trig Event : changing-over " Right " event trigger>*/
    CHANGING_OVER_RIGHT_TRIGGER,

    /** 运行中的其他事件 */
    /** 更新天气信息 */
    M_INSTANCE_RUNNING_UPDATE_WEATHER,
    /** 运行中的电流 */
    M_INSTANCE_RUNNING_CURRENT_NOTIFY,
    /** 运行中的电流 中断事件*/
    M_INSTANCE_RUNNING_CURRENT_NOTIFY_INTERRUPT,
    /** 运行中的电流 超出事件*/
    M_INSTANCE_RUNNING_CURRENT_NOTIFY_VALUE_OUT,
    // M_INSTANCE_RUNNING_,
};

/**
 * @brief 在个是添加年计划的时候区分环境的
 * M_INSTANCE_ADD_MODE_ONLINE M_INSTANCE_ADD_MODE_RECOVER_ENABLE
 */
enum inst_add_mode
{
    M_INSTANCE_ADD_MODE_ONLINE,
    M_INSTANCE_ADD_MODE_RECOVER_ENABLE,
    M_INSTANCE_ADD_MODE_RECOVER_DISABLE,
};

/**
 * @brief brief
 *
 */
typedef struct
{
    /** 上一个姿態*/
    uint8_t rb_pre_stated;
    /** 執行状态 開始 運行切換 */
    enum execution_staus rb_execution_staus;
    /** 上一个区域指针*/
    uint8_t rb_pre_pc;
    /** 下一个区域指针*/
    uint8_t rb_next_pc;
    /** 当前指针*/
    uint8_t rb_pc;
    /** 最大运行区域*/
    uint8_t maxChan;
    /** 最大运行时间*/
    uint64_t maxTime;
    /** 可变时间缓存*/
    uint64_t rb_WSTime_clone;
    /** 跳过信息缓存*/
    uint32_t wind_water_ice_skip;
    /** 区域完成*/
    uint32_t cb_channel_finish;
    /** 区域已经运行时间总和*/
    uint32_t cb_already_run_time_sum;
    /** 总已经运行时间 一輪*/
    uint32_t cb_total_already_run_time_one_round;
    /** 运行时间范围*/
    m_time_range current_running_time_region;

} m_run_data;

#if false && !CONFIG_NEW_CHANNEL_INFO

/** 运行数据*/
// typedef struct run_buff ;

/**
 * @brief brief
 *
 */
struct chan_manage
{
    /** 区域id*/
    uint8_t chanID;
    /** 区域开始时间*/
    uint64_t chanStartTime;
    /** 区域结束时间*/
    uint64_t chanEndTime;
    /** 区域应该运行时间*/
    uint32_t shouldRunTime;
    /** 区域已经运行时间*/
    uint32_t realityRunTime;
    /* The actual running time of the area and realityruntime is that he expresses the actual watering time, also called the effective cumulative running time  */
    uint32_t has_been_watering_time;
    /** 总的不中断浇水时间*/
    uint32_t has_been_power_watering_time;
    /* Current zone cumulative run time, ( A pause in any area will interrupt it  )*/
    uint32_t thisOperationTime;
    /** 区域状态*/
    int enable;
    // uint32_t RunTimeSum;
    bool isRun;
};
typedef struct chan_manage m_chan_manage;
// #else
/**
 * @brief brief
 *
 */
struct chan_managec
{
    /** 区域id*/
    uint8_t id;
    /** 区域状态*/
    int enable;
    /** 区域开始时间*/
    uint64_t startTime;
    /** 区域运行时间 unit:s */
    uint64_t in_sack_param_run_time;
    /** 区域浸泡时间 unit:s */
    uint64_t in_sack_param_sack_time;
    // uint32_t RunTimeSum;
    /** 区域运行*/
    bool isRun;
    /** 区域运行时间范围*/
    m_time_range runtime;

    /** 区域应该运行时间*/
    uint32_t shouldRunTime;
    /** 已经运行时间*/
    uint32_t realityRunTime;
    /** The actual running time of the area and realityruntime is that he expresses the actual watering time, also called the effective cumulative running time  */
    uint32_t has_been_watering_time;
    /** 本轮运行时间 */
    uint32_t cruuent_round_running_time;
    /** 总的不中断浇水时间*/
    uint32_t has_been_power_watering_time;
    /** Current zone cumulative run time, ( A pause in any area will interrupt it  )*/
    uint32_t thisOperationTime;
};
/**  None */
typedef struct chan_managec m_chan_managec;

#endif

// /**
//  * @brief channel Property description
//  */
// struct channel_config
// {
//     /** channel id */
//     uint8_t cd_cahnnel_id;

//     /** channel is enable */
//     uint8_t cd_enable;
//     /** channel is opened sack */
//     uint8_t cd_sack_use_open;
//     /** channel use sack : water time*/
//     uint8_t cd_water_time;
//     /** channel use sack : sack time */
//     uint8_t cd_sack_time;
//     /** channel should running time sum*/
//     uint32_t cd_should_run_time;
//     /** channel reality running time sum*/
//     uint32_t cd_reality_run_time;
//     /** 將要運行時間*/
//     uint32_t cd_curr_will_run_time;
//     /** 运行时间范围*/
//     m_time_range channel_running_time_region;
//     /** 暫停的時間 開始暫停和結束暫停的時間*/
//     m_time_range pause_recover_time;
// };
struct channel_config
{
    /** channel id */
    uint8_t cd_cahnnel_id;
    /** channel is enable */
    uint8_t cd_enable;
    /** channel is opened sack */
    uint8_t cd_channel_type;
    /** channel use sack : water time*/
    uint32_t cd_sack_should_water_time;
    /** channel use sack : sack time */
    uint32_t cd_sack_should_sack_time;
    /** channel should running time sum*/
    uint32_t cd_should_run_time;
    /** channel reality running time sum*/
    uint32_t cd_reality_run_time;
    /** 將要運行時間*/
    uint32_t cd_curr_will_run_time;
    /** 运行时间范围*/
    uint64_t cd_running_end_time;
    /** 最后一个区域运行的时间*/
    uint32_t cd_last_running_time;
    // m_time_range channel_running_time_region;
    //     /** 暫停的時間 開始暫停和結束暫停的時間*/
    // m_time_range pause_recover_time;
    /** 单独的区域喷头效率*/
    float cd_ar;
    /** 单独区域的基本吸土率*/
    float cd_ir;
    /** 单独区域的ASA 允许表层累积*/
    float cd_asa;
};
/** 标准的存储结构、、计划信息*/
typedef struct
{
    /** 计划id*/
    uint64_t cu_id;
    /** 智能计划是否启用*/
    bool cu_enable;
    /** 是否多时间计划 */
    bool cu_more_time;
    /** 多时间列表 最大解析的时间应该是 24 个 */
    uint32_t cu_more_time_list[24];
    /** 使用浸泡的参数 */
    uint8_t cu_smart;
    /** 使用浸泡的参数 */
    char cu_sack[15];
    /** 运行有效的时间*/
    m_time_range available_time;
    /** 平替参数 100 08:00 A0B0 */
    char *cu_src_execute_time_str;
    /** 运行的有效区域 1#5600 */
    char *cu_zone;
    /** 运行有效的星期number*/
    char cu_week_list[15];
    /** 运行间隔的小时或者天数*/
    uint8_t cu_interval_hour_or_day;
    /** 周期频率类型*/
    instance_execu_type frequency_type;
} m_schedule_info;

/*
     运行的有效时间参数  18：00
    char cu_execute_time[10];
     运行的有效类型 100  AT
    uint8_t cu_execute_type;

     运行间隔类型  104 - 109
    uint8_t cu_run_interval_type;
     运行时间总和
    uint32_t cu_run_time_sum;
*/
/** 这个是管理计划请求30的信息的 避免和误生成信息 */
enum instance_is_genger_info
{
    /** 没有产生运行信息 或者信息已经被消耗 */
    M_INSTANCE_IS_GENER_INFO_INIT,
    /** 在等待信息返回 */
    M_INSTANCE_IS_GENER_INFO_WAIT,
    /** 信息产生完了 */
    M_INSTANCE_IS_GENER_INFO_DONG,
};
/**
 * @brief  计划详细结构 ，包含运行参数
 *
 */
typedef struct watering_schedule
{
    /**   计划的标准信息*/
    m_schedule_info sch_info;
    /**   计划的禁止有效时间 */
    uint64_t deadlineofsch;
    /**   计划的单独跳过时间*/
    m_time_range skip_range_time;
    /**    计划功能*/
    m_extrafun exfun;
    /** 执行类型   AT*/
    instance_execu_type execu_type;
    /** 执行时间*/
    uint32_t execu_time;

    // /**   计划当前的状态*/
    // m_status state;
    /** 老的计数方式 ，暂时*/
    struct channel_config cmg_channel[32]; /**/
    // m_chan_managec mc_channel[16];
    /** 运行细化时间 参数*/
    uint64_t WSTime;
    /** 缓存时间 现在是用来缓存恢复的上一次时间 */
    uint64_t BufTime;
    /** 用来记录结束时间 保证和开始的结束时间一致 */
    uint64_t inst_end_buf_time;
    /** 是否产生30运行信息*/
    enum instance_is_genger_info is_genger_info;

    /**死亡时间  */
    uint64_t deadlineOfplan;
    /**   运行缓存数据*/
    // m_run_data *running;
    void *instance;

} instance_doc_t;

/**
 * @brief  plan
 *
 */
struct __GLOBAL_SCHEDULE_PARAM
{
    /**   全局计划时间范围*/
    m_time_range water_delay;
    /**   日出时间爱呢范围*/
    m_time_range suninfo;
    /**   区域启用标记*/
    uint8_t zone_enable[16];
};
/**  None */
#define BUG "Imolaza"
/**  None */
#define END "\r\n"

// #define _logi(form, ...) ESP_LOGI(BUG, form, ##__VA_ARGS__)
// #define _logw(form, ...) ESP_LOGW(BUG, form, ##__VA_ARGS__)
// #define _loge(form, ...) ESP_LOGE(BUG, form, ##__VA_ARGS__)

// #define DEBUG_I(form, ...) _logi("{%s} "
//                                  "[%d] " form,
//                                  __FUNCTION__, __LINE__, ##__VA_ARGS__)
// #define DEBUG_W(form, ...) _logw("{%s} "
//                                  "[%d] " form,
//                                  __FUNCTION__, __LINE__, ##__VA_ARGS__)
// #define DEBUG_E(form, ...) _loge("{%s} "
//                                  "[%d] " form,
//                                  __FUNCTION__, __LINE__, ##__VA_ARGS__)
#if CONFIG_LOG_MAXIMUM_LEVEL > 0
#define DEVELOP_TETS_MODE 1
#else
#define DEVELOP_TETS_MODE 0
#endif

/**  None */
// static FILE *fpWrite;
/**  None */
enum db_m
{
    DB_I,
    DB_HIDE,
    DB_SHORT_PRINT,
    DB_NONE_LINE,
    DB_W,
    DB_E,
    DB_ONLY_PROGRESS, /** 仅喜爱年是进度信息*/
};

/**  测试专用 */
#define DEBUG_TEST(level, form, ...)                                                                                                               \
    if (DEVELOP_TETS_MODE)                                                                                                                         \
    {                                                                                                                                              \
        if (level >= DB_ONLY_PROGRESS)                                                                                                             \
        {                                                                                                                                          \
            printf("-%lld- " form "\n", mDeviceSystemTime(), ##__VA_ARGS__);                                                                       \
        }                                                                                                                                          \
        else if (level >= DB_E)                                                                                                                    \
        {                                                                                                                                          \
            printf("\033[0;32m(%lld)Imolaza {%s}[%d] \033[0;31m" form "\033[0;37m\n", mDeviceSystemTime(), __FUNCTION__, __LINE__, ##__VA_ARGS__); \
        }                                                                                                                                          \
        else if (level >= DB_W)                                                                                                                    \
        {                                                                                                                                          \
            printf("\033[0;32m(%lld)Imolaza {%s}[%d] \033[0;33m" form "\033[0;37m\n", mDeviceSystemTime(), __FUNCTION__, __LINE__, ##__VA_ARGS__); \
        }                                                                                                                                          \
        else if (level >= DB_NONE_LINE)                                                                                                            \
        {                                                                                                                                          \
            printf("\033[0;33m" form "\033[0;37m", ##__VA_ARGS__);                                                                                 \
        }                                                                                                                                          \
        else if (level >= DB_SHORT_PRINT)                                                                                                          \
        {                                                                                                                                          \
            printf("\033[0;32m(%lld)Imolaza [%d] \033[0;33m" form "\033[0;37m\n", mDeviceSystemTime(), __LINE__, ##__VA_ARGS__);                   \
        }                                                                                                                                          \
        else if (level >= DB_HIDE)                                                                                                                 \
        {                                                                                                                                          \
            printf("\033[0;36mImolaza {%s}[%d] \033[0;36m" form "\033[0;37m\n", __FUNCTION__, __LINE__, ##__VA_ARGS__);                            \
        }                                                                                                                                          \
        else                                                                                                                                       \
            printf("\033[0;32m(%lld)Imolaza {%s}[%d] \033[0;37m" form "\n", mDeviceSystemTime(), __FUNCTION__, __LINE__, ##__VA_ARGS__);           \
    }

#define DEBUG_TEST_NOT_LINE(form, ...)                          \
    if (DEVELOP_TETS_MODE)                                      \
    {                                                           \
        printf(" \033[0;33m" form "\033[0;37m", ##__VA_ARGS__); \
    }
#define DEBUG_TEST_SHORT_PRINT(form, ...)                                                                                    \
    if (DEVELOP_TETS_MODE)                                                                                                   \
    {                                                                                                                        \
        printf("\033[0;32m(%lld)Imolaza [%d] \033[0;33m" form "\033[0;37m\n", mDeviceSystemTime(), __LINE__, ##__VA_ARGS__); \
    }
#define DEBUG_TEST_W(form, ...)                                                                                                                \
    if (DEVELOP_TETS_MODE)                                                                                                                     \
    {                                                                                                                                          \
        printf("\033[0;32m(%lld)Imolaza {%s}[%d] \033[0;33m" form "\033[0;37m\n", mDeviceSystemTime(), __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    }
#define DEBUG_TEST_E(form, ...)                                                                                                                \
    if (DEVELOP_TETS_MODE)                                                                                                                     \
    {                                                                                                                                          \
        printf("\033[0;32m(%lld)Imolaza {%s}[%d] \033[0;31m" form "\033[0;37m\n", mDeviceSystemTime(), __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    }
#define DEBUG_TEST_HIDE(form, ...)                                                                                  \
    if (DEVELOP_TETS_MODE)                                                                                          \
    {                                                                                                               \
        printf("\033[0;36mImolaza {%s}[%d] \033[0;36m" form "\033[0;37m\n", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    }
/**  None */
#define DEBUG_E(form, ...)                                                                                \
    if (DEVELOP_TETS_MODE)                                                                                \
    {                                                                                                     \
        printf("\033[0;31mImolaza {%s}[%d] " form "\n\033[0;37m", __FUNCTION__, __LINE__, ##__VA_ARGS__); \
    }

/**  None */
// #define DEBUG_OUT_TO_FILE_COLOR(str, ...)
//     {
//         fpWrite = fopen("debug.txt", "a");
//         fprintf(fpWrite, str, ##__VA_ARGS__);
//         fclose(fpWrite);
//     }
// /**  None */
// #define DEBUG_OUT_TO_FILE(str, ...)
//     {
//         fpWrite = fopen("debug.txt", "a");
//         fprintf(fpWrite, "\033[0;33m" str "\n\033[0;m", ##__VA_ARGS__);
//         fclose(fpWrite);
//     }

/**  None */
#define CAREFULY_CHANGE_CONSTVALUE_INT(vue, offset) \
    vue += offset;

extern char *
toname(int en);
/**  None */
// #define VALUE_IS_INVATE(value) (value <= 0 || value > 99999) ? true : false

enum light_anima_mode
{
    // 设备开机状态OR设备找网络状态
    M_DISPLAY_START_UP_OR_FIND_NETWORK_MODE,
    // M_DISPLAY_FIND_NETWORK_MODE,               设备找网络状态
    // 设备空闲状态
    M_DISPLAY_IDLE_STATUS_MODE,
    // 区域运行中
    M_DISPLAY_ZONE_RUNNING_MODE,
    // 区域被选择状态
    M_DISPLAY_ZONE_SELECT_MODE,
    // 设备连接WIFI第一阶段
    M_DISPLAY_WIFI_CONFIG_STATUS1_MODE,
    // 设备连接WIFI第二阶段
    M_DISPLAY_WIFI_CONFIG_STATUS2_MODE,
    // 设备连接服务器状态
    M_DISPLAY_CONNECTING_TO_SERVER_MODE,
    // 设备配网成功状态
    M_DISPLAY_DISTRIBUTION_NETWORK_SUCCESS_MODE,
    // 设备连接wifi失败状态
    M_DISPLAY_WIFI_CONNET_FAIL_MODE,
    // 设备连接的目标wifi无网络
    M_DISPLAY_WIFI_NO_NETWORK_MODE,
    // 设备连接服务器失败状态
    M_DISPLAY_SERVER_CONNET_FAIL_MODE,
    // 设备出现网络问题状态
    M_DISPLAY_NETWORK_PROBLEM_MODE,
    // 设备wifi断开连接状态
    M_DISPLAY_WIFI_DISCONNECTION_MODE,
    // 设备区域短路状态
    M_DISPLAY_REGIONAL_SHORT_CIRCUIT_MODE,
    // 设备异常反馈，暂时不用
    M_DISPLAY_ABNORMAL_FEEDBACK_MODE,
    // 设备OTA更新状态s
    M_DISPLAY_FIRMWARE_UPDATE_MODE,
    // 设置设备区域数量模式
    M_DISPLAY_SET_ZONE_MODE,
    // 设置设备版本号模式
    M_DISPLAY_SET_DEVICE_VERSION_MODE,
    // 设置设备灯光亮度模式
    M_DISPLAY_SET_DEVICE_LIGHTI__STRENGTH_MODE,
    // 设置灯光关闭
    M_DISPLAY_SET_LIGHT_CLOSE,
    // 设置灯光开启
    M_DISPLAY_SET_LIGHT_OPEN,
    // VT电源短路
    M_DISPLAY_VT_SHORT_CIRCUIT_MODE,
    // M端子短路
    M_DISPLAY_M_TERMINAL_SHORT_CIRCUIT_MODE,
    // 手动设置运行时长模式
    M_DISPLAY_SET_MANUAL_DURATION
};

/**
 * @brief 短路一定模式
 *
 */
enum current_short_mode
{
    /** VT 短路模式 */
    M_CURRENT_SHORT_MODE_VT,
    /** 泵短路模式 */
    M_CURRENT_SHORT_MODE_PUMP,
    /** 单个区域短路模式 */
    M_CURRENT_SHORT_MODE_SIGN_ZONE,
    /** 校准短路模式 */
    M_CURRENT_SHORT_MODE_CALIBRATION,
    /** 水锤短路效果区分 */
    M_CURRENT_SHORT_MODE_WATER_HAMMER,
    /** 电流超出  */
    M_CURRENT_OUT_VALUE_MODE,
};

enum light_RGB_color
{
    red = 0xff0000,
    blue = 0xff,
    greed = 0x00ff00,
    purple = 0x800080,
    grey = 0x808080,
    brow = 0xd2691e,
    orange = 0xffd700,
    white = 0xffffff,
    yellow = 0x555500,
    black = 0x0
};

#define COLOR_MAX(color) (color > 0 ? 0x01 : 0x00)

#define MY_COLOR_CONCOC(color) (((COLOR_MAX(((color >> 16) & 0xff))) << 16) | \
                                ((COLOR_MAX(((color >> 8) & 0xff))) << 8) |   \
                                (COLOR_MAX((color & 0xff))))

#ifdef CONFIG_nor_blue
#define LED_NOR_COLOR COLOR_CONCOC(blue)
#endif
#ifdef CONFIG_nor_greed
#define LED_NOR_COLOR COLOR_CONCOC(blue)
#endif
#ifdef CONFIG_nor_purple
#define LED_NOR_COLOR COLOR_CONCOC(purple)
#endif
#ifdef CONFIG_nor_grey
#define LED_NOR_COLOR COLOR_CONCOC(grey)
#endif
#ifdef CONFIG_nor_brow
#define LED_NOR_COLOR COLOR_CONCOC(brow)
#endif
#ifdef CONFIG_nor_orange
#define LED_NOR_COLOR COLOR_CONCOC(orange)
#endif
#ifdef CONFIG_nor_white
#define LED_NOR_COLOR COLOR_CONCOC(white)
#endif
#ifdef CONFIG_nor_red
#define LED_NOR_COLOR COLOR_CONCOC(red)
#endif
#ifdef CONFIG_nor_yellow
#define LED_NOR_COLOR COLOR_CONCOC(yellow)
#endif
#ifdef CONFIG_idle_blue
#define LED_IDLE_COLOR COLOR_CONCOC(blue)
#endif
#ifdef CONFIG_idle_greed
#define LED_IDLE_COLOR COLOR_CONCOC(greed)
#endif
#ifdef CONFIG_idle_purple
#define LED_IDLE_COLOR COLOR_CONCOC(purple)
#endif
#ifdef CONFIG_idle_grey
#define LED_IDLE_COLOR COLOR_CONCOC(grey)
#endif
#ifdef CONFIG_idle_brow
#define LED_IDLE_COLOR COLOR_CONCOC(brow)
#endif
#ifdef CONFIG_idle_orange
#define LED_IDLE_COLOR COLOR_CONCOC(range)
#endif
#ifdef CONFIG_idle_white
#define LED_IDLE_COLOR COLOR_CONCOC(white)
#endif
#ifdef CONFIG_idle_red
#define LED_IDLE_COLOR COLOR_CONCOC(red)
#endif
#ifdef CONFIG_idle_yellow
#define LED_IDLE_COLOR COLOR_CONCOC(yellow)
#endif
#ifdef CONFIG_err_blue
#define LED_ERR_COLOR COLOR_CONCOC(blue)
#endif
#ifdef CONFIG_err_greed
#define LED_ERR_COLOR COLOR_CONCOC(greed)
#endif
#ifdef CONFIG_err_purple
#define LED_ERR_COLOR COLOR_CONCOC(purple)
#endif
#ifdef CONFIG_err_grey
#define LED_ERR_COLOR COLOR_CONCOC(rey)
#endif
#ifdef CONFIG_err_brow
#define LED_ERR_COLOR COLOR_CONCOC(brow)
#endif
#ifdef CONFIG_err_orange
#define LED_ERR_COLOR COLOR_CONCOC(orange)
#endif
#ifdef CONFIG_err_white
#define LED_ERR_COLOR COLOR_CONCOC(white)
#endif
#ifdef CONFIG_err_red
#define LED_ERR_COLOR COLOR_CONCOC(red)
#endif
#ifdef CONFIG_err_yellow
#define LED_ERR_COLOR COLOR_CONCOC(yellow)
#endif
/**  None */
#define LED_WIFI_CONNECT_COLOR 0x100030
/**  None */
#define LED_OTA_COLOR 0x151515

// #define GPIO_KEY_RIGHT CONFIG_KEY_LEFT_PIN  // CONFIG_KEY_START_PIN
// #define GPIO_KEY_PAUSE CONFIG_KEY_PAUSE_PIN // CONFIG_KEY_LEFT_PIN
// #define GPIO_KEY_PLAY CONFIG_KEY_START_PIN  // CONFIG_KEY_RIGHT_PIN
// #define GPIO_KEY_LEFT CONFIG_KEY_RIGHT_PIN  // CONFIG_KEY_PAUSE_PIN
// #define GPIO_MAIN_RESET CONFIG_KEY_RESET_PIN
/**  None */
#define GPIO_TEST_PIN 2

/**  None */
#define LED_START_UP COLOR_CONCOC(greed)
/**  None */
#define I2C_MASTER_SCL_IO 17
/**  None */
#define I2C_MASTER_SDA_IO 16
/**  None */
#define I2C_MASTER_FREQ_HZ CONFIG_IIC_FREQ

/**  None */
#define LED_CONFIG_NET_DONE COLOR_CONCOC(blue)
/**  None */
#define LED_CONFIG_NET_ING COLOR_CONCOC(greed)

/**  None */
#define LED_TEST_COLOR_BLUE COLOR_CONCOC(blue)
/**  None */
#define LED_TEST_COLOR_RED COLOR_CONCOC(red)
/**  None */
#define LED_TEST_COLOR_GREED COLOR_CONCOC(greed)
/**  None */
#define LED_TEST_COLOR_YELLOW COLOR_CONCOC(yellow)
/**  None */
#define LED_TEST_COLOR_PURPLE COLOR_CONCOC(purple)
/**  None */
#define LED_TEST_COLOR_WHITR COLOR_CONCOC(white)

/**  None */
#define NET_ERR_COUNT_MAX 5
/**  None */
#define SOCKET_ERR_COUNT_MAX 5

/**  None */
#define NON_EMPTY(obj) (obj != NULL)
/**  None */
#define NETWORK_DNS_CHECK(a) (a > 3 && (a < NET_ERR_COUNT_MAX))
/**  None */
#define NETWORK_CHECK(a) (a > NET_ERR_COUNT_MAX && (a % 3 == 0))
/**  None */
#define NETWORK_FAIL_FIX(a) (a == NET_ERR_COUNT_MAX)
/**
 * @brief  plan
 *
 */
enum offline_cause_t
{
    /**   ofline: unknow*/
    unknow = -1,
    /**   ofline: power dump*/
    power_dump = 1,
    /**   ofline: notwork anomaly*/
    network_anomaly = 2,
    /**   ofline: wifi disconnect*/
    wifi_disconnect = 3,
    /**   ofline: abnormal restart*/
    device_abnormal_restart = 4,
    /**   ofline: other*/
    other = 5,
};
enum restart_cause_t
{
    /** 有预谋的重启 */
    M_RESTART_CAUSE_PREMEDITATED = 1,
    /** 堆不足重启 */
    M_RESTART_CAUSE_HEAP_SPACE_LACK,
    /** 远程重启 */
    M_RESTART_CAUSE_REMOTE_ACTIVE,
    /** OTA更新成功重启 */
    M_RESTART_CAUSE_OTA_UPDATA_SUCC,
    /** OTA更新失败 */
    M_RESTART_CAUSE_OTA_UPDATA_FAIL,

};

// /**
//  * @brief  plan
//  *
//  */
// struct _errTable_
// {
//     /* wifi */
//     bool functionTest_is_wifi;
//     char functionTest_WIFISSID[32];
//     char functionTest_WIFIPSW[20];
//     int functionTest_not_found_wifi;
//     int functionTest_pswerr;
//     int functionTest_connectTimeOut;
//     int functionTest_succ;

//     /* led */
//     bool functionTest_is_led;
//     int functionTest_not_found_chip;
//     int functionTest_linkTimeOut;

//     /* socket */
//     bool functionTest_is_socket;
//     int functionTest_socketFail;
//     int functionTest_socketFail_count;
//     int functionTest_socketNotFoundDNS;
//     int functionTest_socketSucc;

//     int system_wifi_event_disconn;
//     int offline_hedging;
//     enum offline_cause_t offline_cause;
// };
/**
 * @brief  m_stroage_rescue_message M_GLO_STROAGE_RESCUE_MESSAGE_SET M_GLO_STROAGE_RESCUE_MESSAGE_GET
 *
 */
enum m_stroage_rescue_message
{
    /**  set */
    M_GLO_STROAGE_RESCUE_MESSAGE_SET = 50,
    /**  get */
    M_GLO_STROAGE_RESCUE_MESSAGE_GET = 51,
    /**  del */
    M_GLO_STROAGE_RESCUE_MESSAGE_DEL = 52,
};

/* SERVER TRANSINFO */
// 传感器触发
#define M_CMD_TWOWAY_SENSOR_TRIGE 6
// 传感器离开
#define M_CMD_TWOWAY_SENSOR_LEAVE 7
/** 设置传感器*/
#define M_CMD_TWOWAY_SET_SENSOR 8
/** 查询设备状态*/
#define M_CMD_TWOWAY_GET_DEVICE_STATUS 9

/** 设备配网上线*/
#define M_CMD_NOTIFY_TO_SERVER_DEVICE_CONFIG 10
/** 设备断电上线*/
#define M_CMD_NOTIFY_TO_SERVER_DEVICE_ONLINE 11
/** 设备断网上线*/
#define M_CMD_NOTIFY_TO_SERVER_DEVICE_OFFLINE_TO_ONLINE 67

// 设置时间和向服务器获取时间
#define M_CMD_TWOWAY_SET_DEVICE_TIME 12
// updata device wifi online
/** 返回刚登陆的信息、刚连接 */
#define M_CMD_TOWAY_DEVICE_RECONNECT_TIPS 14
/** 在线修改WIFI */
#define M_CMD_TOWAY_DEVICE_CHANGEWIFI 15
// 同步计划
#define M_CMD_NOTIFY_TO_SERVER_DEVICE_SYNC_SCHEDULE 16
// 远程重启设备
#define M_CMD_NOTIFY_TO_DEVICE_RESTART 17
// 请求更新设备或者要求新设备
#define M_CMD_TWOWAY_GET_OR_DEMAND_DEVICE_UPDTAE 18
// 设备向服务器通知更新开始
#define M_CMD_NOTIFY_TO_SERVER_DEVICE_CONFIRM_UPDATA 19

/* PLAN */
/** 添加一个计划到设备*/
#define M_CMD_TWOWAY_ADD_A_SCHEDULE 20
/** 反馈计划添加成功与否*/
#define M_CMD_NOTIFY_TO_SERVER_ADD_SCH_RESP 21
/** 删除计划*/
#define M_CMD_TWOWAY_DELE_SCHEDULE 22
/** 反馈计划删除成功与否*/
#define M_CMD_TWOWAY_DELE_SCHEDULE_RESP 23
/** 通知计划开始*/
#define M_CMD_NOTIFY_TO_SERVER_SCHEDULE_START 24
/** 通知计划开始切换区域运行*/
#define M_CMD_NOTIFY_TO_SERVER_SCHEDULE_SWITCH 64
/** 通知计划结束*/
#define M_CMD_NOTIFY_TO_SERVER_SCHEDULE_END 25

/** 区域设置 */
#define M_CMD_TWOWAY_SET_ZONE 26
/** 计划启用和禁用 */
#define M_CMD_TWOWAY_SET_SCHEDULE_ABLE 27
/** 同步浇水时间 */
#define M_CMD_TWOWAY_POWEROFF_ONLINE_SYNC_WATERTIME 28

/** 废弃 */
#define M_CMD_TWOWAY_POWEROFF_ONLINE_SE 29

/** 提前 9 分 30 秒 或者 1 分 30 秒 请求 浇水时间 */
#define M_CMD_TWOWAY_REQU_SCHEDULE_WATERTIME 30
/** 计划被分域跳过s */
#define M_CMD_NOTIFY_TO_SERVER_SCHEdULE_SKIP 31
/** 设备更新天气缓存 */
#define M_CMD_TWOWAY_REQU_WEATHER_BUFF 32

/** 未作任何事的传感器接入 */
#define M_CMD_NOTIFY_TO_SERVER_SENSOR_CONNECT 34

/** 新草坪模式 后续时间相关的计划参数 */
#define M_CMD_TWOWAY_NEW_LAWN_TIME_INFO 35

/** 未作任何事的传感器断开 */
#define M_CMD_NOTIFY_TO_SERVER_SENSOR_DIS 36

/** 设备休眠模式 */
#define M_CMD_TWOWAY_HIBERNATION_MODE 37
/** 泵模式开启或者关闭   */
#define M_CMD_TWOWAY_PUMP_MODE 38

/** 快速运行上一个 */
#define M_CMD_TWOWAY_FASTRUN_SWITCH_PRE 39
/** 快速运行开始 */
#define M_CMD_TWOWAY_FASTRUN_START 40
/** 快速运行完成 */
#define M_CMD_TWOWAY_FASTRUN_COMPLETE 42
/** 快速运行下一个 */
#define M_CMD_TWOWAY_FASTRUN_SWITCH_NEXT 43
/** 快速运行暂停 */
#define M_CMD_TWOWAY_FASTRUN_PAUSE 44
/** 快速运行恢复 */
#define M_CMD_TWOWAY_FASTRUN_RECOVER 45

#define M_CMD_TWOWAY_TO_SERVER_FAST_RUN_SWITCH 61

/** 全局停止运行 */
#define M_CMD_TWOWAY_GLOBAL_RUN_STOP 46

/** 手动运行开始 */
#define M_CMD_NOTIFY_TO_SERVER_MANUAL_RUNNING_START 48
/** 手动运行开始 */
#define M_CMD_NOTIFY_TO_SERVER_MANUAL_RUNNING_STOP 49

/** 设备恢复出厂 */
#define M_CMD_TWOWAY_FACTORY_RESET 50

/** 手动运行切换 */
#define M_CMD_NOTIFY_TO_SERVER_MANUAL_RUNNING_SWITCH 51

/** 清除设备信息 */
#define M_CMD_TWOWAY_RESET_DEVICE 52

/** 调节设备灯光亮度 */
#define M_CMD_TWOWAY_DEVICE_LIGHT_CHANGE 56
/** 远程控制设备按键 */
#define M_CMD_TWOWAY_CONTROL_DEVICE_KEY 58

/** 跳过一个时间区域的计划 */
#define M_CMD_TWOWAY_NOT_RUN_SCHEDULE_TIMESPC 62

/** 水锤 */
#define M_CMD_TWOWAY_WATER_HAMMER 63

/** 跳过一个计划 */
#define M_CMD_TWOWAY_NOT_RUN_A_SCHEDULE_TIMESPC 70

/** 设备通知服务器设备被删除 */
#define M_CMD_NOTIFY_TO_SERVER_SELF_BE_DELETE 71

/** 设置各个区域流速 */
#define M_CMD_NOTIFY_TO_SET_FLOW_RATE 72

/** 计划跳过通知 */
#define M_CMD_NOTIFY_TO_SERVER_A_SCHEDULE_BE_SKIP 74

/** 取消跳过一个计划 */
#define M_CMD_TWOWAY_UNDO_NOT_RUN_A_SCHEDULE_TIMESPC 75

/** 取消跳过一段时间 */
#define M_CMD_TWOWAY_UNDO_NOT_RUN_SCHEDULE_TIMESPC 76

/** 流量计脉冲计数反馈 */
#define M_CMD__NUMBER_OF_TRAFFIC_DETECTION_PULSES 77

/** 请求地址信息 */
#define M_CMD_NOTIFY_TO_DEVICE_REQUEST_LOCAL_INFO 80

/** 流量传感器数值反馈 */
#define M_CMD_NOTIFY_TO_SERVER_FLOW_SENSOR_FEEDBACK 81

/** 地址信息 */
#define M_CMD_NOTIFY_TO_SERVER_REQUEST_LOCAL_INFO 82

/** 电流反馈 */
#define M_CMD_NOTIFY_TO_SERVER_CURRENT_BACK 85

/** 获取wifi强度 */
#define M_CMD_TWOWAY_RESP_WIFI_SIGN_STRAGE 86

/** 区域配置指令 */
#define M_CMD_NOTIFY_TO_SERVER_ZONE_CONFIG_ABC 87
/** 离线月份的浇水量调整 */
#define M_CMD_NOTIFY_TO_SERVER_OFFLINE_ADJUST 88
/** 夏令时 */
#define M_CMD_NOTIFY_TO_SERVER_TIME_CHANGE 89

/** 服务器请求检测电流*/
#define M_CMD_NOTIFY_TO_DEVICE_CHECK_CURRENT 91

/** 开启或者关闭离线模式 */
#define M_CMD_NOTIFY_TO_DEVICE_OFFLINE_MODE_CONTROL 92

/** 运行反馈 流量和 电流信息 */
#define M_CMD_NOTIFY_TO_DEVICE_RUNNING_RESP_CURRENT_AND_FLOW 93

/** 设置 传感器损坏检测时间  */
#define M_CMD_NOTIFY_TO_DEVICE_SET_TIME_SENSOR_BAD_CHECK 94
/** 设置 流量漏水检测时间  */
#define M_CMD_NOTIFY_TO_DEVICE_SET_TIME_FLOW_LEAKING_CHECK 95
/** 反馈 流量传感器/普通传感器 漏水/电流异常 通知 */
#define M_CMD_NOTIFY_TO_SERVICE_FLOW_SENSOR_ABNORMAL 96

/** 设置 传感器损坏或者漏水检测 是否需要打开 或者 关闭  */
#define M_CMD_NOTIFY_TO_SERVICE_SENSOR_MONITOR_OPEN_OR_CLOSE 97

/** 获取或者设置静态IP */
#define M_CMD_NOTIFY_TO_SERVICE_STATIC_IP_OPTION 98
/** 获取运行的区域智能信息 */
#define M_CMD_NOTIFY_TO_SERVICE_GET_SMART_ZONE_INFO 99

/** DEBUG */
#define M_CMD_NOTIFY_TO_SERVER_DEBUG_INFO 100

/*通过蓝牙与手机通信指令    登录认证*/
#define M_CMD_LOGIN_AUTHENTICATION 101

/*通过蓝牙与手机通信指令    确认绑定*/
#define M_CMD_CONFIRM_BINDING 102

/*通过蓝牙与手机通信指令    离线运行转在线*/
#define M_CMD_REMOVE_BINDING 103

// SN 设置
#define M_CMD_SN_SET 104

/*rtc电量过低预警*/
#define M_CMD_NOTIFY_TO_DEVICE_RTC_POWER_CHECK 105

/*批量区域电流校准创建*/
#define M_CMD_BATCH_ZONE_CURRENT_CALIBRATION_FOUND 107

/*批量区域电流校准开始*/
#define M_CMD_BATCH_ZONE_CURRENT_CALIBRATION_TOGGLE 108

/*批量区域电流校准结束*/
#define M_CMD_BATCH_ZONE_CURRENT_CALIBRATION_END 109

/*批量区域电流校测试创建*/
#define M_CMD_BATCH_ZONE_CURRENT_TEST_FOUND 110

/*批量区域电流测试开始*/
#define M_CMD_BATCH_ZONE_CURRENT_TEST_TOGGLE 111

/*批量区域电流测试结束*/
#define M_CMD_BATCH_ZONE_CURRENT_TEST_END 112

/*OTA静默更新*/
#define M_CMD_OTA_SILENT_UPDATE 113

/** 查询 计划存储内容 */
#define M_CMD_NOTIFY_TO_SERVER_QUERY_SCHEDULE_STROAGE_INFO 202

enum debug_code
{
    plan_conflict_Cross_running = 1,
    plan_conflict_Start_at_the_same_time,
    plan_Liat_priority_query,
    plan_Have_a_fastrun_is_running,
    plan_Have_a_fastrun_is_pause, // 5
    plan_Have_a_specific_is_occupy,
    plan_idle,
    plan_Invalid_id_Obj_null,
    plan_Now_the_time_is_less_than_the_start_time,
    plan_Today_run,
    plan_dorman_skip_run,
    plan_next_runtime,
    plan_unknow_stop,          // 异常停止
    plan_hang_up_not_need_run, // 冲突之后不需要运行

    fastrun_addFail_isRunning_parameter_err,
    fastrun_addFail_parameter_invail,
    fastrun_addFail_addfastRunNode_fail,
    fastrun_compleFail_objNull_ornotRunornotPause,
    fastrun_recoverContinueFail_objNull_ornotRunornotPause,
    fastrun_pauseContinueFail_objNull_ornotRunornotPause,
    fastrun_continueFail_objNull_ornotRunornotPause,
    fastrun_pauseReFail_objNull_ornotRunornotPause,
    fastrun_pauseFail_objNull_ornotRunornotPause,
    fastrun_stopFail_objNull_ornotRunornotPause,
    fastrun_nextChanFail_objNull_ornotRunornotPause,
    fastrun_preChanFail_objNull_ornotRunornotPause,

    key_channelId_invail,

    cmd_invail,

    node_addfail,
    node_thread_timeout,
    plan_thread_timeout,
    channel_chInvail,
    ictype_Invail,
    open_channel_err,

    plan_delete_info,

    plan_add_parameter_err,
    plan_add_parameter_sign_err,
    plan_add_parameter_suninfo_err,

    timeNode_add_info,

    plan_not_implemented_cycle_day,
    plan_not_implemented_odd_day,
    plan_not_implemented_even_day,
    plan_not_implemented_spec_day,
    sock_send_data_err,
    sock_send_length_err,
    plan_hang_up_obj_null,
    plan_hang_up_wake_up_but_not_running,
    plan_hang_up_not_running,
    plan_hang_up,
    plan_Time_prevents_the_recovery,
    No_zone_to_be_run_was_found,
    plan_Have_a_key_is_running,
    // 修改wifi但是有计划或者快速运行在运行
    change_wifi_but_device_on_running,
    // 正在水锤(使用浸泡)
    use_hammer_sack_come_into_force,
    // 正在水锤（未使用浸泡）
    use_hammer__come_into_force,
    // 新草坪模式的禁用修改
    use_new_lwan_disable_change,
    flash_count,
    /** 时间节点超出时间范围 */
    time_node_time_out_flush_fail,
    /** 时间节点超出计划时间范围 */
    time_node_time_out_schedule_range,
    /** 计划运行的区域都被关闭 */
    debug_schedule_running_but_channel_all_disable,
    /** 更新中 计划开始运行 */
    debug_schedule_running_but_device_update_status,
    /** 电流偏大 */
    debug_running_current_value_out,

};

//****************IO驱动芯片类型************************//
enum io_chip_m
{
    chip_other = 0,
    chip_ahh5,
    chip_hc595
};

typedef enum _PinBit
{
    Pin_0,
    Pin_1,
    Pin_2,
    Pin_3,
    Pin_4,
    Pin_5,
    Pin_6,
    Pin_7
} PinBit;

enum net_connect_option
{
    /** DHCP */
    M_NET_CONNECT_OPTION_DHCP,
    /** 自动静态IP */
    M_NET_CONNECT_OPTION_AUTO_STATIC_IP,
    /** 手动静态IP */
    M_NET_CONNECT_OPTION_MANUAL_STATIC_IP,
    /** 尝试静态IP */
    M_NET_CONNECT_OPTION_TRY_STATIC_IP
};
/************************RTC  定义时间结构体 ****************************/

/********************蓝牙配网 定义wifiname、password、连接类型type******************/

// typedef struct my_mifi_infoma
// {
//     char ssid[32];
//     char psw[32];
// } my_mifi_info;
// my_mifi_info mwifi;

// char connect_type[3];
//*********************************传感器类型*************************//
enum sensor_type_m
{
    SENSOR_CLOSE = 0,
    RAINFALL_SENSOR_CLOSE,
    RAINFALL_SENSOR_OPEN,
    BADGER_228PV15 = 3,
    BADGER_228PV20,
    BADGER_228PV30,
    BADGER_228PV40,
    TORO_TFS_050,
    TORO_TFS_075,
    TORO_TFS_100,
    TORO_TFS_150,
    TORO_TFS_200,
    TORO_TFS_300,
    TORO_TFS_400,
    CST_FSI_T10,
    CST_FSI_T15,
    CST_FSI_T20,
    EVERYBODY_1004,
    FLOMEC_QS200_10_SCH40,
    FLOMEC_QS200_10_SCH80,
    FLOMEC_QS200_15_SCH40,
    FLOMEC_QS200_15_SCH80,
    FLOMEC_QS200_20_SCH40,
    FLOMEC_QS200_20_SCH80,
    FLOMEC_QS200_30_SCH40,
    FLOMEC_QS200_30_SCH80,
    FLOMEC_QS200_40_SCH40,
    FLOMEC_QS200_40_SCH80,
    FLOMEC_QS100_SCH40,
    FLOMEC_QS100_SCH80,

    HC_075_FLOW_B,
    HC_100_FLOW_B,
    HC_150_FLOW_B,
    HC_200_FLOW_B,

    FG100,
    FS350B,
    P_OTHER_SENSOR = 98,
    K_OTHER_SENSOR = 99

};
void mDelay_ms_precision(uint32_t in_ms);

typedef struct
{
    /*字符串输出*/
    char output_str[200];

    /*待运行区域数量*/
    int zonenum;

    /*区域号*/
    int channel_number[40];

    int sensor_mode;

    /*区域剩余运行时长*/
    uint64_t remain_duration_current_area[40];

    /*区域已运行时长*/
    uint64_t puase_duration_current_area[40];

    /*区域总时长*/
    uint64_t total_duration_current_area[40];

} m_run_time_query;

m_run_time_query m_run_time;
// typedef struct sensor_stats_my
// {
//     uint8_t sensor_type;
//     float k_value;
//     float offset_value;
// } sensor_stats;
#include "core/middle/error_handle/error_handle.h"
#endif /* "__CORE__H__"*/
