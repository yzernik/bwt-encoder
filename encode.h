#ifndef ENCODE_H
#define ENCODE_H


/*
 * Allocate an empty string on
 * the heap
 */
char *create_empty_str();


/*
 * Extend a string that is allocated
 * on the heap
 */
char *extend_str(char *str, char *new_str);


/*
 * Deallocate a string
 * on the heap
 */
void free_str(char *str);

/*
 * Write a fasta file given a
 * name and sequence data
 */
void write_fasta_file(FILE *fp, char *name, char *content);


#endif
