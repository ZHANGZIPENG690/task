#ifndef __UTIL__H
#define __UTIL__H

#include "core/core.h"
#include "string.h"



typedef char *QueData;

#define NODE_SIZE 31
#define RECE_BUF_SIZE 200

typedef struct queNode
{
    struct queNode *nextQueNode;
    struct queNode *preQueNode;
    char Qdata[RECE_BUF_SIZE];
    /** The cmd value of the current instruction  */
    /** 当前指令的cmd值  */
    uint64_t spec_main_cmd;
    /** Additional instruction parameters  */
    /** 指令附加参数，可以用表示及当前发送的叠加值  */
    uint64_t spec_cmd_flag;
    uint64_t timeout;
} QueNode;

typedef struct queRoot
{
    char name[10];
    QueNode *headQueNode;
    QueNode *tailQueNode;
    volatile int nodeSum;
} QueRoot;
typedef enum _type
{
    U8 = 0,
    U16,
    U32,
    U32_,
    U64,
    Int,
    Str,
    null
} type_m;

/**
 * @brief
 *
 * @param qRoot queue root node
 * @param outData Out of the data for the column
 * @param speccmd Node tag value
 * @param cmd_flag
 * @param timec
 * @return true
 * @return false
 */
bool m_static_queue_pro_pop(QueRoot *qRoot, char *outData, uint64_t *speccmd, uint64_t *cmd_flag, uint64_t *timec);

/**
 * @brief Processing of a queue adds a content
 *
 * @param qRoot queue root node
 * @param que_data Specific content as deposited
 * @param qn_len Content length saved
 * @param spec_cmd Content cmd saved
 * @param spec_cmd_flag content Value-added saved
 * @param timeout The content is timed out in time
 * @return true success /  false fail
 */
bool m_static_queue_pro_push(QueRoot *, const char *que_data, int qn_len, uint64_t spec_cmd, uint64_t spec_cmd_flag, uint64_t timeout);
/**
 * @brief Initialize nodes, each time each individual node queue needs to be initialized
 *
 * @param qRoot queue root node
 */
void m_static_queue_pro_init(QueRoot *);
/**
 * @brief Delete a head node
 *
 * @param qRoot queue root node
 */
void m_static_queue_pro_rm(QueRoot *);

// void put_in(uint16_t defin, stat_m stat);

// stat_m info_check(uint8_t left, uint8_t right);

// stat_m out_check(uint8_t vue);

#define INIT_NVS 1
#define INIT_LED 2
#define INIT_GPIO 3
#define INIT_HTTP_SER 4
#define INIT_WIFI 5 // 第一阶段
#define CONN_SOME_ONE 6
#define CONN_RECE_REQ 7
#define CONN_INIT_AP 8
#define CONN_RECE_SSID_PSW 9
#define CONN_SUCCESS 10 // 第二个阶�??

#define SYSTEM_OK 11
#define CONN_NETWORD 12
#define RECE_DATA 13
#define HAVE_NETWORD 14

#define SOCK_CREATE_SUCCESS 16
#define SOCK_CONN_SUCCESS 17
#define SOCK_RECEIVE_DATA 18
#define SYNCHRO_TIME 19
#define SYNCHRO_SUCCESS 20

#define WIFI_SERVER_INIT 21
#define AP_INIT 22

// 100--200  状态码     都是编号
#define FRAST_START_UP 101
// #define  SYSTEM_UP       102
#define CONFIG_NET 103
#define SYSTEM_UP 104
#define NO_SYSTEM_UP 105
#define NO_TIME_INFO 106
#define STAT_KEY_LEFT 110
#define STAT_KEY_RIGHT 111
#define STAT_KEY_PLAY 112
#define STAT_KEY_PAUSE 113
#define STAT_KEY_RESET 249
#define STAT_CONN_TEST 114
/// STA
#define STAT_STA_STRAT_READ 115
#define STAT_STA_SCAN_DONE 116
#define STAT_STA_SUCC_CONN 117
#define STAT_STA_STRAT 118
#define STAT_STA_STOP 119
#define STAT_STA_CONNECTED 120
#define STAT_STA_DIS_CONNECTED 121
#define STAT_STA_HANDLE_DONE 122
#define STAT_STA_CONN_RESTART 128
#define STAT_STA_GOT_IP 129
/// AP
#define STAT_AP_START 123
#define STAT_AP_STOP 124
#define STAT_AP_STACONNECTED 125
#define STAT_AP_STADIS_CONNECTED 126
#define STAT_AP_PROBEREQRECVED 127
//  Config
#define STAT_CONF_RECEIVER_PASW 130
//  SYSTEM
#define STAT_SYS_HAVE_PASW 140

