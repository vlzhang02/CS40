/*
 *      restoration.c
 *      HW01: filesofpix
 *      by: Valerie Zhang (vzhang02) and Siara Small (ssmall04)
 *      date: Sept 22, 2022
 *
 *      Purpose: reads in corrupted plain pgm file, restores lines, and creates
 *               "raw" pgm file with restored line
 *                      
 */
 
#include "readaline.h"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <atom.h>
#include <list.h>
#include <table.h>
#include <assert.h>
#include <pnmrdr.h>

extern FILE *openFile(char* filename);
extern void read_and_fill(FILE *fp, const char **key, Table_T table, 
                          Table_T size_table);
extern int find_pattern(char *input, char **pattern, size_t *length);
extern void print_line(char *l, size_t length);
extern void add_to_tables(char **pattern, Table_T table, Table_T size_table,
                          char **line, const char ***correct_pattern, 
                          size_t *size, int *length);
extern void apply_free_List(void **ptr, void *cl);
extern void apply_free(const void *key, void **list, void *cl);
extern void apply_print(void **line, void *cl);
extern List_T restore_line(List_T line_list, List_T size_list, int *width);
extern void print_ints(void **line, void *cl);
extern void convert_and_make(List_T list, int* width);
extern void make_raster (char *line);
extern void freeing(Table_T line_table, Table_T size_table, const char *key);

int main(int argc, char* argv[])
{
        assert(argc <= 2); /* asserts correct number of command line args */
        FILE *fp;
        if (argc == 1) { /* if file is not provided, use standard input*/
                char filename[1000];
                scanf("%s", filename);
                fp = openFile(filename);
        } else { /* if file is provided */
                fp = openFile(argv[1]);
        }
        const char *key;

        Table_T pattern_table = Table_new(1000, NULL, NULL);
        Table_T size_table = Table_new(1000, NULL, NULL);
        assert(pattern_table != NULL); /* asserts memory was allocated */
        assert(size_table != NULL);
        read_and_fill(fp, &key, pattern_table, size_table); /* fill tables */

        List_T line_list = Table_get(pattern_table, key); /* unrestored */
        List_T size_list = Table_get(size_table, key);

        int width = 0; /* will track desired width of raw pgm file */
        line_list = restore_line(line_list, size_list, &width);
        convert_and_make(line_list, &width);

        freeing(pattern_table, size_table, key); /* free all allocated memory */
        exit(EXIT_SUCCESS); }
        
/*
 * openFile()
 * 
 * Purpose:     opens inputed file and checks if file is open
 * Parameters:  filename
 * Effects:     opens file and checks to see if file successfully opened
 * Returns:     an initialized FILE object
 */
FILE *openFile(char* filename) 
{
        FILE *fp;
        fp = fopen(filename, "r");
        assert(fp != NULL); /* makes sure file is not empty */
        return fp; }

/*
 * read_and_fill()
 * 
 * Purpose:     reads in entire file and adds line to table
 * Parameters:  FILE object, const char double pointer, table
 * Effects:     - reads in all lines in a file
                - finds the pattern for each line and adds to corresponding 
                  table entry 
                - finds correct pattern and stores in char **
 * Returns:     none
 */
void read_and_fill(FILE *fp, const char **key, Table_T line_table, 
                   Table_T size_table) 
{
        size_t size_temp = 0;
        char *pattern;
        char *datapp;
        while ((size_temp = readaline(fp, &datapp)) && (datapp != NULL)) {
                size_t *size = malloc(sizeof(size_t));
                assert(size != NULL);
                *size = size_temp;
                int p_size = find_pattern(datapp, &pattern, size);
                /* adds line to table */
                add_to_tables(&pattern, line_table, size_table, 
                                &datapp, &key, size, &p_size); 
        }
        fclose(fp); }

/*
 * 
 * Purpose:     finds pattern of non-digit characters within a line
 * Parameters:  pointer to line, double pointer to pattern, length of line
 * Effects:     sets pattern pointer to initialized and allocated c string 
 *              with pattern
 * Returns:     none
 */
