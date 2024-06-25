


#ifndef __NETWORK_CONNECT_H__
#define  __NETWORK_CONNECT_H__
#include "core/fset_lib.h"
#include "../network.h"
#include "../manage/network_manage.h"


// stat_m m_static_network_connect_to(struct network_man *nt_man, int mode);




/**
 * @brief 服务连接，连接到...
 * 
 * @param nt_man 连接参数
 * @return stat_m 
 */
stat_m m_static_network_server_connect_to(struct network_man *nt_man);

/**
 * @brief 网络连接，连接到...
 * 
 * @param nt_man 连接参数
 * @return stat_m 
 */
stat_m m_static_network_net_connect_to(struct network_man *nt_man);

#endif /* __NETWORK_CONNECT_H__ */