#ifndef _HASH_ALGORITHM_H_
#define _HASH_ALGORITHM_H_


#include "pch.h"


typedef struct {
	uint32_t state[4];                               
	uint32_t count[2];
	uint8_t buffer[64];                    
} md5_t;

using hash_md5_t = uint8_t[16];

using uint8_t_ptr = uint8_t*;

using uint16_t_ptr = uint16_t*;

using uint32_t_ptr = uint32_t*;


#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

void MD5Init(md5_t* md5);

void MD5Update(md5_t* md5, uint8_t* message, uint32_t size);

void MD5Final(hash_md5_t hash, md5_t* md5);

#ifdef __cplusplus
}
#endif // __cplusplus


#endif // _HASH_ALGORITHM_H_