//  Sconfig net
// #define STAT_CONFIG_NET_START           150

#define STAT_TIMER_OUT 130
#define STAT_NET 131
#define STAT_START_SYNC_TIME 132
#define STAT_NEED_SYNC_TIME 133

#define STAT_SOCK_CONNECT 134
#define STAT_SELF_CHECK 150

// plan
#define STAT_PLAN_START 190
#define STAT_PLAN_PAUSE 191
#define STAT_PLAN_FLASH 192
#define STAT_PLAN_FAST_RUN 193

// ota
#define STAT_OTA_START 200
#define STAT_OTA_RUNNING 201
#define STAT_OTA_FAIL 202
#define STAT_OTA_SUCCESS 203
#define STATRUNNING_IS_KEY_START 204
// mode
#define STAT_MODE_CONFIG_NET 99
#define STAT_MODE_DIRECT_CONN 206
#define STAT_KEY 207
#define STAT_KEY_RUN 208
#define STAT_IDLE 209
// #define STAT_DIRECT_SYSTEM_RESET 207
#define STAT_CONFIG_NET_FLISH 210
#define STAT_RUN_MODE_DEBUG 211
#define STAT_PING 212
#define STAT_NEED_WIFI_BREAK 214
#define STAT_NEED_OTAUPDATE 215
#define STAT_DEVICE_SYSYEM_H 220




char *toname(int en);

/**
 * @warning 这会损坏你的原内容
 *
 * @brief 解析一个标准格式 去掉',' 提取下一个到 ',' 为止的String参数
 *
 * @param out_targat  提取到的目标将从这里输出
 * @param in_src  输入提取源
 * @return stat_m 提取失败或者成功
 */
stat_m m_insi_foramt_str(char *out_targat, char *in_src);
/**
 * @warning 这会损坏你的原内容
 *
 * @brief 解析一个标准格式 去掉'[' 提取下一个到 ',' 为止的int参数
 * @param out_targat 解析的cmd
 * @param in_src 源数据
 * @return stat_m
 */
stat_m m_insi_foramt_int_cmd(int *out_targat, char *in_src);
/**
 * @warning 这会损坏你的原内容
 *
 * @brief 解析一个标准格式 去掉',' 提取下一个到 ',' 为止的&{format}参数
 *
 * @param format  example : %lld %hhd %d
 * @param out_targat  提取到的目标将从这里输出
 * @param in_src  输入提取源
 *
 * @return stat_m 提取失败或者成功
 */
stat_m m_insi_foramt_value(char *format, void *out_targat, char *in_src);

/**
 * @brief 从元字符中解析对应的两个参数
 *
 * @param [in] format example : 1&2
 * @param [in] arg1
 * @param [in] arg2
 * @param [in] in_src
 * @return stat_m
 */

/**
 * @warning 这会损坏你的原内容
 *
 * @brief 用一种特定的方式分割源字符
 *
 * @param out_targat
 * @param in_src
 * @return stat_m
 */
stat_m m_insi_foramt_handle_server_sub_str(char *out_targat, char *in_src, int *len);

stat_m m_insi_foramt_param(char *format, void *arg1, void *arg2, char *in_src);

/**
 * @brief 解析星期
 *
 * @param [in] format example : %d
 * @param [in] span 间隔参数  example : ';'
 * @param [in] out_pragma1 输出的参数，解析存放位置
 * @param [in] in_src example : 7;1;2;3;4;5;6
 * @return stat_m
 */
stat_m m_insi_foramt_week_c(char *format, char span, void *out_pragma1, char *in_src);




/**
 * @brief Convert a character into a time
 */
#define  M_INSI_FORMAT_TIME_DECODE(conchr,tm) tm ? (conchr - 'A') : ((conchr >= 'a') ? (((conchr) - 'a') + 36) : ((conchr >= 'A') ? ((conchr) - 'A') + 10 : ((conchr >= '0') ? ((conchr )- '0'): 0)))



/** json  */
/** json  */
/** json  */
/** json  */
/** json  */

/**
 * @brief create a new json table
 *
 * @param out_buf
 * @return stat_m
 */
stat_m m_mjson_create_new_table(char *out_buf);

/**
 * @brief
 *
 * @param out_buf  
 * @param cmd
 * @return stat_m
 */
