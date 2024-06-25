#ifndef __NETWORK_H__ 
#define __NETWORK_H__ 


#define BIT14 0x01
#define BIT15 0x10
#define BIT16 0x11



extern struct mnetwork _link;



enum
{
    M_EVNBIT_RXHANDLE = BIT15,
    M_EVNBIT_TXHANDLE = BIT16,
    M_EVNBIT_CONNRCT_SERVER = BIT14
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
// 设置时间和向服务器获取时间
#define M_CMD_TWOWAY_SET_DEVICE_TIME 12
// updata device wifi online  
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
/** 全局停止运行 */
#define M_CMD_TWOWAY_GLOBAL_RUN_STOP 46

/** 手动运行开始 */
#define M_CMD_NOTIFY_TO_SERVER_MANUAL_RUNNING_START 48
/** 手动运行开始 */
#define M_CMD_NOTIFY_TO_SERVER_MANUAL_RUNNING_STOP 49
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

/** 跳过一个计划 */
#define M_CMD_TWOWAY_NOT_RUN_A_SCHEDULE_TIMESPC 70

/** 计划跳过通知 */
#define M_CMD_NOTIFY_TO_SERVER_A_SCHEDULE_BE_SKIP 74

/** 取消跳过一个计划 */
#define M_CMD_TWOWAY_UNDO_NOT_RUN_A_SCHEDULE_TIMESPC 75

/** 取消跳过一段时间 */
#define M_CMD_TWOWAY_UNDO_NOT_RUN_SCHEDULE_TIMESPC 76



struct link_base
{
    /* data */
    char addr[20];
    char addr_buf[120];
    int port;
};

struct link_buf_flag
{
    uint64_t buf_flag_cmd;
    uint64_t buf_flag_cmd_temp;
    uint64_t buf_flag_tx_count;
    /** 当收到反馈时，值为true ，表示可以删除*/
    bool allow_deletion;
};



/// @brief  表征网络服务状态
struct mnetwork
{
    int link_is_connected;
    struct link_base base;
    struct link_buf_flag buf_flag_top;
};


















#endif /** __NETWORK_H__ */
