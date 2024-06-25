








#ifndef  __CONFIG_NET__H__
#define  __CONFIG_NET__H__


#include "core/fset_lib.h"




/**
 * @brief 配网的错误摩
 * 
 */
enum config_net_resp
{
    /** 未找到到目标wifi */
    M_CONFIG_MODE_RESP_NOT_FOUND_TARGET_WIFI,
    /** 连接超时 */
    M_CONFIG_MODE_RESP_CONNECT_TIME_OUT,
    /** 密码错误 */
    M_CONFIG_MODE_RESP_PASSWORD_ERR,
    /** 未知错误 */
    M_CONFIG_MODE_RESP_UNKNOW,
};

/**
 * @brief 配置设备的入网 username
 * 
 * @param in_psw 
 * @param len 
 * @return stat_m 
 */

stat_m m_callable_config_net_set_username(char * in_username);

/**
 * @brief 配置设备的token（密码）
 * 
 * @param in_psw 密码
 * @param len 长度
 * @return stat_m 
 */

stat_m m_callable_config_net_set_token(char * in_token);

/**
 * @brief  开始配网
 * 
 * @return stat_m 
 */
stat_m m_callable_config_net_start(void);






/**
 * @brief 配置网络成功或者失败的反馈
 * 
 * @param stat 
 * @return stat_m 
 */
stat_m m_callable_config_net_resp(stat_m stat);


/**
 * @brief 配网断开连接
 * 
 * @return stat_m 
 */
stat_m m_callable_config_net_recode(void);

/**
 * @brief 保存
 * 
 * @return stat_m 
 */
stat_m m_callable_config_net_save(void);




/**
 * @brief 吧指定字符替换
 * 
 * @param strc 
 * @return stat_m 
 */
stat_m m_callable_str_transf_str_resp(char *strc);
#endif /* _CONFIG_NET__H__ */




