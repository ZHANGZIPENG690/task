#include "core/fset_lib.h"
#include "core/utils/util.h"

#define NEDU "#@#\r\n"

volatile static bool anquan = false;

void m_static_queue_pro_init(QueRoot *qr)
{
    qr->headQueNode = qr->tailQueNode = NULL;
    anquan = false;
}

bool m_static_queue_pro_push(QueRoot *qroot, const char *que_data, int qn_len, uint64_t spec_cmd, uint64_t spec_cmd_flag, uint64_t timeout)
{
    // anquan = true;
    bool result = false;
    if (que_data == NULL || qn_len <= 0)
    //  || (qroot->nodeSum >= NODE_SIZE - 1))
    {
        printf("NULL LEN < 0");
        anquan = false;
        return result;
    }
    // if (qroot->headQueNode != NULL && memcmp(que_data, qroot->headQueNode->Qdata, (qn_len > 15 ? 15 : qn_len)) == 0)
    // //  || (qroot->nodeSum >= NODE_SIZE - 1))
    // {
    //     DEBUG_E("FAIL ( %s <> %s)", qroot->headQueNode->Qdata, que_data);
    //     anquan = result;
    //     return result;
    // }

    static QueNode *qNode = NULL;
    qNode = (QueNode *)malloc(sizeof(QueNode));

    memset(qNode, 0, sizeof(QueNode));
    memcpy(qNode->Qdata, que_data, qn_len);
    qNode->spec_main_cmd = spec_cmd;
    qNode->spec_cmd_flag = spec_cmd_flag;
    qNode->timeout = timeout;
    strcat(qNode->Qdata, NEDU); // 添加分隔符
    // printf("-> %s" , qNode->Qdata);
    if (qroot->tailQueNode == NULL || qroot->headQueNode == NULL)
    {
        result = true;
        qNode->nextQueNode = NULL;
        qroot->headQueNode = qroot->tailQueNode = qNode;
        qroot->nodeSum++;
        DEBUG_TEST(DB_I," Push %s  Sum :%d+", qNode->Qdata, qroot->nodeSum);
    }
    else
    {
        QueNode *temp_Node = qroot->headQueNode;
        qNode->nextQueNode = qroot->headQueNode;
        qroot->headQueNode = qNode;
        temp_Node->preQueNode = qNode;

        result = true;
        if (qroot->nodeSum >= NODE_SIZE - 1)
        {
            // 当内部指令超过一定数量之后，挤掉最后一个 1987654321987654321  -->  1987654321
            if (qroot->tailQueNode != NULL)
            {
                result = true;
                // 记录下队列尾部的上一个节点
                temp_Node = qroot->tailQueNode->preQueNode;
                DEBUG_E("DELE %s (%d)", qroot->tailQueNode->Qdata, qroot->nodeSum);
                // 删除尾部节点
                free(qroot->tailQueNode);
                temp_Node->nextQueNode = NULL;
                qroot->tailQueNode = NULL;
                // 恢复尾部节点，数量不变
                qroot->tailQueNode = temp_Node;
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
            qroot->nodeSum++;
            DEBUG_TEST(DB_I," Push %s  Sum :%d+", temp_Node->Qdata, qroot->nodeSum);
            // DEBUG_TEST(DB_W,"(%s)nodeSum++(%d)", qroot->name, qroot->nodeSum);
        }
    }
    // DEBUG_TEST(DB_I," qroot->nodeSum %d", qroot->nodeSum);
    anquan = false;
    return result;
}
bool m_static_queue_pro_pop(QueRoot *qroot, char *out_que_data, uint64_t *spec_cmd, uint64_t *spec_cmd_flag, uint64_t *timec)
{
    bool re_data = false;
    // DEBUG_E("%s --->pop" , qroot->name);
    // while (anquan)
    //     mDelay_ms(1);
    // // DEBUG_E("pop1");
    anquan = true;
    // QueNode *temp_Node = NULL;

    {
        if (qroot->tailQueNode == NULL)
        {
            qroot->headQueNode = qroot->tailQueNode = NULL;
            anquan = false;
            return re_data;
        }
        re_data = true;

        strcpy(out_que_data, qroot->tailQueNode->Qdata);
        if (spec_cmd != NULL && spec_cmd_flag != NULL)
        {
            *spec_cmd = qroot->tailQueNode->spec_main_cmd;
            *spec_cmd_flag = qroot->tailQueNode->spec_cmd_flag;
            *timec = qroot->tailQueNode->timeout;
        }
    }
    anquan = false;

    // DEBUG_E("pop3(%d)" ,  re_data);
    return re_data;
}
void m_static_queue_pro_rm(QueRoot *qroota)
{
    // while (anquan)
    //     mDelay_ms(1);
    // anquan = true;
    QueNode *temp_Node = NULL;

    if (qroota != NULL )
    if (qroota->headQueNode == NULL )
    {
        qroota->headQueNode = qroota->tailQueNode = NULL;
        // qroota->nodeSum = 0;
        DEBUG_TEST(DB_I,"---------------------------%d-", qroota->nodeSum);
        // DEBUG_TEST(DB_I,"-- qroot->nodeSum %d ", qroot->nodeSum);
        anquan = false;
        return;
    }
    // DEBUG_E(" Dele -- (%s) %d",qroota->tailQueNode->Qdata, qroota->nodeSum);
    temp_Node = qroota->tailQueNode->preQueNode;

    free(qroota->tailQueNode);
    qroota->tailQueNode = NULL;
    qroota->nodeSum--;
    // DEBUG_TEST(DB_I,"-%d-", qroot->nodeSum);

    // DEBUG_TEST(DB_W,"(%s)nodeSum--(%d)", qroot->name, qroot->nodeSum);
    qroota->tailQueNode = temp_Node;
    // qroot->tailQueNode->nextQueNode = NULL;

    anquan = false;
}
