




/**
 * @file production_conf.h
 * @brief
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @note Read and write, write as a test entry, its test results: compare the read out values consistent with the write
 * @date 2023-07-10
 */







/**
 * @brief 
 * 
 * @return stat_m 
 */
stat_m m_callable_production_conf_init(void);
/**
 * @brief 
 * 
 * @param key 
 * @return stat_m 
 */
stat_m m_callable_production_conf_write_key(char * key);
/**
 * @brief 
 * 
 * @param IMEI 
 * @return stat_m 
 */
stat_m m_callable_production_conf_write_imei_id(char * IMEI);

/**
 * @brief 
 * 
 * @param key 
 * @param IMEI 
 * @return stat_m 
 */
stat_m m_callable_production_conf_write_key_and_imei_id(char * key,char * IMEI);







