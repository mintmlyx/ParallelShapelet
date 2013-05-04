//
//  PextractUShapelets.h
//  ParallelComputeGap
//
//  Created by Mushroom on 4/30/13.
//  Copyright (c) 2013 Self. All rights reserved.
//

#ifndef ParallelComputeGap_PextractUShapelets_h
#define ParallelComputeGap_PextractUShapelets_h
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parallel_compute.h"
#include<unistd.h>

extern double** dataset;//store all the data value
extern int set_no;
extern int* data_len;
extern double **pow2map;

struct computegap_spwn_param {
    
	int subseqlen;
	int cnt_indx;
	double *ts;
	int ts_len;
	int subseq_row;
	int dataset_no;
	int *old_map;
	double *dt;
	double *gap;
	int cluster_no;
	int *subseq_col;
	double** p_subseq;
	int * ps_len;
};

//private
void compute_mean_stddev(int *dataset_A, int dataset_Alen, double *dist, int newsize, double *mean, double *stddev);
int clustered(int newcluster, int newsize);
int max_index(double *gap, int total);
void spawn_distance(int shapelet_row, int shapelet_col, int shapelet_len, int *old_map, int newsize, double *dist, double thresh,
                    int *dataset_A, int *dataset_Alen);
void* compute_gap_thread(void* arg);
void spawn_thread(int* subseqlen, double *ts,int ts_len, int subseq_row, int dataset_no, int *old_map, double *dt, double *gap, int cluster_no, int *subseq_col, double *p_subseq[], int *ps_len, int x);

//public
//void extractU_Shapelets(double **pd_Dataset, int* ds_len, int n_sample, int sLen);
void extractU_Shapelets(double **pd_Dataset, int* ds_len, int n_sample, int sLen, char appname[][100], char inputfiles[][100], char *outputname, int input_cluster_no,int start_ts_id);

#endif
