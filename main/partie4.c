//
//  partie4.c
//  partie4
//

#include "partie4.h"

//Exercice 7 :

//Q.1

void write_block(Block* b, char* file){
    char* str = key_to_str(b->author);
    CellProtected* tmp = b->votes;
    FILE* f = fopen(file,"w");
    fprintf(f,"%s\n%s\n%s\n%d\n",str,b->hash,b->previous_hash,b->nonce);
    while(tmp!=NULL){
        char* ftr = protected_to_str(tmp->data);
        fprintf(f,"%s\n",ftr);
        free(ftr);
        tmp = tmp->next;
    }
    free(str);
    fclose(f);
}

//Q.2

Block* read_block(char* file){
    FILE* f = fopen(file,"r");
    Block* block = (Block*) malloc(sizeof(Block));

    char buffer[255];

    fgets(buffer,255,f);
    block->author = str_to_key(buffer);
    
    fgets(buffer,255,f);
    block->hash = malloc(sizeof(char)*strlen(buffer)+1);
    sscanf(buffer,"%s",block->hash);
    block->hash[strlen(buffer)] = '\0';

    fgets(buffer,255,f);
    block->previous_hash = malloc(sizeof(char)*strlen(buffer)+1);
    sscanf(buffer,"%s",block->previous_hash);
    block->previous_hash[strlen(buffer)] = '\0';

    fgets(buffer,255,f);
    sscanf(buffer,"%d",&block->nonce);
    fgets(buffer,255,f);
    block->votes = create_cell_protected(str_to_protected(buffer));
    CellProtected* tmp = block->votes;
    while(!feof(f)){
        char bufferprime[255];
        strcpy(bufferprime,buffer);
        fgets(buffer,255,f);
        if(strcmp(buffer,bufferprime)!=0){
            block->votes->next = create_cell_protected(str_to_protected(buffer));
            block->votes = block->votes->next;
        }
    }
    block->votes = tmp;
    fclose(f);
    return block;
}

//Q.3

char* block_to_str(Block* block){

    char* strkey = key_to_str(block->author);

    char strnonce[100];
    sprintf(strnonce,"%d",block->nonce);
    char* buffer = malloc(strlen(strkey)+strlen(block->previous_hash)+sizeof(char));
    strcat(strcpy(buffer,strkey),block->previous_hash);

    CellProtected* tmp = block->votes;
    while(tmp!=NULL){
        char* temp = protected_to_str(tmp->data);
        buffer = realloc(buffer,strlen(buffer)+strlen(temp)+sizeof(char));
        strcat(buffer,temp);
        tmp=tmp->next;
        free(temp);
    }

    buffer = realloc(buffer,strlen(buffer)+strlen(strnonce)+sizeof(char));
    strcat(buffer,strnonce);

    free(strkey);
    return buffer;
}

//Q.5

unsigned char* hash_SHA256(unsigned char* str){
    if(str!=NULL){
        unsigned char* d = SHA256(str, strlen(str), 0);
        unsigned char* l = malloc(sizeof(unsigned char)*SHA256_DIGEST_LENGTH*2+1);
        sprintf(l,"%02x",d[0]);
        for(int i=1; i < SHA256_DIGEST_LENGTH; i++){
            char* tmp = malloc(sizeof(char)*3);
            sprintf(tmp,"%02x",d[i]);
            tmp[strlen(tmp)] = '\0';
            strcat(l,tmp);
            free(tmp);
        }
        free((unsigned char*)str);
        str = malloc(sizeof(unsigned char)*SHA256_DIGEST_LENGTH*2+1);
        strcpy(str,l);
        free((unsigned char*)l);
        return str;
    }
    
}

//Q.6

int nb_zero_succ(unsigned char* str){
    int i=0;
    while(i<strlen(str) && str[i]=='0'){
        i++;
    }
    return i;
}

void compute_proof_of_work(Block *B, int d){
    B->nonce = 0;
    unsigned char* str = block_to_str(B);
    while(nb_zero_succ(str)<d){
        str=hash_SHA256(str);
        B->nonce++;
    }
    free(str);
}

