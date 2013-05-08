
#include "PextractUShapelets.h"

#define UPPER 2
#define LOWER 1
#define STEP 1

void compute_mean_stddev(int *dataset_A, int dataset_Alen, double *dist, int newsize, double *mean, double *stddev)
{
    
	double sum = 0.0, avg = 0.0, dev = 0.0;
	int i;
    
    
	for(i = 0; i < dataset_Alen; i++) {
        
		sum += dist[dataset_A[i]];
		
	}
    
    
	avg = sum/dataset_Alen;
    
	for(i=0; i < dataset_Alen; i++) {
        
        dev += pow((dist[dataset_A[i]] - avg),  2.0);
        
	}
    
	*mean = avg;
	*stddev = sqrt(dev / dataset_Alen);
}


int clustered(int newcluster, int newsize) {
    
	printf("Newsize is %d Newcluster is %d\n", newsize, newcluster);
	return ((newsize == newcluster) || (newcluster < 2));
}


int max_index(double *gap, int total)
{
	int max = 0;
	int i;
	for(i=1; i< total; i++) {
		if(CompareDoubles2(gap[max],  gap[i]) < 0) {
			max = i;
		}
	}
	return max;
}

void* compute_gap_thread(void* arg)
{
    
	int index, i;
    
	struct computegap_spwn_param *param = (struct computegap_spwn_param*) arg;
    
	//param.tid = tid;
	int subseqlen = param->subseqlen;
	int cnt_indx = param->cnt_indx;
	double *ts = param->ts;
	int ts_len = param->ts_len;
	int subseq_row = param->subseq_row;
	int dataset_no = param->dataset_no;
	int *old_map = param->old_map;
	double *dt = param->dt;
	double *gap = param->gap;
	int cluster_no = param->cluster_no;
	int *subseq_col = param->subseq_col;
	double** p_subseq = param->p_subseq;
	int * ps_len = param->ps_len;

	int total = ts_len-subseqlen+1;


	struct computeGap_para *para = (struct computeGap_para *) malloc(total * sizeof (struct computeGap_para));
    
	for(index=cnt_indx, i=0; index < (cnt_indx + total); index++, i++) {
        
        
		/*Compute the gap and threshold for each of the subsequence*/
        
		p_subseq[index] = ts + i;
        	ps_len[index] = subseqlen;
        
        
		subseq_col[index] = i;
		dt[index] = 0.0;
		gap[index] = 0.0;
        
        //initialize parameter
		para[i].threshold_dt = &dt[index];
		para[i].threshold_maxGap = &gap[index];
        	para[i].cluster_no = cluster_no;
        	para[i].shapelet_row_id = subseq_row;
        	para[i].shapelet_col_start_id =  i;
        	para[i].shapelet_len = subseqlen;
        	para[i].index_marker = old_map;
        	para[i].index_marker_len = dataset_no;
        

        	computeGap((void*) &para[i]);
        
        	printf("Computed gap  %f dt %f index %d \n", gap[index], dt[index], index);
	}
	free(para);
	pthread_exit(NULL);
}

void spawn_thread(int* subseqlen, double *ts,int ts_len, int subseq_row, int dataset_no, int *old_map, double *dt, double *gap, int cluster_no, int *subseq_col, double *p_subseq[], int *ps_len, int x)
{
    
	int k;
    
	pthread_t tid[x];
	int prev = 0;
	struct computegap_spwn_param *param = malloc(x * sizeof(struct computegap_spwn_param));
	int cnt_indx = 0;
	
	for(k=0 ; k<x; k++) {
        	
		//param.tid = tid;
        	param[k].subseqlen = subseqlen[k];
        	param[k].cnt_indx = cnt_indx;
        	param[k].ts = ts;
        	param[k].ts_len = ts_len;
        	param[k].subseq_row = subseq_row;
        	param[k].dataset_no = dataset_no;
        	param[k].old_map = old_map;
        	param[k].dt = dt;
        	param[k].gap = gap;
        	param[k].cluster_no = cluster_no;
        	param[k].subseq_col = subseq_col;
        	param[k].p_subseq =  p_subseq;
        	param[k].ps_len = ps_len;
        
        
    		int ret = pthread_create(&tid[k], NULL, compute_gap_thread, (void*) &param[k]);
        
        	if(ret) {
            
            		printf("Pthread creation failed %d\n", ret);
        	}
        
        	prev = ts_len - subseqlen[k] +1;
        	cnt_indx += prev;
	}
    
    
	for(k=0 ; k<x; k++) {
		pthread_join(tid[k], NULL);
	}

	free(param);
    
}


