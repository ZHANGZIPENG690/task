
#ifndef __ERROR_HANDLE__H__
#define __ERROR_HANDLE__H__

#include "core/fset_lib.h"

#define M_ERROR_CODE_NETWORK_SERVER_CONNECT_BASE 0x0400
/** 整个网络连接过程中 */
#define M_ERROR_CODE_NETWORK__CONNECT_BASE 0x0600
enum error_net_code
{
    /** 主动断开连接服务器 */
    M_ERROR_CODE_NETWORK_DISCONNECT_CAUSE_VOLUNTEERED = M_ERROR_CODE_NETWORK_SERVER_CONNECT_BASE,
    /** 收到-1 表示被断开 */
    M_ERROR_CODE_NETWORK_DISCONNECT_CAUSE_RECEIVE_RETURN_1,
    /** 未收到PING回复 表示断开 */
    M_ERROR_CODE_NETWORK_DISCONNECT_CAUSE_NO_PING_RETURN,
    /** 获取Hostname错误 */
    M_ERROR_CODE_NETWORK__CAUSE_HOSTNAME_FAIL,
    /** 创建Http的Socket失败 */
    M_ERROR_CODE_NETWORK__CAUSE_HTTP_FAIL_CREATE_SOCKET_FAIL,
    /** 创建连接服务器的 Socket 失败 */
    M_ERROR_CODE_NETWORK__CAUSE_SERVER_FAIL_CREATE_SOCKET_FAIL,
};
/**
 * @brief 网络连接代码，表述流程
 *
 */
enum net_connect_code
{
    /** Init */
    M_NET_CONNECT_CODE_INIT = M_ERROR_CODE_NETWORK__CONNECT_BASE,
    /** 收到的配置数据无效 */
    M_CONNECT_ERROR_CODE_CONFIG_RECEIVE_DATA_INVALID,
    /** 配置数据正在连接 */
    M_CONNECT__CODE_CONFIG_CONFIG_TRYING_TO_NET,
    /** WiFi连接失败 - 密码错误 */
    M_CONNECT_ERROR_CODE_CONFIG_FAIL_PASSWORD_ERR,
    /** WiFi不存在 */
    M_CONNECT_ERROR_CODE_CONFIG_WIFI_NOT_FIND,
    /** WiFi连接失败 - 超时 */
    M_CONNECT_ERROR_CODE_CONFIG_FAIL_TIMEOUT,
    /** WiFi等待分配 IP */
    M_CONNECT__CODE__WIFI_IP_WAIT_ASSIGNMENT,  //*
    /** WiFi IP 分配失败 */
    M_CONNECT_ERROR_CODE__WIFI_IP_ASSIGNMENT_FAIL,  //*
    /** WiFi被断开 */
    M_CONNECT__CODE__WIFI_DISCONNECT,
    /** WiFi连接成功 */
    M_CONNECT__CODE__WIFI_SUCCESS,
    /** DNS解析中 */
    M_CONNECT__CODE_DNS_RESOLVEING,
    /** DNS解析失败 */
    M_CONNECT_ERROR_CODE_DNS_RESOLVE_FAIL,
    /** WIFI-无网络 */
    M_CONNECT_ERROR_CODE__WIFI_NOT_NAT,
    /** HTTP-连接失败 */
    M_CONNECT_ERROR_CODE_HTTP_CONNECT_FAIL,
    /** HTTP-Socket失败 */
    M_CONNECT_ERROR_CODE_HTTP_SOCKET_CREATE_FAIL,
    /** HTTP-连接成功 */
    M_CONNECT__CODE_HTTP_CONNECT_SUCCESS,
    /** HTTP-返回数据解析失败 */
    M_CONNECT_ERROR_CODE_HTTP_RESP_PARSE_FAIL,
    /** Socket 创建失败 */
    M_CONNECT_ERROR_CODE_SOCTET_CREATE_FAIL,
    /** Socket 连接失败 */
    M_CONNECT_ERROR_CODE_SOCTET_CONNECT_FAIL,
    /** Socket 连接成功 */
    M_CONNECT__CODE_SOCKET_CONNECT_SUCCESS,
    /** 服务器登陆 进行中 */
    M_CONNECT__CODE_SERVER_LOGIN_LOGGING,
    /** 服务器登陆 失败/超时 */
    M_CONNECT_ERROR_CODE_SERVER_LOGIN_TIMEOUT_FAIL,  //*
    /** 服务器登陆 超时 170S */
    M_CONNECT_ERROR_CODE_SERVER_LOGIN_TIMEOUT_170S_AND_MORE_FAIL,  //*
    /** 服务器登陆成功 */
    M_CONNECT__CODE_SERVER_LOGIN_SUCCESS,
    /** 服务器登陆成功，无其他数据返回 */
    M_CONNECT_ERROR_CODE_SERVER_LOGIN_NO_RESPONSE,
    /** 服务器登陆成功，连接正常 */
    M_CONNECT__CODE_SERVER_LOGIN_AND_CONNECT_OK,

};


/**
 * @brief 设置错误信息
 *
 * @param ecd
 * @return stat_m
 */
stat_m m_callable_middle_err_handle_set_code(enum error_net_code ecd);
/**
 * @brief 输出信息到服务器
 *
 * @return stat_m
 */
stat_m m_callable_middle_err_handle_printf(void);

/**
 * @brief 流程代码设置 包含配置网络过程和整个连接服务器过程
 *
 * @param hcd
 * @return stat_m
 */
stat_m m_callable_middle_connect_handle_set_code(enum net_connect_code hcd);
/**
 * @brief 获取当前连接流程
 * 
 * @param hcd 
 * @return stat_m 
 */
stat_m m_callable_middle_connect_handle_get_code(int *hcd);
#endif /* __ERROR_HANDLE__H__ */