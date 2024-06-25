
/**
 * @file flash.h
 * @brief
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @note Read and write, write as a test entry, its test results: compare the read out values consistent with the write
 * @date 2023-07-10
 */
#ifndef __FLASH___H__
#define __FLASH___H__

#include "core/fset_lib.h"

#define INIT_NVS 1
#define YOUR_BUFFER_SIZE 101  // 缓冲区大小为 101，包括字符串结尾的空字符('\0')

/**
 * @brief Initialize the data store
 */
stat_m m_callable_flash_init(void);

/**
 * @brief Use the key to remove the stored value
 * @todo Unit Input ...
 *
 * @param in_key The key to remove
 * @return m_stat
 */
stat_m m_callable_drive_flash_remove(const char *in_key);
/**
 * @brief Add a new value to the store, corresponding to the key passed in
 * @todo Unit Input ...
 *
 * @param in_type The type of value added
 * @param in_key The key of value added
 * @param in_data  The alue of value added
 * @return m_stat
 */
stat_m m_callable_flash_write(enum  m_paramete_enum  in_type, const char *in_key, void *in_data);

/**
 * @brief The value is obtained in the storage from the existing key
 * @todo Unit Out ...
 *
 * @param in_type The type to read
 * @param in_key The key to read
 * @param out_resp The value returned
 * @return m_stat
 */
stat_m m_callable_flash_read(enum  m_paramete_enum  in_type, const char *in_key, void *out_resp);


/**
 * @brief 操作一个存储标记
 * 
 * @param in_flag 句柄
 * @param in_status 设置/获取/删除
 * @return stat_m 
 */
stat_m m_callable_flash_rescue_message_flag(const char *in_flag, enum m_stroage_rescue_message in_status);

/**
 * @brief Use the key to remove the all
 * @todo Unit Input ...
 *
 * @return m_stat
 */
stat_m m_callable_drive_flash_remove_all(void);
#endif 