//
//  partie4.h
//  partie4
//

#ifndef partie4_h
#define partie4_h

#include "partie3.h"
#include <openssl/sha.h>
#include <dirent.h>

typedef struct block{
    Key* author;
    CellProtected* votes;
    unsigned char* hash;
    unsigned char* previous_hash;
    int nonce;
}Block;

typedef struct block_tree_cell{
    Block* block;
    struct block_tree_cell* father;
    struct block_tree_cell* firstChild;
    struct block_tree_cell* nextBro;
    int height;
}CellTree;

void write_block(Block* b, char* file);

Block* read_block(char* file);

char* block_to_str(Block* block);

unsigned char* hash_SHA256(unsigned char* str);

int nb_zero_succ(unsigned char* str);

void compute_proof_of_work(Block *B, int d);

int verify_block(Block* B, int d);

int docprime(Block* b, int d);

void perf_compute(Block* b);

void delete_block(Block* b);

CellTree* create_node(Block* b);

int update_height(CellTree* father, CellTree* child);

void add_child(CellTree* father, CellTree* child);

int print_tree(CellTree* tree);

void delete_node(CellTree* node);

void delete_tree(CellTree* tree);

CellTree* highest_child(CellTree* cell);

CellTree* last_node(CellTree* tree);

CellProtected* fusion(CellProtected* cell1, CellProtected* cell2);

CellProtected* fusion_ultime(CellTree* tree);

void submit_vote(Protected* p);

void create_block(CellTree** tree, Key* author, int d);

void add_block(int d, char* name);

CellTree* read_tree();

Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV);

#endif /* partie4_h */