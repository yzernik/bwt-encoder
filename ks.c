/**
 * @file   ks.c
 * @Author Jonathan Zernik
 * @date   2012
 * @brief  Implementation of the KS
 * Algorithm for direct suffix sorting.
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "ks.h"


/*
 * Given a string of length n, copies suffix array 
 * into an int array of length n+1.
 */
void ks(int *sa, char *in, int n)
{
  int i;
  int t[n+2];
  for(i = 0; i < n; i++)
    {
      t[i] = (char)in[i];
    }
  t[n] = 0;
  t[n+1] = 0;
  ks_helper(sa, t, n);
}


/*
 * Given an int array of length n, copies suffix array 
 * into an int array of length n+1.
 */
void ks_helper(int *sa, int *t, int n)
{
  int b0_size, b1_size, b2_size, r_len;

  /*
   * Calculate the size of sets B0, B1, B2.
   */
  b0_size = ((int)((n)/3))+1;
  b1_size = ((int)((n-1)/3))+1;
  b2_size = ((int)((n-2)/3))+1;
  r_len = b1_size + b2_size;

  /*
   * Get the dc3 R string.
   */
  triple_char_t r[r_len];
  r_string(r, t, n, b1_size, b2_size);

  /*
   * Get the dc3 R prime list.
   */
  int rp[r_len+2];
  r_prime(rp, r, r_len);

  /*
   * Get the dc3 sample suffix array.
   */
  int sample_sa[r_len+1];
  sample_suffix_array(sample_sa, rp, r_len);

  /*
   * Get the ranks of known suffixes.
   */
  int rank_si[n+3];
  set_rank_si(rank_si, n, sample_sa, b1_size, b2_size);

  /*
   * Sort the sample and non-sample suffixes
   * and merge them.
   */
  int ss[b1_size + b2_size];
  sorted_sample(ss, sample_sa, b1_size, b2_size);
  int sn[b0_size];
  sorted_nonsample(sn, t, rank_si, b0_size);
  merge_suffixes(sa, ss, sn, t, rank_si, b0_size, b1_size, b2_size);
}


/*
 * Copies a dc3 string (R) into r.
 */
void r_string(triple_char_t *r, int *t, int n, int b1_size, int b2_size)
{
  int i, k;
  for(i = 0; i < b1_size; i++)
    {
      k = 1;
      r[i].chars[0] = (k+3*i) < n ? t[k+3*i] : 0;
      r[i].chars[1] = (k+1+3*i) < n ? t[k+1+3*i] : 0;
      r[i].chars[2] = (k+2+3*i) < n ? t[k+2+3*i] : 0;
    }
  for(i = 0; i < b2_size; i++)
    {
      k = 2;
      r[b1_size+i].chars[0] = (k+3*i) < n ? t[k+3*i] : 0;
      r[b1_size+i].chars[1] = (k+1+3*i) < n ? t[k+1+3*i] : 0;
      r[b1_size+i].chars[2] = (k+2+3*i) < n ? t[k+2+3*i] : 0;
    }
  for(i = 0; i < (b1_size + b2_size); i++)
    {
      r[i].pos = i;
    }
}


/*
 * Copies ranks of radix-sorted R string 
 * of length n into an int array 
 * of length n.
 */
void r_prime(int *rp, triple_char_t *r, int r_len)
{
  triple_char_t radixes[3+1][r_len];
  int i, k, c, pos, copies;
  int lowest, highest;

  memcpy(radixes[0], r, r_len*sizeof(triple_char_t));
  for(k = 0; k < 3; k++)
    {
      lowest = highest = 0;
      for(i = 0; i < r_len; i++)
	{
	  if(radixes[k][i].chars[2-k] < lowest)
	    {
	      lowest = radixes[k][i].chars[2-k];
	    }
	  if(radixes[k][i].chars[2-k] > highest)
	    {
	      highest = radixes[k][i].chars[2-k];
	    }
	}
      pos = 0;
      for (c = lowest; c <= highest; c++)
	{
	  for(i = 0; i < r_len; i++)
	    {
	      if(radixes[k][i].chars[2-k] == c)
		{
		  memcpy(&radixes[k+1][pos], &radixes[k][i], sizeof(triple_char_t));
		  pos++;
		}
	    }
	}
    }

  copies = 0;
  for(i = 0; i < r_len; i++)
    {
      if((i == 0) || (memcmp(&radixes[3][i], &radixes[3][i-1], 3*sizeof(int)) != 0))
	{
	  copies++;
	}
      rp[radixes[3][i].pos] = copies;
    }
  rp[r_len] = 0;
  rp[r_len+1] = 0;
}


/*
 * Given an  R prime (an int array), copies 
 * values of the sample suffix array 
 * into an int array.
 */
