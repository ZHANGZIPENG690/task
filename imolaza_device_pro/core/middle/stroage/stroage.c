

#include "stroage.h"

// stat_m m_callable_stroage_rescue_message_flag(char *in_flag, enum m_stroage_rescue_message in_status)
// {
//     stat_m stat = fail_r;
//     uint8_t xx = 0;
//     switch (in_status)
//     {
//     case M_RESCUE_MESSAGE_SET:
//         DEBUG_E("---->%d (%s)", in_status, in_flag);
//         xx = 1;
//         // (nvs_set_u8(my_handle, in_flag, 1) == ESP_OK) ? (state = succ_m) : (state = fail_m);
//         stat = m_ext_flash_write(M_TYPE_U8, in_flag,  (void *)&xx);

//         break;
//     case M_RESCUE_MESSAGE_GET:
//         if ((stat = m_ext_flash_read(M_TYPE_U8, in_flag, (void *)&xx)) == succ_r)
//             (xx == 1) ? (stat = succ_r) : (stat = fail_r);
//         if (stat == succ_r)
//             DEBUG_TEST(DB_I,"(%s)rescue_message_flag %d fined", in_flag, xx);
//         break;
//     case M_RESCUE_MESSAGE_DEL:
//         // (nvs_delete_key(in_flag) == ESP_OK) ? (state = succ_m) : (state = fail_m);
//         stat = m_callable_drive_flash_remove(in_flag); 

//         break;

//     default:
//         break;
//     }

//     return stat;
// }


// stat_m m_callable_stroage_rescue_message_str(char *in_flag, char *inout_str, enum m_stroage_rescue_message in_status)
// {
//     stat_m stat = fail_r;
// switch (in_status)
//     {
//     case M_RESCUE_MESSAGE_SET:
//         DEBUG_E("---->%d (%s)", in_status, in_flag);
//         // (nvs_set_u8(my_handle, in_flag, 1) == ESP_OK) ? (state = succ_m) : (state = fail_m);
//         stat = m_callable_drive_flash_write(M_TYPE_Str, in_flag,  (void *)inout_str);

//         break;
//     case M_RESCUE_MESSAGE_GET:
//         if ((stat = m_callable_drive_flash_read(M_TYPE_Str, in_flag, (void *)inout_str)) == succ_r)
//             DEBUG_TEST( DB_I,(%s)rescue_message_str %s fined", in_flag, inout_str);
//         break;
//     case M_RESCUE_MESSAGE_DEL:
//         // (nvs_delete_key(in_flag) == ESP_OK) ? (state = succ_m) : (state = fail_m);
//         stat = m_ext_flash_remove(in_flag); 
//         break;

//     default:
//         break;
//     }

//     return stat;
// }

