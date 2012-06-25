#ifndef KS_H
#define KS_H

//#include "list.h"


/*
 * three chars representing a single dc3 char
 */
struct triple_char
{
  int chars[3];
  int pos;
};
typedef struct triple_char triple_char_t;


/*
 * Given a string of length n, copies suffix array 
 * into an int array of length n+1.
 */
void ks(int *sa, char *str, int n);


/*
 * Given an int array of length n, copies suffix array 
 * into an int array of length n+1.
 */
void ks_helper(int *sa, int *str, int n);


/*
 * Copies a dc3 string (R) into r.
 */
void r_string(triple_char_t *r, int *t, int n, int r1_len, int r2_len);


/*
 * Copies ranks of radix-sorted R string 
 * of length n into an int array 
 * of length n.
 */
void r_prime(int *rp, triple_char_t *r, int r_len);


/*
 * Given an  R prime (an int array), copies 
 * values of the sample suffix array 
 * into an int array.
 */
void sample_suffix_array(int *sample_sa, int *rp, int r_len);


/*
 * Given a sample suffix array (an int array), 
 * copies ranks of all known suffixes to 
 * an int array.
 */
void set_rank_si(int *rank_si, int n, int *sample_sa, int r1_len, int r2_len);


/*
 * Copy the sorted sample suffixes
 * into ss
 */
void sorted_sample(int *ss, int *sample_sa, int b1_size, int b2_size);


/*
 * Copy the sorted nonsample suffixes
 * into sn
 */
void sorted_nonsample(int *sn, int *t, int *rank_si, int b0_size);


/*
 * Merge sorted suffixes for indices that
 * are multiples of 3 and non multiples of 3.
 */
void merge_suffixes(int *sa, int *ss, int *sn, int *t, int *ranks, int b0_size, int b1_size, int b2_size);


/*
 * Comparison function for merging the 
 * sample and non sample suffixe indices.
 */
int dc3_compare(int i, int j, int *t, int *ranks);


#endif
