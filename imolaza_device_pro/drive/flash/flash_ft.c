#include "core/fset_lib.h"
#if UNIT_TEST


// #include "flash.h"
// #include "nvs_flash.h"
// #include "esp_err.h"
// #include "nvs.h"

/**  None */
static FILE *fpWrite;

const char M_BK_PAGE[10][8] = {"pag1", "P2", "P1", "P2", "P1", "P2", "P1", "P2", "P1", "P2"};
// nvs_handle_t my_handle;
const char *m_flash_ft_use_equilibrium(void)
{
    return M_BK_PAGE[0];
}

stat_m m_ext_flash_init(void)
{
    stat_m stat = fail_r;

    return stat;
}
stat_m m_ext_flash_remove_all()
{
    fpWrite = fopen("../out/debug.txt", "w");
    fprintf(fpWrite, " ");
    fclose(fpWrite);
}

char Array[200][300] = {0};
stat_m m_ext_flash_remove(const char *in_key)
{
    stat_m stat = fail_r;
    int i = 0;
    i = 0;
    while (strlen(Array[i]) > 1 || strlen(Array[i + 1]) > 1)
    {
        if (strstr(Array[i], in_key) != NULL)
        {
            stat = succ_r;
            memset(Array[i], 0, sizeof(Array[i]));
        }
        i++;
    }
    if (stat == succ_r)
    {
        i = 0;
        fpWrite = fopen("../out/debug.txt", "w");

        while (strlen(Array[i]) > 1 || strlen(Array[i + 1]) > 1)
        {
            // printf("xie : %s" , Array[i]);
            if (strlen(Array[i]) > 1)
                fprintf(fpWrite, "%s\n", Array[i]);
            fclose(fpWrite);
            fpWrite = fopen("../out/debug.txt", "a+");
            i++;
        }
    }
    return stat;
}

stat_m m_ext_flash_write(enum m_paramete_enum in_type, const char *in_key, void *in_data)
{
    stat_m stat = fail_r;
    char gun[300];
    char gun_buf[300];
    int i = 0, resp = 0;
    fpWrite = fopen("../out/debug.txt", "a+");
    switch (in_type)
    {
    case M_TYPE_U8:
        sprintf(gun, "%s:%hhd\n", in_key, *((uint8_t *)in_data));
        break;
    case M_TYPE_U16:
        sprintf(gun, "%s:%hd\n", in_key, *((uint16_t *)in_data));
        break;
    case M_TYPE_Int:
        sprintf(gun, "%s:%d\n", in_key, *((int *)in_data));
        break;
    case M_TYPE_U32:
        sprintf(gun, "%s:%d\n", in_key, *((uint32_t *)in_data));
        break;
    case M_TYPE_U64:
        sprintf(gun, "%s:%lld\n", in_key, *((uint64_t *)in_data));
        break;
    case M_TYPE_Str:
        sprintf(gun, "%s:%s\n", in_key, in_data);
        break;
    default:
        printf("Data type Err !");
        break;
    }
    DEBUG_TEST(DB_W,"%s", gun);
    while (fscanf(fpWrite, "%[^\n] ", gun_buf) != EOF)
    {
        // printf("--> %s\n" , gun_buf  );
        if (strstr(gun_buf, in_key) != NULL)
        {
            stat = succ_r;
            strcpy(Array[i], gun);
        }
        else
            strcpy(Array[i], gun_buf);

        i++;
    }
    if (stat != succ_r)
    {
        strcpy(Array[i], gun);
    }
    fclose(fpWrite);
    fpWrite = fopen("../out/debug.txt", "w");
    if (i == 0)
        fprintf(fpWrite, "%s", gun);
    else
    {
        i = 0;
        while (strlen(Array[i]) > 1 || strlen(Array[i + 1]) > 1)
        {
            // printf("xie : %s" , Array[i]);
            fprintf(fpWrite, "%s\n", Array[i]);
            fclose(fpWrite);
            fpWrite = fopen("../out/debug.txt", "a+");
            i++;
        }
    }

    fclose(fpWrite);
    return 0;
}

stat_m m_ext_flash_read(enum m_paramete_enum in_type, const char *in_key, void *out_resp)
{
    stat_m stat = fail_r;
    fpWrite = fopen("../out/debug.txt", "a+");
    char gun[300];
    // printf("h\n");
    while (fscanf(fpWrite, "%[^\n] ", gun) != EOF && stat == fail_r)
    {
        if (strstr(gun, in_key) != NULL)
        {
            stat = succ_r;
            switch (in_type)
            {
            case M_TYPE_U8:
                // fscanf(fpWrite, "%s:%hhd", in_key, ((uint8_t*)out_resp));
                M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(gun, "%*[^:]:%hd", ((uint8_t *)out_resp));
                break;
            case M_TYPE_U16:
                // fscanf(fpWrite, "%s:%hd", in_key, *((uint16_t*)out_resp));
                M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(gun, "%*[^:]:%hd", ((uint16_t *)out_resp));
                break;
            case M_TYPE_Int:
                // fscanf(fpWrite, "%s:%d", in_key, *((int*)out_resp));
                M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(gun, "%*[^:]:%d", ((int *)out_resp));
                break;
            case M_TYPE_U32:
                // fscanf(fpWrite, "%s:%d", in_key, *((uint32_t*)out_resp));
                M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(gun, "%*[^:]:%d", ((uint32_t *)out_resp));
                break;
            case M_TYPE_U64:
                M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(gun, "%*[^:]:%lld", ((uint64_t *)out_resp));
                // fscanf(fpWrite, "%s:%lld", in_key, *((uint64_t*)out_resp));
                break;
            case M_TYPE_Str:
                // fscanf(fpWrite, "%s:%s", in_key, out_resp);
                M_CALLABLE_DATA_PARSE_FORAMT_PARAM_SSCANF(gun, "%*[A-Z0-9]:%[^]]", out_resp);
                break;

            default:
                printf("Data type Err !");
                break;
            }
            // printf("> %s\n", gun);
        }
    }
    fclose(fpWrite);
    // printf("l\n");
    return stat;
}


#endif