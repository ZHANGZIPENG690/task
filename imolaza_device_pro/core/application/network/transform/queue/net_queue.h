







#ifndef __NET_QUEUE__H__
#define __NET_QUEUE__H__

#include "core/fset_lib.h"

typedef char *QueData;

#define NODE_SIZE 31


typedef struct qnod_
{
    struct qnod_ *next_node;
    struct qnod_ *pre_node;
    char *Qdata;
    /** The cmd value of the current instruction  */
    /** 当前指令的cmd值  */
    uint64_t spec_main_cmd;
    /** Additional instruction parameters  */
    /** 指令附加参数，可以用表示及当前发送的叠加值  */
    uint64_t spec_cmd_flag;
    uint64_t timeout;
} qnode;

typedef struct 
{
    char name[10];
    qnode *head_node;
    qnode *tail_node;
    volatile int node_sum;
} queue_root;




/**
 * @brief 初始表
 * 
 * @param qr 表
 */
void m_static_net_queue_init(queue_root *qr);

/**
 * @brief 加入队列
 * 
 * @param qroot 表
 * @param que_data 数据
 * @param qn_len 数据长度
 * @param spec_cmd 数据特征
 * @param spec_cmd_flag 特征的特征
 * @param timeout 数据有效时长
 * @return true 
 * @return false 
 */
bool m_static_net_queue_push(queue_root *qroot, const char *que_data, int qn_len, uint64_t spec_cmd, uint64_t spec_cmd_flag, uint64_t timeout);
/**
 * @brief 
 * 
 * @param qroot 表
 * @param out_que_data 取出的数据
 * @param spec_cmd 取出的特征 
 * @param spec_cmd_flag 取出的特征的特征
 * @param timec 时间
 * @return true 
 * @return false 
 */
bool m_static_net_queue_pop(queue_root *qroot, char *out_que_data, uint64_t *spec_cmd, uint64_t *spec_cmd_flag , uint64_t *timec);
/**
 * @brief 删除一个 尾部数据
 * 
 * @param qroot 表
 */
void m_static_net_queue_dele(queue_root *qroot);




#endif













