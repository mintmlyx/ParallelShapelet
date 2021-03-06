//
//  parallel_compute.c
//  ComputeGap
//
//  Created by Mushroom on 4/12/13.
//  Copyright (c) 2013 Self. All rights reserved.
//

#include <stdio.h>
#include<string.h>
#include <errno.h>
#include "parallel_compute.h"


//paralleled--------------------------------------------------------------------
int compare (const void * a, const void * b)
{
    return CompareDoubles2(*(double*) a,*(double*)b);
}

int CompareDoubles2(double A, double B){
    
	if((nearbyint(A*1000000.0)-nearbyint(B*1000000.0))>0.0){
		return 1;
	}else if((nearbyint(A*1000000.0)-nearbyint(B*1000000.0))==0.0){
		return 0;
	}else{
		return -1;
	}
    
}

void* calcPow2Thread(void *threadarg) {
	
  
	int row_id = 0; 
    	int start_row_id= ((threadInfo *)threadarg)->start;
	int n_row = ((threadInfo *)threadarg)->count;


	for(row_id = start_row_id; row_id < start_row_id + n_row; row_id++) {

	   	int vecLen = data_len[row_id]; 

        	pow2map[row_id] = (double*) malloc(sizeof(double)*vecLen);

		computeSquareVec(dataset[row_id], pow2map[row_id], 0, vecLen);

	}    
    	pthread_exit(NULL);
    
}

//paralleled--------------------------------------------------------------------
void calcPow2(){
    
    	//initialization
    	pow2map = (double**) malloc(sizeof(double*)*set_no);
    
    	//parallel the calculate square value of each row
	
	int nThreads;
	nThreads  = (set_no / bfactor) + ((set_no % bfactor) > 0) ;
	int offset = 0;
	int remainder = set_no;
    	pthread_t pow2_threads[nThreads];
	threadInfo threadArg[nThreads];
    
    	int create_result = 1;
    

    	for (int i=0; i<nThreads; i++) {
		
		threadArg[i].start = offset;
		threadArg[i].count = ((bfactor < remainder) ? bfactor : remainder);


		create_result = pthread_create(&pow2_threads[i], NULL, calcPow2Thread, &threadArg[i]);

        	while (create_result){
            
            		printf("ERROR; return code from !calcPow2! pthread_create() is %s at thread no %d\n", strerror(create_result), i);
            		create_result = pthread_create(&pow2_threads[i], NULL, calcPow2Thread, &threadArg[i]);
           	}

		offset +=  threadArg[i].count;
		remainder = set_no - offset; 
       
        
	}
    
    	//join the threads
    	for (int i=0; i<nThreads; i++) {
        
		pthread_join(pow2_threads[i], NULL);
        
    	}
    
    
}
void* calcDistThread(void *args) {

	threadInfo* threadArg = (threadInfo*) args;

	int index_start = threadArg->start;
	int index_len = threadArg->count;
	struct computeGap_para* para = (struct computeGap_para*) threadArg->data;


    	int shapelet_row_id = para->shapelet_row_id;
    	int shapelet_col_start_id = para->shapelet_col_start_id;
    	int shapelet_len = para->shapelet_len;
    	int* index_marker = para->index_marker;

	//we wont look at this here any more
    	//int index_marker_len = para->index_marker_len;


    	//create an argument array
    	struct computeDistance_para *computeDistance_para_array = ( struct computeDistance_para* ) malloc(sizeof(struct computeDistance_para)*index_len);

    	for (int i=0,j=index_start; i< index_len; j++,i++) {
        
        	//initial the computeDistance_para
        	computeDistance_para_array[i].shapelet_row_id = shapelet_row_id;
        	computeDistance_para_array[i].shapelet_col_start_id = shapelet_col_start_id;
        	computeDistance_para_array[i].shapelet_len = shapelet_len;
        	computeDistance_para_array[i].data_row_id = index_marker[j];
        	computeDistance_para_array[i].distance = &(para->distarray[j]);
        
        	computeDistance((void*) &computeDistance_para_array[i]);
        
        
    	}
	free(computeDistance_para_array);

	pthread_exit(NULL);    
}

