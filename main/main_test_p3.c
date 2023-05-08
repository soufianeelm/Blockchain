//
//  main_test_p3.c
//  

#include "partie3.h"

int main(){
        srand(time(NULL));
        generate_random_data(20,5);

        CellKey* cellk = read_public_keys("keys.txt");
        print_list_keys(cellk);
        printf("\n");

        CellKey* cellc = read_public_keys("candidates.txt");
        print_list_keys(cellc);
        printf("\n");

        CellProtected* cellp = read_protected("declarations.txt");
        print_list_protected(cellp);
        
        printf("\n");
        validation(&cellp);
        Key* k = compute_winner(cellp,cellc,cellk,100,100);
        char* str = key_to_str(k);
        printf("vainqueur : %s\n",str);
        free(str);
        free(k);
        return 0;
}