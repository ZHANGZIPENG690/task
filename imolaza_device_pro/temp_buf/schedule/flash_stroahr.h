
/**
 * @file flash_stroahr.h
 * @brief 
 * @mainpage 存储相关实现接口
 * @author 毛威鹏、陈前治
 * @version 1.0.1
 * @date 2023-07-10
 */



/**
 * @brief Initialize the data store
 * 
 */
stat_m m_ext_flash_ft_init(void);
/**
 * @brief Use the key to remove the stored value
 * 
 * @param in_key 
 * @return m_stat 
 */
stat_m m_ext_flash_remove(const char *in_key);
/**
 * @brief Add a new value to the store, corresponding to the key passed in
 * 
 * @param in_type The type of value added
 * @param in_key The key of value added
 * @param in_data  The alue of value added 
 * @return m_stat 
 */
stat_m m_ext_flash_write(int in_type, const char *in_key, void  *in_data);
/**
 * @brief The value is obtained in the storage from the existing key
 * 
 * @param in_type The type to read
 * @param in_key The key to read
 * @param out_resp The value returned
 * @return m_stat 
 */
stat_m m_ext_flash_read(int in_type, const char *in_key, void *out_resp);



/**
 * @brief Store/Delete/Get a short message for a given key value
 * 
 * @param in_flag Key 
 * @param in_status (Store/Delete/Get )
 * @return stat_m 
 */
stat_m m_callable_flash_flag_rescue_message(char *in_flag, int in_status);

/**
 * @brief Load the device configuration information from the memory
 * 
 */
void m_callable_flash_stroage_load_memory(void);
/**
 * @brief Store/Delete/Get a short message for a given key value
 * 
 * @param in_flag Key 
 * @param inout_vue vue 
 * @param in_status (Store/Delete/Get )
 * @return stat_m 
 */
stat_m m_callable_flash_flag_special_value_str(char *in_flag, char * inout_vue , int in_status);





