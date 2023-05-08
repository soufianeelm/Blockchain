//
//  partie1.c
//  partie1
//

#include "partie1.h"

//Exercice 1 :

//Q.1

int is_prime_naive(long p){
    for(int i=3;i<p;i++){
        if (p%i==0){ // on teste pour chaque i entre 3 et p-1 si i divise p
            return 0;
        }
    }
    return 1;
}

//Q.3

long modpow_naive(long a, long m, long n){
    long k = 1;
    while(m>0){
        k = k*a; // on multiplie la valeur courante par a
        k = k%n; // puis resultat au mod n
        m--;
    }
    return k;
}

//Q.4

int modpow(long a, long m, long n){
    if(m==0){
        return 1;
    }
    int b = modpow(a,m/2,n);  // un seul appel à la fois
    
    if(!(m & 1)){  // on teste la parité de m
        return (int)((b*b)%n);   // si m pair
    } else {
        return (int)((a*b*b)%n); // si m impair
    }
    
}

//Q.5

int doc(long a, long m, long n){
    FILE* f = fopen("perf_modpow.txt","a");
    if(!f){
        printf("Erreur lors de l'ouverture du fichier");
        return 0;
    }
    clock_t ti;
    clock_t tf;
    double tmp_cpu1;
    double tmp_cpu2;
    
    ti = clock();
    modpow_naive(a,m,n);
    tf = clock();
    tmp_cpu1 = ((double)(tf - ti)/CLOCKS_PER_SEC); // temps de calcule de modpow_naive
    
    ti = clock();
    modpow(a,m,n);
    tf = clock();
    tmp_cpu2 = ((double)(tf - ti)/CLOCKS_PER_SEC); // temps de calcule de modpow
    
    if(m == 0){
        fprintf(f,"Valeur de M – Perf naives – Perf\n"); //en-tête du fichier
    } fprintf(f,"   %ld   –   %f   –   %f\n",m,tmp_cpu1,tmp_cpu2);
    
    fclose(f);
    return 1;
}

void perf_modpow(long a, long n){
    for(int i=0;i<500000000;i+=10000000){ // m varie de 0 à 490 000 000 par incrémentation de 10 000 000
        doc(a,i,n);
    }
}

//Q.6

int witness(long a, long b, long d, long p){
    long x = modpow(a,d,p);
    if(x == 1){
        return 0;
    }
    for(long i = 0; i<b; i++){
        if(x == p-1){
            return 0;
        }
        x = modpow(x,2,p);
    }
    return 1;
}

long rand_long(long low, long up){
    return rand() % (up - low +1)+low;
}

int is_prime_miller(long p, int k){
    if (p == 2) {
        return 1;
    }
    if (!(p & 1) || p <= 1) { //on verifie que p est impair et different de 1
        return 0;
    }
    //on determine b et d :
    long b = 0;
    long d = p - 1;
    while (!(d & 1)) { //tant que d n'est pas impair
        d = d/2;
        b = b+1;
    }
    //On genere k valeurs de a, et on teste si c'est un temoin :
    long a;
    int i;
    for(i = 0; i < k; i++){
        a = rand_long(2, p-1);
        if (witness(a,b,d,p)){
            return 0;
        }
    }
    return 1;
}

//Q.8

long random_prime_number(int low_size, int up_size, int k){
    int min = pow(2,low_size-1), max = pow(2,up_size)-1;
    //On genere un long random :
    long p = rand_long((long)min,(long)max);
    while(is_prime_miller(p,k) != 1){ //on continue a en generer tant qu'il n'est pas premier
        p = rand_long((long)min,(long)max);
    }
    return p;
}


//Exercice 2 :

//Q.1

long extended_gcd(long s, long t, long* u, long* v){
    if (s == 0){
        *u = 0;
        *v = 1;
        return t;
    }
    long uPrim=0, vPrim=0;
    long gcd = extended_gcd(t%s, s, &uPrim, &vPrim);
    *u = vPrim -(t/s)*uPrim;
    *v = uPrim;
    return gcd;
}

void generate_keys_values(long p, long q, long* n, long* s, long*u){
    *n = p*q; //on suppose p et q superieurs a 0
    long t = (p-1)*(q-1),v=0;
    *s = rand_long(1,t-1);
    while(extended_gcd(*s,t,u,&v)!=1){
        *s = rand_long(1,t-1);
    }
}

//Q.2

long* encrypt(char* chaine, long s, long n){
    int i=0;
    long* crypted=(long*) malloc(sizeof(long)*strlen(chaine));
    while(chaine[i]!='\0'){
        //cryptage de chaque char de la chaine en long avec la fonction modpow et la cle secrete
        crypted[i]=(long) modpow((long)chaine[i],s,n);
        i++;
    }
    return crypted;
}

//Q.3

char* decrypt(long* crypted, int size, long u, long n){
    char* chaine=(char*) malloc(sizeof(char)*size+1);
    for(int i=0;i<size;i++){
        //decryptage de chaque long du tableau crypted avec la fonction modpow et la cle publique
        chaine[i]=(char) modpow(crypted[i],u,n);
    }
    chaine[size]='\0';
    return chaine;
}


