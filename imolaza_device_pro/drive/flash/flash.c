// #include "flash.h"
#include "core/fset_lib.h"
#if !UNIT_TEST

#include "nvs_flash.h"
#include "esp_err.h"
#include "nvs.h"

#define DATA_SIZE 32

const char M_BK_PAGE[10][8] = {"pag1", "P2", "P1", "P2", "P1", "P2", "P1", "P2", "P1", "P2"};

nvs_handle_t my_handle;
#define YOUR_BUFFER_SIZE 120

const char *m_flash_ft_use_equilibrium(void)
{
    return M_BK_PAGE[0];
}

stat_m m_ext_flash_init(void)
{
    stat_m stat = fail_r;
    esp_err_t err = nvs_flash_init();

    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        nvs_flash_erase();
        err = nvs_flash_init();
    }

    err = nvs_open(m_flash_ft_use_equilibrium(), NVS_READWRITE, &my_handle);

    if (err != ESP_OK)
    {
        // put_in(INIT_NVS, ERR);
        printf("Error (%s) opening NVS handle!\n", esp_err_to_name(err));
    }
    else
    {
        // put_in(INIT_NVS, OKK);
    }
    stat = err == ESP_OK ? (succ_r) : fail_r;
    return stat;
}

stat_m m_ext_flash_remove(const char *in_key)
{
    stat_m stat = fail_r;
    // printf("%s" , esp_err_to_name(nvs_erase_key(my_handle, in_key)));
    if (nvs_erase_key(my_handle, in_key) == ESP_OK)
    {
        if (nvs_commit(my_handle) == ESP_OK)
            stat = succ_r;
    }
    return stat;
}

stat_m m_ext_flash_write(enum m_paramete_enum in_type, const char *in_key, void *in_data)
{
    esp_err_t err = 1;
    uint8_t result_u8 = 0;
    // float result_floa = 0.0;
    // DEBUG_TEST(DB_W,"%s" , in_key);
    switch (in_type)
    {
    case M_TYPE_U8:
        result_u8 = *((uint8_t *)in_data);
        err = nvs_set_u8(my_handle, in_key, result_u8);
        break;
    case M_TYPE_U16:
        err = nvs_set_u16(my_handle, in_key, *((uint16_t *)in_data));
        break;
    case M_TYPE_I16:
        err = nvs_set_i16(my_handle, in_key, *((int16_t *)in_data));
        break;
    case M_TYPE_Int:
    case M_TYPE_U32:
        err = nvs_set_u32(my_handle, in_key, *((uint32_t *)in_data));
        break;
    case M_TYPE_U64:
        err = nvs_set_u64(my_handle, in_key, *((uint64_t *)in_data));
        break;
    case M_TYPE_Float:
        // result_floa = *((float *)in_data);
        // err = nvs_set_blob(my_handle, in_key, &result_floa, sizeof(float));
        break;
    case M_TYPE_Array:
        err = nvs_set_blob(my_handle, in_key, (uint16_t *)in_data, sizeof(uint16_t) * DATA_SIZE);
        break;
    case M_TYPE_Str:
        DEBUG_TEST(DB_I, "tips: save Str data: %s ", (char *)in_data);
        err = nvs_set_str(my_handle, in_key, in_data);
        break;

    default:
        DEBUG_TEST(DB_I, "Data type Err !");
        break;
    }
    err = nvs_commit(my_handle);
    if (err != ESP_OK)
    {
        DEBUG_TEST(DB_I, "key %s , ERR: %s", in_key, esp_err_to_name(err));
    }
    return err == ESP_OK ? succ_r : fail_r;
}
/**
 * @brief 单独读取一个字符串对象
 *
 * @param in_key 读取的key
 * @param out_resp 得到的值
 * @return stat_m
 */
stat_m m_ext_flash_read_str(const char *in_key, char *out_resp)
{
    size_t strLength = 0;
    esp_err_t err = nvs_get_str(my_handle, in_key, NULL, &strLength); // 先获取字符串长度
    if (err == ESP_OK)
    {
        {
            err = nvs_get_str(my_handle, in_key, (char *)out_resp, &strLength); // 强制转换为 char* 类型
        }
    }
    return succ_r;
}

