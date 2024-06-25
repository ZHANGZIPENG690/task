
/**
 * @file wifi.h
 * @brief 
 * @public (Drive)stroage interface
 * @author Mao .
 * @version 1.0.1
 * @note Read and write, write as a test entry, its test results: compare the read out values consistent with the write 
 * @date 2023-07-10
 */


struct m_wifi_status
{
    /** is connected */
    bool wifi_is_connected;
    /** is need auto connect */
    bool wifi_need_auto_connect;
    /** disconnect reason */
    uint8_t dis_con_reason;
};

typedef struct wific
{
    /**< SSID of ESP32 soft-AP. If ssid_len field is 0, this must be a Null terminated string. Otherwise, length is set according to ssid_len. */
    uint8_t ssid[32]; 
    /**< Password of ESP32 soft-AP. */
    uint8_t psw[64];  
    /**< Optional length of SSID field. */
    uint8_t ssid_len; 
    /**< Optional length of PSW field. */
    uint8_t psw_len;  
    /**< Auth mode of ESP32 soft-AP. Do not support AUTH_WEP in soft-AP mode */
    int authmode;     
    
    /* wifi connrction status */
    struct m_wifi_status wifi_status; 

    // struct m_static_wifi_ip_info static_ip_dec; /*  status ip use ? */
};










/**
 * @brief wifi core init 
 *
 * @return stat_m
 */
stat_m m_ext_wifi_core_init(void);

/**
 * @brief 
 * 
 * @param in_ssid 
 * @return stat_m 
 */
stat_m m_ext_wifi_sta_scan_spec(char *in_ssid);

/**
 * @brief wifi ap initial
 *
 * @param m_wifi_info
 * @return stat_m
 */
stat_m m_ext_wifi_ap_init(wific *m_wifi_info);

/**
 * @brief disconnect wifi 
 * 
 * @return stat_m 
 */
stat_m m_ext_wifi_disconnect();

/**
 * @brief wifi connect test 
 * 
 * @param in_wifi 
 * @return stat_m 
 */
stat_m m_callable_wifi_connect_test(wific *in_wifi);

/**
 * @brief wifi ap and sta mode initial
 * 
 * @return stat_m 
 */
stat_m m_ext_wifi_apsta_init(void) ;
/**
 * @brief wifi sta init
 * 
 * @param in_wifi 
 * @return stat_m 
 */
stat_m m_ext_wifi_sta_init(wific *in_wifi) ;

/**
 * @brief wifi connect to 
 * 
 * @param in_wifi 
 * @return stat_m 
 */
stat_m m_ext_wifi_connect_to(wific *in_wifi);

