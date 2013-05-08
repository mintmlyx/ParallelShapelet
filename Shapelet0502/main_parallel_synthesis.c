//
//  main_parallel_synthesis.c
//  Shapelet0502
//
//  Created by Mushroom on 5/2/13.
//  Copyright (c) 2013 Self. All rights reserved.
//

#include <stdio.h>
#include "PextractUShapelets.h"
#include "parallel_compute.h"

double** dataset;//store all the data value
int set_no;
int* data_len;
double **pow2map;
int bfactor = 7;
//sine wave generation
double sin(double x)
{
    double res=0, pow=x, fact=1;
    
    for(int i=0; i<5; ++i)
    {
        res+=pow/fact;
        pow*=x*x;
        fact*=(2*(i+1))*(2*(i+1)+1);
    }
    
    return res;
}

double* generate_sinewave(double phase, double amp, double freq, int len){
    
    double* result = (double*) malloc(sizeof(double)*len);
    
    for (int i=0; i<len; i++) {
        
        result[i] = amp*sin(phase+(double)i/freq);
        
    }
    
    return result;
    
}

double* generate_stepwave(int step_len, int len, double amp){
    
    double* result = (double*) malloc(sizeof(double)*len);
    
    for (int i=0; i<len; i++) {
        
        result[i] = (double)(((i/step_len)%2)*2 - 1)*amp;
    }
    
    return result;
    
}

int main(int argc, char *argv[])
{
    printf("\nParallel shapelet starting.......");
    set_no = 30;
    dataset = (double**) malloc(sizeof(double*)*set_no);
    data_len = (int*) malloc(sizeof(int)*set_no);
    char appname[set_no][100];
    char inputfile[set_no][100];
    
    //output file
    FILE* parallel_synthesis_file = fopen("parallel_synthesis_input.txt", "w");
    
    //three sets of data
    //set 1 sine
    for (int i=0; i<15; i++) {
        
        data_len[i] = rand()%5 + 5;
        double phase = ((double) (rand()%10))/3.0;
        double amp = ((double) (rand()%10))/3.0+1.0;
        double freq = (double) (rand()%10)+1.0;
        dataset[i] = generate_sinewave(phase, amp, freq, data_len[i]);
        sprintf(appname[i], "sinewave");
        sprintf(inputfile[i], "sinewave_%d.txt", i);
        
    }
    
    //set 2 step
    for (int i=15; i<30; i++) {
        
        data_len[i] = rand()%5 + 5;
        int step = rand()%5+1;
        double amp = ((double) (rand()%10))/3.0+1.0;
        dataset[i] = generate_stepwave(step, data_len[i],amp);
        sprintf(appname[i], "stepwave");
        sprintf(inputfile[i], "stepwave_%d.txt", i);
        
    }
    /*
     //set 3 random
     for (int i=20; i<30; i++) {
     
     data_len[i] = rand()%5 + 10;
     sprintf(appname[i], "random");
     sprintf(inputfile[i], "random_%d.txt", i);
     dataset[i] = (double*) malloc(sizeof(double)*data_len[i]);
     
     for (int j=0; j<data_len[i]; j++) {
     
     dataset[i][j] = (double) (rand()%10);
     }
     
     }*/
    
    //print out the dataset
    printf("\nInput set generated......");
    fprintf(parallel_synthesis_file, "Input set:\n");
    for (int i=0; i<set_no; i++) {
        
        for (int j=0; j<data_len[i]; j++) {
            fprintf(parallel_synthesis_file, "%f  ", dataset[i][j]);
        }
        
        fprintf(parallel_synthesis_file, "\n\n");
        
    }
    
    fclose(parallel_synthesis_file);
    
    printf("\nCalc calcPow2.......");
    calcPow2();
    
    printf("\nStart extractU_shapelet.......");
    char* outputfile = "parallel_synthesis_output.txt";
	for (int i=0; i<set_no; i++) {
        	 extractU_Shapelets(dataset, data_len, set_no, 3, appname, inputfile, outputfile, 3, i);
	}

    for (int i=0; i<set_no; i++) {
        free(dataset[i]);
    }
    
    free(data_len);
    
}
