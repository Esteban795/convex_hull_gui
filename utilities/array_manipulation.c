#include <stdio.h>
#include <stdlib.h>
#include "../convex_hull.h"

point* copy_points_arr(point* points,int n){
    point* copy_arr = malloc(sizeof(point) * n);
    for (int  i = 0;i < n;i++){
        point p = {.x = points[i].x,.y = points[i].y};
        copy_arr[i] = p;
    }
    return copy_arr;
}

void print_points_arr(point* points, int n){
    for (int i = 0; i < n;i++){
        printf("Point (%.3f,%.3f)",points[i].x,points[i].y);
    }
}

