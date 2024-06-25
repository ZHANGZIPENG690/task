/**
 * @file main.h
 * @brief 计划运行接口文件,包含计划运行,计划开始
 * @mainpage 计划测试工程
 * @author 毛威鹏、陈前治
 * @bug AAA@sina.cn
 * @bug 调整1.0.0版本的区域参数浸泡和运行时间都是以分钟计数  改单位为秒
 * @bug 调整1.0.0版本的区域参数浸泡和运行时间都是以分钟计数  改单位为秒
 * @version 1.0.1
 * @date 2023-05-10
 * @test <h2> Python code example : </h2>
 * @code
 * 
 * 
 *  # 导入库文件
 *  import ctypes
 * 
 * 
    if __name__ == '__main__' :
        # 定义区域信息 (1,120) 1区域 120秒
        brr = ((ctypes.c_int*2)*3)((ctypes.c_int*2)(1,120),(ctypes.c_int*2)(3,120),(ctypes.c_int*2)(5,120))

        #定义运行时间 86400 一天
        RunningSum = 86400 

        # 加载c语言共享库
        Test = ctypes.CDLL('./libTestBen.so')

        #  在调试文件中生成信息 区分调试次数和开始结束
        Test.the_program_begins()

        # 初始化计划
        Test.m_schedule_init() 

        # 创建一个计划
        if( Test.StartSchedule( 35, 5 * 60 , 0 ,   1 ,         1 ,            2   , 60  , brr) == 13) :
            print("create succ"  )
        else :
            print("create fail"  )

        # 测试运行时间 ，执行单位:秒
        for i in range(RunningSum ) :
            Test.Test_time( i  ) 

        #  在调试文件中生成信息 区分调试次数和开始结束
        Test.the_program_end()
        print("-- Over !"  )






    c

 * @endcode
 */

/**
 * 
 * @brief Python 进阶
 * @code
 * 
 * 
 *  # 导入库文件
 *  import ctypes


    if __name__ == '__main__' :
        # 定义区域信息 (1,120) 1区域 120秒
        brr = ((ctypes.c_int*2)*3)((ctypes.c_int*2)(1,120),(ctypes.c_int*2)(3,120),(ctypes.c_int*2)(5,120))

        #定义运行时间 86400 一天
        RunningSum = 86400 + 6 * 60 + 30

        # 加载c语言共享库
        Test = ctypes.CDLL('./libTestBen.so')

        #  在调试文件中生成信息 区分调试次数和开始结束
        Test.the_program_begins()

        # 初始化计划
        Test.m_schedule_init() 

        # 创建一个计划                end      
                                                #Run type 0 : schedule 1 : QuickRun (not support) 
                                                    # none and pump mode and sack mode ( 0 : 1 : 2) 
                                                        # water hammer  
                                                            # water time (min)
                                                                # sack time 
                                                                    # Schedule start time 
        if( Test.StartSchedule( 35, RunningSum ,1 ,1,  1 , 1,  2 ,  30, brr) == 13) :
            print("create succ"  )
        else :
            print("create fail"  )

        # 测试运行时间 ，执行单位 : 秒  
        for i in range( 86400 , RunningSum + 1) :
            if i == 86400 + 75: #触发事件 传感器触发
                Test.ScheduleEventTriggerEncry( 9, i)
            if i == 86400 + 330: #触发事件 传感器断开
                Test.ScheduleEventTriggerEncry( 10, i)
            Test.Test_time( i ) 

        #  在调试文件中生成信息 区分调试次数和开始结束
        Test.the_program_end()
        
        print("-- Over !"  )






    c

 * @endcode
 */


#include "core/fset_lib.h"
#include "core/schedule/mschedule.h"



/**
 * @brief The call creates a new schedule 
 */
