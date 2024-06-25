#include "fset_lib.h"
/**
 * @file synchro.h
 * @brief 
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @note Read and write, write as a test entry, its test results: compare the read out values consistent with the write 
 * @date 2023-07-10
 */






/**
 * @brief Synchronize time
 * @todo Unit Input ...
 * 
 * @param device_time time
 * @param tz  tinezone
 * @return stat_m 
 */
stat_m m_callable_sync_time(uint64_t device_time , int tz);


/**
 * @brief Synchronize location information
 * @todo Unit Input ...
 * 
 * @param lon Longitude, 
 * @param lat latitude
 * @return stat_m 
 */
stat_m m_callable_sync_location(double lon,double lat );
