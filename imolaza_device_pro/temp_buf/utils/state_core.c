

#include "core/fset_lib.h"
#include "core/utils/util.h"


typedef struct _err_state_handle
{
    int index; // index
    stat_m state; // state
} err_state_handle_t;


err_state_handle_t m_stat[300];



void put_in(uint16_t defin, stat_m stat)
{
	m_stat[defin].index = defin;
	m_stat[defin].state = stat;
}



stat_m info_check(uint8_t left, uint8_t right)
{
	stat_m state = M_ERR;

	for (int i = left; i <= right; i++)
	{
		if (m_stat[i].state != M_OKK)
		{
			printf("tool.c : %d  \n", i);
			return M_ERR;
		}
	}
	state = M_OKK;
	return state;
}



stat_m out_check(uint8_t vue)
{
	// printf("sta :%hhd\r\n" , vue);
	return m_stat[vue].state ;
}