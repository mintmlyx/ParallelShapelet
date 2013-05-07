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


void* calcPow2_row(void* id){
    
    int row_id;
    row_id = *((int*) id);
    
    for (int i=0; i<data_len[row_id]; i++) {
        
        //calcuate the pow of 2
        pow2map[row_id][i] = pow(dataset[row_id][i], 2.0);
        
    }
    
    pthread_exit(NULL);
    
}

//paralleled--------------------------------------------------------------------
void calcPow2(){
    
    //initialization
    pow2map = (double**) malloc(sizeof(double*)*set_no);
    
    //parallel the calculate square value of each row
    //total barrier wait is the total number of rows + the main program
    pthread_t pow2_threads[set_no];
    
    int create_result = 1;
    
    int* index = (int*) malloc(sizeof(int)*set_no);
    for (int i=0; i<set_no; i++) {
        
        pow2map[i] = (double*) malloc(sizeof(double)*data_len[i]);
        
        index[i] = i;
        create_result = pthread_create(&pow2_threads[i], NULL, calcPow2_row, (void *)&index[i]);
        
        //create_result == 0 when it succeeds
        while (create_result){
            
            printf("ERROR; return code from !calcPow2! pthread_create() is %s at thread no %d\n", strerror(create_result), i);
            create_result = pthread_create(&pow2_threads[i], NULL, calcPow2_row, (void *)&index[i]);
            
        }
        
    }
    
    //join the threads
    for (int i=0; i<set_no; i++) {
        
        pthread_join(pow2_threads[i], NULL);
        
    }
    
    //destory thr int* index
    free(index);
    //print the pow2map
	//printf("\nStart printing pow2map\n");
    
    /*
     for(int i=0; i<set_no; i++){
     
     printf("\n");
     for(int j=0;j<data_len[i];j++){
     
     printf("%f ",pow2map[i][j]);
     
     }
     
     }*/
    
}