int find_pattern(char *input, char **patternpp, size_t *length) 
{
        int count = 0;
        int size = *length;
        char *pattern = malloc(size * sizeof(char)); 
        int i = 0;
        assert(pattern != NULL); /* asserts memory was allocated */
        while (input[i] != '\n') { 
                if ((input[i] < 48) || (input[i] > 57)) { /* if isn't digit */
                        pattern[count] = input[i];
                        count++; 
                }
                i++;
        }
        pattern[count] = '\n';
        *patternpp = pattern; 
        return count; 
}

/*
 * add_to_tables()
 * 
 * Purpose:     determines if an instance of the given pattern exists as a 
                key in the table; if not, add instance to table with 
                original line as value. if so, update the value list to 
                include the given original line.
 * Parameters:  double pointer to char array of the non-digit pattern, table, 
                double pointer to char array of the original line,  and 
                a reference to the correct_pattern to be updated, size of line
                returned by readaline 
 * Returns:     none
 *
 */
void add_to_tables(char **pattern, Table_T table, Table_T size_table, 
                   char **line, const char ***correct_pattern, 
                   size_t *size, int *length) 
{
        const char *key = Atom_new(*pattern, *length); /* chars -> atom */
        free(*pattern);
        List_T line_value = Table_get(table, key);
        List_T size_list = Table_get(size_table, key);
        if (line_value != NULL) { /* if this key exists, update value list */
                
                line_value = List_push(line_value, *line); 
                size_list = List_push(size_list, size);
                Table_put(table, key, line_value);
                Table_put(size_table, key, size_list);
                **correct_pattern = key;
        } else { /* if there isn't an instance of pattern, add as key */
                line_value = List_list(*line, NULL);
                size_list = List_list(size, NULL);
                Table_put(table, key, line_value);
                Table_put(size_table, key, size_list);
        } }

/*
 * restore_line()
 * 
 * Purpose:     restore a list of unrestored lines
 * Parameters:  list of unrestored lines, list of corresponding line sizes,
                 and pointer to width to update in caller
 * Effects:     updates width with the number of integers in the restored lines
 * Returns:     List of restored lines
 *
 */
List_T restore_line(List_T line_list, List_T size_list, int *width) 
{
        char *line;
        int *size;
        int length = List_length(line_list);
        List_T new_llist = List_list(NULL); /* list of restored lines */
        while (length != 0) {
                *width = 0;
                line_list = List_pop(line_list, (void **)&line);
                size_list = List_pop(size_list, (void **)&size);
                length--;
                bool previous = false; /* tracks if prev char is a digit */
                int i;
                for (i = 0; i < *size; i++) {
                        if ((line[i] > 47) && (line[i] < 58)) { 
                                previous = true;
                        } else { /* curr char is non-digit- make whitespace */
                                if (line[i] != '\n') {
                                        line[i] = ' ';
                                }
                                if (previous == true) { 
                                        (*width)++;        
                                        previous = false; 
                                }      
                        }
                }
                if (previous == true) { /* takes care of last value */
                        (*width)++;
                }
                if (new_llist == NULL) {
                        new_llist = List_list(line, NULL);
                } else {
                        new_llist = List_push(new_llist, line);
                }
                free(size);
        }
        return new_llist; }

/*
 * convert_and_make()
 * 
 * Purpose:     makes a raster of restored lines
 * Parameters:  list of restored lines represented by int arrays and a 
                pointer to the width of the raw pgm file
 * Effects:     -creates a raw pgm file with header
                -passes lines to be added to the raster
 * Returns:     none
 *
 */
void convert_and_make(List_T list, int *width) 
{
        (void) width;
        printf("%s ", "P5"); /* pgm header */
        printf("%d %d %d\n", *width, List_length(list), 255);
        char *line;       
        int length = List_length(list);
        while (length != 0) { /* iterate through list of restored lines */
                list = List_pop(list, (void **)&line);
                length--;
                make_raster(line);
                free(line);
        }
        }

/*
 * make_raster()
 * 
 * Purpose:     converts given int array into binary and adds to the raw pgm 
                file in standard output
 * Parameters:  restored line as an array
 * Effects:     -passes line of raw pgm file to standard output
 * Returns:     none
 *
 */
