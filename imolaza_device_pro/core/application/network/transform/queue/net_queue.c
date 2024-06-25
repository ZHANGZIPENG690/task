

#include "net_queue.h"

// #define NEDU "#@#\r\n"
#define NEDU " "

void m_static_net_queue_init(queue_root *qr)
{
    qr->head_node = qr->tail_node = NULL;
}
/**
 * @brief  存入一个可以发送的数据
 *
 * @param qroot 根节点
 * @param out_que_data 存入的char数据
 * @param spec_cmd 存入的指令
 * @param spec_cmd_flag 存入计数
 * @param timec 设置超时时间 Min
 * @return true
 * @return false
 */
bool m_static_net_queue_push(queue_root *qroot, const char *que_data, int qn_len, uint64_t spec_cmd, uint64_t spec_cmd_flag, uint64_t timeout)
{
    bool result = false;
    if (que_data == NULL || qn_len <= 0)
    //  || (qroot->nodeSum >= NODE_SIZE - 1))
    {
        DEBUG_TEST( DB_I,"NULL LEN < 0");
        return result;
    }
    // if (qroot->head_node != NULL && memcmp(que_data, qroot->head_node->Qdata, (qn_len > 15 ? 15 : qn_len)) == 0)
    // //  || (qroot->nodeSum >= NODE_SIZE - 1))
    // {
    // DEBUG_E("FAIL ( <> %s)",  que_data);
    //     return result;
    // }
    // m_callable_system_printf_heap_free_info(__FUNCTION__ , __LINE__);

    // printf("是哦大数据的花     -> %s ， %d" , que_data , qn_len);
    qnode *qNode = NULL;
    qNode = (qnode *)M_MALLOC(sizeof(qnode));
    memset(qNode, 0, sizeof(qnode));

    if (qNode != NULL)
    {
        qNode->Qdata = NULL;
        qNode->Qdata = (char *)M_MALLOC(qn_len);
        memset(qNode->Qdata, 0, qn_len);
        // printf("是哦大数据的花     -> %s  %d" , qNode->Qdata , qn_len);
        memcpy(qNode->Qdata, que_data, qn_len - 1);
        qNode->spec_main_cmd = spec_cmd;
        qNode->spec_cmd_flag = spec_cmd_flag;
        qNode->timeout = timeout;
        // printf("是哦大数据的花     -> %s" , que_data);
        // printf("是哦大数据的花     -> %s" , qNode->Qdata);
        // strcat(qNode->Qdata, NEDU); // 添加分隔符
        if (qroot->tail_node == NULL || qroot->head_node == NULL)
        {
            result = true;
            qNode->next_node = NULL;
            qroot->head_node = qroot->tail_node = qNode;
            qroot->node_sum++;
            // DEBUG_TEST(DB_I,"+%d+", qroot->node_sum);
        }
        else
        {
            qnode *temp_Node = qroot->head_node;
            qNode->next_node = qroot->head_node;
            qroot->head_node = qNode;
            temp_Node->pre_node = qNode;

            result = true;
            if (qroot->node_sum >= NODE_SIZE - 1)
            {
                // 当内部指令超过一定数量之后，挤掉最后一个 1987654321987654321  -->  1987654321
                if (qroot->tail_node != NULL)
                {
                    result = true;
                    // 记录下队列尾部的上一个节点
                    temp_Node = qroot->tail_node->pre_node;
                    DEBUG_E("DELE %s (%d)", qroot->tail_node->Qdata, qroot->node_sum);
                    // 删除尾部节点
                    M_FREE(qroot->tail_node->Qdata);
                    qroot->tail_node->Qdata = NULL;
                    M_FREE(qroot->tail_node);
                    temp_Node->next_node = NULL;
                    qroot->tail_node = NULL;
                    // 恢复尾部节点，数量不变
                    qroot->tail_node = temp_Node;
                    // qroot->nodeSum -= 1;
                }
                else
                {
                    result = false;
                    printf("Fail !....\r\n");
                }
            }
            else
            {
                qroot->node_sum++;
                // DEBUG_TEST(DB_I,"+%d+", qroot->node_sum);
                // DEBUG_TEST(DB_W,"(%s)nodeSum++(%d)", qroot->name, qroot->nodeSum);
            }
        }
    }
    // DEBUG_TEST(DB_I," qroot->nodeSum %d", qroot->node_sum);
    // m_callable_system_printf_heap_free_info(__FUNCTION__ , __LINE__);
    return result;
}
/**
 * @brief  拿出一个可以发送的数据
 *
 * @param qroot 根节点
 * @param out_que_data 发送的char数据
 * @param spec_cmd 发送的指令
 * @param spec_cmd_flag 发送计数
 * @param timec 超时时间 Min
 * @return true
 * @return false
 */
