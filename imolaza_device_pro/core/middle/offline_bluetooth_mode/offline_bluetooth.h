#if !defined(OFFLINE_BLUETOOTH_H)
#define OFFLINE_BLUETOOTH_H
#include "core/fset_lib.h"

typedef struct offline_bluetooth
{
    bool auth_flag;
};

/**
 * @brief 离线蓝牙模式 手机与设备 进行 登录认证
 * @param token 登录认证密钥
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_login_authentication(char *token);

/**
 * @brief 离线蓝牙模式 手机登录
 * @param tmp_sid 登录认证密钥
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_login(char *tmp_sid);
/**
 * @brief 离线蓝牙模式  设备向手机返回设备状态
 * @param void
 * @return device_status
 */
enum device_status m_callable_offline_bluetooth_get_device_status(void);

/**
 * @brief 离线蓝牙模式  蓝牙登录认证是否成功 标志位
 * @param void
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_login_flag(void);

/**
 * @brief 离线蓝牙模式 手机与设备 解除绑定
 * @param token 登录认证密钥
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_unbind(void);

/**
 * @brief 离线蓝牙模式 手机与设备 解除绑定 标志位设置
 * @return stat_m
 */
bool m_callable_offline_bluetooth_unbind_flag_get(void);


/**
 * @brief 离线蓝牙模式 手机与设备 绑定
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_bind_set(void);

/**
 * @brief   筛选出在离线蓝牙模式中，需要直接返回结果的指令
 * @param in_spec_cmd
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_instruction_filtering(uint64_t in_spec_cmd);


/**
 * @brief 在离线蓝牙模式下，判断是否处于登录状态
 * @param void
 * @return stat_m
 */
stat_m m_callable_offline_bluetooth_logged_or_not_logged(void);
#endif