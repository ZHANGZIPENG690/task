

// #include "instance_algorithm.h"

// #define M_NEXT_STATUS_SELECT 0

// /**   多种模式  */

// stat_m m_static_instance_tools_calculate_next_running_status(
//     uint8_t cur_pc,
//     enum m_funcation m_fun,
//     uint8_t max_chann,
//     struct channel_config *channels,
//     uint64_t WSTime_clone,
//     uint8_t *next_pc,
//     enum base_state *next_state,
//     uint64_t current_time)
// {
//     stat_m stat = fail_r;
//     uint8_t out_pc_x = cur_pc + 1;
//     enum base_state out_state_x = 0;
//     uint32_t temp_c = 0;

//     if (M_NEXT_STATUS_SELECT == 0)
//     {
//         for (size_t i = 0; i < max_chann; i++)
//         {
//             if (out_pc_x >= max_chann)
//                 out_pc_x = 0;
//             if (channels[out_pc_x].cd_should_run_time > channels[out_pc_x].cd_reality_run_time + 1)
//             {
//                 // 不使用浸泡
//                 if (m_fun == M_M_FUNCATION_NONE)
//                 {
//                     if (out_pc_x == cur_pc)
//                         continue;
//                     out_state_x = M_BASE_STATUS_RUNNING;
//                     break;
//                 }
//                 // 使用浸泡
//                 else if (m_fun == M_M_FUNCATION_SACK)
//                 {
//                     // 本次運行時間
//                     temp_c = WSTime_clone - current_time;
//                     if (out_pc_x == cur_pc)
//                     {
//                         // 如果下一個將完成
//                         // printf("%lld-%lld\n",current_time , channels[out_pc_x].channel_running_time_region.tr_end_time );
//                         if (current_time > channels[out_pc_x].channel_running_time_region.tr_end_time && channels[out_pc_x].cd_should_run_time - channels[out_pc_x].cd_reality_run_time <= channels[out_pc_x].cd_water_time)
//                             // if (channels[out_pc_x].cd_reality_run_time + current_time >= channels[out_pc_x].cd_should_run_time + channels[out_pc_x].channel_running_time_region.tr_end_time)
//                             continue;
//                         // 如果下一個將浸泡或者恢復 當前時間和結束時間來判定
//                         else if (current_time <= channels[out_pc_x].channel_running_time_region.tr_end_time && channels[out_pc_x].channel_running_time_region.tr_end_time != 0)
//                             out_state_x = M_BASE_STATUS_RUNNING;
//                         else
//                             out_state_x = M_BASE_STATUS_SACK;
//                     }
//                     else
//                     {
//                         // printf("%lld < %d+%d(%hhd out:%hhd) \n", WSTime_clone,
//                         //        channels[out_pc_x].channel_running_time_region.tr_end_time,
//                         //        channels[out_pc_x].cd_sack_time,
//                         //        channels[cur_pc].cd_cahnnel_id,
//                         //        channels[out_pc_x].cd_cahnnel_id);
//                         // 这里应该有两种情况1、下一个区域浸泡时间不够 2、小于应该浇水时间 在应该是在开始计算时间爱你的时候应该做的
//                         // 下一個時間節點  <  當前区域结束 + 应该浸泡时间
//                         if (WSTime_clone < channels[out_pc_x].channel_running_time_region.tr_end_time + channels[out_pc_x].cd_sack_time && channels[out_pc_x].channel_running_time_region.tr_end_time != 0)
//                             out_state_x = M_BASE_STATUS_SACK;
//                         else
//                             out_state_x = M_BASE_STATUS_RUNNING;
//                     }
//                     break;
//                 }
//                 // 使用泵模式
//                 else if (m_fun == M_M_FUNCATION_PUMP)
//                 {
//                 }
//             }
//             out_pc_x++;
//         }

//         if (out_state_x == 0)
//         {
//             out_state_x = M_BASE_STATUS_IDLE;
//             DEBUG_TEST(DB_W,"[%lld]next next will Stop(%d) ...", current_time, out_state_x);
//         }
//         else
//             DEBUG_TEST(DB_W,"[%lld]next next will (%d) ...", current_time, out_state_x);

//         *next_pc = out_pc_x;
//         *next_state = out_state_x;
//     }
//     return stat;
// }

// stat_m m_static_instance_tools_calculate_next_running_pc(
//     uint8_t cur_pc,
//     uint8_t max_chann,
//     struct channel_config *channels,
//     uint8_t *next_pc)
// {
//     stat_m stat = fail_r;
//     uint8_t out_pc_x = cur_pc + 1;

//     for (size_t i = 0; i < max_chann; i++)
//     {
//         if (out_pc_x >= max_chann)
//             out_pc_x = 0;
//         if (channels[out_pc_x].cd_should_run_time > channels[out_pc_x].cd_reality_run_time + 1)
//         {
//             out_pc_x = out_pc_x;
//             stat = succ_r;
//             break;
//         }
//         if (out_pc_x != max_chann - 1)
//             out_pc_x++;
//     }
//     *next_pc = out_pc_x;
//     return stat;
// }
// stat_m m_static_instance_tools_calculate_pre_running_pc(
//     uint8_t cur_pc,
//     uint8_t max_chann,
//     struct channel_config *channels,
//     uint8_t *pre_pc)
// {
//     stat_m stat = fail_r;

//     int out_pc_x = cur_pc - 1;
//     for (size_t i = 0; i < max_chann; i++)
//     {
//         if (out_pc_x < 0)
//             out_pc_x = max_chann;
//         if (channels[out_pc_x].cd_should_run_time > channels[out_pc_x].cd_reality_run_time + 1)
//         {
//             out_pc_x = out_pc_x;
//             stat = succ_r;
//             break;
//         }
//         if (i != max_chann - 1)
//             out_pc_x--;
//     }
//     *pre_pc = out_pc_x;
//     return stat;
// }

// // stat_m m_static_instance_tools_calculate_switch_status(
// //     uint8_t cur_pc,
// //     uint8_t max_chann,
// //     instance_obj * wobj,
// //     enum base_state *next_state)
// // {
// //     stat_m stat = fail_r;
// //     enum base_state next_state_x = M_BASE_STATUS_RUNNING;
// //       if (wobj->sio_status == M_BASE_STATUS_RUNNING)
// //       {
// //             if(wobj->sio_channel[wobj->running_buff->rb_pc].cd_reality_run_time >= wobj->sio_channel[wobj->running_buff->rb_pc].cd_should_run_time)
// //             {
// //                 if(m_static_instance_tools_calculate_next_running_pc(wobj->running_buff->rb_pc, wobj->running_buff->maxChan, wobj->sio_channel, &wobj->running_buff->rb_pc) == succ_r)
// //                 {

// //                 }else  next_state_x = M_BASE_STATUS_IDLE;
// //             }
// //       }

// //     return stat;
// // }
