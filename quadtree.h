#include <stdio.h>

// data definitions
struct point_t {
    long double x;
    long double y;
} ;
typedef struct point_t point2D;

typedef struct rectangle2D {
    point2D *center;
    long double x_half;
    long double y_half; 
} rectangle2D;

typedef struct QuadTree {
    rectangle2D *boundary;
    point2D **points;

    struct QuadTree* NW;
    struct QuadTree* NE;
    struct QuadTree* SW;
    struct QuadTree* SE;

} QuadTree;

// function definitions

/* Creates a point using given coordinates and stores their values */
point2D *create_point(long double x, long double y);

/* Specifies a rectangle given bottom-left 2D point and an upper right 2D point */
rectangle2D *create_rectangle(point2D *center, long double x_mid, long double y_mid);

/* Tests whether a given 2D point lies within the rectangle and returns 1 (TRUE) if it does */
int inRectangle(rectangle2D *boundary, point2D *point);

/* Tests whether two rectangles overlap and returns 1 (TRUE) if they do  */
int rectangleOverlap(rectangle2D *self, rectangle2D *other);

/* Creates a new QuadTree given the 2D coordinates of its upper left and bottom right points of its root node */
QuadTree *new_Quadtree(rectangle2D *boundary);

/* Allocates space required for each of the 2D pointers referring to the child nodes SW, NW, NE and SE of the quadtree */
size_t QuadTree_points_size(point2D *points[]);

/* Creates new children nodes for each internal node and further divides the root node rectangle to insert points */
QuadTree *create_quadNode(QuadTree *root);

/*Tests whether a datapoint given by its 2D coordinates lies within a quadtree and returns the datapoint along with its stored information */
point2D **searchPoint(QuadTree *root, rectangle2D *range,point2D *search);

/* Adds a point given with its 2D coordinates to the quadtree */
int addPoint(QuadTree *root, point2D *point);

/* Returns the quadrant of the rectangle that the point lies in */
int determineQuadrant(rectangle2D *range, point2D *point,FILE *summaryFile);