bool m_static_net_queue_pop(queue_root *qroot, char *out_que_data, uint64_t *spec_cmd, uint64_t *spec_cmd_flag, uint64_t *timec)
{
    bool re_data = false;
    // DEBUG_E("%s --->pop" , qroot->name);

    // m_callable_system_printf_heap_free_info(__FUNCTION__ , __LINE__);
    {
        if (qroot->tail_node == NULL)
        {
            qroot->head_node = qroot->tail_node = NULL;
            // DEBUG_TEST( DB_I,"五" );
            return re_data;
        }
        re_data = true;

        // DEBUG_TEST( DB_I,"五 %s",qroot->tail_node->Qdata );
        strcpy(out_que_data, qroot->tail_node->Qdata);
        // memcpy(out_que_data, qroot->tail_node->Qdata , sizeof(qroot->tail_node->Qdata));
        if (spec_cmd != NULL && spec_cmd_flag != NULL)
        {
            *spec_cmd = qroot->tail_node->spec_main_cmd;
            *spec_cmd_flag = qroot->tail_node->spec_cmd_flag;
            *timec = qroot->tail_node->timeout;
        }
        // temp_Node = qroot->tail_node->preqnode;

        // free(qroot->tail_node);
        // qroot->tail_node = NULL;

        // qroot->tail_node = temp_Node;
        // // qroot->tail_node->nextqnode = NULL;
        // printf("是llllllll数据的花     -> %s ， %d" , out_que_data , 0);

        // qroot->nodeSum--;
    }

    // DEBUG_E("pop3(%d)    -- > %s" ,  re_data , qroot->tail_node->Qdata);
    // m_callable_system_printf_heap_free_info(__FUNCTION__ , __LINE__);
    return re_data;
}
void m_static_net_queue_dele(queue_root *qroot)
{
    qnode *temp_Node = NULL;

    // m_callable_system_printf_heap_free_info(__FUNCTION__ , __LINE__);
    if (qroot->tail_node == NULL)
    {
        qroot->head_node = qroot->tail_node = NULL;
        qroot->node_sum--;
        // DEBUG_TEST(DB_I,"-%d-", qroot->node_sum);

        // DEBUG_TEST(DB_I," qroot->nodeSum %d ", qroot->node_sum);

        return;
    }
    temp_Node = qroot->tail_node->pre_node;
    M_FREE(qroot->tail_node->Qdata);

    qroot->tail_node->Qdata = NULL;

    M_FREE(qroot->tail_node);
    qroot->tail_node = NULL;
    // DEBUG_TEST(DB_I," qroot->nodeSum %d", qroot->node_sum);
    qroot->node_sum--;
    // DEBUG_TEST(DB_I,"-%d-", qroot->node_sum);

    // DEBUG_TEST(DB_W,"(%s)nodeSum--(%d)", qroot->name, qroot->nodeSum);
    qroot->tail_node = temp_Node;
    // qroot->tail_node->nextqnode = NULL;
    // m_callable_system_printf_heap_free_info(__FUNCTION__ , __LINE__);
}
