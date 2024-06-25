#include "option.h"

/**
 * 
 *  文件说明   ： 在这文件里面所有的操作都是基于结构体的，全局结构体表示了所有的功能可选项，相当于功能控制中心
 * 
 * 
*/

#define MSYSTEM_TEXT_INDEX 0
#define MSYSTEM_TEST_HANDLE "hei"

struct __CONFIG_C 
{
    int index;
    char handle[8];

};

/**   所有需要添加爱的数据直接加入 即可*/
struct __CONFIG_C SYSTEM_CONFIG_LIST [50]
=
{
    {MSYSTEM_TEXT_INDEX , MSYSTEM_TEST_HANDLE },
    {MSYSTEM_TEXT_INDEX , MSYSTEM_TEST_HANDLE },
    {MSYSTEM_TEXT_INDEX , MSYSTEM_TEST_HANDLE },
};





stat_m m_option_load_all(void)
{

}



stat_m m_option_configure_option()
{

}


























