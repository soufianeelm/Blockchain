//
//  partie3.c
//  partie3
//

#include "partie3.h"

//Exercice 5 :

//Q.1

CellKey* create_cell_key(Key* key){
    CellKey* cell = (CellKey*) malloc(sizeof(CellKey));
    cell->data = key;
    cell->next = NULL;
    return cell;
}

//Q.2

void add_key(CellKey** cell, Key* clef){
    CellKey* p = create_cell_key(clef);
    p->next = (*cell);
    *cell=p;
}

//Q.3

CellKey* read_public_keys(char* file){
    CellKey* p=NULL;
    char buffer[255];
    char str[20];
    FILE* fi = fopen(file,"r");
    FILE* f = fopen(file,"r");
    int i=0;
    while(!feof(f)){
        fgets(buffer,255,f);
        i++;
    }
    while(i>1){
        fgets(buffer,255,fi);
        sscanf(buffer,"%s",str);
        str[strlen(str)]='\0';
        add_key(&p,str_to_key(str));
        i--;
    }
    
    fclose(fi);
    fclose(f);
    return p;
}

//Q.4

void print_list_keys(CellKey* LCK){
    CellKey* tmp = LCK;
    while(tmp!=NULL){
        char* k = key_to_str(tmp->data);
        printf("%s\n",k);
        tmp=tmp->next;
        free(k);
    }
    
}

//Q.5

void delete_cell_key(CellKey* c){
    if(c!=NULL){
        free(c->data);
        free(c);
    }
}

void delete_list_keys(CellKey** C){
    CellKey* cour=NULL;
    CellKey* next=NULL;
    if(*C!=NULL){
        cour = *C;
        while(cour!=NULL){
            next = cour->next;
            delete_cell_key(cour);
            cour = next;
        }
        *C = NULL;
    }
}

//Q.6

CellProtected* create_cell_protected(Protected* pr){
    CellProtected* cellp = (CellProtected*) malloc(sizeof(CellProtected));
    cellp->data = pr;
    cellp->next = NULL;
    return cellp;
}

//Q.7

void add_pr(CellProtected** cellp, Protected* pr){
    CellProtected* p = create_cell_protected(pr);
    CellProtected* pl = *cellp;
    p->next = pl;
    *cellp = p;
}

//Q.8

CellProtected* read_protected(char* file){
    CellProtected* p=NULL;
    
    char buffer[255];
    char str1[20];
    char str3[100];
    char str2[20];
    char str[255];

    FILE* fi = fopen(file,"r");
    
    int i = 0;
    while(!feof(fi)){
        fgets(buffer,255,fi);
        i++;
    }
    fclose(fi);

    FILE* f = fopen(file,"r");
    for(int j = 1;j<i;j++){
        fgets(buffer,255,f);
        sscanf(buffer,"%s %s %s",str1,str2,str3);
        str1[strlen(str1)]='\0';
        str2[strlen(str2)]='\0';
        str3[strlen(str3)]='\0';
        sprintf(str,"%s %s %s",str1,str2,str3);
        str[strlen(str)]='\0';
        add_pr(&p,str_to_protected(str));
    }
    fclose(f);

    return p;
}

//Q.9

void print_list_protected(CellProtected* LCK){
    if(LCK!=NULL){
        CellProtected* cell = LCK;
        while(cell!=NULL){
            char* str = protected_to_str(cell->data);
            printf("%s\n",str);
            cell=cell->next;
            free(str);
        }
    }
    
}

//Q.10

void delete_cell_protected(CellProtected* c){
    if(c!=NULL){
        free(c->data->mess);
        free(c->data->pKey);
        free(c->data->sgn->content);
        free(c->data->sgn);
        free(c->data);
        free(c);
    }
}

void delete_list_protected(CellProtected** c){
    CellProtected* cour=NULL;
    CellProtected* next=NULL;
    if(*c!=NULL){
        cour = *c;
        while(cour!=NULL){
            next = cour->next;
            delete_cell_protected(cour);
            cour = next;
        }
        *c = NULL;
    }
    
}


