//
//  partie2.c
//  partie2
//

#include "partie2.h"

//Exercice 3 :

//Q.2

void init_key(Key* key, long val, long n){
    if(key!=NULL){
        key->val = val;
        key->n = n;
    }
}

//Q.3

void init_pair_keys(Key* pKey, Key* sKey, long low_size, long up_size){
    //generation de keys :
    long p = random_prime_number(3,7,5000);
    long q = random_prime_number(3,7,5000);
    while (p == q){
        q = random_prime_number(3,7,5000);
    }
    long n=0, s=0, u=0;
    generate_keys_values(p,q,&n,&s,&u);
    //pour avoir des clefs positives :
    if (u < 0){
        long t = (p-1)*(q-1);
        u += t;
    }
    init_key(pKey,s,n);
    init_key(sKey,u,n);
}

//Q.4

char* key_to_str(Key* key){
    char* str = (char*) malloc((sizeof(char)*4)+sizeof(long)*2);
    //on print les valeurs de la cle sur une chaine de caracteres :
    sprintf(str,"(%lx,%lx)",key->val,key->n); 
    str[strlen(str)]='\0';
    return str;
}

Key* str_to_key(char* str){
    long m, n;
    //on cherche les valeurs de m et n dans str :
    sscanf(str,"(%lx,%lx)",&m,&n);
    Key* k = (Key*)malloc(sizeof(Key));
    //on initialise une key avec ces valeurs : 
    init_key(k,m,n);
    return k;
}

//Q.6

Signature* init_signature(long* content, int size){
    Signature* s = (Signature*)malloc(sizeof(Signature));
    s->content = content;
    s->size = size;
    return s;
}

//Q.7

Signature* sign(char* mess, Key* sKey){
    //on initialise une signature avec le cryptage de mess :
    return init_signature(encrypt(mess,sKey->val,sKey->n),strlen(mess));
}

//Q.8

char* signature_to_str(Signature* sgn){
    char* result =  malloc(10*sgn->size*sizeof(char));
    result[0]='#';
    int pos = 1;
    char buffer[156];
    for(int i=0; i<sgn->size; i++){
        sprintf(buffer,  "%lx", sgn->content[i]);
        for(int j=0; j<strlen(buffer); j++){
            result[pos] = buffer[j];
            pos = pos+1;
        }
        result[pos] = '#';
        pos = pos+1;
    }
    result[pos] = '\0';
    result = realloc(result, (pos+1)*sizeof(char));
    return result;
}

Signature* str_to_signature(char* str){
    int len = strlen(str);
    long* content = (long*)malloc(sizeof(long)*len);
    int num = 0;
    char buffer[256];
    int pos = 0;
    for(int i=0; i<len; i++){
        if(str[i] != '#'){
            buffer[pos] = str[i];
            pos=pos+1;
        } else {
            if (pos != 0){
                buffer[pos] = '\0';
                sscanf(buffer, "%lx", &(content[num]));
                num = num + 1;
                pos = 0;
            }
        }
    }
    content=realloc(content, num*sizeof(long));
    return init_signature(content, num);
}

//Q.10

Protected* init_protected(Key* pKey, char* mess, Signature* sgn){
    Protected* Pr = (Protected*)malloc(sizeof(Protected));
    Pr->pKey=pKey;
    Pr->mess=(char*)malloc(sizeof(char)*strlen(mess)+1);
    strcpy(Pr->mess,mess);
    Pr->mess[strlen(mess)]='\0';
    Pr->sgn=sgn;
    return Pr;
}

//Q.11

int verify(Protected* pr){
    //on decrypte la signature dans pr avec la cle publique dans pr :
    char* str = decrypt(pr->sgn->content,pr->sgn->size,pr->pKey->val,pr->pKey->n);
    //on compare avec le message dans pr :
    if(strcmp(str,pr->mess)==0){
        free(str);
        return 1; // 1 si le decryptage est bon
    }
    free(str);
    return 0;
}

