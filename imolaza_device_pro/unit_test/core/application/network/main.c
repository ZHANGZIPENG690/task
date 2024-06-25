






#include "main.h"



/** 需要是实现的外部函数 */

/**  获取ip地址的函数  */
stat_m m_ext_network_get_target_addr(char *out_addr , int *port)
{
    DEBUG_TEST( DB_I,"get addr" );
    return succ_r;
}
/**  mqtt连接的函数  */
stat_m m_ext_network_link_to(char *out_addr , int *port)
{
    DEBUG_TEST( DB_I,"link  to " );
    return succ_r;
}

/**  mqtt发送的函数  */
stat_m m_ext_network_link_transformission_tx(char *data , int data_len)
{
    DEBUG_TEST( DB_I,"%s" , data);
    return succ_r;
}
/**  mqtt接收的函数  */
stat_m m_ext_network_link_transformission_rx()
{
    return succ_r;
}

/**  网络层连接实现*/
stat_m m_ext_network_net_connect_to(char * username , char*token){return succ_r;}
/**  服务层连接实现*/
stat_m m_ext_network_server_connect_to(char * username , int port){return succ_r;}






void main()
{
    printf("Start .. \n\n");

    char buf[520] = {0};
    /** 初始化 */
    m_callable_network_init();
    /** 网络连接 */
    m_callable_network_connect( );
    /** 发送数据 */
    m_callable_network_data_tx(false , "你是sb"  , 5);

    while(1)
    {
        /** 循环接收数据*/
        m_callable_network_data_rx(buf , 520);
    }
    printf("Done .. \n\n");
}