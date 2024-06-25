

#include "encrypt.h"

// #define DELTA 0x9e3779b9
// #define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (key[(p&3)^e] ^ z)))

#define MX (((z >> 5) ^ (y << 2)) + ((y >> 3) ^ (z << 4))) ^ ((sum ^ y) + (key[(p & 3) ^ e] ^ z))
#define DELTA 0x9e3779b9

#define FIXED_KEY                                     \
    size_t i;                                         \
    uint8_t fixed_key[16];                            \
    memcpy(fixed_key, key, 16);                       \
    for (i = 0; (i < 16) && (fixed_key[i] != 0); ++i) \
        ;                                             \
    for (++i; i < 16; ++i)                            \
        fixed_key[i] = 0;

static uint32_t *xxtea_to_uint_array(const uint8_t *data, size_t len, int inc_len, size_t *out_len)
{
    uint32_t *out;
#if !(defined(BYTE_ORDER) && (BYTE_ORDER == LITTLE_ENDIAN))
    size_t i;
#endif
    size_t n;

    n = (((len & 3) == 0) ? (len >> 2) : ((len >> 2) + 1));

    if (inc_len)
    {
        out = (uint32_t *)calloc(n + 1, sizeof(uint32_t));
        if (!out)
            return NULL;
        out[n] = (uint32_t)len;
        *out_len = n + 1;
    }
    else
    {
        out = (uint32_t *)calloc(n, sizeof(uint32_t));
        if (!out)
            return NULL;
        *out_len = n;
    }
#if defined(BYTE_ORDER) && (BYTE_ORDER == LITTLE_ENDIAN)
    memcpy(out, data, len);
#else
    for (i = 0; i < len; ++i)
    {
        out[i >> 2] |= (uint32_t)data[i] << ((i & 3) << 3);
    }
#endif

    return out;
}

static uint8_t *xxtea_to_ubyte_array(const uint32_t *data, size_t len, int inc_len, size_t *out_len)
{
    uint8_t *out;
#if !(defined(BYTE_ORDER) && (BYTE_ORDER == LITTLE_ENDIAN))
    size_t i;
#endif
    size_t m, n;

    n = len << 2;

    if (inc_len)
    {
        m = data[len - 1];
        n -= 4;
        if ((m < n - 3) || (m > n))
            return NULL;
        n = m;
    }

    out = (uint8_t *)malloc(n + 1);

#if defined(BYTE_ORDER) && (BYTE_ORDER == LITTLE_ENDIAN)
    memcpy(out, data, n);
#else
    for (i = 0; i < n; ++i)
    {
        out[i] = (uint8_t)(data[i >> 2] >> ((i & 3) << 3));
    }
#endif

    out[n] = '\0';
    *out_len = n;

    return out;
}

static uint32_t *xxtea_uint_encrypt(uint32_t *data, size_t len, uint32_t *key)
{
    uint32_t n = (uint32_t)len - 1;
    uint32_t z = data[n], y, p, q = 6 + 52 / (n + 1), sum = 0, e;

    if (n < 1)
        return data;

    while (0 < q--)
    {
        sum += DELTA;
        e = sum >> 2 & 3;

        for (p = 0; p < n; p++)
        {
            y = data[p + 1];
            z = data[p] += MX;
        }

        y = data[0];
        z = data[n] += MX;
    }

    return data;
}

static uint32_t *xxtea_uint_decrypt(uint32_t *data, size_t len, uint32_t *key)
{
    uint32_t n = (uint32_t)len - 1;
    uint32_t z, y = data[0], p, q = 6 + 52 / (n + 1), sum = q * DELTA, e;

    if (n < 1)
        return data;

    while (sum != 0)
    {
        e = sum >> 2 & 3;

        for (p = n; p > 0; p--)
        {
            z = data[p - 1];
            y = data[p] -= MX;
        }

        z = data[n];
        y = data[0] -= MX;
        sum -= DELTA;
    }

    return data;
}

static uint8_t *xxtea_ubyte_encrypt(const uint8_t *data, size_t len, const uint8_t *key, size_t *out_len)
{
    uint8_t *out;
    uint32_t *data_array, *key_array;
    size_t data_len, key_len;

    if (!len)
        return NULL;

    data_array = xxtea_to_uint_array(data, len, 1, &data_len);
    if (!data_array)
        return NULL;

    key_array = xxtea_to_uint_array(key, 16, 0, &key_len);
    if (!key_array)
    {
        free(data_array);
        return NULL;
    }

    out = xxtea_to_ubyte_array(xxtea_uint_encrypt(data_array, data_len, key_array), data_len, 0, out_len);

    free(data_array);
    free(key_array);

    return out;
}

