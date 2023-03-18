#include "dictionary.h"
#include "record_struct.h"
#include "record_struct.c"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define NUMERIC_BASE 10
#define KEY_FIELD 1
#define NOTFOUND "NOTFOUND"
#define NOTDOUBLE (-1)
#define MAXPRECISION (-2)
#define NUM_FIELDS 19
#define QT_NODE_CAPACITY (4)
#define MAX_ARRAY_SIZE (1024)
#define FIELDLOOKUPFAILURE (-1)
#define INDEXINITIAL 1
#define APPROXIMATE_VALUE 0.000000000000001

// Field names by index.
static char *fieldNames[] = {"footpath_id", "address", "clue_sa", 
    "asset_type", "deltaz", "distance", "grade1in", "mcc_id", "mccid_int", 
    "rlmax", "rlmin", "segside", "statusid", "streetid", "street_group", 
    "start_lat", "start_lon", "end_lat", "end_lon"};

/* 

Field double precision by index. -1 signifies that the value should not
be printed as a double value.
*/
static int fieldPrecision[] = {NOTDOUBLE, NOTDOUBLE, NOTDOUBLE, 
    NOTDOUBLE, 2, 2, 1, NOTDOUBLE, NOTDOUBLE, 
    2, 2, NOTDOUBLE, NOTDOUBLE, NOTDOUBLE, NOTDOUBLE, 
    MAXPRECISION, MAXPRECISION, MAXPRECISION, MAXPRECISION};

struct queryResult {
    char *lon;
    char *lat;
    int numRecords;
    struct data **records;
    long double x_val;
    long double y_val;
};

/* CSV records. */
struct data;
struct data {
    int footpath_id;
    char *address;
    char *clue_sa;
    char *asset_type;
    double deltaz;
    double distance;
    double grade1in;
    int mcc_id;
    int mccid_int;
    double rlmax;
    double rlmin;
    char *segside;
    int statusid;
    int streetid;
    int street_group;
    double start_lat;
    double start_lon;
    double end_lat;
    double end_lon;
};

/* A node in the dictionary, used to allow quick lookup. */
struct dictionaryNode;

struct dictionaryNode{
    struct data *record;
    struct dictionaryNode *next;
};

struct index;

struct index {
    int valueCount;
    struct dictionaryNode **nodes;
};

/* Dictionary - concrete data structure is linked list. */
struct dictionary {
    struct dictionaryNode *head;
    struct dictionaryNode *tail;
    struct index **indices;
};

/* Reads a given string as an integer and returns the integer. */
int readIntField(char *fieldString);

/* Reads a given string as a string and returns a copy of the string. */
char *readStringField(char *fieldString);

/* Reads a given string as a double value and returns the double value. */
double readDoubleField(char *fieldString);

int readIntField(char *fieldString){
    return strtol(fieldString, NULL, NUMERIC_BASE);
}

char *readStringField(char *fieldString){
    char *str = strdup(fieldString);
    assert(str);
    return str;
}

double readDoubleField(char *fieldString){
    return strtod(fieldString, NULL);
}

/* Sets the relevant field. */
void setField(struct data *record, int fieldIndex, char *fieldVal);

void setField(struct data *record, int fieldIndex, char *fieldVal){
    
    
    switch(fieldIndex){
        case 0:
            record->footpath_id = readIntField(fieldVal);
            break;
        case 1:
            record->address = readStringField(fieldVal);
            break;
        case 2:
            record->clue_sa = readStringField(fieldVal);
            break;
        case 3:
            record->asset_type = readStringField(fieldVal);
            break;
        case 4:
            record->deltaz = readDoubleField(fieldVal);
            break;
        case 5:
            record->distance = readDoubleField(fieldVal);
            break;
        case 6:
            record->grade1in = readDoubleField(fieldVal);
            break;
        case 7:
            record->mcc_id = readIntField(fieldVal);
            break;
        case 8:
            record->mccid_int = readIntField(fieldVal);
            break;
        case 9:
            record->rlmax = readDoubleField(fieldVal);
            break;
        case 10:
            record->rlmin = readDoubleField(fieldVal);
            break;
        case 11:
            record->segside = readStringField(fieldVal);
            break;
        case 12:
            record->statusid = readIntField(fieldVal);
            break;
        case 13:
            record->streetid = readIntField(fieldVal);
            break;
        case 14:
            record->street_group = readIntField(fieldVal);
            break;
        case 15:
            record->start_lat = readDoubleField(fieldVal);
            break;
        case 16:
            record->start_lon = readDoubleField(fieldVal);
            break;
        case 17:
            record->end_lat = readDoubleField(fieldVal);
            break;
        case 18:
            record->end_lon = readDoubleField(fieldVal);
            break;
        default:
            fprintf(stderr, "%d: Unhandled field number %d\n", __LINE__, 
                fieldIndex);
            assert(fieldIndex >= 0 && fieldIndex < NUM_FIELDS);
    }



}

/* Returns the double value for the given field index. */
double getDoubleVal(struct data *record, int fieldIndex);

