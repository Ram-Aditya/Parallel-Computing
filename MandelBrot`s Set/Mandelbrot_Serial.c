#include <stdio.h>
#include <math.h>
#include<time.h>

 int main()
 {	clock_t ti;
	ti=clock();
	int num_thrds=4;
    
        int ix,iy;
        const int ix_max = 800;
        const int iy_max = 800;
        
        double x,y;
        const double x_min=-2, x_max=2.0,y_min=-2.0,y_max=2.0;
    
        double pix_w=(x_max-x_min)/ix_max;
        double pix_h=(y_max-y_min)/iy_max;
  	int t;
        const int t_max=200;
	// radius of circle
        const double esc_rad=2;	
       
        const int max_color_comp=255;
        FILE * fp;
        char *filename="new1.ppm";
        char *comment="# ";
	static unsigned char color[3];
  
        double zx,zy,zx2,zy2;
     
        fp= fopen(filename,"wb"); 
        fprintf(fp,"P6\n %s\n %d\n %d\n %d\n",comment,ix_max,iy_max,max_color_comp);
        
        for(iy=0;iy<iy_max;iy++)
        {
             y=y_min + iy*pix_h;
             if (fabs(y)< pix_h/2) y=0.0; //Antenna
	    
             for(ix=0;ix<ix_max;ix++)
             {
                        x=x_min + ix*pix_w;
                       
                        zx=0.0;
                        zy=0.0;
                        zx2=zx*zx;
                        zy2=zy*zy;
  			
			
                        for (t=1;t<t_max && (zx2+zy2)<esc_rad*esc_rad;t++)
                        {
                            zy=2*zx*zy +y;
                            zx=zx2-zy2 +x;
                            zx2=zx*zx;
                            zy2=zy*zy;
                        }

                        if (t==t_max)
                        { 
                           color[0]=0;
                           color[1]=0;
                           color[2]=0;
                        }
                     else
                        { 
                             color[0]=255; /* Red*/
                             color[1]=255;  /* Green */
                             color[2]=255;/* Blue */
                        };
                       //make file pointer move according to the iteration number or pixel co-ordinate.
			fseek(fp,(iy*800+ix)*3+22,0);
                        fwrite(color,1,3,fp);
                }
        }
        fclose(fp);
	ti=clock()-ti;
	printf("Execution Time: %f \n",((double)ti)/CLOCKS_PER_SEC); 
        return 0;
 
 }
