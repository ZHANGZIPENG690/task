/**
 * @file instance_recover.h
 * @author your name (you@domain.com)
 * @brief 计划恢复相关
 * @version 0.1
 * @date 2023-09-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */




#ifndef __INSTANCE_RECOVER__h__
#define __INSTANCE_RECOVER__h__

#include "core/fset_lib.h"
#include "../instance.h"


/**
 * @brief 计划恢复
 * 
 * @param src_str 
 * @param en_dis_able 
 * @param in_time_s 
 */
void m_static_instance_manage_instance_recover(char *src_str,  bool en_dis_able, uint64_t in_time_s);

/**
 * @brief 计划恢复 全部完成
 * 
 * @param in_time_s 时间
 * @return stat_m 
 */
stat_m m_callable_instance_manage_recover_done(uint64_t in_time_s);

#endif