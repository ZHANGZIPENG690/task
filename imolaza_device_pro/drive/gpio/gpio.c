#include "../drive.h"

#if UNIT_TEST

int vop = 0;
void m_set_level(int you)
{
	vop = you;
}


#endif

int m_ext_drive_gpio_get_level(int pin)
{
#if !UNIT_TEST
	return gpio_get_level(pin);
#else
	return vop;
#endif
}