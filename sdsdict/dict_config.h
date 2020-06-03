#pragma once
#include "dict.h"
#include "sds.h"
#ifndef __DICT_CONFIG__
#define __DICT_CONFIG__
//void dictVanillaFree(void* privdata, void* val);
//int dictSdsKeyCompare(void* privdata, const void* key1, const void* key2);
//int dictSdsKeyCaseCompare(void* privdata, const void* key1, const void* key2);
//void dictSdsDestructor(void* privdata, void* val);
//unsigned int dictSdsHash(const void* key);

extern dictType SdsDictType;
extern dictType VanillaDictType;

#endif // !__DICT_CONFIG__

