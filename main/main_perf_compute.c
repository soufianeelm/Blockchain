//
//  main_perf_compute.c
//  main_perf_compute
//

#include "partie4.h"

int main(void){
    generate_random_data(20,5);

    Block* b = (Block*) malloc(sizeof(Block));
    b->nonce = 0;
    b->author = str_to_key("(c7,229)");
    b->votes = read_protected("declarations.txt");
    b->previous_hash = malloc(sizeof(char)*6);
    sprintf(b->previous_hash,"debut");
    b->hash = block_to_str(b);
    hash_SHA256(b->hash);
    compute_proof_of_work(b,4);
    verify_block(b,4);
    perf_compute(b);

    free(b->author);
    CellProtected* p = b->votes;
    while(p!=NULL){
        free(p->data->mess);
        free(p->data->pKey);
        free(p->data->sgn->content);
        free(p->data->sgn);
        free(p->data);
        p = p->next;
    }
    delete_block(b);
    return 0;
}