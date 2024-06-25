
/**
 * @file pump.h
 * @brief
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @note Read and write, write as a test entry, its test results: compare the read out values consistent with the write
 * @date 2023-07-10
 */

/** */
enum pump_type
{
    /** */
    M_PUMP_TYPE_WELL_PUMP,
    /** */
    M_PUMP_TYPE_OTHER,
    /** */
    M_PUMP_TYPE_CLOSE,
};
/**
 * @brief initialize
 * 
 * @return stat_m 
 */
stat_m m_ext_drive_pump_init(void);

/**
 * @brief Configure the type of pump
 *
 * @param type
 * @return stat_m
 */
stat_m m_ext_drive_pump_config_type(enum pump_type type);

/**
 * @brief Turn on the pump
 *
 * @return stat_m
 */
stat_m m_ext_drive_pump_control_open(void);
/**
 * @brief Turn off the pump
 *
 * @return stat_m
 */
stat_m m_ext_drive_pump_control_close(void);
