

/**
 * @file network_manage.h
 * @brief File containing example of doxygen usage for quick reference.
 *
 * Here typically goes a more extensive explanation of what the header
 * defines. Doxygens tags are words preceeded by either a backslash @\\
 * or by an at symbol @@.
 *
 * @author Mao .
 * @date 24 Oct 2023
 */

#ifndef NETWORK_MANAGE_H__
#define NETWORK_MANAGE_H__

#include "core/fset_lib.h"
#include "../network.h"
#include "../connect/network_connect.h"











/**
 * @brief 网络管理初始化
 * 
 * @return stat_m 
 */
stat_m m_static_network_manage_init(void);




/**
 * @brief 获取服务器的ip地址和端口   当处于非离线模式的时候
 * 
 * @return stat_m 
 */
stat_m m_static_network_get_server_target_addr(void);


/**
 * @brief 连接成功开始认证
 * 
 * @return stat_m 
 */
stat_m m_static_network_manage_login(void);




/**
 * @brief 离线模式的 在登陆之前试探性认证口令消息
 * 
 * @return stat_m 
 */
stat_m m_static_network_offline_test_login_msg(void);




/**
 * @brief 使用指定的连接方式连接诶服务器
 * 
 * @return stat_m 
 */
stat_m m_static_network_manage_connect(void);


/**
 * @brief 设置自动连接
 * 
 * @param auto_conn 
 * @return stat_m 
 */
stat_m m_static_network_set_auto_connect(bool auto_conn);


/**
 * @brief 获取当前的自动连接
 * 
 * @return stat_m 
 */
stat_m m_static_network_get_auto_connect(void );


/**
 * @brief 返回当前连接状态    和 状态处理回调函数 流程基本一致
 * 
 * @return enum connect_progress 
 */
enum connect_progress m_static_network_get_cnnect_state(void );

/**
 * @brief 设置连接模式
 * 
 * @param sm_mode  服务连接模式
 * @param nm_mode  网络连接模式
 * @return stat_m 
 */
// stat_m m_static_network_manage_set_connect_mode(enum connect_mode nm_mode , enum connect_mode sm_mode);
stat_m m_static_network_manage_set_connect_mode(int nm_mode , int sm_mode);
/**
 * @brief 主动断开网络连接
 *
 * @return stat_m
 */
 stat_m m_callable_network_disconnect(void);
/**
 * @brief 主动断开服务器连接
 *
 * @return stat_m
 */
 stat_m m_callable_network_server_disconnect(void);
/**
 * @brief 设置自动连接
 *
 * @param auto_conn
 * @return stat_m
 */
stat_m m_static_network_set_net_auto_connect(bool auto_conn);

/**
 * @brief 设置网络的配置
 *
 * @param user_name 用户名
 * @param user_token  认证
 * @return stat_m
 */
stat_m m_static_newwork_manage_set_net_conf(char *user_name, char *user_token)
;
/**
 * @brief 设置 服务器 连接状态
 *
 * @return stat_m
 */
stat_m m_static_network_manage_set_server_connect_status(int stus);
/**
 * @brief 获取服务器的连接状态
 * 
 * @param connect_progress 
 * @return stat_m 
 */
stat_m m_static_network_manage_get_server_connect_status(enum connect_progress *connect_progress);

/**
 * @brief 获取连接状态
 * 
 * @return stat_m 
 */
stat_m m_static_network_manage_get_net_connect_status(void);

/**
 * @brief 设置 网络 连接状态
 *
 * @return stat_m
 */
stat_m m_static_network_manage_set_connect_status(bool stus);

/**
 * @brief 设置 服务器登陆 状态
 *
 * @return stat_m
 */
stat_m m_static_network_manage_set_server_login_status(bool stus);
/**
 * @brief 获取 server login
 *
 * @return stat_m
 */
stat_m m_static_network_manage_get_server_login_status(void);


/**
 * @brief 获取连接次数
 *
 * @return stat_m
 */
stat_m m_callable_network_get_connect_count(int *count);


#endif