double getDoubleVal(struct data *record, int fieldIndex){
    switch(fieldIndex){
        case 4:
            return record->deltaz;
            break;
        case 5:
            return record->distance;
            break;
        case 6:
            return record->grade1in;
            break;
        case 9:
            return record->rlmax;
            break;
        case 10:
            return record->rlmin;
            break;
        case 15:
            return record->start_lat;
            break;
        case 16:
            return record->start_lon;
            break;
        case 17:
            return record->end_lat;
            break;
        case 18:
            return record->end_lon;
            break;
        case 0:
        case 1:
        case 2:
        case 3:
        case 7:
        case 8:
        case 11:
        case 12:
        case 13:
        case 14:
            fprintf(stderr, "%d: Unhandled non-double field number %d\n", __LINE__, 
                fieldIndex);
            assert(0);
            break;
        default:
            fprintf(stderr, "%d: Unhandled field number %d\n", __LINE__, 
                fieldIndex);
            assert(fieldIndex >= 0 && fieldIndex < NUM_FIELDS && 0);
    }
    return FIELDLOOKUPFAILURE;
}

/* Prints a given value. */
void printIntField(FILE *f, int value);

/* Prints a given value. */
void printStringField(FILE *f, char *value);

/* Prints a given value. */
void printDoubleField(FILE *f, double value, int fieldPrecision);

void printIntField(FILE *f, int value){
    fprintf(f, "%d", value);
}

void printStringField(FILE *f, char *value){
    fprintf(f, "%s", value);
}

void printDoubleField(FILE *f, double value, int fieldPrecision){
    fprintf(f, "%.*f", fieldPrecision, value);
}

/* Prints the relevant field. */
void printField(FILE *f, struct data *record, int fieldIndex);

void printField(FILE *f, struct data *record, int fieldIndex){
    switch(fieldIndex){
        case 0:
            printIntField(f, record->footpath_id);
            break;
        case 1:
            printStringField(f, record->address);
            break;
        case 2:
            printStringField(f, record->clue_sa);
            break;
        case 3:
            printStringField(f, record->asset_type);
            break;
        case 4:
            printDoubleField(f, record->deltaz, fieldPrecision[fieldIndex]);
            break;
        case 5:
            printDoubleField(f, record->distance, fieldPrecision[fieldIndex]);
            break;
        case 6:
            printDoubleField(f, record->grade1in, fieldPrecision[fieldIndex]);
            break;
        case 7:
            printIntField(f, record->mcc_id);
            break;
        case 8:
            printIntField(f, record->mccid_int);
            break;
        case 9:
            printDoubleField(f, record->rlmax, fieldPrecision[fieldIndex]);
            break;
        case 10:
            printDoubleField(f, record->rlmin, fieldPrecision[fieldIndex]);
            break;
        case 11:
            printStringField(f, record->segside);
            break;
        case 12:
            printIntField(f, record->statusid);
            break;
        case 13:
            printIntField(f, record->streetid);
            break;
        case 14:
            printIntField(f, record->street_group);
            break;
        case 15:
            printDoubleField(f, record->start_lat, fieldPrecision[fieldIndex]);
            break;
        case 16:
            printDoubleField(f, record->start_lon, fieldPrecision[fieldIndex]);
            break;
        case 17:
            printDoubleField(f, record->end_lat, fieldPrecision[fieldIndex]);
            break;
        case 18:
            printDoubleField(f, record->end_lon, fieldPrecision[fieldIndex]);
            break;
        default:
            fprintf(stderr, "%d: Unhandled field number %d\n", __LINE__, 
                fieldIndex);
            assert(fieldIndex >= 0 && fieldIndex < NUM_FIELDS);
    }
}

// Initialises a new dictionary to store all the structs of records
struct dictionary *newDict(){
    struct dictionary *ret = (struct dictionary *) 
        malloc(sizeof(struct dictionary));
    assert(ret);
    ret->head = NULL;
    ret->tail = NULL;
    ret->indices = NULL;
    return ret;
}

/* Read a record into a typed data structure. */
struct data *readRecord(struct csvRecord *record);

struct data *readRecord(struct csvRecord *record){
    struct data *ret = (struct data *) malloc(sizeof(struct data));
    assert(ret);
    assert(record->fieldCount == NUM_FIELDS);
    for(int i = 0; i < NUM_FIELDS; i++){
        setField(ret, i, record->fields[i]);
        
    }
    return ret;
};

// Inserts a struct representing a record into the dictionary and adds the coordinate points into the quadtree
void insertRecord(struct dictionary *dict, struct csvRecord *record, QuadTree *qt){
    if(! dict){
        return;
    }
    struct dictionaryNode *newNode = (struct dictionaryNode *) 
        malloc(sizeof(struct dictionaryNode));
    assert(newNode);
    newNode->record = readRecord(record);
    
    // Create a new point with the start latitude and longitude of the record as coordinates
    point2D *start_p = create_point(newNode->record->start_lon, newNode->record->start_lat);

    // Insert the point into existing quadtree
    addPoint(qt,start_p);

    // Create another point with the end latitude and longitude of the record as coordinates
    point2D *end_p = create_point(newNode->record->end_lon, newNode->record->end_lat);

    // Insert the point into existing quadtree
    addPoint(qt,end_p);
    newNode->next = NULL;

    if(! (dict->head)){
        /* First insertion, insert new node as head and tail. */
        dict->head = newNode;
        dict->tail = newNode;
    } else {
        /* Insert at tail. */
        dict->tail->next = newNode;
        dict->tail = newNode;
    }
}

