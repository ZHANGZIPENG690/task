

#include "core/fset_lib.h"
#include "../drive.h"

stat_m m_ext_drive_sensor_init(int s1, int s2)
{
#if !UNIT_TEST
    GPIO_INIT(GPIO_MODE_INPUT, s1, 1, GPIO_INTR_ANYEDGE);
    GPIO_INIT(GPIO_MODE_INPUT, s2, 1, GPIO_INTR_ANYEDGE); // GPIO_PIN_INTR_NEGEDGE
#endif
    return succ_r;
}