stat_m m_mjson_prepare_cmd(char *out_buf, int cmd);
/**
 * @brief 组建一个json元素
 *
 * @param out_buf  缓冲
 * @param type 元素类型
 * @param in_vue 值
 * @return stat_m
 */
stat_m m_mjson_append_new_element(char *out_buf, int type, void *in_vue);

/**
 * @brief 完成
 *
 * @param out_buf
 * @param in_timesp
 * @return stat_m
 */
stat_m m_mjson_declare_table_finish(char *out_buf, uint64_t in_timesp);

/**
 * @brief 完成一个表格
 *
 * @param out_buf
 * @return stat_m
 */
stat_m m_mjson_done_a_table(char *out_buf);

// /**
//  * @brief Create a send data to server login in object
//  *
//  * @param cmd 指令 11 10 15 67
//  * @param name WiFi名字有的话，没有不传
//  * @param version 版本号有的话，没有必须有
//  * @param off_line_time 67  ->  离线时间
//  * @param sch_id  67 running  --> 计划id
//  * @param running_state  计划的运行状态
//  * @param remain_time  剩余运行时间
//  * @param off_line_resu  离线原因
//  * @param sch_sum  计划数量
//  * @return stat_m
//  */
// stat_m create_send_data_to_server_login_in(int cmd, char *name, char *version, int off_line_time,
//                                            uint64_t sch_id, int running_state, int remain_time, int off_line_resu, int sch_sum);

// /**
//  * @brief Create a send data to server 22 03 02 fastRun object
//  *
//  * @param cmd
//  * @param tid
//  * @param cv1
//  * @param cv1t
//  * @param tid2
//  * @param cv2
//  * @param cvt
//  * @param curt
//  * @param Tpause_Fstop
//  * @return stat_m
//  */
// stat_m create_send_data_to_server_22_03_02_fastRun(int cmd,
//                                                    uint64_t tid,
//                                                    uint8_t cv1,
//                                                    uint32_t cv1t,
//                                                    uint64_t tid2,
//                                                    uint8_t cv2,
//                                                    uint32_t cvt,
//                                                    uint32_t curt,
//                                                    bool Tpause_Fstop);
// /**
//  * @brief Create a send data to server object
//  *
//  * @param cmd
//  * @param type_1
//  * @param cv1
//  * @param type_2
//  * @param cv2
//  * @param type_3
//  * @param cv3
//  * @return stat_m
//  */
// stat_m create_send_data_to_server(int cmd, type_m type_1, void *cv1, type_m type_2, void *cv2, type_m type_3, void *cv3);
// /**
//  * @brief Create a send data to server 22 06 13 plan start object(2022-09-12 edit .. add chan_already_RunTimeSum)
//  * @param cmd
//  * @param schedule_id schedule_id
//  * @param start_channel_id start_channel_id
//  * @param shouldRunTime shouldRunTime
//  * @param current_will_run_time current_will_run_time
//  * @param schedule_end_time schedule_end_time
//  * @return stat_m
//  */
// stat_m create_send_data_to_server_22_06_13_plan_start(int cmd,
//                                                       uint64_t schedule_id,
//                                                       uint8_t start_channel_id,
//                                                       uint64_t shouldRunTime,
//                                                       uint32_t current_will_run_time,
//                                                       uint64_t schedule_end_time,
//                                                       uint64_t chan_already_RunTimeSum);

// // [64,开始计划ID,开始通道,开始通道运行总时长, 开始通道本次将运行时间,结束通道,结束通道累积运行时间,结束通道本次运行时间,通道结束状态,计划ID结束时间,时间戳]
// stat_m create_send_data_to_server_22_06_13(int cmd, uint64_t in_schid, uint8_t stc, uint32_t in_chanRunningSum, uint32_t in_currChanWillt,
//                                            uint8_t enc, uint32_t enct, uint32_t in_end_chanRunningt, bool Tpause_Fstop, uint64_t in_end_plant);
// /**   一般市计划跳过的采用这个*/
// stat_m create_send_data_to_server_22_07_29(int cmd, uint64_t in_schid, 
//                                             uint8_t stc, uint32_t in_chanShouldRunningSum,
//                                             uint32_t in_chanRunningSum, uint32_t in_plan_stop_time,
//                                             uint8_t stype, uint8_t sensor_zone, uint64_t runtime_intermediate);
void sch_print(m_watering_schedule *water_d);


stat_m m_operation_schedule_next_running_time(m_watering_schedule *in_Obj, uint64_t present_hhmmss);



#endif /* __UTIL__H */