//Q.7

int verify_block(Block* B, int d){
    int tmp = B->nonce;
    compute_proof_of_work(B,d);
    return tmp == B->nonce;
}

//Q.8

int docprime(Block* b, int d){
    FILE* f = fopen("perf_compute.txt","a");
    if(!f){
        printf("Erreur lors de l'ouverture du fichier");
        return 0;
    }
    clock_t ti;
    clock_t tf;
    double tmp_cpu;
    
    ti = clock();
    compute_proof_of_work(b,d);
    tf = clock();
    tmp_cpu = ((double)(tf - ti)/CLOCKS_PER_SEC);

    if(d == 0){
        fprintf(f,"Valeur de d – Perf\n");
    } fprintf(f,"   %d   –   %f\n",d,tmp_cpu);
    
    fclose(f);
    return 1;
}

void perf_compute(Block* b){
    for(int i=0;i<6;i++){ // d varie de 0 à 5
        docprime(b,i);
    }
}

//Q.9

void delete_block(Block* b){
    if(strcmp(b->previous_hash,"Debut")==0){
        free((unsigned char*)b->previous_hash);
    }
    free((unsigned char*)b->hash);
    free(b);
}

//Exercice 8 :

//Q.1

CellTree* create_node(Block* b){
    CellTree* cellt = (CellTree*) malloc(sizeof(CellTree));
    if(!cellt){
        printf("Erreur lors de l'allocation memoire");
        return NULL;
    }
    cellt->block = b;
    cellt->father = NULL;
    cellt->firstChild = NULL;
    cellt->nextBro = NULL;
    cellt->height = 0;
    return cellt;
}

//Q.2

int update_height(CellTree* father, CellTree* child){
    if(child->height+1 > father->height){ // if hauteur de son + 1 superieur a hauteur courante de father
        father->height = child->height+1;
        return 1;
    } else {
        return 0;
    }
}

//Q.3

void add_child(CellTree* father, CellTree* child){
    if(father!=NULL){
        child->father = father;
        if(father->firstChild!=NULL){
            CellTree* tmp = father->firstChild->nextBro;
            while(tmp!=NULL){
                tmp = tmp->nextBro;
            }
            tmp = child;
        }else{
            father->firstChild = child;
        }
        update_height(father,child);
        CellTree* tmpf = father;
        while(tmpf->father!=NULL){
            update_height(tmpf->father,tmpf);
            tmpf = tmpf->father;
        }
    }else{
        father = child;
    }
}

//Q.4

int print_tree(CellTree* tree){
    if(tree==NULL){
        return 0;
    }
    printf("hauteur noeud : %d\n",tree->height);
    printf("block : %s\n\n",tree->block->hash);
    if(tree->firstChild!=NULL){
        print_tree(tree->firstChild);
        if(tree->firstChild->nextBro!=NULL){
            CellTree* tmp = tree->firstChild->nextBro;
            while(tmp!=NULL){
                print_tree(tmp);
                tmp=tmp->nextBro;
            }
        }
    }
    return 1;
}

//Q.5

void delete_node(CellTree* node){
    if(node!=NULL){
        delete_block(node->block);
        free(node);
    }
}

void delete_tree(CellTree* tree){
    if(tree!=NULL){
        delete_tree(tree->firstChild);
        delete_tree(tree->nextBro);
        delete_node(tree);
    }
}

//Q.6

CellTree* highest_child(CellTree* cell){
    if(cell->firstChild!=NULL){
        CellTree* tmp = cell->firstChild;
        if(cell->firstChild->nextBro!=NULL){
            CellTree* tmpb = cell->firstChild->nextBro;
            while(tmpb!=NULL){
                if(tmp->height<tmpb->height){
                    tmp = tmpb;
                }
                tmpb = tmpb->nextBro;
            }
        }
        return tmp;
    }
    return NULL;
}

//Q.7

CellTree* last_node(CellTree* tree){
    if(tree!=NULL){
        CellTree* tmp = tree;
        while(tmp->firstChild!=NULL){
            tmp = highest_child(tmp);
        }
        return tmp;
    }
    return tree;
}

