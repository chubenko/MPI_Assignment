#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char * argv[])
{
	int i, j, n;
	int N = 5; //array size
	int Nc = 10; //number of colors
	int total_image[N][N], scattered_image[N], local_hist[Nc], total_hist[Nc];
	
	MPI_Init(&argc,&argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&n);

	for(i = 0; i < Nc; i++){
		local_hist[i] = 0;
	//	total_hist[i] = -1;
	}

	if(n == 0){
		for(i = 0; i < N; i++){
			for(j = 0; j < N; j++){
				total_image[i][j] = rand()%Nc;
			}
		}
		for(i = 0; i < N; i++){
			for(j = 0; j < N; j++){
 	   			printf("%i \t",total_image[i][j]);
			}
			printf("\n");
		}
	}

 
	MPI_Scatter(&total_image,N,MPI_INT,&scattered_image,N,MPI_INT,0,MPI_COMM_WORLD);
        printf("\n Rank: %i \n ", n);
	for(i = 0; i < N; i++){
		printf("%i \t",scattered_image[i]);
	}

	for(i = 0; i < N; i++){
		for(j = 0; j < Nc; j++){
			if (scattered_image[i] == j)
				local_hist[j] += 1;
		}
	}
        
        printf("\n Rank: %i \n", n);
	for(i = 0; i < Nc; i++) 
		printf("%i ",local_hist[i]);
		printf("\n");

        MPI_Barrier(MPI_COMM_WORLD);
	MPI_Reduce(&local_hist,&total_hist,Nc,MPI_INT,MPI_SUM,0,MPI_COMM_WORLD);
       
        MPI_Barrier(MPI_COMM_WORLD);
	if(n == 0) {
		printf("Total: \n");
		for(i = 0; i < Nc; i++)	printf(" %i ",total_hist[i]); 
		printf("\n");
	}
	
	MPI_Finalize();
}
