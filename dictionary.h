#include "record_struct.h"
#include <stdio.h>
#include "quadtree.h"


#define REGIONQUERY 3
#define PROXIMITYSTAGE 2

/* Result of a query. */
struct queryResult;

/* Dictionary. */
struct dictionary;

/* Returns an empty dictionary. */
struct dictionary *newDict();

/* Insert a given record into the dictionary. */
void insertRecord(struct dictionary *dict, struct csvRecord *record, QuadTree *qt);

/* Search for a given key in the dictionary. */
struct queryResult *lookupRecord(struct dictionary *dict, char *query);



/* Output the given query result */
void printQueryResult(struct queryResult *r, FILE *summaryFile, 
    FILE *outputFile, int stage,QuadTree *qt);

/* Free the given query result. */
void freeQueryResult(struct queryResult *r);

/* Free a given dictionary. */
void freeDict(struct dictionary *dict);

/* Print the coordinates of a given point */
void Point_print(point2D *point);
