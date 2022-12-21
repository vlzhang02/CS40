/*
 *      readaline.c
 *      HW01: filesofpix
 *      by: Valerie Zhang (vzhang02) and Siara Small (ssmall04), Sept 2022
 *
 *      Purpose: Implementation of readaline function to read a line from a 
 *               onto the heap.
 */

#include "readaline.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
 * readaline()
 * 
 * Purpose:     read in one line from a file and store in a char array on the 
                heap
 * Parameters:  input stream and double pointer to char array
 * Effects:     points datapp to the line
 * Returns:     size_t representing the size in bytes of the line
 */
size_t readaline(FILE *inputfd, char **datapp)
{ 
        assert((inputfd != NULL) && (datapp != NULL)); 
        size_t size = 1001;
        char *line = malloc(size * sizeof(char));
        assert(line != NULL); /* asserts memory was allocated */
        size_t num_bytes = 0;
        int c; 
        while ((c = fgetc(inputfd)) && (c != '\n')) {
                if (feof(inputfd)) { /* if EOF is reached */
                        free(line); /* free allocated memory */
                        *datapp = NULL; /* set datapp to NULL */
                        return 0;
                }
                if (num_bytes >= size - 1) { /* if line exceeds 1000 chars */
                        size = size * 3;
                        line = realloc(line, size * sizeof(char));
                        assert(line != NULL); /* assert correct reallocation */
                }
                line[num_bytes] = c; /* add to char array */
                num_bytes++;
        } 
        if (num_bytes == 0) {
                free(line);
                *datapp = NULL;
                return 0;
        }
        line[num_bytes] = '\n';
        num_bytes++;
        *datapp = line; /* point datapp to char array */
        return num_bytes; }