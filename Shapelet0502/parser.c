//
//  parser.c
//  ParallelComputeGap
//
//  Created by Mushroom on 4/30/13.
//  Copyright (c) 2013 Self. All rights reserved.
//
#include "parser.h"

int Contains(char* MyChar, char* Search)
{
    int LoopOne = 0;
    int LoopTwo;
    int LoopThree;
    int MyCharSize = (int) strlen(MyChar);
    int SearchSize = (int) strlen(Search);
    int Yes = 0;
    
    while(LoopOne < MyCharSize)
    {
        if(MyChar[LoopOne] == Search[0])
        {
            LoopTwo = 0;
            LoopThree = LoopOne;
            while(LoopTwo < SearchSize)
            {
                if(MyChar[LoopThree] == Search[LoopTwo])
                    Yes++;
                
                LoopTwo++;
                LoopThree++;
            }
            if(Yes == SearchSize)
                return 1;
        }
        LoopOne++;
    }
    return 0;
}

double* getColumn(int field, int nrow, Dataset *s)
{
    
	double *output = (double *) malloc (sizeof(double) * nrow);
    
	for (int i = 0; i < nrow; i++) {
		
		output[i] = s[i].count[field - 1];
	}
    
	return output;
	
}

int parse(FILE* fp, Dataset *s , int cnt){
    
  	int endoffile = 0;
   	int i = 0;
	int err = 1;
	int num=0;
    
    while(i< cnt){
        
		if(feof(fp)) {
            endoffile=1;
            break;
		}
		err = fread(&s[i], sizeof(int), NCOLS, fp);
		if(s[i].count[0] == 0 || err < NCOLS) {
			//printf("read less\n");
			continue;
		}
		i++;
    }
    
	if(endoffile || feof(fp))
		return -1;
    
	return i;
}

void Dumpdata(double * data, int cnt)
{
	
	for(int i=0; i < cnt; i++) {
        
		printf("%2.4f \n", data[i]);
		
	}
    
	printf("\n");
    
}

int readFile(int field, char *filename, char *m_appname, double **extractset)
{
	FILE *fp;
	int offset = 0;
	int NREC = 2;
	int i;
	int TOTAL = NREC;
  	int endoffile = 0;
	Dataset *store;
	char m_pathname[LEN];
    
	fp=fopen(filename,"r");
    
    
	if(fp==NULL || feof(fp)) {
		perror("File open failed\n");
		return -1;
	}
    
	unsigned char ignore;
	char* tracename = NULL;
	size_t t_n = 0;
	getline(&tracename, &t_n, fp);
    //	tracename[t_n]='\0';
	free(tracename);
    
	fread(&ignore, 1, 1, fp);
    
    
	for( i =0; i < LEN; i++){
        
		fread(&m_appname[i], 1, 1, fp);
		if(m_appname[i] == '\0') {
			break;
		}
	}
    
	//printf("%s %d\n\n", m_appname, i);
    
	for(i =0; i < LEN; i++){
		fread(&m_pathname[i], 1, 1, fp);
		if(m_pathname[i] == '\0') {
			break;
		}
	}
    
    
	store= (Dataset*) malloc(NREC * sizeof(Dataset));
	int cnt = 0;
	while((cnt = parse(fp, store+offset, NREC)) >= 0) {
		if(cnt < NREC)
			TOTAL = TOTAL - NREC + cnt;
		offset = TOTAL;
		TOTAL += NREC;
		//printf("Reallocation %d and reading %d\n", TOTAL, NREC);
		store = (Dataset*) realloc((void*) store, TOTAL * sizeof(Dataset));
	}
    
    
	if(cnt < NREC)
		TOTAL = TOTAL - NREC + cnt;
    
	*extractset = getColumn(field, TOTAL, store);
    
	/*for (int i = 0; i < TOTAL; i++) {
     
     extractset[i] = (double) store[i].count[field - 1];
     }*/
    
	//Dumpdata(*extractset, TOTAL)；
    fclose(fp);
    
	free((void*) store);
	return TOTAL;
}

/*
 int main(int argc, char *argv[]) {
 
 int err;
 int TOTALFILES = 1;
 int field = 0;
 //for all the files in the directory
 if (argc != 3) {
 
 printf("Check usage: %s field(2-7) path/filename\n", argv[0]);
 return 0;
 }
 
 //get the field
 field = atoi(argv[1]);
 
 if (field < 1 || field > 7) {
 
 printf("Field beyond range 1...7 \n");
 return 0;
 }
 
 
 char appname[TOTALFILES][LEN];
 double *dataset[TOTALFILES];
 char filename[TOTALFILES][LEN];
 
 //0...TOTALFILES - 1
 int index = 0;
 double *arr = dataset[index];
 strcpy(filename[index], argv[2]);
 err = readFile(	field, filename[index], appname[index], &arr);
 //get the app dataset
 
 Dumpdata(arr, err);
 printf("appname is %s\n", appname[index]);
 printf("Total records is %d\n", err);
 
 free(arr);
 
 //extractUshapelet();
 }*/