//Q.12

char* protected_to_str(Protected* pr){
    char* str1 = key_to_str(pr->pKey);
    char* str2 = signature_to_str(pr->sgn);
    char* str=(char*)malloc(sizeof(char)*(strlen(pr->mess)+strlen(str2)+strlen(str1)+3));
    sprintf(str,"%s %s %s",str1,pr->mess,str2);
    str[strlen(str)]='\0';
    free(str1);
    free(str2);
    return str;
}

Protected*  str_to_protected(char* str){
    char k[50];
    char m[50];
    char sgn[100];
    sscanf(str,"%s %s %s",k,m,sgn);
    k[strlen(k)]='\0';
    m[strlen(m)]='\0';
    sgn[strlen(sgn)]='\0';
    return init_protected(str_to_key(k),m,str_to_signature(sgn));
}


//Exercice 4 :

int exists(int* tab, int size, int k){
    //on cherche l'entier k dans tab
    for(int i=0;i<size;i++){
        if(tab[i]==k){
            return 1;
        }
    }
    return 0;
}

//Q.1

void generate_random_data(int nv, int nc){
    Key* pKey = (Key*) malloc(sizeof(Key));
    Key* sKey = (Key*) malloc(sizeof(Key));
    int* tab;
    
    //on initialise des couples de clefs puis on les notes sur un fichier txt :
    FILE* keys = fopen("keys.txt","w");
    for(int i=0; i<nv; i++){
        init_pair_keys(pKey,sKey,3,7);
        char* str1 = key_to_str(pKey);
        char* str2 = key_to_str(sKey);
        fprintf(keys,"%s %s\n",str1,str2);
        free(str1);
        free(str2);
    }
    fclose(keys);
    
    //on choisi au hasard des candidats parmis les keys puis on les notes sur un fichier txt :
    FILE* candidates = fopen("candidates.txt","w");
    tab =(int*) malloc(sizeof(int)*nc);
    for(int i=0; i<nc; i++){
        keys = fopen("keys.txt","r"); 
        char buff[255];
        char str[255];
        int k = (rand()%nv)+1;
        while(exists(tab,i,k)){ //pour ne pas avoir le meme candidat
            k = (rand()%nv)+1;
        } tab[i] = k;
        while (k>0){
            fgets(buff,255,keys);
            k--;
        }
        sscanf(buff,"%s",str);
        str[strlen(str)]='\0';
        fprintf(candidates,"%s\n",str);
        fclose(keys);
    }
    fclose(candidates);

    free(tab);
    
    //on initialise des declarations en choisissant au hasard les votes, on les note sur un fichier txt :
    FILE* declarations = fopen("declarations.txt","w");
    keys = fopen("keys.txt","r");
    for(int i=0; i<nv; i++){
        candidates = fopen("candidates.txt","r");
        char buff1[255];
        char buff2[255];
        char mess[20];
        char str2[20];
        char str3[20];
        int k  = (rand()%nc)+1;
        while(k>0){
            fgets(buff1,255,candidates);
            k--;
        }
        sscanf(buff1,"%s",mess);
        fgets(buff2,255,keys);
        sscanf(buff2,"%s %s",str2,str3);
        mess[strlen(mess)]='\0';
        str2[strlen(str2)]='\0';
        str3[strlen(str3)]='\0';
        Key* pK = str_to_key(str2);
        Key* sK = str_to_key(str3);
        Signature* sgn = sign(mess,sK);
        Protected* pr = init_protected(pK,mess,sgn);
        char* l = protected_to_str(pr);
        fprintf(declarations,"%s\n",l);
        fclose(candidates);
        free(pK);
        free(sK);
        free(l);
        free(pr->mess);
        free(pr);
        free(sgn->content);
        free(sgn);
    }
    fclose(declarations);
    fclose(keys);
    
    free(pKey);
    free(sKey);
}
