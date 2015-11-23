#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

double get_wall_time();

int main(int argc, char * argv[])
{
	int i, j, n, size;
	int N = 8; //size of the matrix
	int Nc = 10; //number of colors
	int total_image[N*N];
	int local_hist[Nc], total_hist[Nc];
	double time1, time2, time3, time4, time5, time6;

	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&n);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	
        int scat_size = N*N/size;
	int scattered_image[scat_size];

	for(i = 0; i < Nc; i++){
		local_hist[i] = 0;
	}

	if(n == 0){
		printf("\nSize:%i\n",size);
		for(i = 0; i < N; i++){
			for(j = 0; j < N; j++){
				total_image[i*N+j] = rand()%Nc;
				printf("%i \t",total_image[i*N+j]);
			}
			printf("\n");
		}
	}

	printf("\n Rank: %i ", n);
	printf("\n------------------------------------\n");

 	time1 = get_wall_time();
	MPI_Scatter(&total_image,scat_size,MPI_INT,&scattered_image, scat_size,MPI_INT,0,MPI_COMM_WORLD);
        time2 = get_wall_time();
	
	for(i = 0; i < scat_size; i++){
		printf("%i \t",scattered_image[i]);
	}

	time3 = get_wall_time();
	for(i = 0; i < scat_size; i++){
		for(j = 0; j < Nc; j++){
			if (scattered_image[i] == j)
				local_hist[j] += 1;
		}
	}
//	printf("\n Rank:%i Time to scatter pixels to processes = \t%lf\n",n, time2-time1);
	time4 = get_wall_time();        
	printf("\n Time to calculate local histogram on process %i  = \t%lf\n",n,time4-time3);
/*
         printf("\n Rank: %i \n", n);
         for(i = 0; i < Nc; i++) 
		printf("%i ",local_hist[i]);
		printf("\n");
*/
        MPI_Barrier(MPI_COMM_WORLD);
	time5 = get_wall_time();
	MPI_Reduce(&local_hist,&total_hist,Nc,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
	time6 = get_wall_time();       
//	printf("\n Rank:%i Time to reduce local histograms into a global histogram = \t%lf\n",n,time6-time5);

        MPI_Barrier(MPI_COMM_WORLD);
	if(n == 0) {
		printf("\nTotal histogram:");
		printf("\n-----------------------------------\n");
		for(i = 0; i < Nc; i++)	printf(" %i ",total_hist[i]); 
		printf("\n");
	}
	
	MPI_Finalize();
}


double get_wall_time(){
	struct timeval time;
	gettimeofday(&time,NULL);
	return (double)time.tv_sec+(double)time.tv_usec/1000000; 
}






