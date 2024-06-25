

#include "bluetool_ft.h"
// uint16_t connid_m = 0;

static uint8_t char1_str[] = {0x11, 0x22, 0x33};

const char GATTS_IMOLAZA_TAG[] = "CC";
const char GATTS_TAG[] = "CC0";

uint8_t p_manufacturer_data[14] = {0};
extern struct receive_data re_data;

static bool is_connected = false;
#if !UNIT_TEST

static void gatts_profile_a_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

static void gatts_profile_b_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
}

static esp_attr_value_t gatts_demo_char1_val =
    {
        .attr_max_len = GATTS_DEMO_CHAR_VAL_LEN_MAX,
        .attr_len = sizeof(char1_str),
        .attr_value = char1_str,
};

static struct gatts_profile_inst gl_profile_tab[PROFILE_NUM] = {
    [PROFILE_A_APP_ID] = {
        .gatts_cb = gatts_profile_a_event_handler,
        .gatts_if = ESP_GATT_IF_NONE, /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
    },
    [PROFILE_B_APP_ID] = {
        .gatts_cb = gatts_profile_b_event_handler, /* This demo does not implement, similar as profile A */
        .gatts_if = ESP_GATT_IF_NONE,              /* Not get the gatt_if, so initial is ESP_GATT_IF_NONE */
    },
};
static uint8_t service_uuid[32] = {
    0xfb,
    0x34,
    0x9b,
    0x5f,
    0x80,
    0x00,
    0x00,
    0x80,
    0x00,
    0x5f,
    0x80,
    0x10,
    0x00,
    0x00,
    0xEE,
    0x00,
    0x00,
    0x00,
    0xfb,
    0x34,
    0x9b,
    0x5f,
    0x80,
    0x00,
    0x00,
    0x80,
    0x00,
    0x10,
    0x00,
    0x00,
    0xFF,
    0x00,
    0x00,
    0x00,
};
static esp_ble_adv_data_t heart_rate_adv_config =
    {
        .set_scan_rsp = false,
        .include_name = true,
        .include_txpower = true,
        .min_interval = 0x0020, // slave connection min interval, Time = min_interval * 1.25 msec
        .max_interval = 0x0040, // slave connection max interval, Time = max_interval * 1.25 msec
        .appearance = 0,
        .manufacturer_len = 15,      // TEST_MANUFACTURER_DATA_LEN,
        .p_manufacturer_data = NULL, // test_manufacturer,
        .service_data_len = 0,
        .p_service_data = NULL,
        .service_uuid_len = sizeof(service_uuid),
        .p_service_uuid = service_uuid,
        .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};
// typedef struct
// {
//     uint8_t *prepare_buf;
//     int prepare_len;
// } prepare_type_env_t;

static prepare_type_env_t a_prepare_write_env;
// static prepare_type_env_t b_prepare_write_env;

