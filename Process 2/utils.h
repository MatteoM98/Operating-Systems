#define _GNU_SOURCE
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//prototipi
int *random_array(int n);
int intcmpadre(const void *a, const void *b);
void stampa_array(int *a,int n);
bool check_sort(int *a, int n);
int intcmpfiglio(const void *a, const void *b);