static uint8_t *xxtea_ubyte_decrypt(const uint8_t *data, size_t len, const uint8_t *key, size_t *out_len)
{
    uint8_t *out;
    uint32_t *data_array, *key_array;
    size_t data_len, key_len;

    if (!len)
        return NULL;
    data_array = xxtea_to_uint_array(data, len, 0, &data_len);
    if (!data_array)
        return NULL;

    key_array = xxtea_to_uint_array(key, 16, 0, &key_len);
    if (!key_array)
    {
        free(data_array);
        return NULL;
    }

    out = xxtea_to_ubyte_array(xxtea_uint_decrypt(data_array, data_len, key_array), data_len, 1, out_len);

    free(data_array);
    free(key_array);

    return out;
}

// public functions

void *xxtea_encrypt(const void *data, size_t len, const void *key, size_t *out_len)
{
    FIXED_KEY
    return xxtea_ubyte_encrypt((const uint8_t *)data, len, fixed_key, out_len);
}

void *xxtea_decrypt(const void *data, size_t len, const void *key, size_t *out_len)
{
    FIXED_KEY
    return xxtea_ubyte_decrypt((const uint8_t *)data, len, fixed_key, out_len);
}

const char *key = "r8,:c^)DWp5f?eh";
/**
 * @brief TEA-加密数据，输入代价数据，输出base64
 *
 * @param in_enda 输入数据
 * @param in_len 数据长度
 * @param out_dat 数据数据 bate64
 * @return stat_m
 */
stat_m m_callable_crypt_xxtea_encrypt(char *in_enda, int *in_len, char *out_dat)
{
    stat_m stat = fail_r;
    size_t len = 0;

    // uint64_t start = mDeviceSystemTime();
    unsigned char *encrypt_data = xxtea_encrypt(in_enda, *in_len, key, &len);
    char *base64_data = base64_encode(encrypt_data, len);
    // uint64_t end = mDeviceSystemTime();
    // printf("\n length: %d  startTime:%lldms  endTime: %lldms  (cost:%lld)\n", strlen(a[i]), start, end, end - start);
    // printf("Out: %s  (%d) \n", base64_data, strlen(base64_data));
    strcpy(out_dat, (char *)base64_data);
    *in_len = strlen((char*)base64_data);
    free(base64_data);
    free(encrypt_data);
    { // char src[300] = "[82,-118.4036052,33.942153,-7,1714093303]#@#ping5#@#[24,8655,1,420,420,1714089540,107020703070407050706070707080709070A070B070C060D060E060F060G060H060I060*null,0,0,1714082400]";
        // printf("no src : %s len :%d\n" , src ,strlen(src));
        // unsigned char *encrypt_data = xxtea_encrypt(src, strlen(src), key, &len);
        // // printf("tea after : %s len :%d\n" , encrypt_data , strlen((char*)encrypt_data));
        // char *base64_data = base64_encode(encrypt_data, len);
        // printf("base64 after : %s len :%d\n" , base64_data , strlen((char*)base64_data));

        // printf("-------------------- Decrypt -----------------\n");
        // printf("no-de src : %s len :%d\n" , base64_data ,strlen((char*)base64_data));
        // unsigned char * buf = base64_decode((const char *)base64_data ,&len );
        // // printf("de base64 after : %s len :%d\n" , buf , strlen((char*)buf));
        // unsigned char *decrypt_data = xxtea_decrypt((void *)buf, len, key, &len);
        // printf("de tea after : %s len :%d\n" , decrypt_data , strlen((char*)decrypt_data));
    }

    // m_callable_crypt_xxtea_decrypt((char *)encrypt_data, len, NULL);
    return stat;
}

/**
 * @brief TEA-解密数据，输入加密后的数据，输出字符
 *
 * @param in_deda 输入待解密数据
 * @param in_len 数据长度
 * @param out_dat 输出数据
 * @return stat_m
 */
stat_m m_callable_crypt_xxtea_decrypt(char *in_deda, int *in_len, char *out_dat)
{
    stat_m stat = fail_r;
    size_t len = 0;
    unsigned char * buf = base64_decode((const char *)in_deda ,&len );
    unsigned char *decrypt_data = xxtea_decrypt((void *)buf, len, key, &len);
    // if (decrypt_data != NULL)
    //     printf("%s\n", decrypt_data);
    strcpy(out_dat, (char *)decrypt_data);
    *in_len = len;
    free(decrypt_data);
    free(buf);
    return stat;
}

//   const char *text = "Hello World! 你好，中国！";
//     const char *key = "1234567890";
//     size_t len;
// char *decrypt_data = xxtea_decrypt(encrypt_data, len, key, &len);
// if (strncmp(text, decrypt_data, len) == 0) {
//     printf("success!\n");
// }
// else {
//     printf("fail!\n");
// }
// free(encrypt_data);
// free(decrypt_data);
// free(base64_data);
