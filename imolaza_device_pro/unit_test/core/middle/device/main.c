

#include "main.h"











// extern stat_m m_static_dormant_timestamp_transform_mdh(uint64_t,uint64_t*);

extern stat_m m_callable_dormant_current_is_effective(uint64_t current_time);

extern stat_m m_callable_dormant_set(uint64_t start_time, uint64_t end_time, uint8_t mode);


void main()
{
    uint64_t in_ttime = 1722440599;
    uint64_t result = 0;
    // m_static_dormant_timestamp_transform_mdh(in_ttime, &result);
    m_callable_dormant_set( 1698162199 ,1698075799   , 2);
    if(m_callable_dormant_current_is_effective(in_ttime) == succ_r)
        printf("可以运行\n");
    else
        printf(" NONONO   运行\n");
}
