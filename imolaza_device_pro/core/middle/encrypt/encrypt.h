















#ifndef __ENCRYPT__H__
#define __ENCRYPT__H__

#include "core/fset_lib.h"
#include <stdio.h>  
#include <stdint.h>  



/**
 * Function: base64_encode
 * @data:    Data to be encoded
 * @len:     Length of the data to be encoded
 * Returns:  Encoded data or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer.
 */
char * base64_encode(const unsigned char * data, size_t len);

/**
 * Function: base64_decode
 * @data:    Data to be decoded
 * @out_len: Pointer to output length variable
 * Returns:  Decoded data or %NULL on failure
 *
 * Caller is responsible for freeing the returned buffer.
 */
 unsigned char * base64_decode(const char * data, size_t * out_len);


/**
 * @brief TEA-加密数据，输入代价数据，输出base64
 * 
 * @param in_enda 输入数据
 * @param in_len 数据长度
 * @param out_dat 数据数据 bate64
 * @return stat_m 
 */
stat_m  m_callable_crypt_xxtea_encrypt(char * in_enda , int *in_len  , char* out_dat);

/**
 * @brief TEA-解密数据，输入加密后的数据，输出字符
 * 
 * @param in_deda 输入待解密数据
 * @param in_len 数据长度
 * @param out_dat 输出数据
 * @return stat_m 
 */
stat_m  m_callable_crypt_xxtea_decrypt(char * in_deda , int *in_len  , char* out_dat);

 #endif














