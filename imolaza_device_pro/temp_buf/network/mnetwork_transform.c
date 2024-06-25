
#include "core/fset_lib.h"
#include "mnetwork.h"




bool if_allow_dele(uint64_t dele_cmd_flag, uint64_t dele_cmd_count_flag)
{
    bool state_c = false;
    _link.buf_flag_top.buf_flag_cmd_temp = dele_cmd_flag;
    if (dele_cmd_flag == 12)
        _link.buf_flag_top.allow_deletion = true;

    if (dele_cmd_count_flag > 0 && (dele_cmd_flag == 61 ||
                                    dele_cmd_flag == M_CMD_NOTIFY_TO_SERVER_MANUAL_RUNNING_SWITCH ||
                                     dele_cmd_flag == M_CMD_NOTIFY_TO_SERVER_SCHEDULE_SWITCH))
    {
        if (_link.buf_flag_top.buf_flag_tx_count != dele_cmd_count_flag)
            return state_c;
    }
    DEBUG_TEST(DB_I,"%lld , %lld", _link.buf_flag_top.buf_flag_cmd_temp, dele_cmd_count_flag);
    if (_link.buf_flag_top.buf_flag_cmd_temp == _link.buf_flag_top.buf_flag_cmd ||
        // 21 - 16 - 22
        // ID_RESP_ADD_PLAN_TO_DEVICE 21 ID_ADD_PLAN_TO_DEVICE  20
        // 1.发16 有可能会回 20、22
        // 2.发21 有可能会回  20、22
        // 3.发23 有可能会回  20、22
        (((_link.buf_flag_top.buf_flag_cmd_temp == M_CMD_TWOWAY_ADD_A_SCHEDULE ||
           _link.buf_flag_top.buf_flag_cmd_temp == M_CMD_TWOWAY_DELE_SCHEDULE) &&
          (_link.buf_flag_top.buf_flag_cmd == M_CMD_NOTIFY_TO_SERVER_DEVICE_SYNC_SCHEDULE ||
           _link.buf_flag_top.buf_flag_cmd == M_CMD_NOTIFY_TO_SERVER_ADD_SCH_RESP ||
           _link.buf_flag_top.buf_flag_cmd == M_CMD_TWOWAY_ADD_A_SCHEDULE)) ||
         // 80  -82
         (_link.buf_flag_top.buf_flag_cmd_temp == 82 &&
          _link.buf_flag_top.buf_flag_cmd == 80)))
    {
        DEBUG_TEST(DB_W,"RECE_OK (%lld)", _link.buf_flag_top.buf_flag_cmd_temp);
        // if (_link.buf_flag_top.buf_flag_cmd_temp == M_CMD_NOTIFY_TO_SERVER_DEVICE_CONFIRM_UPDATA)
        //     waitingForServerResp_ota = false;
        _link.buf_flag_top.allow_deletion = true;
        state_c = true;
    }
    return state_c;
}

// 服务器消息控制中转函数，调用配置设备信息和功能
stat_m m_static_link_service_control_handle(uint32_t in_cmd,char*in_data,uint64_t times)
{
    return 0;
}










