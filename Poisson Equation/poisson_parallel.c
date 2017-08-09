#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include<omp.h>
#include<time.h>
#define niters 10000
#define nthrds 4
int n=200,nth_res,nth_step;
double res[200][200],pot[200][200],rho[200][200],lnorm,h,alpha;
int sum;

void residual(void)
{   sum=0;
    int j;
    omp_set_num_threads(nthrds);
    #pragma omp parallel
    {   int i;
        int ID=omp_get_thread_num();
        if(ID==0)nth_res=omp_get_num_threads();
        #pragma omp for reduction(+:sum)
            for(j=1;j<n-1;j++)
                for(i=1;i<n-1;i++)
                {   res[i][j]=(pot[i-1][j]+pot[i+1][j]+pot[i][j-1]+pot[i][j+1]-4*pot[i][j])/(h*h)-4*3.14*rho[i][j];
                    sum+=res[i][j]*res[i][j];
                }
    }
}

void step(void)
{
    int j;
    omp_set_num_threads(nthrds);
    #pragma omp parallel
    {
        int i;
        int ID=omp_get_thread_num();
        if(ID==0)nth_step=omp_get_num_threads();
        #pragma omp for
        for(j=0;j<n;j++)
            for(i=0;i<n;i++)
                pot[i][j]+=alpha*h*h*res[i][j];
    }
}
int main()
{   clock_t time;
    time=clock();
    h=1/(double)n;alpha=.24;
    int i;
    omp_set_num_threads(nthrds);
     #pragma omp parallel
     {  int j;
        #pragma omp for
         for(i=0;i<n;i++)
          for(j=0;j<n;j++)
        {
        res[i][j]=0;
        pot[i][j]=0;
        rho[i][j]=0;
        }
     }
    #pragma omp barrier
       rho[n/2][n/2]=n*n;
    residual();
    lnorm=sqrt(sum)/(double)(n-1);
    printf("Iteration0\t%g\n",lnorm);
     omp_set_num_threads(nthrds);
    #pragma omp for
    for(i=1;i<=niters;i++)
    {
        step();
        residual();
        if(i%10==0)
        {
            lnorm=sqrt(sum)/(double)(n-1);
            printf("Iteration %d\t%g\n",i,lnorm);
        }
    }
    time=clock()-time;
    printf("Execution Time: %f \n",((double)time)/CLOCKS_PER_SEC);
}