void make_raster (char *line) 
{
        int num = -1; /* defaults to dummy value to verify first pixel */
        bool prev = false; /* keeps track of if previous char was a digit */
        int i = 0;
        while (line[i] != '\n') { /* iterate until length */
                if ((line[i] >= '0') && (line[i] <= '9')) { /* is digit */
                        if (prev == true) { /* convert to multi-dig */
                                 num = (num * 10) + (line[i] - 48); 
                        } else { /* convert to digit & add to arr */
                                num = line[i] - 48; 
                                prev = true;
                        }
                 } else { /* whitespace- add prev num to pgm */
                        if ((prev == true) && (num > -1)) { 
                                
                                putchar(num); /* add to pgm file */
                        }
                        prev = false;
                }
                i++;
        }
        if ((prev == true)) {               
                putchar(num); /* add to pgm file */
        }
}

/*
 * freeing()
 * Purpose:     frees any allocated memory 
 * Parameters:  line and size tables, pointer to correct key
 * Effects:     - removes entries in tables with already freed lists
 *              - deallocates values in tables
 *              - deallocates tables
 * Returns:     none
 */
void freeing(Table_T line_table, Table_T size_table, const char *key)
{
        Table_remove(line_table, key);
        Table_remove(size_table, key);
        Table_map(line_table, apply_free, NULL); /* free allocated mem */
        Table_free(&line_table);
        Table_map(size_table, apply_free, NULL); /* free allocated mem */
        Table_free(&size_table);
}

/*
 * print_line()
 * 
 * Purpose:     prints line- used in debugging
 * Parameters:  pointer to char array and lenghth of char array
 * Effects:     prints contents of char array within []
 * Returns:     none
 */
void print_line(char *l, size_t length)
{
        fprintf(stderr, "[");
        for (size_t i = 0; i < length; i++) {
                fprintf(stderr, "%c", l[i]);
        }
        fprintf(stderr, "]");
        fprintf(stderr, "\n"); }

/******************************************************************************
 *
 *
 *                  List_map() and Table_map() apply functions
 *
 *
 *****************************************************************************/
        /*---------------------------------------------------*
        |                                                   |
        |                printing functions                 |
        |                   (for testing)                   |
        *---------------------------------------------------*/

 /*
 * print_ints()
 * 
 * Purpose:     used in debugging- prints int array of restored line
 * Parameters:  double pointer to char array of line and void pointer
 * Returns:     none
 *
 */
void print_ints(void **line, void *cl) 
{
        int *arr = *(int**)line;
        fprintf(stderr, "[");
        int i = 0;
        while (arr[i] != -1) {
                fprintf(stderr, "%d ", arr[i]);
                i++;
        }
        fprintf(stderr, "]\n");
        (void) cl; }

/*
 * apply_print()
 * 
 * Purpose:     visualize debugging by printing each line to be used in 
                list_map
 * Parameters:  double pointer to line and void pointer
 * Returns:     none
 *
 */
void apply_print(void **line, void *cl) 
{
        char *l = *line;
        print_line(l, strlen(l));
        (void) cl; }

        /*---------------------------------------------------*
        |                                                    |
        |                freeing functions                   |
        |                                                    |
        *---------------------------------------------------*/

/*
 * apply_free_list()
 * 
 * Purpose:     frees all memory allocated for objects within a list    
 * Parameters:  void ** to object to be freed, closure/callback arg(not used)
 * Effects:     frees memory allocated by object stored in list
 * Returns:     none
 */
void apply_free_List(void **ptr, void *cl) 
{
        if (*ptr != NULL) {
                free(*ptr);
        }
        (void) cl; }

/*
 * apply_free()
 * 
 * Purpose:     frees all memory used by value(a list) in each table entry
 * Parameters:  pointer to key, double pointer to value(list), closure/
                callback arg (not used)
 * Effects:     frees objects in list and list for each table entry
 * Returns:     none
 */
void apply_free(const void *key, void **list, void *cl) 
{
        if (*list != NULL) {
                List_map(*list, apply_free_List, NULL);
                List_free((List_T *)list);
        } 
        (void) key;
        (void) cl; 
}

