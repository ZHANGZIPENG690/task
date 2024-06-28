

#include "../instance.h"

#include "../ibuild/instance_build.h"

// #include <strlib.h>

#define M_MALLOC malloc
#define M_FREE free

/** 运行相关的队列 包括了 增加删除修改等等 */

int u = 0;

int m_static_instance_runnning_queue_append(struct instance_running_sequences *root_inst_queue, struct instance_running_unit *in_running_unit)
{
    int state = 0;

    // static
    struct instance_running_unit *new_unit_d = NULL;
    new_unit_d = malloc(sizeof(struct instance_running_unit));

    new_unit_d->unit_running_channel = in_running_unit->unit_running_channel;
    new_unit_d->unit_should_running_time = in_running_unit->unit_should_running_time;
    new_unit_d->unit_this_time_running_end_time = in_running_unit->unit_this_time_running_end_time;
    new_unit_d->unit_already_running_time = in_running_unit->unit_already_running_time;
    new_unit_d->unit_this_time_will_running_time = in_running_unit->unit_this_time_will_running_time;
    new_unit_d->unit_running_type = in_running_unit->unit_running_type;
    new_unit_d->unit_done = in_running_unit->unit_done;
    new_unit_d->unit_this_time_cost_running_time = 0;
    new_unit_d->unit_this_time_real_water_statistics = 0;
    new_unit_d->unit_progress = in_running_unit->unit_progress;
    new_unit_d->unit_zone_real_total_time = in_running_unit->unit_zone_real_total_time;
    new_unit_d->state = M_STATIC_UNIT_STATUS_NEW;

    // printf("u = %d\n" , u++);
    // if(new_unit_d  == NULL)
    //     printf("9999\n");
    // memcpy(new_unit_d, in_running_unit, sizeof(in_running_unit));

    // new_unit_d->next_running_unit = NULL;

    // printf("id %d , should %d already %d \n" , new_unit_d->unit_running_channel , new_unit_d->unit_should_running_time , new_unit_d->unit_already_running_time);

    if (root_inst_queue->head_running_unit == NULL || root_inst_queue->tail_running_unit == NULL)
    {
        new_unit_d->pre_running_unit = root_inst_queue->head_running_unit;
        root_inst_queue->head_running_unit = new_unit_d;
        root_inst_queue->tail_running_unit = root_inst_queue->head_running_unit;
    }
    else
    {
        new_unit_d->pre_running_unit = root_inst_queue->tail_running_unit;
        root_inst_queue->tail_running_unit->next_running_unit = new_unit_d;
    }
    root_inst_queue->tail_running_unit = new_unit_d;

    root_inst_queue->head_running_unit->pre_running_unit = root_inst_queue->tail_running_unit;
    root_inst_queue->tail_running_unit->next_running_unit = NULL;

    return state;
}
int m_static_instance_runnning_queue_clear(struct instance_running_sequences *root_inst_queue)
{
    struct instance_running_unit *pc = NULL;

    if (root_inst_queue != NULL && root_inst_queue->head_running_unit != NULL)
    {
        pc = root_inst_queue->head_running_unit;
        while (pc != NULL)
        {
            pc = pc->next_running_unit;
            M_FREE(root_inst_queue->head_running_unit);
            root_inst_queue->head_running_unit = NULL;
            root_inst_queue->head_running_unit = pc;
        }
    }
    return 0;
}

/** 打印队列信息 */
void m_static_instance_runnning_queue_print(struct instance_running_sequences *root_inst_queue)
{
    struct instance_running_unit *pc = NULL;

    if (root_inst_queue != NULL && root_inst_queue->head_running_unit != NULL)
    {
        pc = root_inst_queue->head_running_unit;
        while (pc != NULL)
        {
            if (pc->unit_running_type == M_BASE_STATUS_RUNNING)
            {
                DEBUG_TEST(DB_I, "%d区域 %d , 应该运行 %d  将要运行 %d  完成后区域累积运行 %d (完成 %d)", pc->unit_progress,
                           pc->unit_running_channel, pc->unit_should_running_time,
                           pc->unit_this_time_will_running_time, pc->unit_already_running_time, pc->unit_done);
            }
            else
            {
                DEBUG_TEST(DB_I, "%d区域 %d , 浸泡 %d  完成后区域累积运行 %d ", pc->unit_progress, pc->unit_running_channel, pc->unit_this_time_will_running_time, pc->unit_already_running_time);
            }
            pc = pc->next_running_unit;
        }
    }
}
