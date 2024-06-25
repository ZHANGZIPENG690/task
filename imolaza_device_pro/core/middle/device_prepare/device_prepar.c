#include "device_prepar.h"

stat_m m_callable_device_device_factoryconfig(void)
{
    stat_m stat = fail_r;
    char str[100] = {};
    char str_cmp[100] = {};
    memset(str, 0, sizeof(str));
    scanf("%s", str);

    if (strstr(str, SERVERNUM_HEAD) != NULL && strstr(str, SERVERNUM_END) != NULL) // SN 格式
    {
        // M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(str, SERVERNUM_HEAD "%s" SERVERNUM_END, str_cmp);
        M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(str, SERVERNUM_HEAD "%[0-9A-Z]" , str_cmp);
        stat = m_callable_device_attribute_set_sn(str_cmp);
        if (stat == succ_r)
            printf("SN_SUCC : %s", str_cmp);
    }
    if (strstr(str, ENCRYPTED_KEY_HEAD) != NULL && strstr(str, ENCRYPTED_KEY_END) != NULL) // KEY
    {
        M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(str, ENCRYPTED_KEY_HEAD "%[0-9a-z]", str_cmp);
        stat = m_callable_device_attribute_set_key(str_cmp);
        if (stat == succ_r)
            printf("KEY_SUCC : %s", str_cmp);
    }
    return stat;
}
