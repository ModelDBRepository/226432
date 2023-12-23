// Program for Simulating MAT* Model (Front. Comput. Neurosci. 3, 9)
// Dec 22, 2009, Ryota Kobayashi
//
// The fluctuating current should be written in the input file "current.txt"
// This file consists of a column, i.e.
//    Current[nA].
// 
// This program generates the output file "voltage.txt" and "spiketime.txt".
// "voltage.txt" consists of two columns, i.e.
//   Time[ms], Voltage[mV] (measured from the resting potential)
// "spiketime.txt" consists of a column, i.e.
//   SpikeTimes [ms]
//
// See details in Kobayashi R., Tsubo Y., and Shinomoto S.,
// Made-to-order spiking neuron model equipped a multi-timescale adaptive threshold, Front. Comput. Neurosci. 3, 9, (2009).
//
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{       
  FILE  *f_cur, *f_volt, *f_sp;
  if ( argc!= 4 )
    {  printf("./a.out alpha_1 alpha_2 omega\n");   exit(1);  }
  
  // Parameters for Leaky Integrator.
  double v= 0, tau= 5.0, R= 50.0;

  // Parameters for Adaptive Threshold
  int  t_f=0, t_ref= 20;   double v_th, x[2]= {0};
  double alpha[2], omega, tau_1=10, tau_2= 200;
  alpha[0]= atof(argv[1]);    alpha[1]= atof(argv[2]);   omega= atof(argv[3]);
  
  // Simulation Parameters
  int  i, num= 0;    double tmp, dt= 0.1;  // dt: Sampling Interval.
  
  f_cur= fopen("current.txt","r");    // Current File
  if ( f_cur == NULL)  {  printf("Cannot open *f_in\n");  exit(1);   }
  
  while (  (fscanf( f_cur, "%lf", &tmp)) != EOF )  {  num ++;  }
  fclose(f_cur);
  printf("T: %d [ms]\n", (int)(num*dt) );
  
  double I_ex;
  f_cur = fopen("current.txt","r");    // Input File 
  f_volt= fopen("voltage.txt","w");    // Voltage File
  f_sp  = fopen("spiketime.txt","w");  // Spike Times File
  
  for(i=0; i<num; i++)   // Simulating MAT model (Fig 5.B)
    { 
      fscanf( f_cur, "%lf", &I_ex);
      // Leaky Integrator: Corrected on 8th, June, 2016 
      v= (1- 1.0/tau*dt )* v+ (R/tau)*dt* I_ex;
      
      // Adapting Threshold
      x[0]= (1-1.0/tau_1*dt )* x[0];    // Fast：tau_1= 10 ms
      x[1]= (1-1.0/tau_2*dt )* x[1];    // Slow: tau_2= 200 ms
      v_th= omega+ x[0]+ x[1];
      
      // Condition for Spike Generation (v_th: threshold, t_ref: reflactory period)
      if ( v_th< v && t_f+ t_ref< i+1 )
	{  
	  x[0] += alpha[0];    x[1] += alpha[1];   t_f= i+1;
	  fprintf( f_volt, "%f 100\n", i*dt);  // Output Voltage [mV]
	  fprintf( f_sp, "%f\n", t_f*dt );    // Output SpikeTime [ms]
	}      
      else  // Output Voltage [ms]
	{  fprintf( f_volt, "%f %f\n", i*dt, v);   }
    }
  
  fclose(f_cur);    fclose(f_volt);    fclose(f_sp);
  return 0;
}
