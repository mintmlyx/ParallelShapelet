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
extern int bfactor;
//argument struct
struct computeDistance_para{
    
    int shapelet_row_id;
    int shapelet_col_start_id;
    int shapelet_len;
    int data_row_id;
    double *distance;
    
//    double distance;
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
    double *distarray;
    
};


typedef struct {

        int start;
        int count;
        void *data;
        pthread_mutex_t* lock;
} threadInfo;




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

void* calcDistThread(void* arg_para);

int computeSquareVec(double *inp, double *out, int from, int len);
double computeScalarSum(double *inp, int from, int len);
int computeDiffVec(double *inp, double *inp2, double* out, int from, int len);
void divByScalarVec(double *source, double factor, int len);

void scalarDiffVec(double *source, double factor, double *dest, int len);

void initVec(double *source, double val, int len);
#endif
