//
//  partie3.h
//  partie3
//

#ifndef partie3_h
#define partie3_h

#include "partie2.h"

typedef struct cellKey{
    Key* data;
    struct cellKey* next;
} CellKey;

typedef struct cellProtected{
    Protected* data;
    struct cellProtected* next;
} CellProtected;

typedef struct hashcell{
    Key* key;
    int val;
} HashCell;

typedef struct hashtable{
    HashCell** tab;
    int size;
} HashTable;

CellKey* create_cell_key(Key* key);

void add_key(CellKey** cell, Key* clef);

CellKey* read_public_keys(char* file);

void print_list_keys(CellKey* LCK);

void delete_cell_key(CellKey* c);

void delete_list_keys(CellKey** C);

CellProtected* create_cell_protected(Protected* pr);

void add_pr(CellProtected** cellp, Protected* pr);

CellProtected* read_protected(char* file);

void print_list_protected(CellProtected* LCK);

void delete_cell_protected(CellProtected* c);

void delete_list_protected(CellProtected** c);

void validation(CellProtected** c);

HashCell* create_hashcell(Key* key);

int hash_function(Key* key, int size);

int find_position(HashTable* t, Key* key);

HashTable* create_hashtable(CellKey* keys, int size);

void delete_hashtable(HashTable* t);

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);

#endif /* partie3_h */
