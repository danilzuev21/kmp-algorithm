/*
 * ECE 5720 Parallel Computing Final Project
 * KMP parallel on MPI
 * Feng Qi, fq26
 * Ying Zong, yz887
 * Cornell University
 *
 * Compile : gcc -std=gnu11 -o out kmp_simple.c
 * Run 	   : ./out 
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#define MILLION 1000000L
int* kmptable(char* pattern, int len);
void* kmp(char* target, char* pattern, int* table);

int main(int argc, char** argv){
	int n = 100000;
	int m = 3; 
	int i,j;
	char* target = (char*)malloc(n * sizeof(char));
	char* pattern = (char*)malloc(m * sizeof(char));
	FILE * file = fopen( "data.txt" , "r");
	int CurrentIndex = 0;

    while (CurrentIndex < 2) {
        if (CurrentIndex == 0) {
            fgets(target, n+1, file);
        } else if (CurrentIndex == 1) {
            fgets(pattern, m+1, file);
        }
        CurrentIndex++;
    }
    fclose(file);	

	struct timespec start1, end1;
	double diff;

	printf("----- This is sequential results using KMP Algo. -----\n");
	
	int* table = kmptable(pattern, m);
	clock_gettime(CLOCK_MONOTONIC, &start1);
	kmp(target, pattern, table);
	clock_gettime(CLOCK_MONOTONIC, &end1);
	diff =(end1.tv_sec - start1.tv_sec)*MILLION + (end1.tv_nsec - start1.tv_nsec);
	printf("When the target length is %d, pattern length is %d, the elapsed time is %0.3f ms.\n", n, m, diff); 
	free(table);
	free(target);
	free(pattern);
	printf("\n");
	return 0;
}
void* kmp(char* target, char* pattern, int* table){
	int n = strlen(target);
	int m = strlen(pattern);
	int j=0;
	int i=0;
	while(i<n){
		if(pattern[j]== target[i])
		{
			j++;
			i++;
		}
		if(j == m){
			printf("Find a matching substring starting at: %d.\n", i-j);
			j = table[j-1];

		}else if(i < n && pattern[j] != target[i]){
			if(j!=0)
				j = table[j-1];
			else
				i++;
		}

	}
}

int* kmptable(char* pattern, int len){
	int k = 0;
	int i = 1;
	int* table = (int*)malloc(len * sizeof(int));
	table[0] = k;
	for(i=1;i<len;i++){
		while(k > 0 && pattern[i-1] != pattern[i]){
			table[i] = k-1;
			k = table[i];
		}
		if(pattern[i] == pattern[k])
			k++;
		table[i] = k;
	}
	return table;
}

/*
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdint.h>
#include "al.h"

#define BILLION 1000000000L
int* table(char* t);
void KMP_sequential(char* s, char* t, ArrayList table);
ArrayList KMP_table(char* t);
void bruteforce_sequential(char* s, char* t);

int main(int argc, char** argv){
	char* s = "ABCE DBA AABAA AABAA";
	char* t = "AABAA";
	struct timespec start1, end1;
	struct timespec start2, end2;
	uint64_t diff;

	printf("-----This is sequential results using brute force method.-----\n");
	clock_gettime(CLOCK_MONOTONIC, &start1);
	bruteforce_sequential(s,t);
	clock_gettime(CLOCK_MONOTONIC, &end1);
	diff =(end1.tv_sec - start1.tv_sec)*BILLION + (end1.tv_nsec - start1.tv_nsec);
	printf("The execution time of sequential algo using brute force is %llu ns.\n", diff);
	ArrayList table = KMP_table(t);

	printf("\n-----This is sequential results using KMP algo.-----\n");
	clock_gettime(CLOCK_MONOTONIC, &start2);
	KMP_sequential(s, t, table);
	clock_gettime(CLOCK_MONOTONIC, &end2);
	diff =(end2.tv_sec - start2.tv_sec)*BILLION + (end2.tv_nsec - start2.tv_nsec);
	printf("The execution time of sequential algo using KMP algo is %llu ns.\n", diff);
	free(table.body);
}

ArrayList KMP_table(char* t){
	int m = strlen(t);
	ArrayList table = initial_arr(m);
	table = add(table, 0);
	int temp;
	for(int i=1;i<m;i++){
		table = add(table, get(table, i-1));
		temp = get(table, i);
		while(temp > 0 && t[i] != t[i-1]){
			if(temp <= i+1){
				table = set(table, i, get(table, temp-1));
				temp = get(table, i);
			}
		}
		if(t[i] == t[temp])
			table = set(table, i, temp+1);
	}
	return table;
}

void KMP_sequential(char* s, char* t, ArrayList table){
	int result;
	int n = strlen(s);
	int m = strlen(t);
	int seen = 0;
	for(int i=0;i<n;i++){
		while(seen > 0 && s[i] != t[seen])
			seen = get(table, seen-1);
		if(s[i] == t[seen]) seen++;
		if(seen == m){
			printf("this is matching %d.\n", i-m+1);
			seen = 0;
		}
	}
}

void bruteforce_sequential(char* s, char* t){
	int n = strlen(s);
	int m = strlen(t);
	int j;
	for(int i=0;i<n-m+1;i++){
		for(j=i;j<m+i;j++){
			if(s[j] != t[j-i]) break;
		}
		if(j == m+i) printf("this is matching %d\n", j-m);
	}
}
*/
