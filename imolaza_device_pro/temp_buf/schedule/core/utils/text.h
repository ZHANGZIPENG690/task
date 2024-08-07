
#ifndef __TEXT_H
#define __TEXT_H



/* WIFI */
#define AP_CHINA_HCTJ_SSID "Intell"
#define AP_CHINA_HCTJ_PASSWORD "123123123"
#define STA_CHINA_HCTJ_SSID "HUAWEI_B316_AB84"
#define STA_CHINA_HCTJ_PASSWORD "sunnykong5555sdo"

#define SPTES "8cce4e9afcb8"
#define SP1 "a4e57cd69884"
#define SP2 "a4e57cd6a0c4"
#define SP3 "8cce4e9ad1b0"

#define SOCK_TIMEOUT 5

/* SERVER TRANSINFO */
#define HOME_SSID "HOME_SSID"
#define SET_DEVICE_SENSOR 8
#define QUERY_DEVICE_STATE 9
#define ID_HOME_SSID 10
#define ID_DEVICE_CNT 11
// updata wifi info
#define UPDATE_WIFI_INFO "DO_UPDATE_WIFI_INFO"
#define ID_UPDATE_WIFI_INFO 15
// time
#define SYN_TIME_RESP "SYN_TIME_RESP"
#define ID_SYN_TIME_RESP 12
#define SYN_WEATHER_INFO "SYN_WEATHER_INFO"
#define ID_SYN_WEATHER_INFO 0x22
#define GET_SUN_INFO "SYN_SUN_INFO"
#define ID_SYN_SUN_INFO 0x23
#define GET_DEVICE_TYPE "GET_DEVICE_TYPE"
#define ID_GET_DEVICE_TYPE 18
#define DEVICE_TYPE "DEVICE_TYPE"
#define DEV_REQ_OTAUPDATE 18
#define DEVICE_PREPARE_OTA_UPDATE 19
// #define DEVICE_PREPARE_OTA_UPDATE 16
#define ID_DEVICE_TYPE 18
#define SSID_WIFI "ssid_wifi0"
#define PSW_WIFI "psw_wifi0"
/* HTTP/HTTPS */
/* FAST RUN */
#define FAST_RUN "QUICK_CHANNEL_START"
#define ID_FAST_RUN 40
#define FAST_RUN_STOP "QUICK_CHANNEL_DELETE"
#define ID_FAST_RUN_NEXT 43
#define ID_FAST_RUN_PRE 39
#define RESP_FAST_RUN "QUICK_CHANNEL_START"
#define ID_FAST_RUN_COMPLETE 42
#define FAST_RUN_COMPLETE "QUICK_CHANNEL_COMPLETE"

/* PLAN */
#define ADD_PLAN_TO_DEVICE "SCH_ADD"
#define ID_ADD_PLAN_TO_DEVICE 20
#define RESP_ADD_PLAN_TO_DEVICE "SCH_ADD_REP"
#define ID_RESP_ADD_PLAN_TO_DEVICE 21 
#define DELETE_SCHDULES "SCH_DELETE"
#define ID_DELETE_SCHDULES 22
#define RESP_DELETE_SCHDULES "SCH_DELETE_REP"
#define ID_RESP_DELETE_SCHDULES 23
#define SCH_WATER_START "SCH_WATER_START"
#define ID_SCH_WATER_START 24
#define SCH_WATER_EDN "SCH_WATER_END"
#define ID_SCH_WATER_EDN 25
#define SCH_RAIN_BREAK "SCH_RAIN_BREAK"
#define ID_SCH_RAIN_BREAK 30
#define SCH_WIND_BREAK "SCH_WIND_BREAK"
#define ID_SCH_WIND_BREAK 31
#define SCH_FREEZ_BREAK "SCH_FREEZ_BREAK"
#define ID_SCH_FREEZ_BREAK 32
#define SYN_SCH_CHANNEL_TIME "SYN_SCH_CHANNEL_TIME"
#define ID_SYN_SCH_CHANNEL_TIME 28
//   plan start zone
#define SCH_CHANNEL_WATER_START "SCH_CHANNEL_WATER_START"
#define ID_SCH_CHANNEL_WATER_START 26
#define SCH_CHANNEL_WATER_END "SCH_CHANNEL_WATER_END"
#define ID_SCH_CHANNEL_WATER_END 27
// #define RESP_SCH_WATER_START "SCH_WATER_START"
// #define RESP_SCH_WATER_EDN "SCH_WATER_END"

#define ID_MANUAL_CHANNEL_START 48
#define ID_MENUAL_CHANNEL_COMPLETE 49
#define DEVICE_OFFLINE_KEY_WATERING_START 85

