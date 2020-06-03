#include "dict_config.h"
#include "zmalloc.h"
#include "win_interop.h"
#include "sds.h"

#include <string.h>
#include <ctype.h>
#include <stdarg.h>

/*====================== Hash table type implementation  ==================== */

/* This is a hash table type that uses the SDS dynamic strings library as
 * keys and disque objects as values (objects can hold SDS strings,
 * lists, sets). */
void* dictVanillaNew(void* privdata, void* val)
{
    DICT_NOTUSED(privdata);
    size_t len = strlen(val);
    char* ptr = zmalloc(len+1);
    memcpy(ptr, val,len);
    ptr[len] = '\0';
    return ptr;
}
void dictVanillaFree(void* privdata, void* val)
{
    DICT_NOTUSED(privdata);
    zfree(val);
}

int dictSdsKeyCompare(void* privdata, const void* key1,
    const void* key2)
{
    int l1, l2;
    DICT_NOTUSED(privdata);

    l1 = sdslen((sds)key1);
    l2 = sdslen((sds)key2);
    if (l1 != l2) return 0;
    return memcmp(key1, key2, l1) == 0;
}

int dictVanillaKeyCompare(void* privdata, const void* key1,
    const void* key2)
{
    int l1, l2;
    DICT_NOTUSED(privdata);

    l1 = strlen((char*)key1);
    l2 = strlen((char*)key2);
    if (l1 != l2) return 0;
    return memcmp(key1, key2, l1) == 0;
}


/* A case insensitive version used for the command lookup table and other
 * places where case insensitive non binary-safe comparison is needed. */
int dictSdsKeyCaseCompare(void* privdata, const void* key1,
    const void* key2)
{
    DICT_NOTUSED(privdata);

    return strcasecmp(key1, key2) == 0;
}

void* dictSdsNew(void* privdata, void* val)
{
    DICT_NOTUSED(privdata);

    return sdsnew(val);
}

void dictSdsDestructor(void* privdata, void* val)
{
    DICT_NOTUSED(privdata);

    sdsfree((sds)val);
}

unsigned int dictSdsHash(const void* key) {
    return dictGenHashFunction((unsigned char*)key, sdslen((char*)key));
}

unsigned int dictVanillaHash(const void* key) {
    return dictGenHashFunction((unsigned char*)key, strlen((char*)key));
}



dictType SdsDictType = {
    dictSdsHash,             /* hash function */
    NULL,                    /* key dup *//*dictSdsNew*/
    NULL,                    /* val dup *//*dictSdsNew*/
    dictSdsKeyCompare,       /* key compare */
    dictSdsDestructor,       /* key destructor */
    dictSdsDestructor        /* val destructor */
};

dictType VanillaDictType = {
    dictVanillaHash,             /* hash function */
    dictVanillaNew,          /* key dup */
    dictVanillaNew,          /* val dup */
    dictVanillaKeyCompare,   /* key compare */
    dictVanillaFree,         /* key destructor */
    dictVanillaFree          /* val destructor */
};
