#include "fset_lib.h"
/**
 * @file self_test.h
 * @brief
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @note Read and write, write as a test entry, its test results: compare the read out values consistent with the write
 * @date 2023-07-10
 */

/** */
enum test_mode
{
    /** */
    M_TEST_MODE_CHANNEL_ONLY,
    /** */
    M_TEST_MODE_LIGHT_ONLY,
    /** */
    M_TEST_MODE_ALL,
};
/** */
typedef struct self_test_t
{
    /** */
    enum test_mode test_mode;
    /** */
    uint8_t device_type;
    /** */
    uint32_t channel_running_s;
};

// stat_m m_callable_self_test_conf();

/**
 * @brief Start a self-test, test according to the parameters passed in, do not start the test in any run ,The test results are called in m_ext_self_test_result_cb()
 * @todo Unit Input ...
 *
 * @param st_conf The test parameters, both partial and all, require configuration of the type of equipment and the duration of operation of each zone and the duration of the light
 * @return stat_m
 */
stat_m m_callable_self_test_start(self_test_t *st_conf);

/** */
enum test_rssult
{
    /** */
    M_TEST_RESULT_NOT_FOUND_CHIP_HC595,
    /** */
    M_TEST_RESULT_NOT_FOUND_CHIP_,
    /** */
    M_TEST_RESULT_SUCC,
};


/**
 * @brief Test results callback
 * @todo Unit Out ...
 * 
 * @param result Test results
 * @return stat_m
 */
stat_m m_ext_self_test_result_cb(enum test_rssult result);