static esp_ble_adv_params_t heart_rate_adv_params = {
    .adv_int_min = 0x20,
    .adv_int_max = 0x40,
    .adv_type = ADV_TYPE_IND,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .channel_map = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

char ble_name[40] = {0};
char mac_suffix[20] = {0};
esp_gatt_if_t *mgatts_if;
char m_resp[500] = "init..";

uint16_t conn_id = 0;
esp_ble_gatts_cb_param_t *mparam;
void m_ads_tool(uint8_t *dec_array, char *array, int src_fc, int fc)
{
    // DEBUG_TEST(DB_I,"---%c", array[fc+1]);
    uint8_t temp = 0;

    dec_array[src_fc] = 0;

    for (size_t i = 0; i < 2; i++)
    {
        if (array[fc + i] == 0)
            array[fc + i] = 49;
        // if (array[fc + 1] < '0' || array[fc + 1] > '9')
        if (array[fc + i] >= 'a')
            dec_array[src_fc] |= (array[fc + i] - 87) << 4 * (1 - i);
        else if (array[fc + i] >= 'A')
            dec_array[src_fc] |= (array[fc + i] - 55) << 4 * (1 - i);
        else
            dec_array[src_fc] |= (array[fc + i] - '0') << 4 * (1 - i);

        // dec_array[src_fc] &= 0xf0;
        // dec_array[src_fc] |= ((array[fc + 1] - 48));
        // DEBUG_TEST_HIDE("%02x", (array[fc + i] - 55) << 4 * (1 - i));
    }
}

static void config_adv(uint8_t *p_manufacturer_data)
{
    int i = 0;
    memset(mac_suffix, 0, sizeof(mac_suffix));

    m_callable_device_attribute_get_sn(mac_suffix);
    for (i = 0; i < 6; i++)
    {
        // if (i < 1)
        //     m_ads_tool(p_manufacturer_data, GATTS_IMOLAZA_TAG, i, i);
        // else
        // if (i < 6)
        m_ads_tool(p_manufacturer_data, mac_suffix, i, i * 2);
    }
    memset(mac_suffix, 0, sizeof(mac_suffix));
    m_callable_device_attribute_get_mac_addr(mac_suffix);
    for (; i < 12; i++)
    {

        // DEBUG_TEST(DB_I,"---%c", mac_suffix[(i - 6) * 2]);
        // DEBUG_TEST(DB_I,"---%c", mac_suffix[(i - 6) * 2 + 1]);
        m_ads_tool(p_manufacturer_data, mac_suffix, i, (i - 6) * 2);
        // p_manufacturer_data[i] = 1 << mac_suffix[(i - 6) * 2];
        // p_manufacturer_data[i + 1] |= (1 << mac_suffix[(i - 6) * 2 + 1]);
        // p_manufacturer_data[i] = mac_suffix[i - 7];
        // DEBUG_TEST_HIDE("%02x", p_manufacturer_data[i]);
        // DEBUG_TEST_HIDE("%02x", p_manufacturer_data[i + 1]);
    }
}
// uint8_t aaad[40] = {0x02,0x01,0x06,
// 0x0d,0x09,0x6d,0x6d,0x6f,0x6c,0x61,0x7a,0x61,0x5f,0x62,0x66,0x63,
// 0x0d,0xff,0xcc,0x28,0x22,0x09,0x23,0xa0,0x01,0x78,0x21,0x84,0x51,0xa8,0x02};
/**
 * @brief  GATT 层的事件处理   包含连接 写入 读取
 *
 * @param event 事件类型
 * @param gatts_if 事件结构体
 * @param param 参数
 */
static void gatts_profile_a_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    int nmod = 0;
    uint16_t length = 0;
    const uint8_t *prf_char;
    esp_gatt_rsp_t rsp;
    int out_mode = 0;
    stat_m stc = fail_r;
    mgatts_if = &gatts_if;
    mparam = param;
    conn_id = param->read.conn_id;
    uint8_t datc[15] = {0};
    int dev_run_mode = 0;
    m_callable_device_attribute_get_running_mode(&out_mode);
    switch (event)
    {
    case ESP_GATTS_REG_EVT:
    {
        memset(ble_name, 0, sizeof(ble_name));
        m_callable_device_attribute_get_running_mode(&dev_run_mode);
        if (dev_run_mode == M_DEVICE_RUNNING_MODE_OFFLINE_DIRECT || dev_run_mode == M_DEVICE_RUNNING_MODE_OFFLINE_CONFIG)
        {
            strcat(ble_name, "Offline_Imolaza_");
        }
        else
        {
            strcat(ble_name, "Imolaza_");
        }

        m_callable_device_attribute_get_mac_suffix_addr(mac_suffix);
        strcat(ble_name, mac_suffix);
    }
        gl_profile_tab[PROFILE_A_APP_ID].service_id.is_primary = true;
        gl_profile_tab[PROFILE_A_APP_ID].service_id.id.inst_id = 0x00;
        gl_profile_tab[PROFILE_A_APP_ID].service_id.id.uuid.len = ESP_UUID_LEN_16;
        gl_profile_tab[PROFILE_A_APP_ID].service_id.id.uuid.uuid.uuid16 = GATTS_SERVICE_UUID_TEST_A;
        esp_ble_gap_set_device_name(ble_name);

        config_adv(datc);
        // heart_rate_adv_config.p_service_data = datc;
        heart_rate_adv_config.p_manufacturer_data = datc;
        heart_rate_adv_config.manufacturer_len = sizeof(ble_name);
        heart_rate_adv_config.include_txpower = true;
        // DEBUG_TEST_HIDE("%s", heart_rate_adv_config.p_manufacturer_data);

        // esp_ble_gap_config_adv_data_raw((uint8_t *)aaad, sizeof(aaad));
        esp_ble_gap_config_adv_data(&heart_rate_adv_config);
        // esp_ble_gap_config_adv_data(&scan_rsp_data);
        esp_ble_gatts_create_service(gatts_if, &gl_profile_tab[PROFILE_A_APP_ID].service_id, GATTS_NUM_HANDLE_TEST_A);
        break;
    case ESP_GATTS_ADD_CHAR_EVT:

        DEBUG_TEST(DB_I, "ADD_CHAR_EVT, status %d,  attr_handle %d, service_handle %d",
                   param->add_char.status, param->add_char.attr_handle, param->add_char.service_handle);
        gl_profile_tab[PROFILE_A_APP_ID].char_handle = param->add_char.attr_handle;
        gl_profile_tab[PROFILE_A_APP_ID].descr_uuid.len = ESP_UUID_LEN_16;
        gl_profile_tab[PROFILE_A_APP_ID].descr_uuid.uuid.uuid16 = ESP_GATT_UUID_CHAR_CLIENT_CONFIG;
        esp_err_t get_attr_ret = esp_ble_gatts_get_attr_value(param->add_char.attr_handle, &length, &prf_char);
        if (get_attr_ret == ESP_FAIL)
        {
            ESP_LOGE(GATTS_TAG, "ILLEGAL HANDLE");
        }

        DEBUG_TEST(DB_I, "the gatts demo char length = %x", length);
        for (int i = 0; i < length; i++)
        {
            DEBUG_TEST(DB_I, "prf_char[%x] =%x", i, prf_char[i]);
        }
        esp_ble_gatts_add_char_descr(gl_profile_tab[PROFILE_A_APP_ID].service_handle, &gl_profile_tab[PROFILE_A_APP_ID].descr_uuid,
                                     ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE, NULL, NULL);

        break;
        // case ESP_GATTS_START_EVT:

    case ESP_GATTS_READ_EVT:

        DEBUG_TEST(DB_I, "GATT_READ_EVT, conn_id %d, trans_id %d, handle %d", param->read.conn_id, param->read.trans_id, param->read.handle);
        // esp_gatt_rsp_t rsp;
        gl_profile_tab[PROFILE_A_APP_ID].conn_id = param->read.conn_id;
        gl_profile_tab[PROFILE_A_APP_ID].conn_id = param->connect.conn_id;
        m_callable_middle_connect_handle_get_code(&dev_run_mode);
        memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
        rsp.attr_value.handle = param->read.handle;
        // rsp.attr_value.value = "caji";
        sprintf(m_resp , "%d" , dev_run_mode);
        strcpy((char *)rsp.attr_value.value, m_resp);
        rsp.attr_value.len = strlen(m_resp);
        esp_ble_gatts_send_response(gatts_if, param->read.conn_id, param->read.trans_id,
                                    ESP_GATT_OK, &rsp);

        break;
    case ESP_GATTS_WRITE_EVT:
        m_callable_device_attribute_get_running_mode(&nmod);
        DEBUG_TEST(DB_I, "blue_m_callable_device_attribute_get_running_mode:%d", nmod);
        if (nmod == M_DEVICE_RUNNING_MODE_OFFLINE_DIRECT)
        {
            m_callable_network_set_server_start_connect_server();
            if (strlen((char *)param->write.value) > 2)
            {
                if (!re_data.is_receive)
                {
                    memset(re_data.inout_data, 0, sizeof(re_data.inout_data));
                    strncpy(re_data.inout_data, (char *)param->write.value, strlen((char *)param->write.value));
                    re_data.inout_data_len = strlen((char *)param->write.value);
                    re_data.is_receive = true;
                    memset((char *)param->write.value, 0, strlen((char *)param->write.value));
                    //  stat_m_callable_network_manage_data_rx(event->data, event->data_len);
                    DEBUG_TEST(DB_I, "DATA=%d  %s", strlen((char *)param->write.value), re_data.inout_data);
                }
            }
        }
        else
        {
            // // 所有收到的数据 全部内部处理
            DEBUG_TEST(DB_I, "ESP_GATTS_WRITE_EVT, conn_id %d, trans_id %d, handle %d", param->read.conn_id, param->read.trans_id, param->read.handle);
            gl_profile_tab[PROFILE_A_APP_ID].conn_id = param->read.conn_id;
            gl_profile_tab[PROFILE_A_APP_ID].tan_id = param->read.trans_id;
            // connid_m = param->write.conn_id;
            m_callable_network_data_rx((char *)param->write.value, strlen((char *)param->write.value));
            DEBUG_TEST(DB_I, "DATA=%d  %s", strlen((char *)param->write.value), re_data.inout_data);
        }
        esp_ble_gatts_send_response(gatts_if, param->write.conn_id, param->write.trans_id, 0, NULL);
        // 所有收到的数据 全部内部处理
        // DEBUG_TEST(DB_I, "ESP_GATTS_WRITE_EVT, conn_id %d, trans_id %d, handle %d", param->read.conn_id, param->read.trans_id, param->read.handle);
        // gl_profile_tab[PROFILE_A_APP_ID].conn_id = param->read.conn_id;
        // gl_profile_tab[PROFILE_A_APP_ID].tan_id = param->read.trans_id;
        // // connid_m = param->write.conn_id;
        // m_callable_network_data_rx((char *)param->write.value, strlen((char *)param->write.value));

        // DEBUG_TEST(DB_I, "DATA=%d  %s", strlen((char *)param->write.value), re_data.inout_data);

        break;
    case ESP_GATTS_CREATE_EVT:
        DEBUG_TEST(DB_I, "CREATE_SERVICE_EVT, status %d,  service_handle %d", param->create.status, param->create.service_handle);
        gl_profile_tab[PROFILE_A_APP_ID].service_handle = param->create.service_handle;
        gl_profile_tab[PROFILE_A_APP_ID].char_uuid.len = ESP_UUID_LEN_16;
        gl_profile_tab[PROFILE_A_APP_ID].char_uuid.uuid.uuid16 = GATTS_CHAR_UUID_TEST_A;

        esp_ble_gatts_start_service(gl_profile_tab[PROFILE_A_APP_ID].service_handle);
        // a_property = ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_NOTIFY;
        esp_err_t add_char_ret = esp_ble_gatts_add_char(gl_profile_tab[PROFILE_A_APP_ID].service_handle, &gl_profile_tab[PROFILE_A_APP_ID].char_uuid,
                                                        ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE,
                                                        ESP_GATT_CHAR_PROP_BIT_READ | ESP_GATT_CHAR_PROP_BIT_WRITE | ESP_GATT_CHAR_PROP_BIT_NOTIFY,
                                                        &gatts_demo_char1_val, NULL);
        if (add_char_ret)
        {
            ESP_LOGE(GATTS_TAG, "add char failed, error code =%x", add_char_ret);
        }
        break; /*  */
    case ESP_GATTS_CONNECT_EVT:
    { // GATT 有人连接
        esp_ble_conn_update_params_t conn_params = {0};
        memcpy(conn_params.bda, param->connect.remote_bda, sizeof(esp_bd_addr_t));
        /* For the IOS system, please reference the apple official documents about the ble connection parameters restrictions. */
        conn_params.latency = 0;
        conn_params.max_int = 0x20; // max_int = 0x20*1.25ms = 40ms
        conn_params.min_int = 0x10; // min_int = 0x10*1.25ms = 20ms
        conn_params.timeout = 400;  // timeout = 400*10ms = 4000ms
        DEBUG_TEST(DB_I, "ESP_GATTS_CONNECT_EVT, conn_id %d, remote %02x:%02x:%02x:%02x:%02x:%02x:",
                   param->connect.conn_id,
                   param->connect.remote_bda[0], param->connect.remote_bda[1], param->connect.remote_bda[2],
                   param->connect.remote_bda[3], param->connect.remote_bda[4], param->connect.remote_bda[5]);
        gl_profile_tab[PROFILE_A_APP_ID].conn_id = param->connect.conn_id;
        gl_profile_tab[PROFILE_A_APP_ID].tan_id = param->write.trans_id;
        // start sent the update优化注释-按键优化注释-按键 connection parameters to the peer device.
        esp_ble_gap_update_conn_params(&conn_params);
        if (out_mode == M_DEVICE_RUNNING_MODE_OFFLINE_DIRECT || out_mode == M_DEVICE_RUNNING_MODE_OFFLINE_CONFIG)
        {
            m_callable_network_set_connect_status(true);
            // m_callable_networ优化注释-按键k_set_server_start_connect_server();
            m_callable_network_set_server_status(M_NETWORK_CONNECT_PROGRESS_START_LOGIN);
        }
    }
    break;
    case ESP_GATTS_DISCONNECT_EVT:
        if (out_mode == M_DEVICE_RUNNING_MODE_OFFLINE_DIRECT || out_mode == M_DEVICE_RUNNING_MODE_OFFLINE_CONFIG)
        {
            m_callable_network_set_connect_status(false);
            m_callable_network_set_server_status(M_NETWORK_CONNECT_PROGRESS_CONNECT_READY);
        }
        break;
    default:
        break;
    }

    // printf("gatts_profile_a_event_handler%d\n", event);
}
static void gatts_pro_event_handle(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param)
{
    // DEBUG_TEST(DB_I,"gatts : %d\n", event);
    if (event == ESP_GATTS_REG_EVT)
    {
        if (param->reg.status == ESP_GATT_OK)
        {
            gl_profile_tab[param->reg.app_id].gatts_if = gatts_if;
        }
    }
    do
    {
        int idx;
        for (idx = 0; idx < PROFILE_NUM; idx++)
        {
            if (gatts_if == ESP_GATT_IF_NONE || /* ESP_GATT_IF_NONE, not specify a certain gatt_if, need to call every profile cb function */
                gatts_if == gl_profile_tab[idx].gatts_if)
            {
                if (gl_profile_tab[idx].gatts_cb)
                {
                    gl_profile_tab[idx].gatts_cb(event, gatts_if, param);
                }
                
            }
        }
    } while (0);
    switch (event)
    {
    case ESP_GATTS_REG_EVT:
        break;

    case ESP_GATTS_WRITE_EVT:
        // esp_ble_gatts_send_indicate(gatts_if, param->write.conn_id, gl_profile_tab[PROFILE_A_APP_ID].char_handle,
        //                             sizeof(m_resp), (uint8_t *)m_resp, false);
        break;
    case ESP_GATTS_CONNECT_EVT:
        DEBUG_TEST(DB_I, "connect . !");
        is_connected = true;
        esp_ble_gatt_set_local_mtu(500);

        // esp_ble_gatts_send_indicate(gatts_if, param->write.conn_id, gl_profile_tab[PROFILE_A_APP_ID].char_handle,
        //                     8, (uint8_t *)"hello...", false);
        break;
    case ESP_GATTS_DISCONNECT_EVT:
        is_connected = false;
        DEBUG_TEST(DB_I, "dis connect !");
        esp_ble_gap_start_advertising(&heart_rate_adv_params);
        // int runningmode = 0;

        if (m_callable_offline_bluetooth_login_flag() == succ_r)
        {
            m_callable_network_reset_server_start_connect_server();
            m_callable_offline_bluetooth_unbind();
            m_callable_network_set_server_login_status(false);
        }
        break;

    default:
        break;
    }
}
static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    DEBUG_TEST(DB_I, "gap %d\r\n", event);
    switch (event)
    {
    // case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
    //     esp_ble_gap_start_advertising(&heart_rate_adv_params);
    //     break;
    case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
        adv_config_done &= (~adv_config_flag);
        if (adv_config_done == 0)
        {
            esp_ble_gap_start_advertising(&heart_rate_adv_params);
        }
        break;
    case ESP_GAP_BLE_SCAN_RSP_DATA_SET_COMPLETE_EVT:
        adv_config_done &= (~scan_rsp_config_flag);
        if (adv_config_done == 0)
        {
            esp_ble_gap_start_advertising(&heart_rate_adv_params);
        }
        break;
    case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
        // esp_ble_gap_start_advertising(&heart_rate_adv_params);
        if (param->adv_start_cmpl.status != ESP_BT_STATUS_SUCCESS)
        {
            DEBUG_TEST(DB_I, "fail");
        }
        else
            DEBUG_TEST(DB_I, "success");

        break;
    case ESP_GAP_BLE_SEC_REQ_EVT:
        /* send the positive (true) security response to the peer device to accept the security request.
        If not accept the security request, should send the security response with negative(false) accept value*/
        esp_ble_gap_security_rsp(param->ble_security.ble_req.bd_addr, true);
        break;
    case ESP_GAP_BLE_AUTH_CMPL_EVT:
    {
        esp_bd_addr_t bd_addr;
        memcpy(bd_addr, param->ble_security.auth_cmpl.bd_addr,
               sizeof(esp_bd_addr_t));
        DEBUG_TEST(DB_I, "remote BD_ADDR: %08x%04x",
                   (bd_addr[0] << 24) + (bd_addr[1] << 16) + (bd_addr[2] << 8) +
                       bd_addr[3],
                   (bd_addr[4] << 8) + bd_addr[5]);
        DEBUG_TEST(DB_I, "address type = %d",
                   param->ble_security.auth_cmpl.addr_type);
        DEBUG_TEST(DB_I, "pair status = %s",
                   param->ble_security.auth_cmpl.success ? "success" : "fail");
        break;
    }
    case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:
        DEBUG_TEST(DB_I, "update connection params status = %d, min_int = %d, max_int = %d,conn_int = %d,latency = %d, timeout = %d",
                   param->update_conn_params.status,
                   param->update_conn_params.min_int,
                   param->update_conn_params.max_int,
                   param->update_conn_params.conn_int,
                   param->update_conn_params.latency,
                   param->update_conn_params.timeout);
        break;
    default:
        break;
    }
}
#endif

