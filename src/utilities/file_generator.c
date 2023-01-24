#include <stdlib.h>
#include <stdio.h>


/*
Input : ./file_generator filename n low high

- filename : filename of output
- n : number of 2D coordinates points you wanna get.
- low_x high_x low_y high_y : interval for which random coordinates will emerge from
*/

int rand_between(int l, int r) {
  return (int)( (rand() / (RAND_MAX * 1.0f)) * (r - l) + l);
}


void write_file(FILE* f,int n,int l_x,int h_x,int l_y,int h_y){
    int temp_1,temp_2;
    fprintf(f,"%d\n",n);
    for (int i = 0; i < n;i++){
        temp_1 = rand_between(l_x,h_x);
        temp_2 = rand_between(l_y,h_y);
        fprintf(f,"%d;%d\n",temp_1,temp_2);
    }
}

int main(int argc,char* argv[]){
    if (argc != 7) return EXIT_FAILURE;
    FILE* out_f = fopen(argv[1],"w");
    int n = atoi(argv[2]);
    int low_x_bound = atoi(argv[3]);
    int high_x_bound = atoi(argv[4]);
    int low_y_bound = atoi(argv[5]);
    int high_y_bound = atoi(argv[6]);
    write_file(out_f,n,low_x_bound,high_x_bound,low_y_bound,high_y_bound);
    return 0;
}