//Exercice 6 :

//Q.1

void validation(CellProtected** c){
    CellProtected* tmpc = NULL;
    CellProtected* prec = NULL;
    if(*c!=NULL){
        tmpc = *c;
        while(tmpc!=NULL){
            if(verify(tmpc->data)==0){
                prec->next = tmpc->next;
                delete_cell_protected(tmpc);
                tmpc = prec->next;
            } else {
                prec = tmpc;
                tmpc = tmpc->next;
            }
        }
    }
}

//Q.2

HashCell* create_hashcell(Key* key){
	HashCell* hc = (HashCell*) malloc(sizeof(HashCell));
	hc->key=key;
	hc->val=0;
	return hc;
}

//Q.3

int hash_function(Key* key, int size){
	return key->val%size;
}

//Q.4

int find_position(HashTable* t, Key* key){
 	int k=hash_function(key,t->size);
    
	for(int i=0;i<t->size;i++){
		int j = (k+i)%t->size;
        if(t->tab[j]==NULL || (t->tab[j]->key->val==key->val && t->tab[j]->key->n==key->n)){
		      //on cherche grace au probing lineaire une position pour key
			return j;      
        }   
	}
	
    
}


//Q.5

HashTable* create_hashtable(CellKey* keys, int size){
	HashTable* t = (HashTable*) malloc(sizeof(HashTable));
    CellKey* tmp = keys;
	t->tab = (HashCell**) malloc(sizeof(HashCell*)*size);
    t->size = size;
    for(int k=0;k<size;k++){
        t->tab[k]=NULL;
    }
    while(tmp!=NULL){
        int i = find_position(t,tmp->data);
        t->tab[i] = create_hashcell(tmp->data);
        tmp = tmp->next;
    }
    
	return t;
}

//Q.6

void delete_hashtable(HashTable* t){
    if(t!=NULL){
        for(int i=0;i<t->size;i++){
            free(t->tab[i]);
        }
        free(t->tab);
        free(t);
    }
}

//Q.7

Key* compute_winner(CellProtected* decl, CellKey* candidates, CellKey* voters, int sizeC, int sizeV){
	
    HashTable* Hc = create_hashtable(candidates,sizeC);
	HashTable* Hv = create_hashtable(voters,sizeV);
    CellProtected* tmp = decl;

	while(tmp!=NULL){
        int i = find_position(Hv,tmp->data->pKey);
        Key* k = str_to_key(tmp->data->mess);
        int j = find_position(Hc,k);
		if(Hv->tab[i]!=NULL){ //on cherche si l'electeur peut voter en le cherchant dans Hv
			if(Hv->tab[i]->val == 0){ // on determine si il a deja vote, si oui alors val = 1 sinon val = 0
                if(Hc->tab[j]!=NULL){ // on cherche si le candidat pour lequel l'electeur a vote est bien un candidat en le cherchant dans Hc
					Hc->tab[j]->val++; // on met a jour les elements de Hc et Hv en mettant a 1 les valeurs des votants pour montrer 
					Hv->tab[i]->val = 1; //qu'ils ont vote puis on augmente val du candidat correspondant de 1 pour compatibiliser le vote
				}
            }
        }
        tmp = tmp->next;
        free(k);
	}
	
	HashCell* h = Hc->tab[find_position(Hc,candidates->data)];
    int i=0;
    while(i<Hc->size){
        if (Hc->tab[i]!=NULL){
            if (Hc->tab[i]->val > h->val){ // on cherche le candidat avec le plus gros nombre de votes c'est a dire le plus gros val
			    h = Hc->tab[i];
		    }
        }
        i++;
	}
    
    Key* f = (Key*) malloc(sizeof(Key));
    init_key(f,h->key->val,h->key->n);
    delete_hashtable(Hc);
    delete_hashtable(Hv);
    delete_list_keys(&candidates);
    delete_list_keys(&voters);
    delete_list_protected(&decl);
    
	return f;
}

