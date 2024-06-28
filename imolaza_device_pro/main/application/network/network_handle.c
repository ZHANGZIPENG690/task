

#include "core/fset_lib.h"

#include "core/text.h"

// #include "../drive/drive.h"

char app_version[40] = {0};
char *output = NULL;
static char out_temp_char_a[40] = {0};
char out_temp_char_b[40] = {0};
/** 是否恢复了计划 标记  false 恢复了*/
bool login_recover_schedule = true;
/** 是否上限 标记 */
static bool on_line = false;
/** 当前是修改wifi 标记 */
static bool login_change_wifi = false;
/** 获取状态 */
int device_status = 0;
/** oTA 更新标记 */
int ota_upadata_start_flag_m = 0;
/** 文件全局的信号强度*/
static int sign_strage = 0;
/**  */
char temp_queue_cc[256];
/** 登陆队列和信息 14 */
char temp_queue_login_str[512];
// int device_status;
/** 离线WiFi 还是 添加设备 */
int is_add_device_or_offline_change_wifi = 0;
/** 临时使用变量*/
int temp_use_int_vue_a = 0;
int temp_use_int_vue_b = 0;
int temp_use_int_vue_c = 0;
uint64_t temp_use_u64_vue_a = 0;
uint64_t temp_use_u64_vue_b = 0;
uint64_t temp_use_u64_vue_c = 0;
int temp_use_int_vue_d = 0;
uint8_t temp_use_u8_vue_a = 0;
double temp_use_double_vue_a = 0.0;
double temp_use_double_vue_b = 0.0;

float temp_use_float_vue_a = 0.00;

static char tmp_sid[25] = {0};
static char tmp_use_str_a[25] = {0};
char tmp_use_str_b[25] = {0};
char tmp_use_str_c[25] = {0};
char tmp_pudate_url[120] = {0};

/**
         // uint64_t current_running_id = 0;
        // int current_running_state = 0;

        //   [67,85eeb05e94ec4a987e371966d9e14d7c,1.2.001,1,0,254,2,-1,2]
        //   cmd,token,version,schedule_number,reason,schedule_id,status,current_channel_remaining_run_time
        // m_callable_local_resp_to_remote_pro_max(M_CMD_NOTIFY_TO_SERVER_DEVICE_OFFLINE_TO_ONLINE,
        //                                         M_TYPE_Str, (void *)key,         // key
        //                                         M_TYPE_Str, (void *)app_version, // 版本信息

        //                                         M_TYPE_Int, (void *)&schedu_count,       // 计划数量
        //                                         M_TYPE_Int, (void *)&temp_use_int_vue_c, // 离线计数

        //                                         M_TYPE_U64, (void *)&temp_use_u64_vue_a, // 运行 id
        //                                         M_TYPE_Int, (void *)&temp_use_int_vue_b, // 运行 状态 1 4 5 6

        //                                         M_TYPE_U64, (void *)&temp_use_u64_vue_b, // 本次剩余运行时间

        //                                         M_TYPE_Int, (void *)(&temp_use_int_vue_c), // 原因
        //                                         M_TYPE_Str, (void *)temp_queue_cc,         // 队列
        //                                         M_TYPE_Int, (void *)&temp_use_int_vue_a,   // 队列位置
        //                                         M_TYPE_Int, (void *)&sign_strage,          // 信号
        //                                         M_TYPE_NULL, NULL,
        //                                         0);
 *
 */

/**
 * @brief 连接验证
 *
 * @param out_cmd
 */
void m_ext_network_config_user_reconnect_cmd(char *out_cmd)
{
    temp_use_u64_vue_c = temp_use_u64_vue_a = temp_use_int_vue_b = temp_use_int_vue_a = temp_use_int_vue_c = 0;
    memset(temp_queue_cc, 0, sizeof(temp_queue_cc));
    m_callable_device_proper_status_get(&device_status);
    m_ext_soft_drive_net_wifi_get_sign_strage(&sign_strage); // 信号强度

    /**
     *
     */
    // uint64_t current_running_remainning_time = 0;
    // m_callable_instance_manage_get_current_instance_sid_abd_cid(&current_running_id, &current_running_state);
    m_callable_instance_manage_get_current_instance_sid_abd_cid_adb_remainder_run_time(device_status, &temp_use_u64_vue_a,
                                                                                       &temp_use_u64_vue_c,
                                                                                       &temp_use_int_vue_c, &temp_use_u64_vue_b,
                                                                                       &temp_use_int_vue_b,
                                                                                       temp_queue_cc,
                                                                                       &temp_use_int_vue_a,
                                                                                       m_callable_timer_manage_get_device_time());
    /** [14,AppVersion,ScheduleNum,OfflineReason,ScheduleId,RunningState,区域本次剩余,QueueString,Postion,ConnectOption,WiFiSign] */
    // [14,version,schedule_id,status,current_channel_remaining_run_time,Queue,Postion,WiFiSign]
    sprintf(out_cmd, "[%d,%s,%lld,%d,%lld,%d,%s,%lld,%d,%d]", 14,
            app_version,        // 版本
            temp_use_u64_vue_a, // 计划 id
            temp_use_int_vue_c, // 区域ID
            temp_use_u64_vue_c, /** 实例ID */
            temp_use_int_vue_b, // 运行状态
            temp_queue_cc,      // 队列
            temp_use_u64_vue_b, // 剩余运行时间
            temp_use_int_vue_a, // 位置
            sign_strage         // 信号强度
    );
    DEBUG_TEST(DB_I, "Login  %s", out_cmd);
}
/**
 * @brief 设置设备状态 1   为离线改Wifi  0
 *
 */
void m_ext_network_set_device_login_state()
{
    is_add_device_or_offline_change_wifi = 1;
}

/** 在线改wifi计数连接超时 */
int online_change_wifi_num = 0;

static bool only_one_11 = true;
static int out_runmode = 0;
/**
 * @brief 返回登陆相关信息
 *
 * @param out_cmd 指令
 * @param count 次数
 * @return stat_m
 */
stat_m m_ext_network_config_user_login_cmd(char *out_cmd, int count)
{

    stat_m stat = succ_r;
    /**   设备发送到服务器的 CMD */
    int glo_cmd = 0;
    /** 计划数量 */
    int schedu_count = 0;
    sign_strage = 0;
    // char out_resp[120] = {0};
    char key[60] = {0};
    memset(out_cmd, 0, strlen(out_cmd));
    m_callable_device_attribute_get_running_mode(&out_runmode);
    m_callable_device_attribute_get_key(key);
    m_callable_device_attribute_get_app_version(app_version);
    m_callable_device_attribute_get_schedule_num(&schedu_count); // 计划数量
    m_callable_device_proper_status_get(&device_status);
    m_ext_soft_drive_net_wifi_get_sign_strage(&sign_strage); // 信号强度
    /** 连接诶方式 */
    int connect_option = m_callable_middle_network_net_connection_option_get_info();

    // 还是第一次登陆
    DEBUG_TEST(DB_W, "....Mode <count %d>  out_runmode: %d  login_change_wifi -> %d (%s)", count, out_runmode, login_change_wifi, out_temp_char_a);
    if (login_change_wifi && out_runmode != M_DEVICE_RUNNING_MODE_CONFIG)
    {
        /** [15,Token,AppVersion,ScheduleNum,Reason,连接方式,WiFiName,WiFiSign] */
        online_change_wifi_num++;
        if (online_change_wifi_num > 5)
        {
            online_change_wifi_num = 0;
            m_callable_network_disconnect();
            m_callable_middle_network_adapter_clear();
            m_callable_wifi_auto_connect();
            m_callable_network_set_connect_status(false);
            strcpy(out_temp_char_b, "no_found_wifi");
            m_callable_device_attribute_get_current_connect_net_name(out_temp_char_a);
        }
        sprintf(out_cmd, "[%d,%s,%s,%d,%s,%d,%s,%d]", 15, key, app_version, schedu_count, out_temp_char_b, connect_option, out_temp_char_a, sign_strage);
        stat = fail_r;
    }

    if (stat == succ_r)
    {
        m_callable_device_attribute_get_current_connect_net_name(out_temp_char_a);
        stat = succ_r;
        if (out_runmode == M_DEVICE_RUNNING_MODE_CONFIG)
        {
            glo_cmd = 10;
            /** [10,Token,AppVersion,ScheduleNum,Reason,连接方式,WiFiName,WiFiSign] */
        }
        // 不是第一次登陆
        else if (on_line || !only_one_11)
        {
            glo_cmd = 67;
            /** [11,Token,AppVersion,ScheduleNum,Reason,连接方式,WiFiName,WiFiSign] */
        }
        else
        {
            only_one_11 = false;
            is_add_device_or_offline_change_wifi = 1;
            /** [11,Token,AppVersion,ScheduleNum,Reason,连接方式,WiFiName,WiFiSign] */
            m_callable_drive_flash_read(M_TYPE_Int, RESTART_REUSE, &is_add_device_or_offline_change_wifi);
            glo_cmd = 11;
        }
        sprintf(out_cmd, "[%d,%s,%s,%d,%d,%d,%s,%d]", glo_cmd, key, app_version,
                schedu_count, is_add_device_or_offline_change_wifi, connect_option, out_temp_char_a, sign_strage);
    }
    DEBUG_TEST(DB_I, "Psw  %s", out_cmd);
    return stat;
}