void* computeGap(void* arg_para){
    
    
    struct computeGap_para* para = (struct computeGap_para*) arg_para;
    double* threshold_dt = para->threshold_dt;
    double* threshold_maxGap = para->threshold_maxGap;
    int cluster_no = para->cluster_no;
    int shapelet_row_id = para->shapelet_row_id;
    int shapelet_col_start_id = para->shapelet_col_start_id;
    int shapelet_len = para->shapelet_len;
    int* index_marker = para->index_marker;
    int index_marker_len = para->index_marker_len;
    
    //create an argument array
    struct computeDistance_para* computeDistance_para_array = ( struct computeDistance_para* ) malloc(sizeof(struct computeDistance_para)*index_marker_len);
    //create thread array
    pthread_t computeDistance_threads[index_marker_len];
    //distance array
    double* dist_array = (double *)malloc(sizeof(double)*index_marker_len);
    //create thread result
    int create_result = 1;
    //compute the distance array paralleled
    for (int i=0; i<index_marker_len; i++) {
        
        //initial the computeDistance_para
        computeDistance_para_array[i].shapelet_row_id = shapelet_row_id;
        computeDistance_para_array[i].shapelet_col_start_id = shapelet_col_start_id;
        computeDistance_para_array[i].shapelet_len = shapelet_len;
        computeDistance_para_array[i].data_row_id = index_marker[i];
        computeDistance_para_array[i].distance = 0.0;
        
        //issue the distance call
        create_result = pthread_create(&computeDistance_threads[i], NULL, computeDistance, (void*) &computeDistance_para_array[i]);
        //create_result == 0 when it succeeds
        
        while (create_result){
            
            printf("ERROR; Parallel_compute.c !computeDistance! pthread_create() is %s at thread no %d\n", strerror(create_result), i);
            create_result = pthread_create(&computeDistance_threads[i], NULL, computeDistance, (void*) &computeDistance_para_array[i]);
            
        }
        
    }
    
    //join all threads
    for (int i=0; i<index_marker_len; i++) {
        
        pthread_join(computeDistance_threads[i], NULL);
        
    }
    
    //copy the value to dist_array
    for (int i=0; i<index_marker_len; i++) {
        
        dist_array[i] = computeDistance_para_array[i].distance;
        //printf("\nDist_array[%d]: %f\n",i , dist_array[i]);
        
    }
    
    //printf("Dist Array Done!");
    
    //free the computeDistance_para_array
    free(computeDistance_para_array);
    
    //sort the distance array in ascending order
    qsort(dist_array, index_marker_len, sizeof(double), compare);
	
    for (int i=0; i<index_marker_len; i++) {
        
        printf("\nDist_array[%d]: %f\n",i , dist_array[i]);
        
    }
    
    double maxGap = 0.0;
    double dt = 0.0;
    
    //set paramter array
    struct findMaxGap_para* findMaxGap_para_array = (struct findMaxGap_para*) malloc(sizeof(struct findMaxGap_para)*index_marker_len);
    //initial the mutex
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);
    //initial thread array
    pthread_t findMaxGap_threads[index_marker_len-1];
    
    //parallel execution of the findMaxGapp
    for (int i=0; i<index_marker_len-1; i++) {
        
        findMaxGap_para_array[i].dist_array = dist_array;
        findMaxGap_para_array[i].cluster_no = cluster_no;
        findMaxGap_para_array[i].cluster_dis = (dist_array[i]+dist_array[i+1])/2.0;
        findMaxGap_para_array[i].index_marker_len = index_marker_len;
        findMaxGap_para_array[i].maxGap = &maxGap;
        findMaxGap_para_array[i].dt = &dt;
        findMaxGap_para_array[i].lock = &lock;
        
        create_result = pthread_create(&findMaxGap_threads[i], NULL, findMaxGap, (void*) &findMaxGap_para_array[i]);
        
        //create_result == 0 when it succeeds
        while (create_result){
            
            printf("ERROR; return code from !findMaxGap! pthread_create() is %s at thread no %d\n", strerror(create_result), i);
            create_result = pthread_create(&findMaxGap_threads[i], NULL, findMaxGap, (void*) &findMaxGap_para_array[i]);
            
        }
        
    }
    
    //join the threads
    for (int i=0; i<index_marker_len-1; i++) {
        
        pthread_join(findMaxGap_threads[i], NULL);
        
    }
    
    //write the output
    *threshold_dt = dt;
    *threshold_maxGap = maxGap;
    printf("\nmaxGap %f dt %f\n",*threshold_maxGap, dt);
    
    free(findMaxGap_para_array);
    free(dist_array);
    //    pthread_exit(NULL);
    
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
            
            printf("ratio: %f",ratio);
            printf("\ntmp_gap: %f VS maxGap: %f\n",tmp_gap,*(para->maxGap));
            
            //printf("\ntmp_gap: %f tmp_dist: %f +++++",tmp_gap, tmp_dist);
            printf("\ncluster_a_mean: %f cluster_a_std: %f cluster_b_mean: %f cluster_b_std: %f\n",cluster_a_mean, cluster_a_std, cluster_b_mean, cluster_b_std);
            for(int k=0;k<index_marker_len;k++){
                //printf("\ndist: %f tmp_dist: %f marker: %d",distance_array[k] ,tmp_dist, data_cluster[k]);
            }
            
            if ( CompareDoubles2(tmp_gap , *(para->maxGap))>0 ) {
                
                *(para->maxGap) = tmp_gap;
                *(para->dt) = tmp_dist;
                //printf("\nmaxGap in thread: %f\n",tmp_gap);
            }
            pthread_mutex_unlock(para->lock);
            
        }else{
            
            printf("\nNot passed: %d",cluster_a);
        }
        
    }
    
    //free the create pointer
    free(data_cluster);
    
    pthread_exit(NULL);
    
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
    
    para->distance = (min_dist/(double)shapelet_len);
    
    free(shapelet_normalized);
    pthread_exit(NULL);
    
}

double euclideanDistance(double* array1, double* array2, int len){
    
    double sum = 0.0;
    for (int i=0; i<len; i++) {
       
        sum = sum + pow((array1[i]-array2[i]), 2.0);

        
    }
    
    
    return sum;
    
}



//extract the data from the square map
//the col_end id is inclusive
double* zNormal(int row, int col_start, int col_end){
//calc the mean

    double mean = 0.0;

    double std = 0.0;

    int array_len = col_end - col_start + 1;

    

    for(int i=col_start; i<=col_end; i++){

        

        mean += dataset[row][i];

        std += pow2map[row][i];

        

    }

    

    //normalize the mean

    	mean = mean / (double) array_len;

    
    	if(CompareDoubles2((std / (double) array_len) - pow(mean , 2.0) ,0.0) <= 0) {

		std = 0.0;

	} else {

    	std = sqrt( (std / (double) array_len) - pow(mean , 2.0) );

   	} 

    double* z_array = (double*) malloc(sizeof(double)*array_len);

    

    if(CompareDoubles2(std,0.0)==0){

        

        for (int i=0; i<array_len; i++) {

            //calc the z-value

            z_array[i] = 0.0;

            

        }

        

    }else{

    

        for (int i=0; i<array_len; i++) {

        

            //calc the z-value

            z_array[i] = (dataset[row][col_start+i] - mean)/std;

	           

        }

    }

    

    return z_array;

    
    
}
