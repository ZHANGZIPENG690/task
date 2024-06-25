#ifndef __NOTIFY__H__
#define __NOTIFY__H__

#include "core/core.h"
#include "core/schedule/mschedule.h"



#define BIT14 0
#define BIT15 1
#define BIT16 2



enum
{
    M_EVNBIT_RXHANDLE = BIT15,
    M_EVNBIT_TXHANDLE = BIT16,
    M_EVNBIT_CONNRCT_SERVER = BIT14
};

/**
 * @brief 添加一个时间节点
 *
 * @param in_id id
 * @param in_notice_time 通知时间节点
 * @param in_tag 通知状态
 * @param in_obj 通知对象
 * @return stat_m
 */
stat_m m_callable_add_notice_schedule_run_node(uint64_t in_id, uint64_t in_notice_time, uint8_t in_tag, void *in_obj);

/**
 * @brief 改变一个节点的事件和目标
 *
 * @param id 修改接待你和计划的id应该是相同
 * @param in_notice_time 修改节点通知的时间
 * @param state 修改节点通知的目标
 */
void m_change_schedule_run_notice_node(uint64_t id, uint64_t in_notice_time, uint8_t state);

/**
 * @brief 计划运行中的事件处理，想用你处理的是一些通知服务器之类的，会传递事件id 和 计划本身
 *
 * @param event_id 事件
 * @param in_data 计划
 * @return stat_m  如果返回fail 会导致计划异常
 */
stat_m m_ext_schedule_event_handle(int event_id, void *in_data, uint64_t);

/**
 * @brief 接收消息处理
 *
 * @param in_msg
 * @return stat_m
 */
stat_m m_msg_handle(char *in_msg);

/**
 * @brief wifi event handle
 *
 * @param arg
 */
void m_wifi_event_notice(void *arg);

/**
 * @brief 事件处理基类
 *
 * @param event_id
 * @param in_data
 */
void m_event_notice(int event_id, void *in_data);

/**
 * @brief 时间通知
 *
 * @param event_id
 * @param in_data
 */
void m_ext_event_time_notice(int event_id, void *in_data);

/**
 * @brief 
 * 
 * @param cmd 
 * @param msg 
 * @return stat_m 
 */
stat_m m_server_message_handle(int cmd, char *msg);


/**
 * @brief 允许删除一个元素
 * 
 * @param dele_cmd_flag 
 * @param dele_cmd_count_flag 
 * @return true 
 * @return false 
 */
bool if_allow_dele(uint64_t dele_cmd_flag, uint64_t dele_cmd_count_flag);




/**
 * @brief 调用和通知计划的执行
 * 
 * @param in_present_time 
 */
void m_callable_query_notice_schedule_run_node(uint64_t in_present_time);

/**
 * @brief 
 * 
 * @param id 
 * @param state 
 */
void m_remove_schedule_run_notice_node(uint64_t id, uint8_t state);
/**
 * @brief 
 * 
 * @param id 
 * @param state 
 */
void m_remove_schedule_run_notice_node_all(void);






/**.
 * 关于计划执行的时间中转中心
*/
stat_m m_event_transit_processing_schedule(void *event_data , uint8_t sta_tag,uint64_t in_dev_time);


/**
 * @brief 关于计划执行中的事件触发
 *
 * @param in_event_data 事件数据
 * @param in_event_data_type 事件数据类型
 * @param in_event_id 事件id
 * @param in_dev_time 触发时间
 * @return stat_m
 */
stat_m m_event_transit_processing_running_trigger(void *in_event_data ,uint8_t in_event_data_type ,uint8_t in_event_id ,uint64_t in_dev_time);


#define SOCK_TIMEOUT 5

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
/** 快速运行变化 （上一个或者下一个） */
#define M_CMD_TWOWAY_FASTRUN_CHANGE_STATUS 61
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












#define ID_SCH_WATER_EDN 25
#define ID_SCH_RAIN_BREAK 30
#define ID_SCH_WIND_BREAK 31
#define ID_SCH_FREEZ_BREAK 32
#define ID_SYN_SCH_CHANNEL_TIME 28
//   plan start zone
#define ID_SCH_CHANNEL_WATER_START 26
#define ID_SCH_CHANNEL_WATER_END 27

