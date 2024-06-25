
#ifndef __NEW_LWAN_MODE_H__
#define __NEW_LWAN_MODE_H__

#include "core/fset_lib.h"
#include "core/schedule/mschedule.h"
#include "core/utils/util.h"


stat_m m_nllist_add_schedule_time(uint64_t in_id , uint32_t *in_time , int in_len);

/**
 * Remove the new lawn mode from a start schedule
*/
stat_m m_nllist_rm_schedule_time(uint64_t in_id );


uint64_t m_nllist_srech_pretime(uint64_t in_id , uint64_t pre_time);


/**
 * Find a recent time based on the current time ( Next Time )
*/
uint64_t m_nllist_srech_nexttime(uint64_t in_id , uint64_t pre_time);








#endif
