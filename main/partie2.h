//
//  partie2.h
//  partie2
//

#ifndef partie2_h
#define partie2_h

#include "partie1.h"

//Q.1

typedef struct key{
    long val;
    long n;
} Key;

//Q.5

typedef struct signature{
    long* content;
    int size;
} Signature;

//Q.9

typedef struct protected{
    Key* pKey;
    char* mess;
    Signature* sgn;
} Protected;

void init_key(Key* key, long val, long n);

void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size);

char* key_to_str(Key* key);

Key* str_to_key(char* str);

Signature* init_signature(long* content, int size);

Signature* sign(char* mess, Key* sKey);

char* signature_to_str(Signature* sgn);

Signature* str_to_signature(char* str);

Protected* init_protected(Key* pKey, char* mess, Signature* sgn);

int verify(Protected* pr);

char* protected_to_str(Protected* pr);

Protected*  str_to_protected(char* str);

void generate_random_data(int nv, int nc);

#endif /* partie2_h */
