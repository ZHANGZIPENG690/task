





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

    def  m_callable_key_handle(self,key_id,  pre_key_index,  current_key_index , current_time_ms):
        # 回调函数 结果处理，具体参数详情API
        print("result even_id :" ,key_id ,pre_key_index, current_key_index ,current_time_ms)
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
        self.Test.m_callable_key_init()
        CFUNC = CFUNCTYPE(c_int,c_int,c_uint8,c_uint8,c_uint64)
        self.c_function_cb = CFUNC(self.m_callable_key_handle)
        # 注册回调处理的函数 所有模块一样
        self.Test.add(self.c_function_cb)
        _thread.start_new_thread(self.Main, ())


# if __name__ == '__main__' :
Object = CDLL('../out/libbutton.so')
btn = Button(Object) 
    
    