#define ZONE_WATER_TIME "ZONE_WATER_TIME"
#define ID_ZONE_WATER_TIME 29
/* SENSOR */
#define SENSOR_ACHIEVED "SENSOR_ACHIEVED"
#define ID_SENSOR_ACHIEVED 34
#define SENSOR_DISCONNECT "SENSOR_DISCONNECT"
#define ID_SENSOR_DISCONNECT 36
#define RESP_SENSER_ACHIVED "SENSER_ACHIVED"
#define ID_RESP_SENSER_ACHIVED 34
/* TEST */
#define DEVICE_INFO_SCH_LIST "DEVICE_INFO_SCH_LIST"
#define ID_DEVICE_INFO_SCH_LIST 57
#define SCH_DEBUG_TIME "DEVICE_INFO_DATE_CHANGE"
#define ID_SCH_DEBUG_TIME 54
// #define UP_DATA "OTA_UPDATE"
// #define ID_UP_DATA 0x62
#define DEVICE_MODE_DEBUG "DEVICE_MODE_DEBUG"
#define ID_DEVICE_MODE_DEBUG 59
// key imitate
#define DEVICE_IMITATE_KEY "DEVICE_IMITATE_KEY"
#define ID_DEVICE_IMITATE_KEY 58

/* DEVICE FUN AND INFO QUERY*/
#define DEVICE_CNT "DEVICE_CNT"
#define DEVICE_INFO_CLEAN "DEVICE_INFO_CLEAN"
#define ID_DEVICE_INFO_CLEAN 52
#define DEVICE_INFO_QUERY "DEVICE_INFO_QUERY"
#define ID_DEVICE_INFO_QUERY 50
#define DEVICE_FUNCTION_LIGHT_ON "DEVICE_FUNCTION_LIGHT_ON"
#define ID_DEVICE_FUNCTION_LIGHT_ON 56
#define DEVICE_FUNCTION_LIGHT_OFF "DEVICE_FUNCTION_LIGHT_OFF"
#define ID_DEVICE_FUNCTION_LIGHT_OFF 55
/* NVS */
#define TEMP_PLAN_RUN_TIME "pl_run_tim"
#define TIME "time"
#define PAGE1 "pag1"
#define PAGE2 "pag2"
#define VERSION "version"
#define NVS_KEY_SN "*SN"
#define NVS_KEY_EN_KEY "*encrKey"
#define STARTUP "STARTUP"
#define PLAN_NUM "PL_NUM"
#define PLAN_LIST "PL_LIST"
#define RESET_SW "rtsw"
#define RESET_HW "rthw"
#define SERVER_IPADDR "serid"

#define DEFINE_ESP_RST_UNKNOWN "Resreset reason could not be determined"
#define DEFINE_ESP_RST_POWERON "Reset due to a power event."
#define DEFINE_ESP_RST_SW "Due to software esp_restart"
#define DEFINE_ESP_RST_PANIC "Software reset due to abnormality/panic"
#define DEFINE_ESP_RST_INT_WDT "Reset due to interrupt watchdog"
#define DEFINE_ESP_RST_TASK_WDT "Reset due to task watchdog"
#define DEFINE_ESP_RST_BROWNOUT "RBrownout reset (software or hardware)"
#define DEFINE_ESP_RST_WDT "DOG BOM"

#define DEVICE_OTASTART "[19]#@#"
#define PING "ping" 

#define PING_TIME 70

#define PING_COUNT 7

#define PING_MAX 150

#define D_PLAN_SKIP "PK"
#define D_PLAN_SKIP_ONCE "PKA"

#define L_LEVEL "LL"
#define D_SENSOR "SS0"
#define D_DBUG "DB"
#define OFF_LINE_WIFI_SATAE "NAT"

/*  启用了传感器的计划标记，会伴随计划id一起保存 （新增和结束时间一起保存 PLAN_SENSOR_SKIP_REMARK） */
#define PLAN_SENSOR_SKIP_FLAG "P_SK"
/*  用于记录跳过标记的计划结束时间，以免计划重复跳过  */
#define PLAN_SENSOR_SKIP_REMARK_FLAG "P_MK"

/*  没有启用传感器时，记录的标记  */
#define PLAN_SENSOR_KXIN_SKIP_FLAG "P_KK"

// other
#define SKIP_ONE_RUN_As "Ss"
#define SKIP_ONE_RUN_Ae "Ss"

#define SKIP_SOMETIME_As "As"
#define SKIP_SOMETIME_Ae "Ae"
#define TEMP_WORD_PUBLIC_CMD "cmd"
#define TEMP_WORD_PUBLIC_SSID "ssid"
#define TEMP_WORD_PUBLIC_PSW "psw"
#define TEMP_WORD_PUBLIC_KEY "key"
#define TEMP_WORD_PUBLIC_INFO "info"
#define TEMP_WORD_PUBLIC_SUCC "success"
#define TEMP_WORD_PUBLIC_WIFI_NOT_F "no_found_wifi"
#define TEMP_WORD_PUBLIC_PSW_ERR "psw_err"