// enum mschedule_event  
// {
//     /**< 停止事件: 手动run stops>* **/
//     MANUAL_STOP, 
//     /**< 停止事件: socket run stops>*/
//     SOCKET_CMD_STOP, 
//     /**< 停止事件: schedule change run stops >*/
//     SCHEDULE_CHANGE_STOP, 
//     /**< 停止事件: schedule delete run stops >*/
//     SCHEDULE_DELETE_STOP, 
//     /**< 停止事件: schedule disable run stops >*/
//     SCHEDULE_DISABLE_STOP, 
//     /**< 停止事件: A single skip run stops>*/
//     A_SIGNLE_SKIP_STOP, 
//     /**< 停止事件: global skip run stops>*/
//     GLOBAL_SKIP_STOP, 
//     /**< 停止事件: Rain/Wind skip run stops>*/
//     WIND_RAIN_SKIP_STOP, 
//     /**< 停止事件: Dormant mode run stops>*/
//     DORMANT_MODE_STOP, 
//     /**< 触发事件: sensor trigger>*/
//     SENSOR_TRIGGER, 
//     /**< 触发事件: sensor dis trigger>*/
//     SENSOR_DIS_TRIGGER, 
//     /**< 触发事件: zone enable trigger>*/
//     CHANNEL_ENABLE_TRIGGER, 
//     /**< 触发事件: zone disable trigger>*/
//     CHANNEL_DISABLE_TRIGGER, 

//     /**< 触发事件: changing-over " Start " event trigger>*/
//     CHANGING_OVER_START_TRIGGER, 
//     /**< 触发事件: changing-over " Stop " event trigger>*/
//     CHANGING_OVER_STOP_TRIGGER, 
//     /**< 触发事件: changing-over " Left " event trigger>*/
//     CHANGING_OVER_LEFT_TRIGGER, 
//     /**< 触发事件: changing-over " Right " event trigger>*/
//     CHANGING_OVER_RIGHT_TRIGGER,
// };





/**
 * @brief The call start a new schedule 
 * @param[in] in_schedule_id schedule id
 * @param[in] in_instance_id instance  id (end Time)
 * @param[in] runtype Run type    0 : schedule  1 : QuickRun (not support)
 * @param[in] pump_sack none and pump mode and  sack mode {3 : Multi-region, multi-time} ( 0 : 1 : 2 : 3) 
 * @param[in] water_hammer water hammer is open   ( disabe : 0 / open : 1) 
 * @param[in] waterTime water time (unit :min) {Invalid parameter when pump_sack is 0}
 * @param[in] soakTime sack time (unit :min)   {Invalid parameter when pump_sack is 0}
 * @param[in] WStime Schedule start time
 * @param[in] channel channel parameter information, two-dimensional array, the first is the channel id, the second is the running time (unit: seconds)
 * @param[in] sack_channel channel parameter information, two-dimensional array, the first is the channel run-time, the second is the sack-time (unit: seconds)
 * @bug Fast running is not supported
 * @bug pump mode is not supported
 * @bug hammer is not supported
 * @warning " in_instance_id " must be the exact time when the schedule ends, or the unpredictable results will occur
 * @return stat_m
 *          @retval fail_r fail
 *          @retval succ_r success
 */
stat_m  StartSchedule(uint64_t in_schedule_id,
                        uint64_t in_instance_id,
                        uint8_t runtype,
                        uint16_t pump_sack,
                        uint16_t water_hammer,
                        uint16_t waterTime,
                        uint16_t soakTime,
                        uint64_t WStime,
                        uint32_t (*channel)[2],
                        uint32_t (*sack_channel)[2]
                        );



    
/**
 * @brief Event trigger entry when scheduled at runtime
 * @param[in] event_id Event id
 * @param[in] trigger_time triggering time
 * @return stat_m
 *          @retval fail_r Execution was successful but may not take effect
 *          @retval succ_r Execution was successful 
 */
stat_m ScheduleEventTriggerEncry(enum mschedule_event event_id , uint64_t trigger_time );




/**
 * @brief Can't call
 * @todo The scheduled content and output results are in the current file "debug.txt"
 * @note The scheduled content and output results are in the current file "debug.txt"
 * @warning The scheduled content and output results are in the current file "debug.txt"
 */
void Schedule_out_file();


