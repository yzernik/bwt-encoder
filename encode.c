#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "bwt.h"
#include "encode.h"



int main(int argc, char *argv[])
{
  if(!((argc == 4) && ((strcmp(argv[1], "-bwt") == 0) || (strcmp(argv[1], "-ibwt") == 0))))
    {
      printf("usage:\n");
      printf("%s -bwt [inputfile] [outputfile]\n", argv[0]);
      printf("%s -ibwt [inputfile] [outputfile]\n", argv[0]);
    }
  else 
    {
      FILE *infile = fopen(argv[2], "r");
      FILE *outfile = fopen(argv[3], "w");
      if(infile == 0 || outfile == 0)
        {
	  printf("Could not open file\n");
        }
      else
	{
	  char *sequence = create_empty_str();
	  char line[100];
	  while((fgets(line, 100, infile) != NULL) && (line[0] != '>'))
	    {
	    }
	  while(fgets(line, 100, infile) != NULL)
	    {
	      if(line[strlen(line)-1] == '\n')
		{
		  line[strlen(line)-1] = '\0';
		}
	      sequence = extend_str(sequence, line);
	    }
	  char out[strlen(sequence)+1];
	  if(strcmp(argv[1], "-bwt") == 0)
	    {
	      encode_bwt(out, sequence, strlen(sequence));
	      write_fasta_file(outfile, "BWT", out);
	    }
	  else if(strcmp(argv[1], "-ibwt") == 0)
	    {
	      decode_bwt(out, sequence, strlen(sequence));
	      write_fasta_file(outfile, "iBWT", out);
	    }
	  free_str(sequence);
	  fclose(infile);
	  fclose(outfile);
	}
    }
  return 0;
}


/*
 * Allocate an empty string on
 * the heap
 */
char *create_empty_str()
{
  char *str;
  if ((str = malloc(1*(sizeof(char)))) == NULL) {
    printf("\nout of memory.\n");
  }
  str[0] = '\0';
  return str;
}

/*
 * Extend a string that is allocated
 * on the heap
 */
char *extend_str(char *str, char *new_str)
{
  int new_len = strlen(str) + strlen(new_str) + 1;
  if ((str = realloc(str, new_len*(sizeof(char)))) == NULL) {
    printf("\nout of memory.\n");
  }
  strcat(str, new_str);
  return str;
}

/*
 * Deallocate a string
 * on the heap
 */
void free_str(char *str)
{
  free(str);
}

/*
 * Write a fasta file given a
 * name and sequence data
 */
void write_fasta_file(FILE *fp, char *name, char *content)
{
  int i, count;
  fputc('>', fp);
  for(i = 0; i < strlen(name); i++)
    {
      fputc(name[i], fp);
    }
  fputc('\n', fp);

  i = 0;
  count = 0;
  for(i = 0; i < strlen(content); i++)
    {
      if(count == 80)
	{
	  fputc('\n', fp);
	  count = 0;
	}
      fputc(content[i], fp);
      count++;
    }
  fputc('\n', fp);
}

