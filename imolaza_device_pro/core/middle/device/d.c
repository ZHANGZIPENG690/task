// #include "hot_so_fast.h"
// #include "tool.h"
// #include "fun/fun.h"
// #include "bsp_hw.h"
// #include "wifi/conn_eth.h"

// extern time_manage _time;
// extern plan_manager _plan;
// extern device_info _device;

// /**  注定查找*/

// /**
//  * 在计划切换的时候 我需要知道下一个应该运行的区域
//  *
//  * 1、判断下一个是否还需要运行
//  * 2、判断下一个是浸泡还是运行
//  * 3、找到下一个运行的区域
//  *
//  */

// extern void m_schedule_flush_start_time_info(uint64_t pretime);

// bool m_find_next_zone(water_task_date_m *obj, uint8_t *cpc)
// {
//     uint8_t x = *cpc;
//     bool boo = false;
//     for (uint8_t i = 1; i < obj->max_chan; i++) // 0 < 2
//     {
//         x = i + *cpc; // 从他后面一个开始找
//         if (x >= obj->max_chan)
//             x -= obj->max_chan;
//         if (obj->chaninfo[i].chanID != 0 && obj->chaninfo[x].shouldRunTime != 0 && obj->chaninfo[x].enable && m_global_zone_query(obj->chaninfo[x].chanID) == succ_m)
//             if (obj->chaninfo[x].shouldRunTime - obj->chaninfo[x].realityRunTime >= 50)
//             {
//                 DEBUG_TEST(DB_I,"sh:%d , re:%d ,dit%d(x= %hhd)", obj->chaninfo[x].shouldRunTime, obj->chaninfo[x].realityRunTime,
//                         obj->chaninfo[x].shouldRunTime - obj->chaninfo[x].realityRunTime > 15, obj->chaninfo[x].chanID);
//                 // x
//                 if (obj->planinfo.planfun.fun_soak_use) // 使用浸泡
//                 {                                       // 上一个通道的结束时间小于浸泡的时间，说明需要浸泡

//                     // The end time of the last channel was less than the soaking time, indicating the immersion     -    1020   < 180
//                     if (_time.time_get_timestamp_t() - obj->chaninfo[x].chanEndTime < ((obj->planinfo.planfun.fun_soak_cycle * 60) - 10))
//                     {
//                     }
//                     else
//                         boo = true;
//                 }
//                 else
//                     boo = true;
//                 break;
//             }
//     }
//     *cpc = x;
//     return boo;
// }

// void m_water_hammer_pretreatment(uint64_t schedule_id, uint32_t current_time)
// {
//     uint8_t next = 0, curre = 0;
//     uint64_t debug_info = 0;
//     water_task_date_m *water_temp = _plan.plan_runnning_obj; // _plan.plan_query_id(schedule_id);
//     // printf("wolail\n");
//     DEBUG_TEST(DB_W,"m_water_hammer_pretreatment");
//     if (water_temp == NULL || !_device.hammer || water_temp->solenoid_mode)
//     {
//         DEBUG_E("---(%d)%d  %d", current_time, _device.hammer, water_temp->solenoid_mode);
//     }
//     else
//     {
//         next = water_temp->pc;
//         curre = water_temp->chaninfo[water_temp->pc].chanID;
//         if (m_find_next_zone(water_temp, &next))
//         {
//             if (water_temp->planinfo.planfun.fun_soak_use)
//             {
//                 if (water_temp->chaninfo[next].shouldRunTime - water_temp->chaninfo[next].realityRunTime > 10) // ||
//                 //     (water_temp->chaninfo[next].realityRunTime >= water_temp->chaninfo[next].shouldRunTime))
//                 {
//                     if (!_plan.is_backstage && !_plan.skip)
//                     {
//                         if (_device.io_chip == chip_hc595)
//                             HC595_SolenoidValve_two_bit(curre /*现在在运行的区域*/, water_temp->chaninfo[next].chanID /*下一个需要开始的区域*/, true),
//                                 printf("(%hhd)\n", water_temp->chaninfo[next].chanID);
//                         else
//                             ahh5_SolenoidValve_two_bit(curre /*现在在运行的区域*/, water_temp->chaninfo[next].chanID /*下一个需要开始的区域*/, true);
//                     }
//                     debug_info = curre + water_temp->chaninfo[next].chanID;
//                     create_send_data_to_server_debug(DEVICE_DEBUG, use_hammer_sack_come_into_force, debug_info);
//                 }
//             }
//             else
//             {
//                 if (!_plan.is_backstage && !_plan.skip)
//                 {
//                     if (_device.io_chip == chip_hc595)
//                         HC595_SolenoidValve_two_bit(curre /*现在在运行的区域*/, water_temp->chaninfo[next].chanID /*下一个需要开始的区域*/, true), printf("(%hhd)\n", water_temp->chaninfo[next].chanID);
//                     else
//                         ahh5_SolenoidValve_two_bit(curre /*现在在运行的区域*/, water_temp->chaninfo[next].chanID /*下一个需要开始的区域*/, true), printf("(%hhd)\n", water_temp->chaninfo[next].chanID);
//                 }
//                 debug_info = curre + water_temp->chaninfo[next].chanID;
//                 create_send_data_to_server_debug(DEVICE_DEBUG, use_hammer__come_into_force, schedule_id);
//             }
//         }
//     }
// }

