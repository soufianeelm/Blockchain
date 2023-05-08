all: main_test_p1 main_test_p2 main_test_p3 main_test_p4

partie1.o : partie1.c partie1.h
	gcc -c partie1.c

main_test_p1.o : main_test_p1.c partie1.h 
	gcc -c main_test_p1.c

main_test_p1 : main_test_p1.o partie1.o
	gcc -o main_test_p1 main_test_p1.o partie1.o -lm 

partie2.o : partie2.c partie2.h
	gcc -c partie2.c

main_test_p2.o : main_test_p2.c partie2.h
	gcc -c main_test_p2.c

main_test_p2 : main_test_p2.o partie1.o partie2.o
	gcc -o main_test_p2 main_test_p2.o partie1.o partie2.o -lm

partie3.o : partie3.c partie3.h
	gcc -c partie3.c

main_test_p3.o : main_test_p3.c partie3.h
	gcc -c main_test_p3.c

main_test_p3 : main_test_p3.o partie1.o partie2.o partie3.o
	gcc -o main_test_p3 main_test_p3.o partie1.o partie2.o partie3.o -lm

partie4.o : partie4.c partie4.h
	gcc -c partie4.c

main_test_p4.o : main_test_p4.c partie4.h
	gcc -c main_test_p4.c 

main_test_p4 : main_test_p4.o partie1.o partie2.o partie3.o partie4.o
	gcc -o main_test_p4 main_test_p4.o partie1.o partie2.o partie3.o partie4.o -lm -lssl -lcrypto

main_perf_modpow.o : main_perf_modpow.c partie1.h
	gcc -c main_perf_modpow.c

main_perf_modpow : main_perf_modpow.o partie1.o
	gcc -o main_perf_modpow main_perf_modpow.o partie1.o -lm

main_perf_compute.o : main_perf_compute.c partie4.o
	gcc -c main_perf_compute.c

main_perf_compute : main_perf_compute.o partie1.o partie2.o partie3.o partie4.o
	gcc -o main_perf_compute main_perf_compute.o partie1.o partie2.o partie3.o partie4.o -lm -lssl -lcrypto