void extractU_Shapelets(double **pd_Dataset, int* ds_len, int n_sample, int sLen, char appname[][100], char inputfiles[][100], char *outputname, int input_cluster_no,int start_ts_id)
{
    
	int sl;
	int iter = 0;
	int cluster_id[n_sample];
	//int dataset_A[n_sample];
	//int dataset_Alen;
	double *ts;
	int ts_len;
	int cnt;
	int newsize;
	int k,j,x;
	int index, index2;
	double mean, stddev, range;
    
	if(sLen - LOWER <=0  ) {
        
		printf("Shapelet avg length not within LOWER BOUND\n");
		return;
	}
    
	FILE *fp;
    
	fp = fopen(outputname, "a");
    
	if(!fp) {
		perror(outputname);
		return;
	}
    
	/*Empty discriminatory ushapelets*/
	double* ushapelet[n_sample];
	int ushapelet_len[n_sample];
	int ushapelet_row[n_sample];
	int ushapelet_col[n_sample];
	
	ts = pd_Dataset[start_ts_id];
	ts_len = ds_len[start_ts_id];
   
    
	printf("\nextractU_Shapelet %d\n", start_ts_id);
    
	printf("result: %s\t%s\n",appname[start_ts_id],inputfiles[start_ts_id]);
	//memset(dataset_A, -1, n_sample *sizeof(int));
	memset(cluster_id, -1, n_sample *sizeof(int));
    
	int subseq_row = start_ts_id;
    
	int total_subs = ((sLen + UPPER - sLen + LOWER) / STEP) +1;
    	int subseq_len[total_subs];
    
	printf("TOTAL SUBS = %d \n", total_subs);
    	fprintf(fp, "\n\n\nNew Clustering Batch-------------\n");
    
	while(1) {
		
		cnt = 0;
		newsize = 0;
        
		fprintf(fp,"************************\n");
		fprintf(fp, "Iteration %d\n", iter);
        
		//printf("%d to %d in steps of %d\n", sLen-LOWER, sLen + UPPER, STEP);
		for(x=0,sl=sLen-LOWER; sl <= sLen+UPPER && sl <=ts_len; sl+=STEP) {
            
			if(sl <= ts_len) {
                		subseq_len[x] = sl;
         		        //printf("len is %d cnt is %d total till now is %d\n", sl, ts_len-sl, cnt);
		                //cnt += ts_len - subseq_len[x]+ 1;
                		cnt += ts_len - sl + 1;
                		++x;
			}
			else {
				printf("Sl is %d ts_len is %d\n", sl, ts_len);
			}
		}
		double* p_subseq[cnt];
		int ps_len[cnt];
		double gap[cnt];
		double dt[cnt];
		int subseq_col[cnt];
        
		for (k = 0, j= 0 ; k< n_sample; k++) {
            
			if(cluster_id[k] == -1)
				newsize++;
		}
        
		double* n_dataset[newsize];
		int n_datalen[newsize];
		int k,j;
        
		int old_id[newsize];
		memset(old_id, 0, newsize);
        
		memset(p_subseq, 0, cnt);
		/*create new unclustered data set*/
        
		for (k = 0, j= 0 ; k< n_sample; k++) {
            
			if(cluster_id[k] == -1) {
                
				// copy the data set and len and keep a mapping
                
				n_dataset[j] = pd_Dataset[k];
				n_datalen[j] = ds_len[k];
				old_id[j] = k;
				j++;
			}
		}
		
		
		double dist[newsize];
        
		/*index of the distance  within threshold*/
		int dataset_A[newsize];
		int dataset_Alen;
       
        
		memset(dataset_A, -1, newsize *sizeof(int));
        
		/*********VERIFY*******/
       
/*		if(ts_len < sLen) {
			
			printf(" The time series is too short to classify\n");
			
			break; // break?
		}*/
        
		int cluster_no = input_cluster_no;
        
        
		spawn_thread(subseq_len, ts, ts_len, subseq_row, newsize, old_id, dt, gap, cluster_no, subseq_col, p_subseq, ps_len, x);
        
        
		/*Find the subsequence which gives the maximum gap for the dataset*/
		printf("\nComputing max gap index\n");
        
		index = max_index(gap, cnt);
        
        
		/*Add the discriminatory subsequence to the ushapelet list*/
        
        	printf("Discovered ushapelet gap is %2.2f dt is %2.2f index %d oldrow %d col %d len %d\n", \
			gap[index], dt[index], index, subseq_row, \
               		subseq_col[index], ps_len[index]);
        
		ushapelet[iter] = p_subseq[index];
		ushapelet_len[iter] = ps_len[index];
		ushapelet_row[iter] = subseq_row;
		ushapelet_col[iter] = subseq_col[index];
        
		fprintf(fp, "Application %s\n Dataset path %s\n", appname[subseq_row], inputfiles[subseq_row]);
		fprintf(fp, "Shapelet row %d col %d len %d\n", ushapelet_row[iter], ushapelet_col[iter], ushapelet_len[iter]);
		dataset_Alen = 0;
		j=0;

        	int nThreads;
        	nThreads  = (newsize / bfactor) + ((newsize % bfactor) > 0) ;
        	int offset = 0;
        	int remainder = newsize;
        	threadInfo threadArg[nThreads];
        	pthread_t idThread[nThreads];
		struct computeGap_para gap_para;

		gap_para.distarray = dist;
        	gap_para.shapelet_row_id = subseq_row;
        	gap_para.shapelet_col_start_id = subseq_col[index];
        	gap_para.shapelet_len = ps_len[index];
        	gap_para.index_marker = old_id;

        	int create_result = 1;
        	for (int i=0; i<nThreads; i++) {

                	threadArg[i].start = offset;
                	threadArg[i].count = ((bfactor < remainder) ? bfactor : remainder);
                	threadArg[i].data = &gap_para;

                	create_result = pthread_create(&idThread[i], NULL, calcDistThread, &threadArg[i]);


                	while (create_result){

                        	printf("ERROR; return code from !calcPow2! pthread_create() is %s at thread no %d\n", strerror(create_result), i);

                        	create_result = pthread_create(&idThread[i], NULL, calcDistThread, &threadArg[i]);
                	}

                	offset +=  threadArg[i].count;
                	remainder -=   offset;


        	}

        	for(int i=0; i<nThreads; i++) {

                	pthread_join(idThread[i], NULL);

        	}

        	int i= 0,m = 0;
        	for(; i<newsize; i++) {

                	//dist[i] = param[i].distance;
                	/*If the computed distance is less than threshold then add to Dataset A*/
                	if (CompareDoubles2(dist[i], dt[index]) <= 0) {
                        	dataset_A[m] = i;
                        	m++;
                	}
        	}

        	dataset_Alen = m;

        
		
		printf("Checking for clustering\n");
        
		if(clustered(dataset_Alen, newsize)){
            		printf("Too small cluster!");
            		break;
        	}
		else {
            
			/*Find the dataset far away from the ushapelet*/
			index2 = max_index(dist, newsize);
			ts = n_dataset[index2];
			ts_len = n_datalen[index2];

			subseq_row = old_id[index2];
            
			printf("Finding next data set is at  index2%d subseqrow%d oldts_len %d ts_len %d\n", index2, subseq_row,ds_len[subseq_row], ts_len );
            
            
			mean = 0.0;
			stddev = 0.0;
			range = 0.0;
            
			/*Compute the mean standard deviation and range of the Dataset A*/
			compute_mean_stddev(dataset_A, dataset_Alen, dist, newsize, &mean, &stddev);
            
			range = mean + stddev;
            
			//printf("%2.2f is the range mean %2.2f stddev %2.2f \n", range, mean, stddev);
            
			/*Exclude all the dataset within the range by marking it as clustered*/
            
			for (k = 0, j= 0 ; k< newsize; k++) {
                
				if(CompareDoubles2(dist[k], range) <= 0) {
					//fprintf(fp, "%s\n", "Clustered dataset\n");
					cluster_id[old_id[k]] = iter;
					fprintf(fp, "Appname: %s Filename: %s\n", appname[old_id[k]], inputfiles[old_id[k]]);
				}
			}
		}
        
		++iter;
		
	}
    
   /* 
     printf("The discovered shapelets are: \n");
     for ( int z=0; z<=iter; z++) {
     
     printf("start %2.3f len %d row %d col %d\n", *(ushapelet[z]), ushapelet_len[z], ushapelet_row[z], ushapelet_col[z]);
     }*/
    
    fprintf(fp,"************************\n");
    fprintf(fp, "Remaining set\n");
	for( int z=0; z< n_sample; z++) {
		
        if(cluster_id[z]==-1)
            fprintf(fp, "Appname: %s Filename: %s\n", appname[z], inputfiles[z]);
        
	}
    
	fclose(fp);
	printf("\n");
}