stat_m m_ext_flash_read(enum m_paramete_enum in_type, const char *in_key, void *out_resp)
{
    esp_err_t err = 1;
    size_t required_size = 0;
    size_t strLength;
    uint8_t result_u8 = 0;
    uint64_t result_u64 = 0;
    size_t data_size = sizeof(uint16_t) * DATA_SIZE;

    switch (in_type)
    {
    case M_TYPE_U8:
        err = nvs_get_u8(my_handle, in_key, (uint8_t *)out_resp);
        break;
    case M_TYPE_U16:
        err = nvs_get_u16(my_handle, in_key, (uint16_t *)out_resp);
        break;
    case M_TYPE_I16:
        err = nvs_get_i16(my_handle, in_key, (int16_t *)out_resp);
        break;
    case M_TYPE_Int:
    case M_TYPE_U32:
        err = nvs_get_u32(my_handle, in_key, (uint32_t *)out_resp);
        break;
    case M_TYPE_U64:
        err = nvs_get_u64(my_handle, in_key, (uint64_t *)out_resp);
        break;
    case M_TYPE_Float:
        // err = nvs_get_blob(my_handle, in_key, (float *)out_resp, &size);

        break;
    case M_TYPE_Array:
        err = nvs_get_blob(my_handle, in_key, (uint16_t *)out_resp, &data_size);
        break;

    case M_TYPE_Str:
        err = nvs_get_str(my_handle, in_key, NULL, &strLength); // 先获取字符串长度
        if (err == ESP_OK)
        {
            // if (strLength > 0 && strLength <= YOUR_BUFFER_SIZE) // 根据你的缓冲区大小进行判断
            {
                err = nvs_get_str(my_handle, in_key, (char *)out_resp, &strLength); // 强制转换为 char* 类型
            }
            // printf("nvs Data -  -- -- - (%s) !", (char*)out_resp);
        }
        break;
    default:
        DEBUG_TEST(DB_I, "nvs Data type Err(%s) !", in_key);
        break;
    }

    // if (err != ESP_OK && strstr(in_key, "PA") == NULL && && strstr(in_key, "Zc") == NULL)
    //     // if (err != ESP_ERR_NVS_NOT_FOUND)
    // DEBUG_TEST(DB_W,"Get : key %s , ERR: %s", in_key, esp_err_to_name(err));
    return err == ESP_OK ? succ_r : fail_r;
}

stat_m m_ext_flash_remove_all(void)
{
    nvs_iterator_t it = nvs_entry_find("nvs", m_flash_ft_use_equilibrium(), NVS_TYPE_ANY);

    while (it != NULL)
    {
        nvs_entry_info_t info;
        nvs_entry_info(it, &info);
        it = nvs_entry_next(it);
        // if (strcmp(info.key, "time") == 0)
        if (strstr(info.key, "*") == NULL)
        {
            DEBUG_TEST(DB_W, "key '%s', type '%d'  spname ' %s'", info.key, info.type, info.namespace_name);
            nvs_erase_key(my_handle, info.key);
        }
        else
            DEBUG_TEST(DB_W, "key '%s', type '%d'  spname ' %s'", info.key, info.type, info.namespace_name);
    };
    nvs_release_iterator(it);
    return succ_r;
}

/**
 * @brief 恢复出厂
 *
 * @return stat_m
 */
stat_m m_ext_flash_factory_reset(void)
{
    nvs_iterator_t it = nvs_entry_find("nvs", m_flash_ft_use_equilibrium(), NVS_TYPE_ANY);

    while (it != NULL)
    {
        nvs_entry_info_t info;
        nvs_entry_info(it, &info);
        it = nvs_entry_next(it);
        // if (strcmp(info.key, "time") == 0)
        if (strstr(info.key, "*") == NULL && strstr(info.key, "time") == NULL && strstr(info.key, "wifi") == NULL)
        {
            DEBUG_TEST(DB_W, "key '%s', type '%d'  spname ' %s'", info.key, info.type, info.namespace_name);
            nvs_erase_key(my_handle, info.key);
        }
        else
            DEBUG_TEST(DB_I, "key '%s', type '%d'  spname ' %s'", info.key, info.type, info.namespace_name);
    };
    nvs_release_iterator(it);
    return succ_r;
    // 5203,2,180&60,1701216000,4101494399,B0C0D0E0F0G0H0I0J0K0L0M0N0O0P0Q0R0S0T0U0V0W0X0,1#180;4#180;5#240;16#240;18#180;24#240;26#120;28#180;29#180,120,null,1]
}
#endif