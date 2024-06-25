



// #include <python3.10/Python.h>
#include "core/fset_lib.h"
#include "core/schedule/mschedule.h"
/**
 * @file TestOut.h
 * @brief 需要实现的python函数
 * @test <h2> Python code example : </h2>
 * @code
 * 
 * 
 * @endcode
 */





/**
 * @brief pt
 * @param[in] wobj instance object
 * @param[in] pin_pc pin
 * @param[in] present_time Time
 */
void pt(schedule_instance_m *wobj, int pin_pc, uint64_t present_time);

/**
 * @brief TestPoint_ABC_schedule_out
 * @param[in] wobj instance object
 * @param[in] event genger event
 * @param[in] present_time Time
 */
void TestPoint_ABC_schedule_out(schedule_instance_m *wobj, int event, uint64_t present_time);



