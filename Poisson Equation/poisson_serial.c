#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include<time.h>
#define niters 10000
int n=200;
double res[2000][2000],pot[2000][2000],rho[2000][2000],lnorm,h,alpha;
int sum;


void residual(void)
{   sum=0;
    int i,j;
    for(j=1;j<n-1;j++)
        for(i=1;i<n-1;i++)
        {res[i][j]=(pot[i-1][j]+pot[i+1][j]+pot[i][j-1]+pot[i][j+1]-4*pot[i][j])/(h*h)-4*3.14*rho[i][j];
        sum+=res[i][j]*res[i][j];}

}

void step(void)
{
    int i,j;
    for(j=0;j<n;j++)
        for(i=0;i<n;i++)
        pot[i][j]+=alpha*h*h*res[i][j];
}
int main()
{   clock_t time;
    time=clock();
    h=1.0/(double)n;alpha=.24;
    int i,j;
     for(i=0;i<n;i++)
        for(j=0;j<n;j++)
    {
        res[i][j]=0;
        pot[i][j]=0;
        rho[i][j]=0;
    }
    rho[n/2][n/2]=n*n;
    residual();
    lnorm=sqrt(sum)/(double)(n-1);
    printf("Iteration0\t%g\n",lnorm);
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
    printf("Execution Time: %f",((double)time)/CLOCKS_PER_SEC);
}
