#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "dictionary.h"

#define QT_NODE_CAPACITY (4)
#define MAX_ARRAY_SIZE (1024)

// Creates a point using given coordinates and stores their values
point2D *create_point(long double x, long double y) {
    point2D *p = (point2D *)malloc(sizeof(point2D));
    p->x = x;
    p->y = y;
    return p;
}

// Specifies a rectangle given bottom-left 2D point and an upper right 2D point
rectangle2D *create_rectangle(point2D *center, long double x_half, long double y_half) {
    rectangle2D *rectangle = (rectangle2D *)malloc(sizeof(rectangle2D));

    // Center point of the rectangle
    rectangle->center = center;

    // Length and breadth of the rectangle divided by 2
    rectangle->x_half = x_half;
    rectangle->y_half = y_half;

    return rectangle;
}

// Tests whether a given 2D point lies within the rectangle and returns 1 (TRUE) if it does
int inRectangle(rectangle2D *boundary, point2D *point) {
    
    // Check if x coordinate of point lies within the rectangle boundary
    if (point->x < boundary->center->x - boundary->x_half || point->x > boundary->center->x + boundary->x_half) {

        return 0;
    }

    // Check if y coordinate of point lies within the rectangle boundary
    if (point->y < boundary->center->y - boundary->y_half || point->y > boundary->center->y + boundary->y_half) {

        return 0;
    }

    return 1;
}

// Returns the quadrant of the rectangle that the point lies in 
int determineQuadrant(rectangle2D *range, point2D *point,FILE *summaryFile) {

    // South-West Quadrant
    if (point->x <= range->center->x && point->y < range->center->y) {

        fprintf(summaryFile, "SW\n");
        return 1;
    }

    // North-West Quadrant
    if (point->x <= range->center->x && point->y >= range->center->y) {

        fprintf(summaryFile, "NW\n");
        return 2;
    }

    // North-East Quadrant
    if (point->x > range->center->x && point->y >= range->center->y) {

        fprintf(summaryFile, "NE\n");
        return 3;
    }

    // South-East Quadrant
    if (point->x > range->center->x && point->y < range->center->y) {

        fprintf(summaryFile, "SE\n");
        return 4;
    }

    return 0;
}

// Adds a point given with its 2D coordinates to the quadtree
int addPoint(QuadTree *root, point2D *point) {

    // If point does not lie within the root node
    if (!inRectangle(root->boundary, point)) {

        return 0;
    }
    
    size_t points_size = QuadTree_points_size(root->points);
    
    // Add points as leaf nodes to an internal node
    if (points_size < QT_NODE_CAPACITY && root->NW == NULL) {
        root->points[points_size] = point;
        return 1;
    }

    // Convert leaf node into internal node to insert more points
    if (root->NW == NULL) {
        create_quadNode(root);
    }

    // Recursively adds points to each quadrant of the root node
    if (addPoint(root->NW, point)) return 1;
    if (addPoint(root->NE, point)) return 1;
    if (addPoint(root->SW, point)) return 1;
    if (addPoint(root->SE, point)) return 1;

    return 0;
}

// Tests whether two rectangles overlap and returns 1 (TRUE) if they do 
int rectangleOverlap(rectangle2D *self, rectangle2D *other) {
    
    // Check if x-coordinates of one rectangle lie beyond the x-coordinates of the other rectangle 
    if (self->center->x + self->x_half > other->center->x - other->x_half) {
        return 1;
    }

    if (self->center->x - self->x_half < other->center->x + other->x_half) {
        return 1;
    }

    // Check if y-coordinates of one rectangle lie beyond the y-coordinates of the other rectangle 
    if (self->center->y + self->y_half > other->center->y - other->y_half) {
        return 1;
    }

    if (self->center->y - self->y_half < other->center->y + other->y_half) {
        return 1;
    }

    return 0;
}