/**
 * @brief 网络连接
 *
 * @param username  网络用户名
 * @param token 认证密钥
 * @return stat_m
 */
// stat_m m_ext_network_server_connect_to(char *username, int port)
// {
//     DEBUG_TEST( DB_I," m_ext_network_server_connect_to [ %s , %d ] ", username, port);
//     return 0;
// }

/**
 * @brief 收到的消息处理
 *
 * @param cmd 消息CMD
 * @param buf 消息内容
 * @return stat_m
 */
stat_m m_ext_server_message_handle(int cmd, char *buf)
{
    // pcf8563_time_t time;
    stat_m stat = succ_r;
    DEBUG_TEST(DB_I, "Msg Handle %d   %s", cmd, buf);
    int connect_count = 0;
    int http_connect_count = 0;
    enum m_paramete_enum mpe = M_TYPE_NULL;
    m_callable_network_get_connect_count(&connect_count);
    m_callable_device_proper_status_get(&device_status);
    uint64_t in_time_s = m_callable_timer_manage_get_device_time();
    DEBUG_TEST(DB_W, "connect count %d", connect_count);
    if (cmd != 16 && cmd != M_CMD_TOWAY_DEVICE_RECONNECT_TIPS && cmd != M_CMD_NOTIFY_TO_DEVICE_RESTART && cmd != M_CMD_TWOWAY_SET_DEVICE_TIME && m_callable_network_manage_get_server_login_status() != succ_r)
    {
        if (cmd != M_CMD_LOGIN_AUTHENTICATION && cmd != M_CMD_CONFIRM_BINDING && cmd != M_CMD_REMOVE_BINDING)
        {
            DEBUG_TEST(DB_W, "Not Login, ignore %d", cmd);
            stat = fail_r;
        }
    }
    // 没有恢复计划 不允许创建快速运行
    if (login_recover_schedule && cmd == M_CMD_TWOWAY_FASTRUN_START)
    {
        DEBUG_TEST(DB_W, "Not Init ! , ignore %d", cmd);
        stat = fail_r;
    }
    if ((m_callable_offline_bluetooth_login_flag() == succ_r) && (cmd == M_CMD_TWOWAY_REQU_SCHEDULE_WATERTIME))
    {
        DEBUG_TEST(DB_W, "ignore %d", cmd);
        stat = fail_r;
    }

    if (stat == succ_r)
    {
        DEBUG_TEST(DB_I, "Entry");
        switch (cmd)
        {
            /**
             * @brief 设置传感器 8
             *
             * @name [ 8 , a ]
             * @param  a 传感器参数格式
             */
        case M_CMD_TWOWAY_SET_SENSOR: /** 8 */
            temp_use_int_vue_a = 0;
            memset(tmp_use_str_b, 0, sizeof(tmp_use_str_b));

            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%d", &temp_use_int_vue_a);
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%*[^,],%d", &temp_use_int_vue_b);
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%*[^,],%*[^,],%s", tmp_use_str_b);

            DEBUG_TEST(DB_W, "temp_use_float_vue_a %s", tmp_use_str_b);

            if (temp_use_int_vue_b == 98 || temp_use_int_vue_b == 99)
            {
                float num1 = 0.0;
                float num2 = 0.0;
                m_callable_parsestring(tmp_use_str_b, &num1, &num2);
                if (temp_use_int_vue_b == 98)
                {
                    num1 = num1 / 3.785;
                }
                DEBUG_TEST(DB_W, "num1 %.05f ---- num2  %.05f", num1, num2);
                m_calllable_sensor_k_or_f_set(num1, num2);
            }

            m_callable_sensor_set(temp_use_int_vue_a, temp_use_int_vue_b, true);
            m_callable_local_resp_to_remote(M_CMD_TWOWAY_SET_SENSOR,
                                            M_TYPE_Int, (void *)&temp_use_int_vue_a,
                                            M_TYPE_Int, (void *)&temp_use_int_vue_b,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, true);
            break;
        case 5: /* 5 */

            break;
        case M_CMD_TWOWAY_SENSOR_TRIGE: /* 6 */

            break;

            /** 查询设备状态*/
        case M_CMD_TWOWAY_GET_DEVICE_STATUS: /* 9 */
            temp_use_int_vue_b = 0;
            temp_use_u64_vue_a = 2; /** 状态 */
            temp_use_int_vue_d = 0;
            temp_use_u64_vue_b = None; /** 运行id */
            temp_use_u64_vue_c = 0;    /** 剩余运行时间 */
            enum seneor_chann sen_chan;
            enum seneor_type sen_type;

            memset(temp_queue_cc, 0, sizeof(temp_queue_cc));
            m_callable_device_attribute_get_sn(out_temp_char_a);
            m_callable_device_attribute_get_schedule_num(&temp_use_int_vue_a); // 计划数量
            m_callable_instance_manage_get_current_instance_sid_abd_cid_adb_remainder_run_time(device_status, &temp_use_u64_vue_b,
                                                                                               &temp_use_u64_vue_a,
                                                                                               &temp_use_int_vue_d, &temp_use_u64_vue_c,
                                                                                               &temp_use_int_vue_c,
                                                                                               temp_queue_cc,
                                                                                               &temp_use_int_vue_b,
                                                                                               m_callable_timer_manage_get_device_time());

            if (m_callable_offline_bluetooth_login_flag() == succ_r)
            {

                if ((stat = m_callable_sensor_current_is_effective(&sen_chan, &sen_type, in_time_s)) == M_ERR)
                {
                    m_run_time.sensor_mode = 0;
                }
                else
                {
                    if ((stat = m_callable_sensor_current_is_effective(&sen_chan, &sen_type, in_time_s)) == succ_r)
                    {
                        m_run_time.sensor_mode = 7;
                    }
                    else
                    {
                        m_run_time.sensor_mode = 6;
                    }
                }

                m_callable_local_resp_to_remote_pro_max(M_CMD_TWOWAY_GET_DEVICE_STATUS,
                                                        M_TYPE_Str, (void *)app_version,           // 版本信息
                                                        M_TYPE_Str, (void *)out_temp_char_a,       // sn
                                                        M_TYPE_Int, (void *)&temp_use_int_vue_a,   // 计划数量
                                                        M_TYPE_Int, (void *)&temp_use_int_vue_b,   // 离线计数
                                                        M_TYPE_U64, (void *)&temp_use_u64_vue_b,   // 运行 id
                                                        M_TYPE_Int, (void *)&temp_use_int_vue_d,   // 运行 区域
                                                        M_TYPE_U64, (void *)&temp_use_u64_vue_c,   // 总剩余运行时间
                                                        M_TYPE_Int, (void *)(&temp_use_int_vue_c), // 运行 状态 1 4 5 6
                                                        M_TYPE_Str, (void *)temp_queue_cc,         // 队列
                                                        M_TYPE_Int, (void *)&temp_use_int_vue_b,   // 队列位置
                                                                                                   // M_TYPE_Int, (void *)&sign_strage,          // 信号
                                                        M_TYPE_Str, (void *)m_run_time.output_str,
                                                        M_TYPE_Int, (void *)&m_run_time.sensor_mode,
                                                        in_time_s);
            }
            else
            {
                m_callable_local_resp_to_remote_pro_max(M_CMD_TWOWAY_GET_DEVICE_STATUS,
                                                        M_TYPE_Str, (void *)app_version,           // 版本信息
                                                        M_TYPE_Str, (void *)out_temp_char_a,       // sn
                                                        M_TYPE_Int, (void *)&temp_use_int_vue_a,   // 计划数量
                                                        M_TYPE_Int, (void *)&temp_use_int_vue_b,   // 离线计数
                                                        M_TYPE_U64, (void *)&temp_use_u64_vue_b,   // 运行 id
                                                        M_TYPE_Int, (void *)&temp_use_int_vue_d,   // 运行 区域
                                                        M_TYPE_U64, (void *)&temp_use_u64_vue_c,   // 总剩余运行时间
                                                        M_TYPE_Int, (void *)(&temp_use_int_vue_c), // 运行 状态 1 4 5 6
                                                        M_TYPE_Str, (void *)temp_queue_cc,         // 队列
                                                        M_TYPE_Int, (void *)&temp_use_int_vue_b,   // 队列位置
                                                                                                   // M_TYPE_Int, (void *)&sign_strage,          // 信号
                                                        M_TYPE_NULL, NULL,
                                                        M_TYPE_NULL, NULL,
                                                        in_time_s);
            }

            break;
        // 设置时间和向服务器获取时间
        case M_CMD_TWOWAY_SET_DEVICE_TIME: /* 12 */
            /**
             * @brief 时间-登陆标记
             *
             * @name [ 12 , time , time_zone ]
             * @param  time 时间
             * @param  time_zone 时区
             */
            DEBUG_TEST(DB_W, "befTime -- > %lld", in_time_s);
            /** 时间、时区、类型 */
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%lld,%d,%d", &temp_use_u64_vue_a, &temp_use_int_vue_a, &temp_use_int_vue_c);
            /** 修改当前时间 */
            in_time_s = temp_use_u64_vue_a + temp_use_int_vue_a * 3600;
            DEBUG_TEST(DB_W, "afterTime -- > %lld", in_time_s);
            /** 计划在运行，前后时间不超过 5 s */
            if (device_status == M_DEVICE_GLOBAL_STATUS_SCHEDULE_RUNNING && temp_use_u64_vue_a - m_callable_timer_manage_get_utc_time() < 5)
            {
                DEBUG_TEST(DB_W, "time update Den : %lld And  %lld", temp_use_u64_vue_a, m_callable_timer_manage_get_utc_time());
            }
            else
                m_callable_timer_manage_time_set(temp_use_u64_vue_a, temp_use_int_vue_a, temp_use_int_vue_c);

            on_line = true;
            m_callable_network_set_server_login_status(true);
            m_ext_software_drive_net_wifi_ip_update();
            m_callable_device_attribute_http_proxy_save_addr();
            // m_callable_middle_connect_handle_set_code(M_CONNECT__CODE_SERVER_LOGIN_AND_CONNECT_OK);
            if (login_change_wifi)
            {
                {
                    DEBUG_TEST(DB_I, "%s %s", out_temp_char_b, out_temp_char_a);
                }
                if ((strstr(out_temp_char_b, "succ") != NULL &&
                     m_callable_device_attribute_get_current_is_net_name_same(out_temp_char_a) != succ_r))
                    m_callable_config_net_save();
                m_callable_device_attribute_get_current_connect_net_name(out_temp_char_a);
                login_change_wifi = false;
            }
            if (out_runmode == M_DEVICE_RUNNING_MODE_CONFIG)
            {
                m_callable_config_net_save();
            }
            if (login_recover_schedule)
            {
                m_callable_local_resp_to_remote(M_CMD_TWOWAY_GET_OR_DEMAND_DEVICE_UPDTAE, M_TYPE_NULL, NULL,
                                                M_TYPE_NULL, NULL,
                                                M_TYPE_NULL, NULL,
                                                M_TYPE_NULL, NULL, in_time_s, false);
            }
            else
            {
                m_callable_device_attribute_get_schedule_num(&temp_use_int_vue_c);
                m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_DEVICE_SYNC_SCHEDULE, M_TYPE_Int, &temp_use_int_vue_c,
                                                M_TYPE_NULL, NULL,
                                                M_TYPE_NULL, NULL,
                                                M_TYPE_NULL, NULL, in_time_s, false);
            }

            break;
        case M_CMD_TOWAY_DEVICE_RECONNECT_TIPS: /* 14 */
            m_ext_network_config_user_reconnect_cmd(temp_queue_login_str);
            m_callable_spec_scene_net_connext_tx(temp_queue_login_str, strlen(temp_queue_login_str), 10, 0, M_TIMEOUT_LONG_LIMIT_MIN, false);

            break;
        case M_CMD_TOWAY_DEVICE_CHANGEWIFI: /* 15 */
            // [15 ,  ssid   psw]
            m_callable_middle_network_adapter_clear();
            m_callable_network_disconnect();
            m_callable_network_server_disconnect();
            m_callable_data_parse_foramt_str(out_temp_char_a, buf);
            m_callable_config_net_set_username(out_temp_char_a);
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%s", tmp_sid);
            m_callable_config_net_set_token(tmp_sid);

            DEBUG_TEST(DB_I, "%s", out_temp_char_a);
            m_callable_network_set_auto_connect(false);
            m_callable_network_set_net_auto_connect(false);
            stat = m_callable_config_net_start();
            login_change_wifi = true;
            m_callable_network_set_status_change_net();
            m_callable_network_set_net_auto_connect(true);
            m_callable_network_set_auto_connect(true);
            if (stat == succ_r)
            {
                strcpy(out_temp_char_b, "success");
            }
            else
            {
                DEBUG_TEST(DB_W, "Change Fail ..");
                m_callable_wifi_auto_connect();
                m_callable_device_attribute_get_current_connect_net_name(out_temp_char_a);
                strcpy(out_temp_char_b, "no_found_wifi");
            }

            break;
        //     // 同步计划, 结束  恢复计划
        case M_CMD_NOTIFY_TO_SERVER_DEVICE_SYNC_SCHEDULE: /* 16 */

            if (login_recover_schedule)
            {
                m_callable_schedule_stroage_recover_item_all();
                login_recover_schedule = false;
            }
            login_recover_schedule = false;
            mDelay_ms(200);
            if (m_callable_ota_silent_updata_flag_get() == false)
            {
                m_callable_ota_silent_updata_start_init(in_time_s);
            }

            break;
        // 远程重启设备
        case M_CMD_NOTIFY_TO_DEVICE_RESTART: /* 17 */

            mReboot(M_RESTART_CAUSE_REMOTE_ACTIVE);
            break;
            // 请求更新设备或者要求新设备
        case M_CMD_TWOWAY_GET_OR_DEMAND_DEVICE_UPDTAE: /* 18*/
            if (strstr(buf, "null,null"))
            {
                temp_use_int_vue_c = 1;
            }
            else if (device_status != M_DEVICE_GLOBAL_STATUS_IDLE)
            {
                m_callable_data_parse_foramt_str(tmp_pudate_url, buf);
                m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_a, buf);
                if (temp_use_int_vue_a == 1)
                {
                    temp_use_int_vue_c = 1;
                }
                else
                {
                    temp_use_int_vue_c = 2;
                }
            }
            else
            {
                temp_use_int_vue_c = 3;
            }

            if (temp_use_int_vue_c == 1)
            {
                m_callable_device_attribute_get_schedule_num(&temp_use_int_vue_a);
                m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_DEVICE_SYNC_SCHEDULE, M_TYPE_Int, &temp_use_int_vue_a,
                                                M_TYPE_NULL, NULL,
                                                M_TYPE_NULL, NULL,
                                                M_TYPE_NULL, NULL, in_time_s, false);
            }
            else if (temp_use_int_vue_c == 2)
            {
                if (temp_use_int_vue_a == 2)
                {
                    if (m_callable_ota_silent_updata_flag_get() == false)
                    {

                        stat = m_callable_drive_flash_write(M_TYPE_Str, OTA_URL, tmp_pudate_url);
                        if (stat == succ_r)
                        {
                            m_callable_ota_silent_updata_start_init(in_time_s);
                        }
                    }
                    else
                    {
                        DEBUG_TEST(DB_E, "OTA静默更新已存在,无需重复发送113指令");
                    }
                }
                else if (temp_use_int_vue_a == 0)
                {
                    m_callable_ota_silent_updata_stop();
                }
            }
            if (temp_use_int_vue_c == 3)
            {
                m_callable_data_parse_foramt_str(tmp_pudate_url, buf);
                m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_a, buf);
                DEBUG_TEST(DB_W, "%s-----   ----%d", tmp_pudate_url, temp_use_int_vue_a);
                if (temp_use_int_vue_a == 1)
                {
                    m_callable_ota_init();

                    for (int i = 0; i < 6; i++)
                    {
                        if (m_callable_ota_enable_silent_updata(tmp_pudate_url) == succ_r)
                        {
                            stat = succ_r;
                            if (m_callable_ota_check_app_version() == succ_r)
                            {
                                ota_upadata_start_flag_m = 1;
                                m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_DEVICE_CONFIRM_UPDATA,
                                                                M_TYPE_Int, (void *)&temp_use_int_vue_a,
                                                                M_TYPE_NULL, NULL,
                                                                M_TYPE_NULL, NULL,
                                                                M_TYPE_NULL, NULL, in_time_s, true);
                            }
                            break;
                        }
                        else
                        {
                            DEBUG_TEST(DB_E, "ota http 请求失败");
                            http_connect_count++;
                        }
                    }
                    if (http_connect_count >= 5)
                    {
                        DEBUG_TEST(DB_E, "ota http 请求失败 大于等于 5次,设备需要重启");
                        mReboot(M_RESTART_CAUSE_PREMEDITATED);
                    }
                }
                else if (temp_use_int_vue_a == 2)
                {
                    if (m_callable_ota_silent_updata_flag_get() == false)
                    {

                        stat = m_callable_drive_flash_write(M_TYPE_Str, OTA_URL, tmp_pudate_url);
                        if (stat == succ_r)
                        {
                            m_callable_ota_silent_updata_start_init(in_time_s);
                        }
                    }
                    else
                    {
                        DEBUG_TEST(DB_E, "OTA静默更新已存在,无需重复发送113指令");
                    }
                }
                else if (temp_use_int_vue_a == 0)
                {
                    m_callable_ota_silent_updata_stop();
                }
            }

            break;
            // 设备向服务器通知更新开始
        case M_CMD_NOTIFY_TO_SERVER_DEVICE_CONFIRM_UPDATA: /* 19 */
            if (ota_upadata_start_flag_m == 1)
            {
                ota_upadata_start_flag_m = 0;
                m_callable_device_proper_status_update_to_update();
                m_callable_network_set_auto_connect(false);
                m_callable_network_server_disconnect();
                m_callable_ota_start();
                m_callable_network_set_auto_connect(true);
            }
            break;
            /* PLAN */
            /** 添加一个计划到设备*/
        case M_CMD_TWOWAY_ADD_A_SCHEDULE: /* 20 */
            temp_use_u64_vue_a = 0;
            stat = m_callable_instance_manage_add(buf, M_INSTANCE_ADD_MODE_ONLINE, in_time_s, &temp_use_u64_vue_a);
            if (stat == succ_r)
                mpe = M_TYPE_Str_OK;
            else
                mpe = M_TYPE_Str_FAIL;
            m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_ADD_SCH_RESP, M_TYPE_U64, (void *)&temp_use_u64_vue_a,
                                            mpe, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, true);
            break;
            //     /** 反馈计划添加成功与否*/
            // case M_CMD_NOTIFY_TO_SERVER_ADD_SCH_RESP: /* 21 */
            //     break;
            /** 删除计划*/
        case M_CMD_TWOWAY_DELE_SCHEDULE: /* 22 */
            m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_a, buf);
            if (temp_use_u64_vue_a == M_CONST_SACK_NUM)
                stat = m_callable_instance_manage_remove_all();
            else
                stat = m_callable_instance_manage_remove(temp_use_u64_vue_a, in_time_s);
            m_callable_local_resp_to_remote(M_CMD_TWOWAY_DELE_SCHEDULE_RESP, M_TYPE_U64, (void *)&temp_use_u64_vue_a,
                                            M_TYPE_Str_OK, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, true);
            break;
            //     /** 反馈计划删除成功与否*/
            // case M_CMD_TWOWAY_DELE_SCHEDULE_RESP: /* 23 */
            //     break;
            //     /** 通知计划开始*/
            // case M_CMD_NOTIFY_TO_SERVER_SCHEDULE_START: /* 24 */
            //     break;
            //     /** 通知计划开始切换区域运行*/
            // case M_CMD_NOTIFY_TO_SERVER_SCHEDULE_SWITCH: /* 64 */
            //     break;
            //     /** 通知计划结束*/
            // case M_CMD_NOTIFY_TO_SERVER_SCHEDULE_END: /* 25 */

            //     break;
            /** 区域设置 */
        case M_CMD_TWOWAY_SET_ZONE: /* 26 */
            /**
             * @brief 区域设置 开关 [26,15,1,1697109978,1697109978]   --> [26,15,11697109978,1697109978,1697109978]
             *
             * @name [ 26 , zone , enable , start , end ]
             * @param  zone 区域 id
             * @param  enable 启用禁用 enable
             * @param  start 如果是关闭的话 这个是时间
             * @param  end 如果是关闭的话 这个是时间
             */
            m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_a, buf);
            m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_b, buf);
            m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_a, buf);
            m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_b, buf);
            stat = m_callable_channel_manage_state_set(temp_use_int_vue_b, temp_use_int_vue_a, temp_use_u64_vue_a, temp_use_u64_vue_b);

            m_callable_local_resp_to_remote(M_CMD_TWOWAY_SET_ZONE, M_TYPE_Int, (void *)&temp_use_int_vue_a,
                                            M_TYPE_Int, (void *)&temp_use_int_vue_b,
                                            M_TYPE_U64, (void *)&temp_use_u64_vue_a,
                                            M_TYPE_U64, (void *)&temp_use_u64_vue_b, in_time_s, true);

            break;
            /** 计划启用和禁用 */
        case M_CMD_TWOWAY_SET_SCHEDULE_ABLE: /* 27*/
            /**
             * @brief 计划启用禁用    [27, 3605 , 1 , 1655321995]
             *
             * @name [ 27 , id , state ]
             * @param  id 计划 id
             * @param  state 启用禁用 enable
             */

            m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_a, buf);
            m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_a, buf);
            if (temp_use_int_vue_a)
                stat = m_callable_instance_manage_enable(temp_use_u64_vue_a);
            else
                stat = m_callable_instance_manage_disable(temp_use_u64_vue_a);

            m_callable_local_resp_to_remote(M_CMD_TWOWAY_SET_SCHEDULE_ABLE, M_TYPE_U64, (void *)&temp_use_u64_vue_a,
                                            M_TYPE_Int, (void *)&temp_use_int_vue_a,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, true);
            break;
            //     /** 同步浇水时间 */
            // case M_CMD_TWOWAY_POWEROFF_ONLINE_SYNC_WATERTIME: /* 28 */

            //     break;
            //     /** 废弃 */
            // case M_CMD_TWOWAY_POWEROFF_ONLINE_SE: /* 29*/

            //     break;
            /** 提前 9 分 30 秒 或者 1 分 30 秒 请求 浇水时间 */
        case M_CMD_TWOWAY_REQU_SCHEDULE_WATERTIME: /* 30 */
            /**
             * @brief 计划启用禁用
             *
             * @name [ 30 , id , skip_zone , time , weather ]
             * @param  id 计划 id
             * @param  skip_zone 1|2|5
             * @param  time 1#30;2#40
             * @param  weather 0 - 1 - 2 - 3
             */
            m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_a, buf);
            m_callable_smart_running_storage_append(temp_use_u64_vue_a, buf, in_time_s);

            break;
            //     /** 计划被分域跳过s */
            // case M_CMD_NOTIFY_TO_SERVER_SCHEdULE_SKIP: /* 31 */
            //                                            /** 设备更新天气缓存 */

            //     break;
            // case M_CMD_TWOWAY_REQU_WEATHER_BUFF: /* 32 */

            //     break;
            //     /** 未作任何事的传感器接入 */
            // case M_CMD_NOTIFY_TO_SERVER_SENSOR_CONNECT: /* 34 */

            //     break;
            //     /** 新草坪模式 后续时间相关的计划参数 */
            // case M_CMD_TWOWAY_NEW_LAWN_TIME_INFO: /* 35 */

            //     break;
            //     /** 未作任何事的传感器断开 */
            // case M_CMD_NOTIFY_TO_SERVER_SENSOR_DIS: /* 36 */

            //     break;
            /** 设备休眠模式 */
        case M_CMD_TWOWAY_HIBERNATION_MODE: /* 37 */
            /**
             * @brief 设备休眠模式
             *
             * @name [ 37 , start , end , mode ]
             * @param  start 计划 id
             * @param  end 1|2|5
             * @param  mode 1#30;2#40
             */
            m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_a, buf);
            m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_b, buf);
            m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_c, buf);

            m_callable_device_dormant_set(temp_use_u64_vue_a, temp_use_u64_vue_b, temp_use_u64_vue_c);

            m_callable_local_resp_to_remote(M_CMD_TWOWAY_HIBERNATION_MODE, M_TYPE_U64, (void *)&temp_use_u64_vue_a,
                                            M_TYPE_U64, (void *)&temp_use_u64_vue_b,
                                            M_TYPE_U64, (void *)&temp_use_u64_vue_c,
                                            M_TYPE_NULL, NULL, in_time_s, true);
            break;
            /** 泵模式开启或者关闭   */
        case M_CMD_TWOWAY_PUMP_MODE: /* 38 */
            m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_a, buf);
            m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_b, buf);
            m_callable_device_attribute_set_soledold(temp_use_int_vue_a, temp_use_int_vue_b);
            m_callable_local_resp_to_remote(M_CMD_TWOWAY_PUMP_MODE, M_TYPE_Int,
                                            (void *)&temp_use_int_vue_a,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, true);

            break;
            /** 快速运行上一个 */
        case M_CMD_TWOWAY_FASTRUN_SWITCH_PRE: /* 39*/
            // m_callable_instance_running_event_input(CHANGING_OVER_LEFT_TRIGGER, in_time_s);
            if (m_callable_current_batch_area_power_calibration_flag_get() == 0)
            {
                m_callable_instance_running_event_input(CHANGING_OVER_RIGHT_TRIGGER, in_time_s);
            }

            // m_callable_instance_running_event_input(CHANGING_OVER_LEFT_TRIGGER, in_time_s);
            break;

            /** 快速运行开始 */
        case M_CMD_TWOWAY_FASTRUN_START: /* 40 */
            /** 添加一个快速运行*/
            m_callable_current_batch_area_power_calibration_flag_set(0);
            m_callable_instance_manage_quick_gener(buf, in_time_s);
            break;
            //     /** 快速运行完成 */
            // case M_CMD_TWOWAY_FASTRUN_COMPLETE: /* 42 */
            break;
            /** 快速运行下一个 */
        case M_CMD_TWOWAY_FASTRUN_SWITCH_NEXT: /* 43 */
            if (m_callable_current_batch_area_power_calibration_flag_get() == 0)
            {
                m_callable_instance_running_event_input(CHANGING_OVER_LEFT_TRIGGER, in_time_s);
            }

            break;
            /** 快速运行暂停 */
        case M_CMD_TWOWAY_FASTRUN_PAUSE: /* 44 */
            if (m_callable_current_batch_area_power_calibration_flag_get() == 0)
            {
                m_callable_instance_running_event_input(CHANGING_OVER_START_TRIGGER, in_time_s);
            }

            break;
            /** 快速运行恢复 */
        case M_CMD_TWOWAY_FASTRUN_RECOVER: /* 45 */
            m_callable_instance_running_event_input(CHANGING_OVER_START_TRIGGER, in_time_s);
            break;
            /** 全局停止运行 */
        case M_CMD_TWOWAY_GLOBAL_RUN_STOP: /* 46 */
            // m_callable_drive_button_event_input(M_KEY_EVENT_STOP);
            m_ext_drive_button_event_handle(M_KEY_EVENT_STOP, M_KEY_EVENT_SIGN_CLICK, 0, M_CONST_SACK_NUM, in_time_s);

            // m_callable_instance_running_event_input(CHANGING_OVER_STOP_TRIGGER, 0);
            break;
            /** 手动运行开始 */
        case M_CMD_NOTIFY_TO_SERVER_MANUAL_RUNNING_START: /* 48 */
            break;
            /** 手动运行开始 */
        case M_CMD_NOTIFY_TO_SERVER_MANUAL_RUNNING_STOP: /* 49 */
            break;
            /** 设备恢复出厂 */
        case M_CMD_TWOWAY_FACTORY_RESET: /* 50 */

            m_ext_drive_button_event_handle(M_KEY_EVENT_STOP, M_KEY_EVENT_SIGN_CLICK, 0, M_CONST_SACK_NUM, in_time_s);
            m_callable_drive_flash_factory_reset();

            m_callable_local_resp_to_remote(M_CMD_TWOWAY_FACTORY_RESET, M_TYPE_Str_OK,
                                            NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, true);
            // mDelay_ms(1000);
            DEBUG_TEST(DB_W, "Reboot for Re Factory !");
            // mReboot();
            break;
            /** 手动运行切换 */
        case M_CMD_NOTIFY_TO_SERVER_MANUAL_RUNNING_SWITCH: /* 51 */
            // M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%lld,%lld", &temp_use_u64_vue_b, &temp_use_u64_vue_c);
            // // m_test_dormant_set(temp_use_u64_vue_b, temp_use_u64_vue_c);
            // int test_a = 0;
            // m_rest_schedule_stroage_add(buf);
            break;
            /** 清除设备信息 */
        case M_CMD_TWOWAY_RESET_DEVICE: /* 52 */
            m_callable_device_attribute_all_clear();
            m_callable_local_resp_to_remote(M_CMD_TWOWAY_RESET_DEVICE, M_TYPE_Str_OK,
                                            NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, true);

            mReboot(M_RESTART_CAUSE_PREMEDITATED);
            break;
            //     /** 调节设备灯光亮度 */
            // case M_CMD_TWOWAY_DEVICE_LIGHT_CHANGE: /* 56 */
            //     m_callable_data_parse_format_vue("%d", (void *)&temp_use_u64_vue_a, buf);
            //     m_callable_display_status(M_DISPLAY_SET_LIGHT_CLOSE_OR_OPEN_MODE, temp_use_u64_vue_a);
            //     // m_callable_display_set_light_strength(temp_use_u64_vue_a);
            //     break;
            /** 调节设备灯光亮度 */
        case M_CMD_TWOWAY_DEVICE_LIGHT_CHANGE: /* 56 */
            m_callable_data_parse_format_vue("%d", (void *)&temp_use_u8_vue_a, buf);

            if (temp_use_u8_vue_a == 0)
            {
                m_callable_display_status(M_DISPLAY_SET_LIGHT_CLOSE, 0);
            }
            else
            {
                m_callable_display_status(M_DISPLAY_SET_LIGHT_OPEN, 0);

                if (temp_use_u8_vue_a == 6)
                {
                    m_callable_display_set_light_strength(30);
                    temp_use_u8_vue_a = 30;
                }
                else if (temp_use_u8_vue_a == 10)
                {
                    m_callable_display_set_light_strength(90);
                    temp_use_u8_vue_a = 90;
                }
                else if (temp_use_u8_vue_a == 19)
                {
                    m_callable_display_set_light_strength(245);
                    temp_use_u8_vue_a = 245;
                }
            }
            if (temp_use_u8_vue_a <= 245)
                m_callable_drive_flash_write(M_TYPE_U8, "LL", &temp_use_u8_vue_a);

            m_callable_local_resp_to_remote(M_CMD_TWOWAY_DEVICE_LIGHT_CHANGE, M_TYPE_U64, (void *)&temp_use_u8_vue_a,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, true);
            break;
            /** 远程控制设备按键 */
        case M_CMD_TWOWAY_CONTROL_DEVICE_KEY: /* 58 */

            break;
            /** 跳过一个时间区域的计划 */
        case M_CMD_TWOWAY_NOT_RUN_SCHEDULE_TIMESPC: /* 62 */
            /**
             * @brief water delay
             *
             * @name [ 62 ,  start , end , e_code ]
             * @param  start 开始时间
             * @param  end 结束时间
             */
            m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_a, buf);
            m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_b, buf);
            m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_c, buf);
            m_callable_water_delay_set(temp_use_u64_vue_a, temp_use_u64_vue_b, in_time_s);

            // m_callable_local_resp_to_remote(M_CMD_TWOWAY_NOT_RUN_SCHEDULE_TIMESPC, M_TYPE_U64, (void *)&temp_use_u64_vue_a,
            //                                 M_TYPE_U64, (void *)&temp_use_u64_vue_b,
            //                                 M_TYPE_U64, (void *)&temp_use_u64_vue_c,
            //                                 M_TYPE_NULL, NULL, in_time_s, true);
            break;
            /** 水锤 */
        case M_CMD_TWOWAY_WATER_HAMMER: /* 63 */
                                        /**
                                         * @brief water hammer
                                         *
                                         * @name [ 63 , Enable  ]
                                         * @param  Enable 开启/关闭 1/0
                                         */
            m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_a, buf);
            m_callable_device_attribute_set_hammer(temp_use_int_vue_a);
            m_callable_local_resp_to_remote(M_CMD_TWOWAY_WATER_HAMMER, M_TYPE_Int, (void *)&temp_use_int_vue_a,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, true);

            break;
            /** 跳过一个计划 */
        case M_CMD_TWOWAY_NOT_RUN_A_SCHEDULE_TIMESPC: /* 70 */
            /**
             * @brief water delay
             *
             * @name [ 70 , id , start , end  ]
             * @param  start 开始时间
             * @param  end 结束时间
             */
            m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_a, buf);
            m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_a, buf);
            m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_b, buf);
            m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_b, buf);
            m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_c, buf);
            m_callable_schedule_skip_set(temp_use_u64_vue_a, temp_use_u64_vue_b, temp_use_u64_vue_c, m_callable_timer_manage_get_device_time());

            // m_callable_local_resp_to_remote(M_CMD_TWOWAY_NOT_RUN_A_SCHEDULE_TIMESPC,
            //                                 M_TYPE_U64, (void *)&temp_use_u64_vue_a,
            //                                 M_TYPE_Int, (void *)&temp_use_int_vue_b,
            //                                 M_TYPE_NULL, NULL,
            //                                 M_TYPE_NULL, NULL,
            //                                 in_time_s, true);
            break;
            //     /** 计划跳过通知 */
            // case M_CMD_NOTIFY_TO_SERVER_A_SCHEDULE_BE_SKIP: /* 74 */

            //     break;
            /** 取消跳过一个计划 */

        case M_CMD_NOTIFY_TO_SET_FLOW_RATE: /* 72 */
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%hhd", &temp_use_u8_vue_a);
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%*[^,],%f", &temp_use_float_vue_a);
            m_callable_data_parse_format_vue("%d", (void *)&temp_use_u8_vue_a, buf);
            m_callable_data_parse_format_vue("%f", (void *)&temp_use_float_vue_a, buf);
            DEBUG_TEST(DB_I, "设置 区域流速====》 区域id : %d     流速：%.03f", temp_use_u8_vue_a, temp_use_float_vue_a);
            if (m_callable_channel_flow_speed_set(temp_use_u8_vue_a, temp_use_float_vue_a) == succ_r)
            {
                m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SET_FLOW_RATE,
                                                M_TYPE_U8, (void *)&temp_use_u8_vue_a,
                                                M_TYPE_Float, (void *)&temp_use_float_vue_a,
                                                M_TYPE_NULL, NULL,
                                                M_TYPE_NULL, NULL, in_time_s, true);
            }

            break;
        case M_CMD_TWOWAY_UNDO_NOT_RUN_A_SCHEDULE_TIMESPC: /* 75 */
            m_callable_data_parse_format_vue("%lld", (void *)&temp_use_u64_vue_a, buf);

            m_callable_local_resp_to_remote(M_CMD_TWOWAY_UNDO_NOT_RUN_A_SCHEDULE_TIMESPC,
                                            M_TYPE_U64, (void *)&temp_use_u64_vue_a,
                                            M_TYPE_Str_OK, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, true);
            m_callable_schedule_skip_clean(temp_use_u64_vue_a);
            break;
            /** 取消跳过一段时间 */
        case M_CMD_TWOWAY_UNDO_NOT_RUN_SCHEDULE_TIMESPC: /* 76 */
            m_callable_water_delay_clean();
            m_callable_local_resp_to_remote(M_CMD_TWOWAY_UNDO_NOT_RUN_SCHEDULE_TIMESPC, M_TYPE_Str_OK,
                                            NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, true);

            break;
            /** 地址设置 */
        case M_CMD_NOTIFY_TO_SERVER_REQUEST_LOCAL_INFO: /* 82 */
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%lf", &temp_use_double_vue_a);
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%*[^,],%lf", &temp_use_double_vue_b);
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%*[^,],%*[^,],%d", &temp_use_int_vue_a);
            m_callable_device_attribute_set_local(temp_use_double_vue_a, temp_use_double_vue_b, temp_use_int_vue_a);
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%*[^,],%*[^,],%*[^,],%lld", &temp_use_u64_vue_a);

            m_callable_timer_manage_time_set(temp_use_u64_vue_a, temp_use_int_vue_a, 0);
            m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_REQUEST_LOCAL_INFO + 1, M_TYPE_Str_OK,
                                            NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, true);

            break;
            /** 获取wifi强度  */
        case M_CMD_TWOWAY_RESP_WIFI_SIGN_STRAGE: /** 86 */
            sign_strage = 0;
            m_ext_soft_drive_net_wifi_get_sign_strage(&sign_strage); // 信号强度
            m_callable_local_resp_to_remote(M_CMD_TWOWAY_RESP_WIFI_SIGN_STRAGE, M_TYPE_Int,
                                            (void *)&sign_strage,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, false);
            break;
            /** 配置AB设置  */
            // [20,5566,3,C2,1704844800,4070995199,100 12:30,9#900;10#900;11#900;12#900;13#900;14#900;15#900;16#900,104,7;1;2;3;4;5;6,1]
            /**
             * @brief [87,1,null,1.0#0.4#0.29#6.234,0]
             *
             * 区域浸泡设置:
                [87,2,60&360,1.50#0.10#0.20#1.003,1]
                [87,区域ID,浇水时间(秒)&浸泡时间(秒),AR#IR#ASA#区域默认流速（GAl/分钟）,状态]
                状态： 0:关闭 1： 使用配置A（手动设置） 2：使用智能计划
                返回[87,区域ID,时间搓]
                清除智能浸泡: ##
             */
        case M_CMD_NOTIFY_TO_SERVER_ZONE_CONFIG_ABC: /** 87 */

            m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_a, buf);

            m_callable_channel_manage_congfig_sack_param(temp_use_int_vue_a, buf);
            m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_ZONE_CONFIG_ABC, M_TYPE_Int,
                                            (void *)&temp_use_int_vue_a,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, true);
            break;
            /** 离线月份的浇水量调整  */
        case M_CMD_NOTIFY_TO_SERVER_OFFLINE_ADJUST: /** 88 */
            // [88,23,0B0]
            m_callable_data_parse_foramt_str(tmp_sid, buf);
            // m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_a, buf);
            m_callable_channel_manage_set_offline_running_percent(tmp_sid);
            m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_OFFLINE_ADJUST, M_TYPE_Str_OK,
                                            NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, true);

            break;
            /**
             * @brief 冬令时、夏令时 调整   [89 , 夏令时开始时间戳 、 夏令时时区 ， 冬令时开始时间戳 ， 冬令时]
             *
             */
        case M_CMD_NOTIFY_TO_SERVER_TIME_CHANGE: /** 89 */
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%lld,%d,%lld,%d", &temp_use_u64_vue_a, &temp_use_int_vue_a, &temp_use_u64_vue_b, &temp_use_int_vue_b);
            m_callable_timer_manage_timezone_switch_set(temp_use_u64_vue_a, temp_use_int_vue_a, temp_use_u64_vue_b, temp_use_int_vue_b);
            m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_TIME_CHANGE, M_TYPE_Str_OK,
                                            NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, true);

            break;

            /** 服务器请求检测电流  */
        case M_CMD_NOTIFY_TO_DEVICE_CHECK_CURRENT: /** 91 */
            temp_use_int_vue_c = 0;
            temp_use_u64_vue_a = 0;
            /** Zone */
            m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_b, buf);
            m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_c, buf);
            // m_callable_current_calibration_channel_select(temp_use_int_vue_b);

            if (device_status == M_DEVICE_GLOBAL_STATUS_IDLE)
            {
                m_callable_current_set_checkout_zone(temp_use_int_vue_b);

                // m_callable_current_monitor_start();
                if (temp_use_int_vue_c == 0)
                {
                    // m_callable_current_set_short_mode(M_CURRENT_SHORT_MODE_CALIBRATION);
                    m_callable_solenoid_manage_open(temp_use_int_vue_b);
                }
                else if (temp_use_int_vue_c == 1)
                {
                    m_callable_solenoid_manage_M_open();
                    m_callable_display_status(M_DISPLAY_ZONE_RUNNING_MODE, 1);
                    m_callable_current_set_short_mode(M_CURRENT_SHORT_MODE_PUMP);
                }
                else if (temp_use_int_vue_c == 2)
                {
                    // m_ext_drive_solenoid_open_and_close(temp_use_int_vue_b, 1);
                    m_callable_current_set_short_mode(M_CURRENT_SHORT_MODE_SIGN_ZONE);
                    m_callable_display_status(M_DISPLAY_ZONE_RUNNING_MODE, 1);
                }
                mDelay_ms(5000);
                temp_use_int_vue_a = m_callable_current_calibration();
                mDelay_ms(1000);
                if (temp_use_int_vue_a == MAX_SHORT_CURRENT)
                    temp_use_u64_vue_a = 1;
                m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_DEVICE_CHECK_CURRENT,
                                                M_TYPE_Int, (void *)&temp_use_int_vue_b,
                                                M_TYPE_Int, (void *)&temp_use_int_vue_a,
                                                M_TYPE_U64, (void *)&temp_use_u64_vue_a,
                                                M_TYPE_NULL, NULL, in_time_s, true);
                m_callable_solenoid_manage_close(temp_use_int_vue_b);
                m_callable_solenoid_manage_M_close();
                m_callable_display_status(M_DISPLAY_IDLE_STATUS_MODE, 0);
            }
            else
                m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_DEVICE_CHECK_CURRENT,
                                                M_TYPE_Int, (void *)&temp_use_int_vue_b,
                                                M_TYPE_Int, (void *)&temp_use_int_vue_a,
                                                M_TYPE_U64, (void *)&temp_use_u64_vue_a,
                                                M_TYPE_NULL, NULL, in_time_s, true);

            break;
            /** 返回内存使用情况 */
        case M_CMD_NOTIFY_TO_DEVICE_OFFLINE_MODE_CONTROL: /** 92 */

            m_callable_system_printf_to_remote_heap_free_info(NULL, &temp_use_int_vue_b);
            m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_DEVICE_OFFLINE_MODE_CONTROL,
                                            M_TYPE_Int, (void *)&temp_use_int_vue_b,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            in_time_s, false);
            // m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_a, buf);
            // m_callable_data_parse_foramt_str(tmp_pudate_url, buf);
            // if (temp_use_int_vue_a)
            // {
            //     temp_use_int_vue_a = M_DEVICE_RUNNING_MODE_OFFLINE_DIRECT;
            //     DEBUG_TEST(DB_W,"离线模式开启 ...");
            //     m_callable_device_attribute_running_set_mode(M_DEVICE_RUNNING_MODE_OFFLINE_CONFIG);
            //     m_callable_network_server_disconnect();
            //     m_callable_network_disconnect();
            //     m_callable_drive_flash_write(M_TYPE_U32, "device_mode", (int *)&temp_use_int_vue_a);

            //     // mReboot();
            // }
            // else
            // {
            //     m_callable_device_attribute_running_set_mode(M_DEVICE_RUNNING_MODE_DERECT);
            //     DEBUG_TEST(DB_W,"离线模式关闭 ...");
            //     m_callable_drive_flash_remove("device_mode");
            // }

            break;

        /** 设置 传感器损坏检测时间  */
        case M_CMD_NOTIFY_TO_DEVICE_SET_TIME_SENSOR_BAD_CHECK: /* 94 */
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%d", &temp_use_int_vue_a);
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%*[^,],%d", &temp_use_int_vue_b);
            m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_a, buf);
            m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_b, buf);
            m_callable_device_sensor_damage_detection_time_set(temp_use_int_vue_a, temp_use_int_vue_b);

            m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_DEVICE_SET_TIME_SENSOR_BAD_CHECK,
                                            M_TYPE_Int, (void *)&temp_use_int_vue_a,
                                            M_TYPE_Int, (void *)&temp_use_int_vue_b,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, true);

            break;
        /** 设置 流量漏水检测时间  */
        case M_CMD_NOTIFY_TO_DEVICE_SET_TIME_FLOW_LEAKING_CHECK: /* 95 */
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%d", &temp_use_int_vue_a);
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%*[^,],%f", &temp_use_float_vue_a);
            m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_a, buf);
            m_callable_data_parse_format_vue("%f", (void *)&temp_use_float_vue_a, buf);

            m_callable_device_flow_leakage_detection_time_set(temp_use_int_vue_a, temp_use_float_vue_a);

            m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_DEVICE_SET_TIME_FLOW_LEAKING_CHECK,
                                            M_TYPE_Int, (void *)&temp_use_int_vue_a,
                                            M_TYPE_Float, (void *)&temp_use_float_vue_a,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, true);

            break;

            /** 设置 传感器损坏或者漏水检测 是否需要打开 或者 关闭  */
            // case M_CMD_NOTIFY_TO_SERVICE_SENSOR_MONITOR_OPEN_OR_CLOSE: /* 97 */
            //     m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_b, buf);
            //     m_callable_early_warning_monitor_open_or_close(temp_use_int_vue_b);

            //     m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVICE_SENSOR_MONITOR_OPEN_OR_CLOSE,
            //                                     M_TYPE_Int, (void *)&temp_use_int_vue_a,
            //                                     M_TYPE_Int, (void *)&temp_use_int_vue_b,
            //                                     M_TYPE_NULL, NULL,
            //                                     M_TYPE_NULL, NULL, in_time_s, true);
            //     break;
            /**
             * @brief [98, null/192.168.2.136, null/255.255.255.1,  null/192.168.2.1 , Dns , 0/1/2]
             *  0 删除  1 设置   2 获取
             */
        case M_CMD_NOTIFY_TO_SERVICE_STATIC_IP_OPTION: /* 98 */
            m_callable_data_parse_foramt_str(tmp_sid, buf);
            m_callable_data_parse_foramt_str(tmp_use_str_a, buf);
            m_callable_data_parse_foramt_str(tmp_use_str_b, buf);
            m_callable_data_parse_foramt_str(tmp_use_str_c, buf);
            m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_b, buf);

            if (temp_use_int_vue_b == 1)
            {
                m_callable_network_disconnect();
                m_callable_middle_network_net_connection_option_set_info(M_NET_CONNECT_OPTION_MANUAL_STATIC_IP);
                m_callable_middle_network_adapter_set_static_ip(tmp_sid, tmp_use_str_a, tmp_use_str_b, tmp_use_str_c);
                m_callable_network_connect();
            }
            else if (temp_use_int_vue_b == 2)
            {
                m_callable_middle_network_adapter_get_static_ip(tmp_sid, tmp_use_str_a, tmp_use_str_b, tmp_use_str_c);
            }
            else if (temp_use_int_vue_b == 0)
            {
                m_callable_network_disconnect();
                m_callable_middle_network_adapter_clear();
                m_callable_wifi_auto_connect();
                strcpy(tmp_sid, "ok");
                strcpy(tmp_use_str_a, "ok");
                strcpy(tmp_use_str_b, "ok");
                strcpy(tmp_use_str_c, "ok");
            }
            m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVICE_STATIC_IP_OPTION,
                                            M_TYPE_Str, (void *)tmp_sid,
                                            M_TYPE_Str, (void *)tmp_use_str_a,
                                            M_TYPE_Str, (void *)tmp_use_str_b,
                                            M_TYPE_Str, (void *)tmp_use_str_c,
                                            in_time_s, true);
            break;
        case M_CMD_NOTIFY_TO_SERVICE_GET_SMART_ZONE_INFO: /* 99 */
            // m_callable_drive_io_pin12_uninit();
            memset(out_temp_char_b, 0, strlen(out_temp_char_b));
            memset(tmp_sid, 0, sizeof(*tmp_sid));
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%[^,],%s", tmp_sid, out_temp_char_b);

            m_callable_channel_smart_config_a_b_get(tmp_sid, out_temp_char_b, tmp_pudate_url);
            m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVICE_GET_SMART_ZONE_INFO,
                                            M_TYPE_Str, (void *)tmp_pudate_url,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            in_time_s, true);
            break;

        case M_CMD_LOGIN_AUTHENTICATION: /* 101 */

            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%16s", tmp_use_str_a);
            m_callable_data_parse_foramt_str(tmp_use_str_a, buf);

            if (m_callable_offline_bluetooth_login_authentication(tmp_use_str_a) == succ_r)
            {
                temp_use_int_vue_a = 1;
                DEBUG_TEST(DB_I, "离线蓝牙模式登录认证成功");
            }
            else
            {
                temp_use_int_vue_a = 0;
                DEBUG_TEST(DB_I, "离线蓝牙模式登录认证失败");
            }
            m_callable_local_resp_to_remote(M_CMD_LOGIN_AUTHENTICATION,
                                            M_TYPE_Int, (void *)&temp_use_int_vue_a,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, false);
            break;

        case M_CMD_CONFIRM_BINDING: /*102*/

            // m_callable_data_parse_format_vue("%d", (void *)&temp_use_int_vue_b, buf);
            if (m_callable_offline_bluetooth_login_flag() == succ_r)
            {
                // stat = m_callable_midlle_rtc_get_time(&temp_use_u64_vue_a);
                // DEBUG_TEST("%d   %lld", stat, temp_use_u64_vue_a);
                // if (stat == succ_r && temp_use_u64_vue_a != 0)
                // {
                //     DEBUG_TEST("RTC当前已存在时间");
                //     DEBUG_TEST_W("-- > %lld", temp_use_u64_vue_a);
                // }
                // else
                // {
                DEBUG_TEST(DB_W, "-- > %lld", in_time_s);
                M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%lld,%d,%d", &temp_use_u64_vue_a, &temp_use_int_vue_a, &temp_use_int_vue_c);

                // temp_use_u64_vue_a = temp_use_u64_vue_a / 1000;

                in_time_s = temp_use_u64_vue_a + temp_use_int_vue_a * 3600;
                DEBUG_TEST(DB_W, "---> %lld", in_time_s);
                m_callable_device_proper_status_get(&temp_use_int_vue_b);
                if ((temp_use_int_vue_b == M_DEVICE_GLOBAL_STATUS_MANUAL_RUNNING || temp_use_int_vue_b == M_DEVICE_GLOBAL_STATUS_SCHEDULE_RUNNING || temp_use_int_vue_b == M_DEVICE_GLOBAL_STATUS_FAST_RUN_RUNNING) && temp_use_u64_vue_a + 5 < m_callable_timer_manage_get_utc_time())
                {
                    DEBUG_TEST(DB_W, "time update Den : %lld And  %lld", temp_use_u64_vue_a, m_callable_timer_manage_get_utc_time());
                }
                else
                    m_callable_timer_manage_time_set(temp_use_u64_vue_a, temp_use_int_vue_a, temp_use_int_vue_c);
                // }

                DEBUG_TEST(DB_I, "登录成功");
                temp_use_int_vue_a = 1;
                m_callable_offline_bluetooth_bind_set();
                if (login_recover_schedule)
                {
                    m_callable_schedule_stroage_recover_item_all();
                    login_recover_schedule = false;
                }
                login_recover_schedule = false;

                m_callable_network_set_server_login_status(true);
            }
            else
            {
                DEBUG_TEST(DB_I, "未认证,登录成功");
                temp_use_int_vue_a = 0;
            }
            m_callable_local_resp_to_remote(M_CMD_CONFIRM_BINDING,
                                            M_TYPE_Int, (void *)&temp_use_int_vue_a,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL,
                                            M_TYPE_NULL, NULL, in_time_s, false);

            break;

        case M_CMD_REMOVE_BINDING: /*103*/

            temp_use_int_vue_a = 0;
            m_callable_data_parse_foramt_str(tmp_use_str_c, buf);
            m_callable_data_parse_foramt_str(tmp_use_str_b, buf);
            // if (m_callable_offline_bluetooth_login_flag() == succ_r)
            // {
            temp_use_int_vue_a = 1;
            m_callable_config_net_set_username(tmp_use_str_c);
            m_callable_config_net_set_token(tmp_use_str_b);

            m_callable_device_attribute_running_mode_set_wifi_info(tmp_use_str_c, tmp_use_str_b);
            m_callable_network_set_net_info(tmp_use_str_c, tmp_use_str_b);

            // m_callable_offline_bluetooth_running_mode_setting(M_DEVICE_RUNNING_MODE_DERECT);
            m_callable_device_attribute_running_save_mode(M_DEVICE_RUNNING_MODE_DERECT);

            // m_callable_offline_bluetooth_unbind();
            // m_callable_local_resp_to_remote(M_CMD_REMOVE_BINDING,
            //                                 M_TYPE_Int, (void *)&temp_use_int_vue_a,
            //                                 M_TYPE_NULL, NULL,
            //                                 M_TYPE_NULL, NULL,
            //                                 M_TYPE_NULL, NULL, in_time_s, false);

            /*重启设备*/
            mReboot(M_RESTART_CAUSE_PREMEDITATED);
            // }

            break;

        case M_CMD_SN_SET: /* 104 */
            m_callable_data_parse_foramt_str(tmp_sid, buf);
            if (strlen(tmp_sid) < 15)
            {
                m_callable_device_attribute_set_sn(tmp_sid);
                m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_DEBUG_INFO,
                                                M_TYPE_Str, (void *)tmp_sid,
                                                M_TYPE_Str_OK, NULL,
                                                M_TYPE_NULL, NULL,
                                                M_TYPE_NULL, NULL, in_time_s, false);
            }
            else
                m_callable_local_resp_to_remote(M_CMD_NOTIFY_TO_SERVER_DEBUG_INFO,
                                                M_TYPE_Str, (void *)tmp_sid,
                                                M_TYPE_Str_FAIL, NULL,
                                                M_TYPE_NULL, NULL,
                                                M_TYPE_NULL, NULL, in_time_s, false);
            break;
            /* 107   批量区域电流校准 */
        case M_CMD_BATCH_ZONE_CURRENT_CALIBRATION_FOUND:
            /**/
            memset(temp_queue_cc, 0, sizeof(temp_queue_cc));
            m_callable_concatenatestring(buf, temp_queue_cc);
            DEBUG_TEST(DB_I, "buf_in %s", buf);
            DEBUG_TEST(DB_I, "buf_out %s", temp_queue_cc);
            m_callable_current_batch_area_power_calibration_flag_set(1);
            m_callable_instance_manage_quick_gener(temp_queue_cc, in_time_s);
            /*还需要做特殊处理，用过电流校准状态去限制、过滤按键切换控制以及 指令切换控制、只做 开始和 停止动作、每自动切换一个区域进行电流值返回*/

            break;

            /* 110   批量区域电流测试 */
        case M_CMD_BATCH_ZONE_CURRENT_TEST_FOUND:
            /**/
            memset(temp_queue_cc, 0, sizeof(temp_queue_cc));
            m_callable_concatenatestring(buf, temp_queue_cc);
            DEBUG_TEST(DB_I, "buf_in %s", buf);
            DEBUG_TEST(DB_I, "buf_out %s", temp_queue_cc);
            m_callable_current_batch_area_power_calibration_flag_set(2);
            m_callable_instance_manage_quick_gener(temp_queue_cc, in_time_s);
            /*还需要做特殊处理，用过电流校准状态去限制、过滤按键切换控制以及 指令切换控制、只做 开始和 停止动作、每自动切换一个区域进行电流值返回*/

            break;

        // case M_CMD_OTA_SILENT_UPDATE: /*113*/
        //     m_callable_data_parse_foramt_str(tmp_pudate_url, buf);

        //     if (m_callable_ota_silent_updata_flag_get() == false)
        //     {

        //         stat = m_callable_drive_flash_write(M_TYPE_Str, OTA_URL, tmp_pudate_url);
        //         if (stat == succ_r)
        //         {
        //             m_callable_ota_silent_updata_start_init(in_time_s);
        //         }
        //     }
        //     else
        //     {
        //         DEBUG_E("OTA静默更新已存在,无需重复发送113指令");
        //     }

        //     break;
        case 199: /* 199 */
            m_callable_network_server_disconnect();

            break;
        case 200: /* 201 */
            m_callable_middle_err_handle_printf();
            break;
        case 202: /* 202 */
            M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(buf, "%d", &temp_use_int_vue_a);
            if (temp_use_int_vue_a)
                m_callable_schedule_storage_buffer_print_to_remote();
            else
                m_static_instance_list_query_print_all();
            break;

        default:
            break;
        }
    }
    return stat;
}
/**
 * @brief 获取是否已经恢复计划
 *
 * @return stat_m
 */
stat_m m_ext_network_get_device_is_recover_schedule()
{
    return !login_recover_schedule ? succ_r : fail_r;
}

/**
 * @brief data send
 *
 * @param in_tx data
 * @param in_len length
 * @return stat_m
 */
// stat_m m_ext_net_link_connection_tx(const char *in_tx, int in_len)
// {
//     return succ_r;
// }
