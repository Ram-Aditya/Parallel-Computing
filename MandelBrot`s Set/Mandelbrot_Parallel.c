
#include <stdio.h>
 #include <math.h>
 #include<omp.h>

 int main()
 {	int num_thrds=4;
    
        int ix,iy;
        const int ix_max = 800;
        const int iy_max = 800;
        
        double x,y;
        const double x_min=-2.5, x_max=1.5,y_min=-2.0,y_max=2.0;
    
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
  
        double Zx, Zy,Zx2,Zy2;
     
        fp= fopen(filename,"wb"); 
        fprintf(fp,"P6\n %s\n %d\n %d\n %d\n",comment,ix_max,iy_max,max_color_comp);
       
	omp_set_dynamic(0);
        omp_set_num_threads(num_thrds);
        #pragma omp parallel for private(y) schedule(runtime)
        for(iy=0;iy<iy_max;iy++)
        {
             y=y_min + iy*pix_h;
             if (fabs(y)< pix_h/2) y=0.0; //Antenna
             #pragma omp parallel for private(x,Zx,Zy,Zx2,Zy2) schedule(runtime)
             for(ix=0;ix<ix_max;ix++)
             {
                        x=x_min + ix*pix_w;
                       
                        Zx=0.0;
                        Zy=0.0;
                        Zx2=Zx*Zx;
                        Zy2=Zy*Zy;
  			
			
                        for (t=1;t<t_max && (Zx2+Zy2)<esc_rad*esc_rad;t++)
                        {
                            Zy=2*Zx*Zy +y;
                            Zx=Zx2-Zy2 +x;
                            Zx2=Zx*Zx;
                            Zy2=Zy*Zy;
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
        return 0;
 
 }