// Creates a new QuadTree given the 2D coordinates of its upper left and bottom right points of its root node
QuadTree *new_Quadtree(rectangle2D *boundary) {
    QuadTree *qt = (QuadTree *)malloc(sizeof(QuadTree));
    qt->NE = NULL;
    qt->NW = NULL;
    qt->SE = NULL;
    qt->SW = NULL;

    qt->boundary = boundary;

    qt->points = (point2D **)malloc(sizeof(point2D*) * QT_NODE_CAPACITY);

    for (size_t i = 0; i < QT_NODE_CAPACITY; i++)
    {
        qt->points[i] = NULL;
    }

    return qt;
}

// Allocates space required for each of the 2D pointers referring to the child nodes SW, NW, NE and SE of the quadtree
size_t QuadTree_points_size(point2D *points[]) {
    size_t i;
    for (i = 0; i < QT_NODE_CAPACITY; i++)
    {
        if (points[i] == NULL) {
            return i;
        }
    }

    return i;
}

// Creates new children nodes for each internal node and further divides the root node rectangle to insert points
QuadTree *create_quadNode(QuadTree *root) {

    long double new_Xhalf = root->boundary->x_half / 2;
    long double new_Yhalf = root->boundary->y_half / 2;


    // North West
    point2D *nw_p = create_point(root->boundary->center->x - new_Xhalf, root->boundary->center->y + new_Yhalf);
    root->NW = new_Quadtree(create_rectangle(nw_p, new_Xhalf, new_Yhalf));
    
    // North East
    point2D *ne_p = create_point(root->boundary->center->x + new_Xhalf, root->boundary->center->y + new_Yhalf);
    root->NE = new_Quadtree(create_rectangle(ne_p, new_Xhalf, new_Yhalf));
    
    // South West
    point2D *sw_p = create_point(root->boundary->center->x - new_Xhalf, root->boundary->center->y - new_Yhalf);
    root->SW = new_Quadtree(create_rectangle(sw_p, new_Xhalf, new_Yhalf));
    
    // South East
    point2D *se_p = create_point(root->boundary->center->x + new_Xhalf, root->boundary->center->y - new_Yhalf);
    root->SE = new_Quadtree(create_rectangle(se_p, new_Xhalf, new_Yhalf));

    return root;
}

// Tests whether a datapoint given by its 2D coordinates lies within a quadtree and returns the datapoint along with its stored information 
point2D **searchPoint(QuadTree *root, rectangle2D *range, point2D *search) {
    
    point2D **result;
    result = (point2D **)malloc(sizeof(point2D *) * MAX_ARRAY_SIZE);

    size_t index = 0;
    for (size_t i = 0; i < MAX_ARRAY_SIZE; i++) {
        result[i] = NULL;
    }
    
    // If datapoint is not found within the quadtree
    if (!rectangleOverlap(root->boundary, range)) {
        return result;
    } 
    
    // Finds the number of points present within the rectangle 
    size_t points_size = QuadTree_points_size(root->points);
    for (size_t i = 0; i < points_size; i++)
    {
        // If datapoint present in the quadtree matches the datapoint that is being searched
        if (inRectangle(range, root->points[i])) {
            
            // Store the coordinates of the found datapoint
            result[index++] = root->points[i];
        }
    }
    
    // If the current node has no child nodes
    if (root->NW == NULL) {
        return result;
    }

    size_t i;

    
    /* Recursively searches for the point in each quadrant of the rectangle */

    // South-West Quadrant
    i = 0;
    point2D **sw_r = searchPoint(root->SW, range,search);   
    while (sw_r[i] != NULL && i < MAX_ARRAY_SIZE) {        
        result[index++] = sw_r[i];
    }

    // North-West Quadrant
    i = 0;
    point2D **nw_r = searchPoint(root->NW, range,search);   
    while (nw_r[i] != NULL && i < MAX_ARRAY_SIZE) {      
        result[index++] = nw_r[i];
    }

    // North-East Quadrant
    i = 0;
    point2D **ne_r = searchPoint(root->NE, range,search);    
    while (ne_r[i] != NULL && i < MAX_ARRAY_SIZE) {        
        result[index++] = ne_r[i];
    }

    // South-East Quadrant
    i = 0;
    point2D **se_r = searchPoint(root->SE, range,search);
    while (se_r[i] != NULL && i < MAX_ARRAY_SIZE) {
        result[index++] = se_r[i];
    }

    return result;
}

