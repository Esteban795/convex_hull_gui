#include "convex_hull.h"

/*
Gives xmin,ymin, xmax,ymax coordinates of a bounding rectangle (generally not the minimal one)
*/
void container_rect(point* points,int n,int* xmin,int* xmax,int* ymin,int* ymax){
    *xmin = points[0].x;
    *xmax = points[0].x;
    *ymin = points[0].y;
    *ymax = points[0].y;
    for (int i = 1; i < n;i++){
        if (points[i].x < *xmin) *xmin = points[i].x;
        if (points[i].x > *xmax) *xmax = points[i].x;
        if (points[i].y < *ymin) *ymin = points[i].y;
        if (points[i].y > *ymax) *ymax = points[i].y;
    }
}

/*
Points have no specific reasons to fall under the 1280x720 resolution used here.
So we gotta modify those coordinates to make sure they fall under the size of the window.

*/
point* adapt_coordinates(point* points,int n,int xmin,int ymin,int xmax,int ymax){
    int rect_width = ceil(xmax - xmin); 
    int rect_height = ceil(ymax - ymin);
    int w_ratio = 1 + rect_width / WIDTH;
    int h_ratio = 1 + rect_height / HEIGHT;
    point* resized_points = malloc(sizeof(point) * n);
    for (int i = 0; i < n;i++){
        point p = {.x = points[i].x / w_ratio, .y = points[i].y / h_ratio};
        resized_points[i] = p;
    }
    free(points);
    return resized_points;
}