// #define HAMMER_AAB "hammer"

// void m_water_hammer_set(uint64_t in_state)
// {
//     if (in_state)
//         rescue_message_flag(HAMMER_AAB, M_RESCUE_MESSAGE_SET), _device.hammer = true;
//     else
//         rescue_message_flag(HAMMER_AAB, M_RESCUE_MESSAGE_DEL), _device.hammer = false;
// }

// void m_water_hammer_load()
// {
//     if (rescue_message_flag(HAMMER_AAB, M_RESCUE_MESSAGE_GET) == succ_m)
//     {
//         _device.hammer = true;
//         DEBUG_TEST(DB_W,"hammer is opened !!");
//     }
//     else
//         _device.hammer = false;
// }

// /**    休眠模式*/

// #define DORMANT_START_AAB "Sdormant"
// #define DORMANT_END_AAB "Edormant"

// void m_device_dormant_del()
// {
//     rescue_message_flag(DORMANT_START_AAB, M_RESCUE_MESSAGE_DEL);
//     rescue_message_flag(DORMANT_END_AAB, M_RESCUE_MESSAGE_DEL);
//     _device.dormant_starttime = 0;
//     _device.dormant_enttime = 0;
// }
// bool global_dormant = false;

// void m_device_dormant_set(uint64_t in_stime, uint64_t in_entime)
// {
//     lay_plan_sometime_dormant_set(DORMANT_START_AAB, DORMANT_END_AAB, in_stime, in_entime);
// }

// void m_device_dormant_load()
// {
//     lay_plan_sometime_dormant_get(DORMANT_START_AAB, DORMANT_END_AAB, &_device.dormant_starttime, &_device.dormant_enttime);
//     DEBUG_TEST(DB_W,"sleep : %lld - %lld", _device.dormant_starttime, _device.dormant_enttime);
//     if (_device.dormant_starttime > 10)
//         global_dormant = true;
// }
// bool m_device_dormant_earlier_check_updata_schedule(void)
// {
//     bool result = false;
//     uint64_t temp_ccp = 0;
//     if (!global_dormant)
//         return result;