void* findMaxThread(void *args) {


	threadInfo* threadArg = (threadInfo*) args;

	int index_start = threadArg->start;
	int index_len = threadArg->count;
	struct computeGap_para* para = (struct computeGap_para*) threadArg->data;

    	int cluster_no = para->cluster_no;
    	int shapelet_row_id = para->shapelet_row_id;
    	int shapelet_col_start_id = para->shapelet_col_start_id;
    	int shapelet_len = para->shapelet_len;
    	int* index_marker = para->index_marker;
    	int index_marker_len = para->index_marker_len;

	
    	//set paramter array
    	struct findMaxGap_para* findMaxGap_para_array = (struct findMaxGap_para*) malloc(sizeof(struct findMaxGap_para)*index_len);


        for (int i=0, j=index_start; i<index_len; i++, j++) {

                findMaxGap_para_array[i].dist_array = para->distarray;
                findMaxGap_para_array[i].cluster_no = cluster_no;
                findMaxGap_para_array[i].cluster_dis = (para->distarray[j]+para->distarray[j+1])/2.0;
                findMaxGap_para_array[i].index_marker_len = index_marker_len;
                findMaxGap_para_array[i].maxGap = para->threshold_maxGap;
                findMaxGap_para_array[i].dt = para->threshold_dt;
                findMaxGap_para_array[i].lock = threadArg->lock;
        
                findMaxGap((void*) &findMaxGap_para_array[i]);
        
        }

	free(findMaxGap_para_array);
	pthread_exit(NULL);
}

void* computeGap(void* arg_para){
    
    	struct computeGap_para* para = (struct computeGap_para*) arg_para;
    	int cluster_no = para->cluster_no;

    	int index_marker_len = para->index_marker_len;

	int nThreads;
	nThreads  = (index_marker_len / bfactor) + ((index_marker_len % bfactor) > 0) ;
	int offset = 0;
	int remainder = index_marker_len;
	threadInfo threadArg[nThreads];
	pthread_t idThread[nThreads];
    	double* dist_array = (double *)malloc(sizeof(double)*index_marker_len);
	
	para->distarray = dist_array;

    	//create thread result
    	int create_result = 1;
    	for (int i=0; i<nThreads; i++) {
		
		threadArg[i].start = offset;
		threadArg[i].count = ((bfactor < remainder) ? bfactor : remainder);
		threadArg[i].data = arg_para;

		create_result = pthread_create(&idThread[i], NULL, calcDistThread, &threadArg[i]);

		
        	while (create_result){
            
            		printf("ERROR; return code from !calcPow2! pthread_create() is %s at thread no %d\n", strerror(create_result), i);

			create_result = pthread_create(&idThread[i], NULL, calcDistThread, &threadArg[i]);
           	}

		offset +=  threadArg[i].count;
		remainder = index_marker_len - offset; 
       
        
	}
    		//join all threads
    		
	for (int i=0; i<nThreads; i++) {
        
       		pthread_join(idThread[i], NULL);
        
    	}
        
    	//sort the distance array in ascending order
    	qsort(dist_array, index_marker_len, sizeof(double), compare);
	
    
    
    	//set paramter array
    	struct findMaxGap_para* findMaxGap_para_array = (struct findMaxGap_para*) malloc(sizeof(struct findMaxGap_para)*index_marker_len);
    	//initial the mutex
    	pthread_mutex_t lock;
    	pthread_mutex_init(&lock, NULL);
    	//initial thread array
    	pthread_t findMaxGap_threads[index_marker_len-1];

	offset = 0;
	remainder = index_marker_len -1;
	nThreads  = (remainder / bfactor) + ((remainder % bfactor) > 0) ;


	for(int i=0; i<nThreads; i++) {

		threadArg[i].start = offset;
		threadArg[i].count = ((bfactor < remainder) ? bfactor : remainder);
		threadArg[i].data = arg_para;
		threadArg[i].lock = &lock;

		create_result = pthread_create(&idThread[i], NULL, findMaxThread, &threadArg[i]);

		
        	while (create_result){
            
            		printf("ERROR; return code from !calcPow2! pthread_create() is %s at thread no %d\n", strerror(create_result), i);

			create_result = pthread_create(&idThread[i], NULL, findMaxThread, &threadArg[i]);
           	}

		offset +=  threadArg[i].count;
		remainder -=  threadArg[i].count;
       
        
	}
    		//join all threads

    	for (int i=0; i<nThreads; i++) {
        
        	pthread_join(idThread[i], NULL);
        
    	}
   
    
    	free(findMaxGap_para_array);
    	free(dist_array);
    
    	pthread_mutex_destroy(&lock);
}

