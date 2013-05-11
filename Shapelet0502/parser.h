//
//  parser.h
//  ParallelComputeGap
//
//  Created by Mushroom on 4/30/13.
//  Copyright (c) 2013 Self. All rights reserved.
//

#ifndef ParallelComputeGap_parser_h
#define ParallelComputeGap_parser_h

#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define LEN 100
#define NCOLS 7
typedef unsigned int uint32_t;


typedef struct Sample{
	
	uint32_t count[NCOLS];
    
}Dataset;

/*
 typedef struct appSample {
 
 char m_appname[100];
 char m_pathname[100];
 Dataset *store;
 }App_dataset;
 */

//App_dataset g_appdata;
/*
 void Sample::printSample(void) {
 
 
 printf("%d %d %d %d %d %d %d\n", cycles,\
 counts[0], counts[1], counts[2], \
 counts[3], counts[4], counts[5]);
 
 }
 */

//private
double * getColumn( int field, int nrow, Dataset *s);
int parse(FILE*  fp, Dataset *s , int cnt);
void Dumpdata(double * data, int cnt);

//public
int Contains(char* MyChar, char* Search);
int readFile(int field, char *filename, char *m_appname, double **extractset);


#endif