/** 蓝牙初始化 */
stat_m m_ext_soft_drive_net_bluetool_init(void)
{
#if !UNIT_TEST
    esp_err_t ret;

    ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));

    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ret = esp_bt_controller_init(&bt_cfg);

    ret = esp_bt_controller_enable(ESP_BT_MODE_BLE);

    ret = esp_bluedroid_init();

    ret = esp_bluedroid_enable();

    esp_ble_gatts_register_callback(gatts_pro_event_handle);
    ret = esp_ble_gap_register_callback(gap_event_handler);

    ret = esp_ble_gatts_app_register(PROFILE_A_APP_ID);
    // 直接开启GATT服务
#endif

    return 0;
}
/** 连接到指定的蓝牙 有需要传入蓝牙的名字   后面可以传入NULL */
// 不是连接 而是直接 是服务器      手机来连接我

#define MAX_NOTIFICATION_SIZE 500 // 设置每次发送通知的最大长度为20

stat_m m_static_split_and_send_notification(char *value, uint16_t value_len)
{
    stat_m stat = succ_r;
    uint16_t i, split_size;
    for (i = 0; i < value_len; i += MAX_NOTIFICATION_SIZE)
    {
        split_size = (i + MAX_NOTIFICATION_SIZE) < value_len ? MAX_NOTIFICATION_SIZE : (value_len - i);

        uint8_t *data_ptr = (uint8_t *)&value[i]; // 将char类型数据转换为uint8_t指针

        esp_err_t ret = esp_ble_gatts_send_indicate(gl_profile_tab[PROFILE_A_APP_ID].gatts_if, gl_profile_tab[PROFILE_A_APP_ID].conn_id, gl_profile_tab[PROFILE_A_APP_ID].char_handle, split_size, data_ptr, false);
        if (ret != ESP_OK)
        {
            DEBUG_E("Failed to send notification. Error code: 0x%x", ret);
            stat = fail_r;
        }

        vTaskDelay(pdMS_TO_TICKS(2)); // 添加小延迟以确保发送顺序
    }
    return stat;
}

