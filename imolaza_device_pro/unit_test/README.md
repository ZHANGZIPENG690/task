





# Unit Develop
#### unit function Documentation
#### waring :  ***
## Overview
<!-- [image](image/enter_python.png) -->
* Contains all functional component projects. The structure is a library of unit functions centered around fset_lib.h
* If you use threads, event groups, messages in your project: Please implement it in core.h

##   API Reference 
* Quick preview of interfaces that distinguish between different functions and levels
* Representation function attribute callable represents a function function, generally as an entry, to provide functions for the caller, ext represents the need for external implementation of hardware operation-based content, generally and platform correlation is strong, the following functions are the above rules.
* more ...

##   Guidelines for function naming
    - m_callable : Interface functions, provided to external calls
    - m_static : The library uses functions internally, does not provide external use, does not require external implementation, and access scope is only "package" internal           --> <protected>
    - m_ext : Some driver components will have such a function corresponding to callable, which may also be an event-handling callback （Functions that require an external implementation）
        - unit nameemsp;&emsp;&emsp;example m_ext_drive
        - components&emsp;&emsp;&emsp;example m_ext_drive_flash
        - function&emsp;&emsp;&emsp;&emsp;&emsp;example m_ext_drive_flash_write


##   Unit Interface
#### Drive
It is divided into hardware-related and platform-related：
-  Chip Drive
    - see lighting.h  
        - The light shows the underlying driver, including the light on, off, and initialization operations
    - see solenoid.h  
        - The chip HC595 driver file encapsulates the status of opening, closing, and reading extended IO
    - see mrtc.h  
        - RTC-related operation interface library
    - see pump.h  
        - About the operating interface of the pump

- Software Drive
    - see flash.h  
        - Hardware-related function interfaces to directly operate hardware devices
    - see wifi.h  
        - Hardware-related function interfaces to directly operate hardware devices
    - see bluetool.h  
        - Hardware-related function interfaces to directly operate hardware devices
    - see button.h  
        - Hardware-related function interfaces to directly operate hardware devices

-----
-----

#### Middle

- Channel
    - see channel_manage.h  
        - Hardware-related function interfaces to directly operate hardware devices
- Current
    - see current_monitor.h  
        - Hardware-related function interfaces to directly operate hardware devices
- Event Distribution
    - see event_distribution.h  
        - Hardware-related function interfaces to directly operate hardware devices
- Headware
    - see self_test.h  
        - Hardware-related function interfaces to directly operate hardware devices
    - see key.h  
        - Hardware-related function interfaces to directly operate hardware devices
    - see sensor.h  
        - Hardware-related function interfaces to directly operate hardware devices
- Net
    - see synchro.h  
        - Hardware-related function interfaces to directly operate hardware devices
- Schedule
    - see water_delay.h  
        - Hardware-related function interfaces to directly operate hardware devices
    - see schedule_skip.h  
        - Hardware-related function interfaces to directly operate hardware devices
- Timer
    - see time_manage.h  
        - Hardware-related function interfaces to directly operate hardware devices
- tool
    - see data_parse.h  
        - Hardware-related function interfaces to directly operate hardware devices

-----
-----

#### Application
- Instance And Schedule
    - see schedule_stroage.h  
        - It includes a plan local storage, read interface, and specifies the storage format, storage rules, and restrictions
    - see schedule_list.h  
        - Use optional options to configure device access, including Bluetooth
    - see instance_running.h  
        - Use optional options to configure device access, including Bluetooth
- network
    - see network_manage.h  
        - Use optional options to configure device access, including Bluetooth
    - see ota.h  
        - Use optional options to configure device access, including Bluetooth
- Device Attribute And Status
    - see device_fsm.h  
        - Use optional options to configure device access, including Bluetooth
    - see device_attribute.h  
        - Use optional options to configure device access, including Bluetooth
- Manual 
    - see manual.h  
        - Use optional options to configure device access, including Bluetooth
- Display 
    - see display.h  
        - Use optional options to configure device access, including Bluetooth


### Python code example : 
``` 
    # 导入库文件
    import ctypes
  
  
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

```
