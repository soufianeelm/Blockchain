//
//  main_test_p4.c
//  main_test_p4
//

#include "partie4.h"
#define test 4
#define electeurs 1000
#define candidats 5

int main(void){
    
    srand(time(NULL));

    unsigned char* s = malloc(sizeof(char)*strlen("Rosetta code")+1);
    sprintf(s,"%s","Rosetta code");
    s[strlen(s)]='\0';
    s = hash_SHA256(s);
    printf("%s\n\n",s);
    free((unsigned char*)s);
    
    generate_random_data(electeurs,candidats);
    CellProtected* cellp = read_protected("declarations.txt");
    CellKey* cellc = read_public_keys("candidates.txt");
    CellKey* cellk = read_public_keys("keys.txt");
    
    int i = 1;
    CellProtected* tmpp = cellp;
    CellKey* tmpk = cellk;
    CellTree* p = NULL;
    
    while(i<=1000){
        submit_vote(tmpp->data);
        if(i%10==0){
            create_block(&p,tmpk->data,test);
            char* str = malloc(sizeof(int)+sizeof(char)+1);
            sprintf(str,"%d",i/10);
            str[strlen(str)]='\0';
            add_block(test,str);
            free(str);
        }
        tmpk = tmpk->next;
        tmpp = tmpp->next;
        i++;
    }
    
    delete_tree(p);

    CellTree* tree = read_tree();
    print_tree(tree);
    int sizeC = 10*candidats;
    int sizeV = 2*electeurs;
    Key* k = compute_winner_BT(tree,cellc,cellk,sizeC,sizeV);
    char* vainqueur = key_to_str(k);
    printf("vainqueur de l'election : %s\n",vainqueur);
    
    free(vainqueur);
    free(k);
    delete_list_protected(&cellp);

    return 0;
}