void* findMaxGap(void* arg_para){
    
    //printf("\nIn findMaxGap!!!\n");
    struct findMaxGap_para* para = (struct findMaxGap_para*) arg_para;
    
    double tmp_dist = para->cluster_dis;
    int cluster_no = para->cluster_no;
    double* distance_array = para->dist_array;
    int index_marker_len = para->index_marker_len;
    
    //record the number of the points in each cluster
    int cluster_a = 0;
    
    int* data_cluster = (int*) malloc(sizeof(int)*index_marker_len);
    
    //check which cluster it is in
    for (int j=0; j<index_marker_len; j++) {
        
        if (CompareDoubles2(distance_array[j],tmp_dist)<0) {
            
            cluster_a ++;
            data_cluster[j] = 1;
            
        }else{
            
            data_cluster[j] = 0;
            
        }
        
    }
    
    if((cluster_a>0)&&(cluster_a<index_marker_len)){
        //balance the set ratio
        double ratio = (double)cluster_a/(double)(index_marker_len-cluster_a);
        
        if ( ( CompareDoubles2(1.0/(double)cluster_no,ratio)<=0 ) && ( CompareDoubles2(ratio, 1.0 - (1.0/(double)cluster_no) ) <=0 ) ) {
            
            
            double cluster_a_mean = 0.0;
            double cluster_a_std = 0.0;
            double cluster_b_mean = 0.0;
            double cluster_b_std = 0.0;
            
            //calculate the distance mean of cluster_a and cluster_b
            for (int j=0; j<index_marker_len; j++) {
                
                if (data_cluster[j]==1) {
                    
                    //in cluster a
                    cluster_a_mean += distance_array[j];
                    //cluster_a_std stores the sum of the power of 2
                    cluster_a_std = cluster_a_std +  pow(distance_array[j], 2.0);
                    
                }else{
                    
                    //in cluster b
                    cluster_b_mean += distance_array[j];
                    cluster_b_std += pow(distance_array[j], 2.0);
                    
                }
            }
            
            //cluster a
            cluster_a_mean = cluster_a_mean/(double)cluster_a;//normalize the sum to the mean
            if(CompareDoubles2( (cluster_a_std/(double)cluster_a)-pow(cluster_a_mean, 2.0), 0.0) == 0)	{
                cluster_a_std = 0.0;
            }else{
                cluster_a_std = sqrt((cluster_a_std/(double)cluster_a) - pow(cluster_a_mean, 2.0));
            }
            
            //cluster b
            cluster_b_mean = cluster_b_mean / (double)(index_marker_len - cluster_a);//normalize the sum to the mean
            if( CompareDoubles2( ( cluster_b_std/(double)(index_marker_len-cluster_a) - pow(cluster_b_mean,2.0) ), 0.0) ==0){
                //printf("cluster_b_std small! %f %f %f", cluster_b_std/(double)(index_marker_len-cluster_a), pow(cluster_b_mean,2.0), ( cluster_b_std/(double)(index_marker_len-cluster_a) - pow(cluster_b_mean,2.0)) );
                cluster_b_std = 0.0;
            }else{
                cluster_b_std = sqrt(( cluster_b_std / (double)(index_marker_len - cluster_a) ) - pow(cluster_b_mean, 2.0));
            }
            
            double tmp_gap = cluster_b_mean - cluster_b_std - cluster_a_mean - cluster_a_std;
            //printf("tmp_gap: %f\n",tmp_gap);
            
            pthread_mutex_lock(para->lock);
            
            //printf("ratio: %f",ratio);
            //printf("\ntmp_gap: %f VS maxGap: %f\n",tmp_gap,*(para->maxGap));
            
            //printf("\ntmp_gap: %f tmp_dist: %f +++++",tmp_gap, tmp_dist);
            //printf("\ncluster_a_mean: %f cluster_a_std: %f cluster_b_mean: %f cluster_b_std: %f\n",cluster_a_mean, cluster_a_std, cluster_b_mean, cluster_b_std);
//            for(int k=0;k<index_marker_len;k++){
                //printf("\ndist: %f tmp_dist: %f marker: %d",distance_array[k] ,tmp_dist, data_cluster[k]);
  //          }
            
            if ( CompareDoubles2(tmp_gap , *(para->maxGap))>0 ) {
                
                *(para->maxGap) = tmp_gap;
                *(para->dt) = tmp_dist;
                //printf("\nmaxGap %f dt %f in thread\n",tmp_gap, tmp_dist);
            }
            pthread_mutex_unlock(para->lock);
            
        }else{
            
            //printf("\nNot passed: %d\n",cluster_a);
        }
        
    }
    
    //free the create pointer
    free(data_cluster);
    
    
}

