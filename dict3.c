#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "read.h"
#include "dictionary.h"


#define MINARGS 7
#define EXPECTED_STAGE "3"
#define STAGE (REGIONQUERY)

int main(int argc, char **argv){
    
    // Checks if seven command line arguments are present 
    if(argc < MINARGS){
        fprintf(stderr, "Insufficient arguments, run in form:\n"
                        "\t./dict1 1 <input dataset> <output file> < <keys file>\n");
        exit(EXIT_FAILURE);
    }

    // If wrong stage is initiated, exit the program 
    if (strcmp(EXPECTED_STAGE, argv[1]) != 0){
        fprintf(stderr, "Expected stage was %s, received %s\n", EXPECTED_STAGE, argv[1]);
        exit(EXIT_FAILURE);
    }

    char *temp;
    long double startLat, startLon, endLat, endLon;
    long double x_mid, y_mid;
    long double x_half, y_half;

    // Accepts filenames and x,y co-ordinate pairs of the root node area from command line arguments

    char *inputCSVName = argv[2];
    char *outputFileName = argv[3];

    // x,y coordinate pair of bottom left corner of root node area
    char *start_lon = argv[4];
    char *start_lat = argv[5];

    // x,y coordinate pair of top right corner of root node area
    char *end_lon = argv[6];
    char *end_lat = argv[7];

    
    // Converts the x,y coordinate values of the root node into long double form
    startLat = strtold (start_lat, &temp);
    temp = "";
    startLon = strtold (start_lon, &temp);
    temp = "";
    endLat = strtold (end_lat, &temp);
    temp = "";
    endLon = strtold (end_lon, &temp);
    temp = "";

    // x and y coordinates of the center point of the root node
    x_mid = (startLon+endLon)/2;
    y_mid = (startLat+endLat)/2;

    // Allocates a file pointer to open the input file and output file
    FILE *csvFile = fopen(inputCSVName, "r");
    assert(csvFile);
    FILE *outputFile = fopen(outputFileName, "w");
    assert(outputFile);

    
    int n;

    // Reads the csv file line by line and stores each record as a struct
    struct csvRecord **dataset = readCSV(csvFile, &n);

    // Create a dictionary to store all the structs
    struct dictionary *dict = newDict();

    // Creates the center point of the root node 
    point2D *center = create_point(x_mid, y_mid);

    x_half = x_mid - startLon;
    y_half = y_mid - startLat;

    // Forms the rectangle around the center point to create the root node rectangle
    rectangle2D *boundary = create_rectangle(center, x_half, y_half);

    // Inserts the rectangle as the root node of the quadtree
    QuadTree *qt = new_Quadtree(boundary);

    point2D *start_p=NULL;
    point2D *end_p=NULL;

    // Inserts each line of the CSV file as a record in the dictionary
    // Simoultaenously adds the coordinate pairs from the records into the quadtree 
    for(int i = 0; i < n; i++){
        insertRecord(dict, dataset[i],qt);   
    }

    free(start_p);
    free(end_p);
    freeCSV(dataset, n);

    
    char *query = NULL;

    // Gets each query line by line from the user
    while((query = getQuery(stdin))){

        // Search for the query within the dictionary
        struct queryResult *r = lookupRecord(dict, query);

        // Output the records matching the query
        printQueryResult(r, stdout, outputFile, STAGE,qt);
          
        freeQueryResult(r);
        free(query);
    }

    freeDict(dict);
    dict = NULL;
    free(center);

    fclose(csvFile);
    fclose(outputFile);

    return EXIT_SUCCESS;
}