/* Search for a given key in the dictionary. */
struct queryResult *lookupRecord(struct dictionary *dict, char *query){
    int numRecords = 0, ctr=0;
    struct data **records = NULL;
    char* lat="";
    char* lon="";
    long double search_lat, search_lon;

    // Extract the coordinates of the point to be searched from the query
    char* token = strtok(query, " ");
    while( token != NULL ) {
      ctr++;

      if (ctr==1) {
         lon=strdup(token);

      } else if (ctr==2) {
         lat=strdup(token);

      }
      token = strtok(NULL, " ");

   }

    // Convert the coordinates from string to long double format
    search_lat = strtod(lat, NULL);
    search_lon = strtod(lon, NULL);

    /* Iterate over all records and collect all matching records. */
    struct dictionaryNode *current = dict->head;
    while(current){
        if(((current->record->start_lat == search_lat) || (current->record->end_lat == search_lat)) && 
        ((current->record->start_lon ==  search_lon) || (current->record->end_lon == search_lon))){

            /* Match. */
            records = (struct data **) realloc(records, 
                sizeof(struct data *) * (numRecords + 1));
            assert(records);
            records[numRecords] = current->record;
            numRecords++;
        }
        current = current->next;
    }

    struct queryResult *qr = (struct queryResult *) 
        malloc(sizeof(struct queryResult));
    assert(qr);

    qr->lon = lon;
    assert(qr->lon);
    qr->lat = lat;
    assert(qr->lat);
    qr->numRecords = numRecords;
    qr->records = records;
    qr->x_val = search_lon;
    qr->y_val = search_lat;

    return qr;
}

/* Output the given query result. */
void printQueryResult(struct queryResult *r, FILE *summaryFile, 
    FILE *outputFile, int stage,QuadTree *qt){


    /* Print details. */
    fprintf(outputFile, "%s %s\n", r->lon, r->lat);
    for(int i = 0; i < r->numRecords; i++){
        fprintf(outputFile, "--> ");
        for(int j = 0; j < NUM_FIELDS; j++){
            fprintf(outputFile, "%s: ", fieldNames[j]);
            printField(outputFile, r->records[i], j);
            fprintf(outputFile, " || ");
        }
        fprintf(outputFile, "\n");
    }

  
    /* Print summary. */
    if(stage == REGIONQUERY){
        if(r->numRecords == 0){
            fprintf(summaryFile, "%s %s --> %s\n", r->lon, r->lat, NOTFOUND);
        } else {
            fprintf(summaryFile, "%s %s --> ", r->lon, r->lat);
        }

    } 

    // Convert the point to be searched into a rectangle with coordinates
    point2D *center_r = create_point(r->x_val, r->y_val);
    rectangle2D *boundary_r = create_rectangle(center_r, APPROXIMATE_VALUE, APPROXIMATE_VALUE);
    
    /* Search for the point within the constructed quadtree
    and return the path traversed with the quadrant names */
    point2D **res = searchPoint(qt, boundary_r,center_r);
    
    size_t j = 0;
    while (res[j] != NULL && j < MAX_ARRAY_SIZE) {

        // Print path of traversal until point was found
        determineQuadrant(qt->boundary, res[j],summaryFile);
        j++;
    }
}

/* Free the given query result. */
void freeQueryResult(struct queryResult *r){
    if(! r){
        return;
    }
    free(r->records);
    free(r->lon);
    free(r->lat);
    free(r);
}

void freeData(struct data *d){
    if(! d){
        return;
    }
    if(d->address){
        free(d->address);
    };
    if(d->clue_sa){
        free(d->clue_sa);
    };
    if(d->asset_type){
        free(d->asset_type);
    };
    if(d->segside){
        free(d->segside);
    };
    free(d);
}

/* Free a given dictionary. */
void freeDict(struct dictionary *dict){
    if(! dict){
        return;
    }
    struct dictionaryNode *current = dict->head;
    struct dictionaryNode *next;
    while(current){
        next = current->next;
        freeData(current->record);
        free(current);
        current = next;
    }
    if(dict->indices){
        for(int i = 0; i < NUM_FIELDS; i++){
            if(dict->indices[i]){
                if(dict->indices[i]->nodes){
                    free(dict->indices[i]->nodes);
                }
                free(dict->indices[i]);
            }
        }
        free(dict->indices);
    }
    free(dict);
}

void Point_print(point2D *point) {
    printf("(%2.4Lf, %2.4Lf)\n", point->x, point->y);
}