//Q.8

CellProtected* fusion(CellProtected* cell1, CellProtected* cell2){
    CellProtected* tmp = cell1;
    while(tmp->next!=NULL){
        tmp = tmp->next;
    }
    tmp->next = cell2;
    return cell1;
}

//Q.9

CellProtected* fusion_ultime(CellTree* tree){
    CellTree* tmp = highest_child(tree);
    while(tmp->firstChild!=NULL){
        fusion(tree->block->votes,tmp->block->votes);
        tmp = highest_child(tmp);
    }
    fusion(tree->block->votes,tmp->block->votes);
    return tree->block->votes;
}

//Exercice 9 :

//Q.1

void submit_vote(Protected* p){
    FILE* f = fopen("Pending_votes.txt","a");
    char* tmp = protected_to_str(p);
    fprintf(f,"%s\n",tmp);
    free(tmp);
    fclose(f);
}

//Q.2

void create_block(CellTree** tree, Key* author, int d){
    Block* b = (Block*) malloc(sizeof(Block));
    b->votes = read_protected("Pending_votes.txt");
    b->author = author;
    b->nonce = 0;
    if(*tree!=NULL){
        b->previous_hash = last_node(*tree)->block->hash;
    }
    else {
        b->previous_hash = malloc(sizeof(char)*strlen("Debut")+1);
        sprintf(b->previous_hash,"Debut");
    }
    b->hash = block_to_str(b);
    b->hash = hash_SHA256(b->hash);
    compute_proof_of_work(b,d);
    if (*tree!=NULL){
        add_child(last_node(*tree),create_node(b));
    }
    else {
        (*tree) = create_node(b);
    }
    write_block(b,"Pending_block");
    delete_list_protected(&b->votes);
    remove("Pending_votes.txt");
}

//Q.3

void add_block(int d, char* name){
    Block* b = read_block("Pending_block");
    if(verify_block(b,d)==1){
        char* str = malloc(sizeof(char)*(strlen("./Blockchain/")+strlen(name)+1));
        sprintf(str,"./Blockchain/%s",name);
        write_block(b,str);
        free(str);
    }
    delete_list_protected(&b->votes);
    free((unsigned char*)b->hash);
    free((unsigned char*)b->previous_hash);
    free(b->author);
    free(b);
    remove("Pending_block");
}

//Q.4

CellTree* read_tree(){
    CellTree** tab = (CellTree**) malloc(sizeof(CellTree*));
    int k = 0;
    DIR* rep = opendir("./Blockchain/");
    if(rep!=NULL){
        struct dirent* dir;
        while((dir = readdir(rep))!=NULL){
            if(strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0){
                char* str = malloc(sizeof(char)*(strlen("./Blockchain/")+strlen(dir->d_name)+1));
                sprintf(str,"./Blockchain/%s",dir->d_name);
                Block* tmp = read_block(str);
                free(tmp->author);
                tab = realloc(tab,sizeof(CellTree*)*(k+1));
                tab[k] = create_node(tmp);
                free(str);
                k++;
            }
        }
        
        free(dir);
        for(int i = 0; i<k; i++){
            for(int j = 0; j<k; j++){
                if(strcmp(tab[j]->block->previous_hash,tab[i]->block->hash)==0){
                    free((unsigned char*)tab[j]->block->previous_hash);
                    tab[j]->block->previous_hash = tab[i]->block->hash;
                    add_child(tab[i],tab[j]);
                }
            }
        }
        
        for(int i = 0; i<k; i++){
            if(tab[i]->father==NULL){
                CellTree* tmp = tab[i];
                free(tab);
                return tmp;
            }
        }
    } else {
        free(tab);
    }
    closedir(rep);
}

//Q.5

Key* compute_winner_BT(CellTree* tree, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
    CellProtected* cellp = fusion_ultime(tree);
    validation(&cellp);
    delete_tree(tree);
    Key* k = compute_winner(cellp,candidates,voters,sizeC,sizeV);
    return k;
}


