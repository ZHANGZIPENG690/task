

/**
 * @file button.h
 * @brief
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @date 2023-07-10
 *
 * @code


    # 导入库文件
    from  ctypes import *
    import _thread
    import time
    import faulthandler



    # 开启DEBUG
    faulthandler.enable()

    class Button :
        Test =  CDLL('../out/libbutton.so')
        c_function_cb = None
        time_ms = 0

        M_KEY_EVENT_LEFT = 0
        M_KEY_EVENT_RIGHT = 1
        M_KEY_EVENT_START = 2
        M_KEY_EVENT_STOP = 3

        def __init__(self,Obj):
            self.Test = Obj
            self.Start()

        # 事件触发 外部随时调用
        def EventEntry(self,kry_event_id):
            # self.Test.m_ext_drive_gpio_get_level()
            # 事件触发
            self.Test.m_callable_key_event_enter( kry_event_id, self.time_ms)
            self.Test.add(self.c_function_cb)

        def  m_callable_key_handle(self,key_id,  pre_key_index,  current_key_index):
            # 回调函数 结果处理，具体参数详情API
            print("result even_id :" ,key_id ,pre_key_index, current_key_index )
            return 0

        # 执行函数 调用按键检测
        def Main(self):
            while True :
                # 1ms
                time.sleep(0.001)
                self.time_ms = self.time_ms + 1
                # 开始检测按键状态
                self.Test.m_static_drive_key_function_monitor(self.time_ms)
                # self.Test.main()

        # 初始化
        def Start(self):
            # 传入设备类型
            self.Test.m_callable_key_init(16)
            CFUNC = CFUNCTYPE(c_int,c_int,c_uint8,c_uint8)
            self.c_function_cb = CFUNC(self.m_callable_key_handle)
            # 注册回调处理的函数 所有模块一样
            self.Test.add(self.c_function_cb)
            _thread.start_new_thread(self.Main, ())


    # if __name__ == '__main__' :
    Object = CDLL('../out/libbutton.so')
    btn = Button(Object)


 * @endcode
 */
#ifndef __BUTTON_H__
#define __BUTTON_H__

#include "core/fset_lib.h"

/** 多长时间处理按键事件*/
#if !UNIT_TEST
    #define M_KEY_LIMIT_TIME 13
    #define M_KEY_LIMIT_TIME_HANDLE_MAX 13
    // #define M_KEY_LIMIT_TIME 5
    // #define M_KEY_LIMIT_TIME_HANDLE_MAX 13
#else
    #define M_KEY_LIMIT_TIME 50
    #define M_KEY_LIMIT_TIME_HANDLE_MAX 50
#endif

/**
 * @brief 按键初始化
 * 
 * @param device_type 设备区域 最大数量
 * @return stat_m 
 */
stat_m m_callable_drive_button_init(uint8_t device_type);
/**
 * @brief
 * @todo Unit Input ...
 *
 * @param time_s
 * @return stat_m
 */
stat_m m_callable_manual_set_auto_stop_s(uint8_t channel);

/**
 * @brief Construct a new m callable key init object
 * @todo Unit Input ...
 *
 */
stat_m m_callable_drive_button_function_monitor(uint64_t current_time_ms);

/**
 * @brief 事件入口
 * @todo Unit Input ...
 *
 * @param kry_event_id
 * @param current_time_ms
 * @return stat_m
 */
stat_m m_callable_drive_button_event_input(enum key_id kry_event_id);
/**
 * @brief 直接驱动时间的调用 而不是硬件响应
 *
 * @param key_event_id
 * @return stat_m
 */
stat_m m_callable_drive_button_event_direct_input(enum key_id key_event_id);
/**
 * @brief 直接停止一个触发
 *
 * @return stat_m
 */
stat_m m_callable_drive_button_direct_stop(void);
/**
 * @brief
 * @todo Unit Out ...
 *
 * @param key_id
 * @param pre_key_index
 * @param current_key_index
 * @param current_time_ms
 * @return stat_m
 */
stat_m m_ext_drive_button_evtnt_handle(enum key_event key_id, uint8_t pre_key_index, uint8_t current_key_index, uint64_t current_time_ms);


/**
 * @brief 获取最大个通道数发给manual_run.c中存放时间的数组
 *
 * @param current_time_ms
 * @return stat_m
 */
int m_static_drive_get_max_channel(void);
#endif /** __BUTTON_H__ */