#define TEMP_WORD_KEY_IMITATE_LEFT "left"
#define TEMP_WORD_KEY_IMITATE_RIGHT "right"
#define TEMP_WORD_KEY_IMITATE_START "start"
#define TEMP_WORD_KEY_IMITATE_STOP "stop"
#define TEMP_WORD_PUBLIC_STAUS "status"
#define TEMP_WORD_DEBUG_OPEN "open"
#define TEMP_WORD_DEBUG_CLOSE "close"

#define TEMP_WORD_SYSTEM_DETYPE "dev_type"

#define TEMP_WORD_PUBLIC_NULL "null"
#define TEMP_WORD_PUBLIC_TYPE "type"
#define TEMP_WORD_PUBLIC_OK "ok"
#define TEMP_WORD_PUBLIC_FAIL "fail"
#define TEMP_WORD_PUBLIC_TIME "time"
#define TEMP_WORD_PUBLIC_PRESENT_TIME "present_time"

#define TEMP_WORD_PLAN_VALUE_SMARTY_CYCLE "smarty_cycle"
#define TEMP_WORD_PLAN_VALUE_START_DATE "start_date"
#define TEMP_WORD_PLAN_VALUE_END_DATE "end_date"
#define TEMP_WORD_PLAN_VALUE_WATER_START_TIME "water_start_time"
#define TEMP_WORD_PLAN_VALUE_ZONE "zones"
#define TEMP_WORD_PLAN_VALUE_CYCLE_TYPE "water_cycle_type"
#define TEMP_WORD_PLAN_VALUE_SKIP_WIND "wind_skip"
#define TEMP_WORD_PLAN_VALUE_SKIP_RAIN "rain_skip"
#define TEMP_WORD_PLAN_VALUE_SKIP_FREEZ "freez_skip"
#define TEMP_WORD_PLAN_VALUE_CYCLE_VALUE "water_cycle_value"
#define TEMP_WORD_PLAN_VALUE_PRIORITY "priority"

#define TEMP_WORD_PUBLIC_PLAN_TASK_ID "schedule_id"
#define TEMP_WORD_PUBLIC_FAST_TASK_ID "task_id"
#define TEMP_WORD_PUBLIC_PLAN_CHANNEL_ID "channel_id"
#define TEMP_WORD_PUBLIC_PLAN_RUNTIME "run_time"
#define TEMP_WORD_PUBLIC_PLAN_STATE "plan_state"
#define TEMP_WORD_PUBLIC_PLAN_CHANNEL "channel"

#define TEMP_SYNTAGMAA_DEBUG_TASK_INVA "task water_start_time invalid !!"
#define TEMP_SYNTAGMAA_DEBUG_TASK_NOT_RUN "The plan is not within the execution scope !"
#define TEMP_SYNTAGMAA_DEBUG_TASK_NOT_FIND "not found sch list !!"
#define TEMP_SYNTAGMAA_DEBUG_TASK_NOT_RUNUN "There are higher priority plans running !!( It could be smart plans )"
#define TEMP_SYNTAGMAA_DEBUG_TASK_NOT_SPEC " This plan should not be run"

#define TEMP_SYNTAGMAA_CAUSE_RESTART_DUE_WIFI "wifi connectiong fail"
#define TEMP_SYNTAGMAA_CAUSE_RESTART_DUE_NETWORK "network connection fail"
#define TEMP_SYNTAGMAA_CAUSE_RESTART_DUE_SOCK "socket connection fail"
#define TEMP_SYNTAGMAA_CAUSE_RESTART_DUE_TASKWDT "task wdt"
#define TEMP_SYNTAGMAA_CAUSE_RESTART_DUE_INWDT "in wdt"
#define TEMP_SYNTAGMAA_CAUSE_RESTART_DUE_HW_IS31 "is31 not found"
#define TEMP_SYNTAGMAA_CAUSE_RESTART_DUE_HW_IOIO "ioio not found"
#define TEMP_SYNTAGMAA_CAUSE_RESTART_DUE_HSW_POWER "sw reset or power reset"
#define TEMP_SYNTAGMAA_CAUSE_RESTART_DUE_OTHER "other"

/*  */
#define TEMP_CONFIG_WIFI_UDP_APINFO "Imolaza"
#define TEMP_CONFIG_WIFI_UDP_IMOLAZA_OFFLINE_WIFI_CHANGE "IOFWC"
#define TEMP_CONFIG_WIFI_UDP_IMOLAZA_DONE_OFFLINE_WIFI_CHANGE "DIOFWC"
#define TEMP_CONFIG_WIFI_UDP_SERVER_IPADDR "192.168.4.255"


#define M_INV1 "|"
#define M_INV7 "&"




#define M_D "%d"
#define M_HD "%hd"
#define M_HHD "%hd"
#define M_LLD "%lld"
#define M_STRM_STR_3(a,b,c) a b c


/* new */
#define SETTIME 60
#define APP_ADD_DEVICE_SUCC 14
#define DEVICE_RESTART 17
#define QuERY_WIFI_SIGNAL_STRENGTH 86

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
#endif

