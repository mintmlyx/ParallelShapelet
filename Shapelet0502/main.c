//
//  parallel_main.c
//  ParallelComputeGap
//
//  Created by Mushroom on 4/30/13.
//  Copyright (c) 2013 Self. All rights reserved.
//

#include <stdio.h>
#include "parser.h"
#include "PextractUShapelets.h"
#include <string.h>
#include <dirent.h>

double** dataset;//store all the data value
int set_no;
int* data_len;
double **pow2map;


int main(int argc, char *argv[]){
    
    /*
     if (argc != 2) {
     
     printf("Check usage: %s field(1-7)\n", argv[0]);
     return 0;
     }
     
     //get the field
     int column_index_for_parser = atoi(argv[1]);*/
    
    int column_index_for_parser = 1;
    
	if (column_index_for_parser < 1 || column_index_for_parser > 7) {
        
		printf("Field beyond range 1...7 \n");
		return 0;
	}
    
    printf("Start to parse training data.......");
    DIR *dir;
    struct dirent *ent;
    int training_file_no = 0;
    
    char* dir_path = "malware_data/traces/";
    
    // first iteration determine the total number of training files
    if ((dir = opendir (dir_path)) != NULL) {
        
        /* print all the files and directories within directory */
        
        while ((ent = readdir (dir)) != NULL) {
            
            //printf ("%s\n", ent->d_name);
            char* filename = ent->d_name;
            char* trainname = "train";
            if (Contains(filename, trainname)>0){
                //only pass the train files
                training_file_no ++;
            }
        }
        closedir (dir);
    } else {
        /* could not open directory */
        perror ("Error: could not open directory");
        return EXIT_FAILURE;
    }
    
    printf("Total training file no: %d", training_file_no);
    //initialize the double dataset
    double** dataset_tmp = (double**) malloc(sizeof(double*)*training_file_no);
    int* data_len_tmp = (int*) malloc(sizeof(int)*training_file_no);
    int* valid_dataset_index = (int*) malloc(sizeof(int)*training_file_no);
    
    //the output txt file marking the index and app name
    FILE* data_app_map_file = fopen("data_app_map.txt", "w");
    if (data_app_map_file == NULL) {
        printf("Error opening file!\n");
        exit(1);
    }
    
    char appname_tmp[training_file_no][LEN];
	char trainfile_path_tmp[training_file_no][LEN];
    int valid_dataset_len = 0;
    
    if ((dir = opendir (dir_path)) != NULL) {
        
        /* print all the files and directories within directory */
        
        int trainfile_id = 0;
        
        while ((ent = readdir (dir)) != NULL) {
            
            char* filename = ent->d_name;
            char* trainname = "train";
            
            if (Contains(filename, trainname)>0){
                //only pass the train files
                //generate the full path
                strcpy(trainfile_path_tmp[trainfile_id], dir_path);
                strcat(trainfile_path_tmp[trainfile_id], filename);
                //printf("Train file path: %s", trainfile_path[trainfile_id]);
                
                //pass it to the parser to read certain index
                //read to dataset and date_len
                
                data_len_tmp[trainfile_id] = readFile(column_index_for_parser, trainfile_path_tmp[trainfile_id], appname_tmp[trainfile_id], &dataset_tmp[trainfile_id]);
                
                if (data_len_tmp[trainfile_id]>=1250 && data_len_tmp[trainfile_id]<=10000) {
                    
                    valid_dataset_index[valid_dataset_len] = trainfile_id;
                    fprintf(data_app_map_file, "%d\t%s\t%s\t%d\n",trainfile_id,appname_tmp[trainfile_id],filename,data_len_tmp[trainfile_id]);
                    valid_dataset_len++;
                    //write to external file
                    //printf("\nlength: %d filename: %s",data_len[trainfile_id], filename);
                    printf("Parser result: %d\t%s\t%s\n",trainfile_id,appname_tmp[trainfile_id],trainfile_path_tmp[trainfile_id]);
                    
                }
                
                trainfile_id++;
                
            }
        }
        
        closedir (dir);
        
    } else {
        /* could not open directory */
        perror ("Error: could not open directory");
        return EXIT_FAILURE;
    }
    
    //close the app_name_map file
    fclose(data_app_map_file);
    
    dataset = (double**) malloc(sizeof(double)*valid_dataset_len);
    data_len = (int*) malloc(sizeof(int)*valid_dataset_len);
    set_no = valid_dataset_len;
    
    //only retrieve the datasets with len>= 1000
    for (int i=0; i<valid_dataset_len; i++) {
        
        //write to files
        dataset[i] = dataset_tmp[valid_dataset_index[i]];
        data_len[i] = data_len_tmp[valid_dataset_index[i]];
        
        if(i < valid_dataset_index[i] ) {
            
            strcpy(appname_tmp[i], appname_tmp[valid_dataset_index[i]]);
            strcpy(trainfile_path_tmp[i], trainfile_path_tmp[valid_dataset_index[i]]);
            
        } else {
            
            printf("Something is awfully wrong\n");
            
        }
        
        //print out the results
        //printf("dataset length: %d",data_len[i]);
    }
    
    //free appname and file path
    printf("\nFree appname and file path");
    
    printf("\ndata_app_map.txt generated.......");
    
    printf("\nCompleted dataparser.......");
    
    printf("\nCalcPow2.......");
    
    //call the parallel ushapelet function
    calcPow2();
    
    printf("\nCompleted CalcPow2.......");
    
    char* shapelet_filename = "shapelet.txt";
    
    extractU_Shapelets(dataset, data_len, set_no, 1000, appname_tmp, trainfile_path_tmp, shapelet_filename);
    
    for (int i=0; i<set_no; i++) {
        
        free(dataset[i]);
        free(pow2map[i]);
        
    }
    
    free(pow2map);
    
    free(dataset);
    free(data_len);
    free(dataset_tmp);
    free(data_len_tmp);
    free(valid_dataset_index);
    
}