//return the distance of each sample
//end id is inclusive
void* computeDistance(void* arg_para){
    
    //parameter casting
    struct computeDistance_para* para = (struct computeDistance_para*) arg_para;
    
    int shapelet_row_id = para->shapelet_row_id;
    int shapelet_col_start_id = para->shapelet_col_start_id;
    int shapelet_len = para->shapelet_len;
    int data_row_id = para->data_row_id;
    
    //z-normal of the shapelet
    double* shapelet_normalized = zNormal(shapelet_row_id, shapelet_col_start_id, shapelet_col_start_id+shapelet_len-1);
    
    //vary the start point in the comparing data row
    double min_dist = INFINITY;
    
    for (int i=0; i<(data_len[data_row_id] - shapelet_len+1); i++) {
        
        //create the normalized comparing array
        double* comp_normalized = zNormal(data_row_id, i, i+shapelet_len-1);
        
        //calculate the euclidean distance
        double dist = euclideanDistance(comp_normalized, shapelet_normalized, shapelet_len);
        
        
        if( CompareDoubles2(dist,min_dist)<0){
            
            min_dist = dist;
            
        }
        
        free(comp_normalized);
    }
    
    *(para->distance) = (min_dist/(double)shapelet_len);
    
    free(shapelet_normalized);
    
}
double euclideanDistance(double* array1, double* array2, int len){

        	
	int vecLen = len;
	double *buffer = (double*) malloc(sizeof(double)*vecLen);
		
	int offset = 0;
	double sum =0.0, temp = 0.0;	

	computeDiffVec(array1, array2, buffer, offset, len);
	computeSquareVec(buffer, buffer, offset, len);
	sum = computeScalarSum(buffer, offset, len);

	//printf("Euclidean distance %f\n", sum);

	free(buffer);
	return sum;

}


//extract the data from the square map
//the col_end id is inclusive
double* zNormal(int row, int col_start, int col_end){
//calc the mean

    	double mean = 0.0;

    	double std = 0.0;

    	int array_len = col_end - col_start + 1;

	mean = computeScalarSum(dataset[row], col_start, array_len);
   	std = computeScalarSum(pow2map[row], col_start, array_len); 

    	//normalize the mean

    	mean = mean / (double) array_len;
    
    	if(CompareDoubles2((std / (double) array_len) - pow(mean , 2.0) ,0.0) <= 0) {

		std = 0.0;

	} else {

	    	std = sqrt( (std / (double) array_len) - pow(mean , 2.0) );

   	} 

	double* z_array = (double*) malloc(sizeof(double)*array_len);

    	if(CompareDoubles2(std,0.0)==0){

		initVec(z_array, 0.0, array_len);
		
/*        	for (int i=0; i<array_len; i++) {


            		//calc the z-value
            		z_array[i] = 0.0;
        	}
*/

    	}else{

			
		scalarDiffVec(&(dataset[row][col_start]), mean, z_array, array_len);

		divByScalarVec(z_array, std, array_len);

        	/*for (int i=0; i<array_len; i++) {

            	//calc the z-value
            		z_array[i] = (dataset[row][col_start+i] - mean)/std;

        	}*/

    	}

    	return z_array;
}