char rssp[200] = {"[11,f606e6ac177fdbabdac5e2801de01c11,8.3.036,5,-55]"};

stat_m m_ext_network_transmisson_bluetool_tx(char *data, int datalen)
{
    stat_m stat = fail_r;
#if !UNIT_TEST
    // esp_gatt_rsp_t rsp;
    // DEBUG_TEST_W("config resp: %s", data);
    // strcpy(m_resp, data);
    if (mgatts_if != NULL)
    {
        // DEBUG_TEST(DB_I," %s ,%d", data, datalen);
        stat = m_static_split_and_send_notification(data, datalen);

        // if (esp_ble_gatts_send_indicate(gl_profile_tab[PROFILE_A_APP_ID].gatts_if, gl_profile_tab[PROFILE_A_APP_ID].conn_id, gl_profile_tab[PROFILE_A_APP_ID].char_handle,
        //                                 datalen, (uint8_t *)m_resp, false) == 0)
        // {
        //     stat = succ_r;
        // }

        // memset(&rsp, 0, sizeof(esp_gatt_rsp_t));
        // strcpy((char *)rsp.attr_value.value, data);
        // rsp.attr_value.len = datalen;

        // DEBUG_E("bluetool_ft.c 486");
        // esp_ble_gatts_send_response(gl_profile_tab[PROFILE_A_APP_ID].gatts_if,
        //                             gl_profile_tab[PROFILE_A_APP_ID].conn_id,
        //                             gl_profile_tab[PROFILE_A_APP_ID].tan_id,
        //                             ESP_GATT_OK, &m_resp);
    }
#endif
    return stat;
}

/**
 * @brief Data is received from the link
 *
 * @param out_data The data received
 * @param len length
 * @return stat_m
 */
stat_m m_ext_net_link_bluetool_receive_data(char *out_data, int *len, uint64_t in_time_out_s)
{
    stat_m stat = fail_r;
    // uint64_t in_soc_time = m_callable_timer_manage_get_utc_time();
    while (1)
    {
        // if (in_time_out_s != M_TIMEOUT_FOREVER && m_callable_timer_manage_get_utc_time() > in_soc_time + in_time_out_s)
        // {
        //     stat = fail_r;
        //     *len = -1;
        //     break;
        // }
        if (re_data.is_receive)
        {
            strcpy(out_data, re_data.inout_data);
            *len = re_data.inout_data_len;
            re_data.is_receive = false;
            DEBUG_TEST_W("m_ext_net_link_mqtt_receive_data -> %s", out_data);
            stat = succ_r;
            break;
        }
        if (!is_connected)
        {
            DEBUG_TEST(DB_W, "m_ext_net_link_bluetool_receive_data 33333333333333333");
            stat = fail_r;
            *len = -1;
            break;
        }
        mDelay_ms(50);
    }
    return stat;
}