#define ID_MANUAL_CHANNEL_START 48
#define ID_MENUAL_CHANNEL_COMPLETE 49
#define DEVICE_OFFLINE_KEY_WATERING_START 85
#define ID_ZONE_WATER_TIME 29
/* SENSOR */
#define ID_SENSOR_ACHIEVED 34
#define ID_SENSOR_DISCONNECT 36
#define ID_RESP_SENSER_ACHIVED 34
/* HTTP/HTTPS */
/* FAST RUN */

#define M_CMD_TWOWAY_FAST_RUN_ADD 40
#define M_CMD_TWOWAY_FAST_RUN_NEXT 43
#define M_CMD_TWOWAY_FAST_RUN_PRE 39
#define M_CMD_TWOWAY_FAST_RUN_COMPL 42

/* TEST */
#define ID_DEVICE_INFO_SCH_LIST 57
#define ID_SCH_DEBUG_TIME 54
// #define UP_DATA "OTA_UPDATE"
// #define ID_UP_DATA 0x62
#define ID_DEVICE_MODE_DEBUG 59
// key imitate
#define ID_DEVICE_IMITATE_KEY 58

/* DEVICE FUN AND INFO QUERY*/
#define ID_DEVICE_INFO_CLEAN 52
#define ID_DEVICE_INFO_QUERY 50
#define ID_DEVICE_FUNCTION_LIGHT_ON 56
#define ID_DEVICE_FUNCTION_LIGHT_OFF 55

#define SETTIME 60
/** 设备添加成功*/
#define APP_ADD_DEVICE_SUCC 14
/** 重新启动设备*/
#define DEVICE_RESTART 17
/** 查询WiFi强度*/
#define QuERY_WIFI_SIGNAL_STRENGTH 86
/** 按键的快速切换*/
#define MANUAL_RUNNING_QUICK_SWITCH 51
#define SCH_WEATHER_SKIP 31
#define SCH_SENSOR_WATER_TRIG 6
#define SCH_SENSOR_WATER_RESEALE 7

#define QUICK_CHANNEL_PRE_CHANNEL 41
#define QUICK_CHANNEL_PAUSE 44
#define QUICK_CHANNEL_RECOVER 45
#define QUICK_CHANNEL_STOP 46
#define QUICK_CHANNEL_NEXT_CHANNEL 47
#define QUICK_RUN_FEEDBACK 61

#define SCH_NEXT_TIME 72
#define SKIP_SOMETIME_PERIOD_WATERING 62
#define NOTIFY_THAT_SOME_PLANS_ARE_BEING_SKIPPED  74
#define UNDO_PLAN_SKIP   75
#define UNDO_PLAN_SKIPS   76


#define UPDAT_SCH_CHANNEL_TIME 64
#define DEV_PLAN_CLASH_HUNG_UP 65
#define DEV_PLAN_CLASH_HUNG_UP_REGAIN 66
#define NETWORK_OUTAGE_RECONNECT 67
#define NETWORK_OUTAGE_KEY_RUNNING 68
#define NETWORK_OUTAGE_QUICK_RUNNING 69
#define SERVER_PLAN_SKIP_A_RUN 70
#define DEV_RESET 71
#define TEST_KEY_IMITATE 73
#define DEVICE_REQUEST_LOCAL_INFO 80
#define DEVICE_LOCAL_INFO_REP 81
#define DEVICE_LOCAL_INFO_CHANGE 82
#define DEVICE_LOCAL_INFO_CHANGE_REP 83
#define DEVICE_WEATHER_INFO_RESP 90
#define CAUSE_RESTARTD 13
#define SYNC_SCHEDULE 16
#define SYNC_SERVER_DEBUG 101
#define DEVICE_CLOSE_BUFF 1
#define DEVICE_SYSTEM_HALTED 2

// 计划通知跳过和恢复
#define NOTICE_PLAN_SKIP 4
#define UNNOTICE_PLAN_SKIP 5





#endif /* __NOTIFY__H__ */