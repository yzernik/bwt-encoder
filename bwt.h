/**
 * @file   bwt.h
 * @Author Jonathan Zernik
 * @date   2012
 * @brief  Burrows Wheeler Transform
 * encode and decode functions.
 */


#ifndef BWT_H
#define BWT_H

#include "ks.h"


/*
 * Copy the encoded Burrows Wheeler Transform 
 * of input string into the output string.
 */
void encode_bwt(char *out, char* in, int n);


/*
 * Copy the decoded Burrows Wheeler Transform 
 * of input string into the output string.
 */
void decode_bwt(char *out, char* in, int n);


/*
 * Compare chars lexicographically
 */
int lex_compare(const void * a, const void * b);


#endif
