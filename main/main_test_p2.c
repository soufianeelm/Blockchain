//
//  main_test_p2.c
//  

#include "partie2.h"

void print_long_vector(long * result, int size){
    printf("Vector: [");
    for (int i=0; i<size; i++){
        printf("%lx \t", result[i]);
    }
    printf("]\n");
}

int main (void) {
    srand(time(NULL));
    
    //Testing Init Keys
    Key* pKey = malloc(sizeof(Key));
    Key* sKey = malloc(sizeof(Key));
    init_pair_keys(pKey, sKey,3,7);
    printf("pKey: %lx , %lx \n", pKey->val, pKey->n);
    printf("sKey: %lx , %lx \n", sKey->val, sKey->n);
    
    //Testing Key Serialization
    char* chaine = key_to_str(pKey);
    printf("key to str : %s \n", chaine);
    Key* k = str_to_key(chaine);
    printf("str to key : %lx , %lx \n", k->val, k->n);
    free(k);
    free(chaine);
    //Testing signature
    //Candidate keys:
    Key* pKeyC = malloc(sizeof(Key));
    Key* sKeyC = malloc(sizeof(Key));
    init_pair_keys(pKeyC, sKeyC,3,7);
    
    //Declaration:
    char* mess = key_to_str(pKeyC);
    char* tmp = key_to_str(pKey);
    printf("%s vote pour %s\n",tmp, mess);
    free(tmp);
    Signature* sgn = sign(mess, sKey);
    printf("signature : ");
    print_long_vector(sgn->content, sgn->size);
    chaine = signature_to_str(sgn);
    free(sgn->content);
    free(sgn);
    printf(" signature to str : %s \n", chaine);
    sgn = str_to_signature(chaine);
    printf("str to signature : ");
    print_long_vector(sgn->content, sgn->size);
    free(chaine);
    //Testing protected:
    Protected* pr = init_protected(pKey, mess, sgn);
    free(mess);
    //Verification:
    if (verify(pr)){ printf("Signature valide\n");
    }else{
    printf("Signature non valide\n");
    }
    chaine = protected_to_str(pr);
    printf(" protected to str : %s\n", chaine);
    free(pr->mess);
    free(pr->pKey);
    free(pr->sgn->content);
    free(pr->sgn);
    free(pr);
    pr = str_to_protected(chaine);
    free(chaine);
    char* tmp1 = key_to_str(pr->pKey);
    char* tmp2 = signature_to_str(pr->sgn);
    printf("str to protected : %s %s %s\n", tmp1, pr->mess,tmp2);
    free(tmp1);
    free(tmp2);
    free(sKey);
    free(pKeyC);
    free(sKeyC);
    free(pr->mess);
    free(pr->pKey);
    free(pr->sgn->content);
    free(pr->sgn);
    free(pr);
    return 0;
}