void sample_suffix_array(int *sample_sa, int *rp, int r_len)
{
  int i, duplicate;

  duplicate = 1;
  for(i = 0; i < r_len; i++)
    {
      if(rp[i] == r_len)
	{
	  duplicate = 0;
	}
    }

  if(duplicate)
    {
      ks_helper(sample_sa, rp, r_len);
    }
  else
    {
      sample_sa[0] = r_len;
      for(i = 0; i < r_len; i++)
	{
	  sample_sa[rp[i]] = i;
	}
    }
}


/*
 * Given a sample suffix array (an int array), 
 * copies ranks of all known suffixes to 
 * an int array.
 */
void set_rank_si(int *rank_si, int n, int *sample_sa, int b1_size, int b2_size)
{
  int i;
  for(i = 0; i <= n+2; i++)
    {
      rank_si[i] = -1;
    }
  for(i = 0; i <= b1_size + b2_size; i++)
    {
      if(sample_sa[i] < b1_size + b2_size)
	{
	  if(sample_sa[i] < b1_size)
	    {
	      rank_si[3*(sample_sa[i])+1] = i;
	    }
	  else
	    {
	      rank_si[3*((sample_sa[i] - b1_size))+2] = i;
	    }
	}
    }
  rank_si[n+1] = 0;
  rank_si[n+2] = 0;
}


/*
 * Copy the sorted sample suffixes
 * into ss
 */
void sorted_sample(int *ss, int *sample_sa, int b1_size, int b2_size)
{
  int i;
  for(i = 0; i < b1_size + b2_size; i++)
    {
      if(sample_sa[i+1] < b1_size + b2_size)
	{
	  if(sample_sa[i+1] < b1_size)
	    {
	      ss[i] = 3*(sample_sa[i+1])+1;
	    }
	  else
	    {
	      ss[i] = 3*((sample_sa[i+1] - b1_size))+2;
	    }
	}
    }
}


/*
 * Copy the sorted nonsample suffixes
 * into sn
 */
void sorted_nonsample(int *sn, int *t, int *rank_si, int b0_size)
{
  int i, pos, rank, c;
  int lowest, highest;

  int unsorted[b0_size];
  for(i = 0; i < b0_size; i++)
    {
      unsorted[i] = i*3;
    }
  
  int ranksorted[b0_size];
  pos = 0;
  lowest = 0;
  highest = b0_size*3 + 2;
  for(rank = lowest; rank < highest; rank++)
    {
      for(i = 0; i < b0_size; i++)
	{
	  if(rank_si[unsorted[i]+1] == rank)
	    {
	      ranksorted[pos] = unsorted[i];
	      pos++;
	    }
	}
    }

  lowest = highest = 0;
  for(i = 0; i < b0_size; i++)
    {
      if(t[ranksorted[i]] < lowest)
	{
	  lowest = t[ranksorted[i]];
	}
      else if(t[ranksorted[i]] > highest)
	{
	  highest = t[ranksorted[i]];
	}
    }
  pos = 0;
  for(c = lowest; c <= highest; c++)
    {
      for(i = 0; i < b0_size; i++)
	{
	  if(t[ranksorted[i]] == c)
	    {
	      sn[pos] = ranksorted[i];
	      pos++;
	    }
	}
    }
}


/*
 * Merge sorted suffixes for indices that
 * are multiples of 3 and non multiples of 3.
 */
void merge_suffixes(int *sa, int *ss, int *sn, int *t, int *ranks, int b0_size, int b1_size, int b2_size)
{
  int i, j, pos;

  i = j = 0;
  pos = 0;
  while((i < (b1_size + b2_size)) || (j < b0_size))
    {
      if(!(i < (b1_size + b2_size)))
	{
	  sa[pos] = sn[j];
	  j++;
	}
      else if(!(j < b0_size))
	{
	  sa[pos] = ss[i];
	  i++;
	}
      else
	{
	  if(dc3_compare(ss[i], sn[j], t, ranks) < 0)
	    {
	      sa[pos] = ss[i];
	      i++;
	    }
	  else
	    {
	      sa[pos] = sn[j];
	      j++;
	    }
	}
      pos++;
    }
}


/*
 * Comparison function for merging the 
 * sample and non sample suffixe indices.
 */
int dc3_compare(int i, int j, int *t, int *ranks)
{
  if(i%3 == 1)
    {
      if(t[i] != t[j])
	{
	  return t[i] - t[j];
	}
      else if(ranks[i+1] != ranks[j+1])
	{
	  return ranks[i+1] - ranks[j+1];
	}
      else
	{
	  return 0;
	}
    }
  else if(i%3 == 2)
    {
      if(t[i] != t[j])
	{
	  return t[i] - t[j];
	}
      if(t[i+1] != t[j+1])
	{
	  return t[i+1] - t[j+1];
	}
      else if(ranks[i+2] != ranks[j+2])
	{
	  return ranks[i+2] - ranks[j+2];
	}
      else
	{
	  return 0;
	}
    }
  else
    {
      printf("something went wrong.\n");
    }
  return 0;
}
