

#ifndef __NETWORK__H__
#define __NETWORK__H__

// #include "./manage/network_manage.h"
#include "core/fset_lib.h"
#include "../../middle/smart_connect/smart_connect.h"

/** 网络事件 触发原因 */
enum net_cause
{
    /**  本地自动操作 ， 导致的结果 */
    M_NETWORK_HAPPEN_CAUSE_LOCAL_AUTO_ACTION,
    /**  本地主动操作 ， 导致的结果 */
    M_NETWORK_HAPPEN_CAUSE_LOCAL_PROACTIVE_ACTION,

    /**  远端主动 ， 导致的结果 */
    M_NETWORK_HAPPEN_CAUSE_REMOTE_PROACTIVE_ACTION,
    /**  远端无响应 ， 导致的结果 */
    M_NETWORK_HAPPEN_CAUSE_REMOTE_UNRESPONSIVE,
};

struct public_connect_conf
{
    // 断开原因
    enum net_cause cause;
    // 连接次数
    int connent_count;
    // 网络错误次数
    int net_err_count;
    // 自动连接
    bool auto_connect;
    // 活跃
    enum connect_progress active;
};

/** 服务连接参数 */
struct server_connect_potocol_conf
{
    // 连接地址
    char sm_addr[30];
    // 连接端口
    int sm_port;
    // 发布订阅主题
    char sm_topic[30];
    /** 状态信息*/
    struct public_connect_conf state;
    /** 服务连接 方式*/
    enum connect_mode sm_mode;
};
/** 网络连接参数 */
struct net_connect_potocol_conf
{
    // 连接name
    char nm_name[30];
    // 连接密钥
    char nm_token[30];
    // 连接uid
    int nm_uuid;
    /** 状态信息*/
    struct public_connect_conf state;
    /** 网络连接 方式*/
    enum connect_mode nm_mode;
};

/** 网络连接管理  */
/**
 *  一个正常的运行应该包含
 *      一个网络连接  （wifi 蓝牙）
 *      一个服务连接   （TCP  MQTT  蓝牙）
 *  任意组合
 */
struct network_man
{
    // 网络连接参数
    struct net_connect_potocol_conf net_con_conf;
    // 服务连接参数
    struct server_connect_potocol_conf server_con_conf;
};
//
#define M_EVNBIT_RXHANDLE 0x05
#define M_EVNBIT_TXHANDLE 0x06
// 等待服务器初始化完成之后   准备开始连接服务器
#define M_EVNBIT_SERVER_INIT 0x04
#define M_EVNBIT_CONNRCT_SERVER 0x03



/**
 * @brief 初始化加载一些配置
 *
 * @return stat_m
 */
stat_m m_callable_network_init(void);

/**
 * @brief 获取当前 wifi 连接的状态
 *
 * @return stat_m
 */
stat_m m_callable_network_get_net_connect_status(void);

/**
 * @brief 设置连接方式
 *
 * @param nm_mode   网络连接方式
 * @param sm_mode   服务器连接方式
 * @return stat_m
 */
stat_m m_callable_network_set_connect_mode(enum connect_mode nm_mode, enum connect_mode sm_mode);

/**
 * @brief 直接连接网络
 *
 * @param sm_mode 网络连接的模式     -1 无效
 * @param nm_mode 服务连接模式      -1 无效
 * @return stat_m
 */
stat_m m_callable_network_connect(void);
/**
 * @brief 设置开启或者关闭自动连接
 *
 * @return stat_m
 */
stat_m m_callable_network_set_net_auto_connect(bool);
/**
 * @brief 设置开启或者关闭自动连接
 *
 * @return stat_m
 */
stat_m m_callable_network_set_server_auto_connect(bool);
/**
 * @brief 保持连接活跃
 *
 * @return stat_m
 */
stat_m m_callable_network_keep_active(void);
// /**
//  * @brief 发送数据
//  *
//  * @param data
//  * @param datalen
//  * @return stat_m
//  */
// stat_m m_callable_network_data_tx(bool is_queue, char *data, int datalen);
/**
 * @brief 接收数据
 *
 * @param data
 * @param datalen
 * @return stat_m
 */
stat_m m_callable_network_data_rx(char *data, int datalen);

/**
 * @brief 注册连接过程函数
 *
 * @param func 需要注册的回调函数
 * @param progress 连接状态
 * @param cause  原因
 * @return stat_m
 */
stat_m m_callable_network_registered_progress_callback(void (*func)(enum connect_progress progress, enum net_cause cause));
/**
 * @brief 一次装载多个参数 到发送队列 1 - 5
 * 
 * @param is_queue 
 * @param parr1 
 * @param args1 
 * @param parr2 
 * @param args2 
 * @param parr3 
 * @param args3 
 * @param parr4 
 * @param args4 
 * @param parr5 
 * @param args5 
 * @return stat_m 
 */
stat_m m_callable_network_data_tx_between_1_5(bool is_queue,
                                              enum m_paramete_enum parr1, void *args1,
                                              enum m_paramete_enum parr2, void *args2,
                                              enum m_paramete_enum parr3, void *args3,
                                              enum m_paramete_enum parr4, void *args4,
                                              enum m_paramete_enum parr5, void *args5);



stat_m m_static_online_config_mode_event_handle(enum running_mode in_mode, enum event_ft evn_id, char *in_data, int in_len);
/**
 * @brief 设置网络信息
 * 
 * @param user_name  用户认证名
 * @param user_token 用户token
 * @return stat_m 
 */
stat_m m_callable_network_set_net_inf0(char *user_name, char *user_token);
/**
 * @brief 设置开始连接服务器
 *
 * @param stus
 * @return stat_m
 */
stat_m m_callable_network_set_server_start_connect_server(void);


/**
 * @brief 复位开始连接服务器  用于离线模式，不然线程会循环执行
 *
 * @param stus
 * @return stat_m
 */
stat_m m_callable_network_reset_server_start_connect_server(void);

/**
 * @brief
 *
 * @param data
 * @param datalen
 * @return stat_m
 */
stat_m m_callable_network_specify_mode_data_tx(enum connect_mode mode, bool is_queue, char *data, int datalen);
/**
 * @brief 修改爲在線改變wifi的狀態
 * 
 * @return stat_m 
 */
stat_m m_callable_network_set_status_change_net(void);

/**
 * @brief 网络自动连接
 * 
 * @return stat_m 
 */
stat_m m_static_network_net_connect(void);


/**
 * @brief 调用收取数据
 *
 * @param smode
 * @param data
 * @param datalen
 * @param in_time_out
 * @return stat_m
 */
stat_m m_callable_network_ext_tcp_mqtt_offline_data_rx(enum connect_mode smode, char *data, int *datalen, uint64_t in_time_out);



/**   完成单元需要实现的函数  */

stat_m m_ext_network_get_target_addr(char *out_addr, int *port);
/**  mqtt连接的函数  */
stat_m m_ext_network_link_to(char *out_addr, int port);
/**  mqtt发送的函数  */
stat_m m_ext_network_link_transformission_tx(char *data, int data_len);
/**  mqtt接收的函数  */
stat_m m_ext_network_link_transformission_rx();
/**  网络层连接实现*/
stat_m m_ext_network_net_connect_to(char *username, char *token);
/**  服务层连接实现*/
stat_m m_ext_network_server_connect_to(char *username, int port);
/**  */

#endif