//     if (_device.dormant_starttime > 100100 && _device.dormant_enttime < 1201223) // 判断是否是特殊格式日期(跨年)
//     {
//         temp_ccp = (_time.time_get_current_month() * 100000) + _time.time_get_current_day() * 100 + _time.new_get_hhmmss() / 3600;
//         if (_device.dormant_enttime == temp_ccp)
//         {
//             result = true;
//             global_dormant = false;
//             DEBUG_TEST(DB_W,"der update time info");
//         }
//     }
//     else if (_device.dormant_starttime > 10100 && _device.dormant_enttime < 123123) // 不跨年
//     {
//         temp_ccp = (_time.time_get_current_month() * 10000) + _time.time_get_current_day() * 100 + _time.new_get_hhmmss() / 3600;
//         if (_device.dormant_enttime == temp_ccp)
//         {
//             result = true;
//             global_dormant = false;
//             DEBUG_TEST(DB_W,"der update time info");
//         }
//     }
//     else if (_device.dormant_enttime - _time.time_get_drive_timestamp_t() < 120 && _device.dormant_enttime - _time.time_get_drive_timestamp_t() > 110)
//     {
//         result = true;
//         global_dormant = false;
//         DEBUG_TEST(DB_W,"der update time info");
//     }
//     // DEBUG_TEST(DB_I,"debug (%lld - %lld )%lld" , _device.dormant_enttime  , temp_ccp, _device.dormant_enttime -  _time.time_get_drive_timestamp_t() );

//     return result;
// }

// // true :  not skip 301206
// bool m_device_dormant_check(bool notify)
// {
//     bool result = true;
//     uint64_t temp_ccp = 0;
//     // DEBUG_TEST(DB_W,"m_device_dormant_check");
//     temp_ccp = (_time.time_get_current_month() * 100000) + _time.time_get_current_day() * 100 + _time.new_get_hhmmss() / 3600;
//     if (_device.dormant_starttime > 100100 && _device.dormant_enttime < 1201223) // 判断是否是特殊格式日期(跨年)
//     {
//         // hour = _device.dormant_starttime % 10000 / 100;
//         // DEBUG_E("buf %d-%d <-> %d-%d", smonth, sday, emonth, eday);
//         DEBUG_E("temp_ccp %lld", temp_ccp);

//         // 要么都大   要么都小
//         if ((temp_ccp >= _device.dormant_starttime && temp_ccp > _device.dormant_enttime) ||
//             (temp_ccp <= _device.dormant_starttime && temp_ccp <= _device.dormant_enttime))
//         {
//             DEBUG_TEST(DB_W,"sleep ...");
//             result = false; // 1210007 312022
//         }
//         if (notify && temp_ccp == _device.dormant_enttime)
//         {
//             result = true;
//         }
//         DEBUG_TEST(DB_W,"mode 2 (跨年) : %lld <-> %lld", _device.dormant_starttime, _device.dormant_enttime);
//     }
//     else if (_device.dormant_starttime > 10100 && _device.dormant_enttime < 123123) // 不跨年
//     {
//         temp_ccp = (_time.time_get_current_month() * 10000) + _time.time_get_current_day() * 100 + _time.new_get_hhmmss() / 3600;
//         if (temp_ccp >= _device.dormant_starttime && temp_ccp <= _device.dormant_enttime)
//         {
//             DEBUG_TEST(DB_W,"sleep ...");
//             // DEBUG_TEST(DB_W,"mode 1 : %lld <-> %lld" ,_device.dormant_starttime , _device.dormant_enttime);
//             result = false;
//         }
//         if (notify && temp_ccp == _device.dormant_enttime)
//         {
//             result = true;
//         }
//         DEBUG_TEST(DB_W,"mode 2 (不跨年) : %lld <-> %lld", _device.dormant_starttime, _device.dormant_enttime);
//     }
//     else if (_time.time_get_drive_timestamp_t() < _device.dormant_enttime && _time.time_get_drive_timestamp_t() >= _device.dormant_starttime)
//     {
//         result = false;
//         if (notify && _time.time_get_drive_timestamp_t() > _device.dormant_enttime - 3600)
//         {
//             result = true;
//         }
//         DEBUG_TEST(DB_W,"mode 1 : %lld <-> %lld", _device.dormant_starttime, _device.dormant_enttime);
//     }
//     if (!result)
//     {
//         create_send_data_to_server(DEVICE_DEBUG, U32_, &temp_ccp, U32, &_device.dormant_starttime, U32, &_device.dormant_enttime);
//         DEBUG_TEST(DB_W,"current %d-%d-%d(%lld) - %lld", _time.time_get_current_month(), _time.time_get_current_day(), _time.new_get_hhmmss() / 3600, _time.time_get_drive_timestamp_t(), temp_ccp);
//     }

