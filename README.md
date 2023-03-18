# pr-quadtrees

The following implementation of a PR quadtree supports a number of functions such as inserting data points, searching for individual datapoints and searching (returning) for all datapoints within a query rectangle. 

## Overview

A quadtree is a data structure that stores d-dimensional points and enables efficient search for the stored points. One particular quadtree which can be used to store 2-dimensional points is the point-region quadtree, simply referred as a PR quadtree. A binary tree can be defined as a finite set of nodes that are either empty or have a root and two binary trees T<sub>l</sub> and T<sub>r</sub> (the left and right subtree). A quadtree has a similar recursive definition: instead of two subtrees we have four subtrees, hence the name quad. This means that each node either has four children or is a leaf node. The four leaf nodes are often referred to as NW, NE, SW, SE (see the figure below).

<br></br>
![image](https://user-images.githubusercontent.com/95140934/226100419-0925bdcc-281d-4522-95ab-d79d98580a17.png)

## Data Structures

- `point2D`: stores the location of a datapoint;
* `rectangle2D`: specifies a rectangle given a bottom-left 2D point and an upper-right 2D point;
+ `dataPoint`: a structure that stores the location and associated information (i.e., the footpath information);
- `quadtreeNode`: stores a 2D rectangle and 4 pointers referring to the four children SW, NW, NE, and SE.

## Helper Functions

- `inRectangle`: Tests whether a given 2D point lies within the rectangle and returns 1 (TRUE) if it does
* `rectangleOverlap`: Tests whether two rectangles overlap and returns 1 (TRUE) if they do
+ `addPoint`: Adds a point given with its 2D coordinates to the quadtree
- `searchPoint`: Tests whether a datapoint given by its 2D coordinates lies within a quadtree and returns the datapoint along with its stored information
* `rangeQuery`: Takes a 2D rectangle as an argument and returns all datapoints in the PR quadtree whose coordinates lie within the query rectangle.

## Supporting Point Region Queries

Firstly, the basic functionality for a quadtree was implemented allowing the lookup of data by longitude (x) and latitude (y) pairs.

The Makefile produces an executable program called `dict3`. This program takes seven command-line arguments.

1. The first argument is the *stage*, which, for this part, the value is always 3.
2. The second argument is the filename of the *data file*.
3. The third argument is the filename of the *output file*.
4. The fourth and fifth arguments specify the x, y coordinate pair of the bottom-left corner of the root node area, with the first value referring to the *longitude* (x) and the second value referring to the *latitude* (y).
5. The sixth and seventh arguments specify the top-right corner of the root node area, following the same convention.

The program reads data from the specified file, constructs a quadtree from the data, and interprets the fourth to seventh command line arguments as long double values. Accepting coordinate pairs from stdin, it searches for the point region containing the pair, and prints all matching records to the output file. The program also outputs the list of quadrant directions followed to find the correct point region. Each footpath's start and end points are inserted into the quadtree, allowing the footpath to be found from either point. In cases where multiple footpaths are found, they are printed in order of `footpath_id`.

The quadrants in relation to the dataset are:

![image](https://user-images.githubusercontent.com/95140934/226100974-4eff3046-712c-4244-994d-036953391702.png)

### Execution

An example execution of the program could be:

```powershell
make -B dict3
# ./dict3 stage datafile outputfile start_longitude start_latitude end_longitude end_latitude
./dict3 3 dataset_2.csv output.txt 144.969 -37.7975 144.971 -37.7955 < queryfile
```

Example of output to the *output file* after 3 queries:

```
144.97056424489568 -37.796155887263744
--> footpath_id: 27665 || address: Palmerston Street between Rathdowne Street and Drummond Street || clue_sa: Carlton || asset_type: Road Footway || deltaz: 3.21 || distance: 94.55 || grade1in: 29.5 || mcc_id: 1384273 || mccid_int: 20684 || rlmax: 35.49 || rlmin: 32.28 || segside: North || statusid: 2 || streetid: 955 || street_group: 28597 || start_lat: -37.796156 || start_lon: 144.970564 || end_lat: -37.796061 || end_lon: 144.969417 || 
144.96941668057087 -37.79606116572821
--> footpath_id: 27665 || address: Palmerston Street between Rathdowne Street and Drummond Street || clue_sa: Carlton || asset_type: Road Footway || deltaz: 3.21 || distance: 94.55 || grade1in: 29.5 || mcc_id: 1384273 || mccid_int: 20684 || rlmax: 35.49 || rlmin: 32.28 || segside: North || statusid: 2 || streetid: 955 || street_group: 28597 || start_lat: -37.796156 || start_lon: 144.970564 || end_lat: -37.796061 || end_lon: 144.969417 || 
144.95538810397605 -37.80355555400948
--> footpath_id: 19458 || address: Queensberry Street between Capel Street and Howard Street || clue_sa: North Melbourne || asset_type: Road Footway || deltaz: 1.44 || distance: 94.82 || grade1in: 65.8 || mcc_id: 1385878 || mccid_int: 20950 || rlmax: 35.75 || rlmin: 34.31 || segside: North || statusid: 2 || streetid: 1008 || street_group: 20939 || start_lat: -37.803461 || start_lon: 144.954243 || end_lat: -37.803556 || end_lon: 144.955388 || 
```


With the following output to *stdout*:

```
144.97056424489568 -37.796155887263744 --> NE SW NE NE
144.96941668057087 -37.79606116572821 --> NE SW NE NW
144.95538810397605 -37.80355555400948 --> SW NW SE
```

## Supporting Range Queries

Secondly, the additional functionality for the quadtree was implemented to support range queries given by `(x, y)` co-ordinate pairs.

The Makefile produces an executable program called `dict4`. This program takes seven command-line arguments, similar to the previous one but the expected value of the first argument changes.

- The first argument will be the *stage*, for this part, the value will always be 4.

The program accepts sets of pairs of co-ordinate long double type values from stdin and efficiently uses the quadtree to find all footpaths within the bounds of the query. Blank lines are not expected in the queries. The output to stdout includes all directions searched in order, with each branch potentially containing points within the query bounds fully explored before proceeding to the next possible branch. The program searches quadrants in the order `SW`,`NW`, `NE`, `SE`, where each direction is separated by a space. Multiple footpaths returned by the range query are sorted by `footpath_id`. Each footpath is output only once for each query, even if both its "start" and "end" points both occur in the searched region.

### Execution

An example execution of the program could be:

```powershell
make -B dict4
# ./dict4 stage datafile outputfile start_longitude start_latitude end_longitude end_latitude
./dict4 4 dataset_2.csv output.txt 144.968 -37.797 144.977 -37.79 < queryfile
```

Example of output to the *output file* after 3 queries:

```
144.968 -37.797 144.977 -37.79
--> footpath_id: 27665 || address: Palmerston Street between Rathdowne Street and Drummond Street || clue_sa: Carlton || asset_type: Road Footway || deltaz: 3.21 || distance: 94.55 || grade1in: 29.5 || mcc_id: 1384273 || mccid_int: 20684 || rlmax: 35.49 || rlmin: 32.28 || segside: North || statusid: 2 || streetid: 955 || street_group: 28597 || start_lat: -37.796156 || start_lon: 144.970564 || end_lat: -37.796061 || end_lon: 144.969417 || 
--> footpath_id: 29996 || address:  || clue_sa: Carlton || asset_type: Road Footway || deltaz: 0.46 || distance: 54.51 || grade1in: 118.5 || mcc_id: 1388910 || mccid_int: 0 || rlmax: 24.91 || rlmin: 24.45 || segside:  || statusid: 0 || streetid: 0 || street_group: 29996 || start_lat: -37.793272 || start_lon: 144.975507 || end_lat: -37.794367 || end_lon: 144.975315 || 
144.9678 -37.79741 144.97202 -37.79382
--> footpath_id: 27665 || address: Palmerston Street between Rathdowne Street and Drummond Street || clue_sa: Carlton || asset_type: Road Footway || deltaz: 3.21 || distance: 94.55 || grade1in: 29.5 || mcc_id: 1384273 || mccid_int: 20684 || rlmax: 35.49 || rlmin: 32.28 || segside: North || statusid: 2 || streetid: 955 || street_group: 28597 || start_lat: -37.796156 || start_lon: 144.970564 || end_lat: -37.796061 || end_lon: 144.969417 || 
144.973 -37.795 144.976 -37.792
--> footpath_id: 29996 || address:  || clue_sa: Carlton || asset_type: Road Footway || deltaz: 0.46 || distance: 54.51 || grade1in: 118.5 || mcc_id: 1388910 || mccid_int: 0 || rlmax: 24.91 || rlmin: 24.45 || segside:  || statusid: 0 || streetid: 0 || street_group: 29996 || start_lat: -37.793272 || start_lon: 144.975507 || end_lat: -37.794367 || end_lon: 144.975315 || 
```

With the following output to *stdout*:

```
144.968 -37.797 144.977 -37.79 --> SW SW SE NE SE
144.9678 -37.79741 144.97202 -37.79382 --> SW SW SE
144.973 -37.795 144.976 -37.792 --> NE SE
```

*** Note: This is my submission for *Project 2 of COMP20003 Algorithms and Data Structures in Sem 2 2022* ***
