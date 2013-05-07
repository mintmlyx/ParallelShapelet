//
//  parallel_compute.h
//  ComputeGap
//
//  Created by Mushroom on 4/12/13.
//  Copyright (c) 2013 Self. All rights reserved.
//

#ifndef ComputeGap_parallel_compute_h
#define ComputeGap_parallel_compute_h

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <pthread.h>

//globals
//store the square value of all the data value
extern double** pow2map;//initialized in the calcPow2 function
extern double** dataset;//store all the data value
extern int set_no;
extern int* data_len;

//argument struct
struct computeDistance_para{
    
    int shapelet_row_id;
    int shapelet_col_start_id;
    int shapelet_len;
    int data_row_id;
    double distance;
    
};

struct findMaxGap_para{
    
    double* dist_array;
    int cluster_no;
    double cluster_dis;
    double* maxGap;
    double* dt;
    int index_marker_len;
    pthread_mutex_t* lock;
    
};

struct computeGap_para{
    
    double* threshold_dt;
    double* threshold_maxGap;
    int cluster_no;
    int shapelet_row_id;
    int shapelet_col_start_id;
    int shapelet_len;
    int* index_marker;
    int index_marker_len;
    
};

//function declaration
void* calcPow2_row(void* id);
void calcPow2();
double* zNormal(int row, int col_start, int col_end);
double euclideanDistance(double* array1, double* array2, int len);
int compare (const void * a, const void * b);
void* computeDistance(void* arg_para);
void* findMaxGap(void* arg_para);
int CompareDoubles2(double A, double B);

void* computeGap(void* arg_para);

#endif