//     return result;
// }

// void m_device_dormant_control(uint64_t in_stime, uint64_t in_etime, uint64_t in_state)
// {
//     if (in_state == 0)
//     {
//         m_device_dormant_del();
//         global_dormant = false;
//         m_schedule_flush_start_time_info(_time.new_get_hhmmss());
//     }
//     else if (in_state == 1)
//     {
//         global_dormant = true;
//         m_device_dormant_set(in_stime, in_etime);
//         _device.dormant_starttime = in_stime;
//         _device.dormant_enttime = in_etime;
//         if (_time.time_get_drive_timestamp_t() >= in_stime && _time.time_get_drive_timestamp_t() <= in_etime)
//         {
//             stop_unconditionally_schedule();
//         }
//     }
//     else
//     {
//         global_dormant = true;
//         time_t ptime = in_stime;
//         struct tm *tps;
//         uint64_t sytime = 0, eytime = 0;
//         int step = 10000, a, b, c;

//         tps = localtime(&ptime);
//         a = tps->tm_mon + 1;
//         b = tps->tm_mday;
//         c = tps->tm_hour;
//         ptime = in_etime;
//         tps = localtime(&ptime);
//         tps->tm_mon += 1;
//         // a表示开始时间的月份
//         DEBUG_E("start %d-%d-%d", a, b, c);
//         DEBUG_E("current %d-%d-%d", _time.time_get_current_month(), _time.time_get_current_day(), _time.new_get_hhmmss() / 3600);
//         DEBUG_E("end %d-%d-%d", tps->tm_mon, tps->tm_mday, tps->tm_hour);
//         if (a >= tps->tm_mon && b > tps->tm_mday) // 跨年 100000  十万
//         {
//             step = 100000, printf("跨(%lld , %lld)", in_stime, in_etime);
//             if ((_time.time_get_current_month() >= tps->tm_mon && _time.time_get_current_month() >= a && _time.time_get_current_day() >= tps->tm_mday && _time.time_get_current_day() >= b) ||
//                 (_time.time_get_current_month() <= tps->tm_mon && _time.time_get_current_month() <= a && _time.time_get_current_day() <= tps->tm_mday && _time.time_get_current_day() <= b))
//                 // 现在的月份大于等于开始的月份 和现在的月份小于等于结束的月份 并且  现在的日大于等于开始的日现在的日小于等于日
//                 stop_unconditionally_schedule();
//         }
//         else //  不跨   10000
//         {
//             step = 10000, printf("不跨(%lld , %lld)", in_stime, in_etime);
//             if ((_time.time_get_current_month() * 100 + _time.time_get_current_day()) <= (tps->tm_mon * 100 + tps->tm_mday) && _time.time_get_current_month() * 100 + _time.time_get_current_day() >= a * 100 + b)
//                 // if(_time.time_get_current_month() <= tps->tm_mon && _time.time_get_current_month() >= a &&   _time.time_get_current_day()  <=  tps->tm_mday &&  _time.time_get_current_day()  >= b  )
//                 // 现在的月份大于等于开始的月份 和现在的月份小于等于结束的月份 并且  现在的日大于等于开始的日现在的日小于等于日
//                 stop_unconditionally_schedule();
//             // if(in_stime / 86400 ) // 想到一个牛逼的方法 误差相等的话  那就是准的 直接诶求得天数 一比较就ok ，实验表明   不行哦
//         }

//         sytime = (a)*step + b * 100 + c;
//         eytime = (tps->tm_mon) * step + (tps->tm_mday) * 100 + tps->tm_hour;

//         DEBUG_E("%lld %lld ", sytime, eytime);
//         _device.dormant_starttime = sytime;
//         _device.dormant_enttime = eytime;
//         m_device_dormant_set(sytime, eytime); // MAX 1231  MIN 101
//     }
// }
