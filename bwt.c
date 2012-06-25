/**
 * @file   bwt.c
 * @Author Jonathan Zernik
 * @date   2012
 * @brief  Burrows Wheeler Transform
 * encode and decode functions.
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "ks.h"
#include "bwt.h"


/*
 * Copy the encoded Burrows Wheeler Transform 
 * of input string into the output string.
 */
void encode_bwt(char *out, char* in, int n)
{
  int i;
  int suff_array[n+1];
  ks(suff_array, in, n);
  for(i = 0; i < n; i++)
    {
      out[i] = in[(suff_array[i+1]-1+n)%n];
    }
  out[n] = '\0';
}


/*
 * Copy the decoded Burrows Wheeler Transform 
 * of input string into the output string.
 */
void decode_bwt(char *out, char* in, int n)
{
  int i, v_k;
  char *memloc;

  char sorted_in[n+1];
  strcpy(sorted_in, in);
  qsort(sorted_in, n, sizeof(char), lex_compare);
  
  int M[128];
  int N[strlen(in)];
  int V[strlen(in)];

  for(i = 0; i < n; i++)
    {
      memloc = strchr(sorted_in, sorted_in[i]);
      M[(int)(sorted_in[i])] = (memloc-sorted_in)/(sizeof(char));
    }

  int c_counts[128] = {0};
  for(i = 0; i < n; i++)
    {
      N[i] = c_counts[(int)(in[i])];
      c_counts[(int)(in[i])]++;
    }

  for(i = 0; i < n; i++)
    {
      V[i] = M[(int)(in[i])] + N[i];
    }

  v_k = 0;
  for(i = 0; i < n; i++)
    {
      out[n-i-1] = sorted_in[v_k];
      v_k = V[v_k];
    }
  out[n] = '\0';
}


/*
 * Compare chars lexicographically
 */
int lex_compare(const void * a, const void * b)
{
  return (*(char*)a - *(char*)b);
}
