

#ifndef __INSTANCE_IBUILD__H__
#define __INSTANCE_IBUILD__H__
#include "core/fset_lib.h"
#include "../instance.h"
#include "../../../middle/event_distribution/event_distribution.h"

// #include "../../fun.h"
// #include "../../channel_manage/channel_manage.h"

// int m_static_instance_sack_algorithm_queue_gener(struct channel_config *arr, int max_channel, int water_start_time, struct instance_running_sequences *root_sequences, long in_tim_s);

/**    固定的区域IR参数 不可修改  */
static const float M_STATIC_FIXED_IR_FOR_CHANNEL[6] = {0.35f, 0.15f, 0.5f, 0.4f, 0.2f, 0.1f};
static const float M_STATIC_FIXED_AR_FOR_CHANNEL[6] = {1.6f, 1.6f, 1.4f, 1.4f, 0.2f, 0.35f};

static const float M_STATIC_FIXED_IR_FOR_CHANNEL_1 = 0.35f;
static const float M_STATIC_FIXED_IR_FOR_CHANNEL_2 = 0.15f;
static const float M_STATIC_FIXED_IR_FOR_CHANNEL_3 = 0.5f;
static const float M_STATIC_FIXED_IR_FOR_CHANNEL_4 = 0.4f;
static const float M_STATIC_FIXED_IR_FOR_CHANNEL_5 = 0.2f;
static const float M_STATIC_FIXED_IR_FOR_CHANNEL_6 = 0.1f;

static const float M_STATIC_FIXED_AR_FOR_CHANNEL_1 = 1.6f;
static const float M_STATIC_FIXED_AR_FOR_CHANNEL_2 = 1.6f;
static const float M_STATIC_FIXED_AR_FOR_CHANNEL_3 = 1.4f;
static const float M_STATIC_FIXED_AR_FOR_CHANNEL_4 = 1.4f;
static const float M_STATIC_FIXED_AR_FOR_CHANNEL_5 = 0.2f;
static const float M_STATIC_FIXED_AR_FOR_CHANNEL_6 = 0.35f;
/***/

/**
 * @brief 生成实例，，，可能是任意时刻 可能是在计划运行的当时    也有可能是计划开始之前
 *
 * @param in_doc
 * @param in_obj
 * @param dis_channel
 * @param skip
 */
stat_m m_callable_instance_manage_instance_gener(instance_doc_t *in_doc, instance_obj *in_obj, uint8_t *dis_channel, int *skip, uint64_t in_time_s);

/**
 * @brief  输出实例运行单元序列
 *
 * @param root_inst_queue  序列根节点
 */
void m_static_instance_runnning_queue_print(struct instance_running_sequences *root_inst_queue);
/**
 * @brief  在根节点的序列基础上添加一个新的节点
 *
 * @param root_inst_queue 根节点
 * @param in_running_unit  新的单元序列
 * @return int
 */
int m_static_instance_runnning_queue_append(struct instance_running_sequences *root_inst_queue, struct instance_running_unit *in_running_unit);

#endif /* __INSTANCE_IBUILD__H__ */
