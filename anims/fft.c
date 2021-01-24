/*****************************************************************************/
/*  Fast Fourier Transform Animation - by Tim Dean:email dean@cc.gatech.edu  */
/*  For use with the XTANGO algorithm animation system.                      */
/*                                                                           */
/*  This demonstrates the computation of the FFT using trhe Cooley-Tukey     */
/*  radix-2 decimation-in-time algorithm. Animation routines are located in  */
/*  the file "fftscenes.c"                                                   */
/*****************************************************************************/


#include <math.h>
#include <stdio.h>
#include "xtango.h"

#define PI (4.0*atan(1.0))
#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

void ANIMInit(), ANIMFlip(), ANIMSwap(),ANIMBitErase(),ANIMShift();
void ANIMUnitCircle(),ANIMGetTwiddle(),ANIMButterfly(),ANIMUpdate();

static NAME_FUNCT fcn[] = { {"Init",      1, {VOID, (FPTR)ANIMInit}},
			    {"Flip",      1, {VOID, (FPTR)ANIMFlip}},
			    {"Swap",      1, {VOID, (FPTR)ANIMSwap}},
			    {"Erase",     1, {VOID, (FPTR)ANIMBitErase}},
			    {"Shift",     1, {VOID, (FPTR)ANIMShift}},
			    {"Circle",    1, {VOID, (FPTR)ANIMUnitCircle}},
			    {"Twiddle",   1, {VOID, (FPTR)ANIMGetTwiddle}},
			    {"Butterfly", 1, {VOID, (FPTR)ANIMButterfly}},
			    {"Update",    1, {VOID, (FPTR)ANIMUpdate}},
			    {NULL,  0, NULL, NULL} };

main(){
  float *data;
  int SIZE,nn,flag1,flag2;
  int n,mmax,m,j,istep,i;
  double wtemp,wr,wpr,wpi,wi,theta;
  float tempr,tempi;

/*    *****   Print Input Instructions    *****    */
  printf("FFT Algorithm Animation - By Tim Dean\n");
  printf("Input Should Be\n");
  printf("\t1)2 Times the size of the FFT, N, which must be power of 2\n");
  printf("\t2)N Complex Values. Alternate Real and Imaginary Parts\n");
  printf("\nFor simplest use, place input in file and pipe through stdin\n");

/*    *****   Initialization    *****    */
  scanf("%d",&SIZE);
  nn=SIZE/2;
  data=(float *)calloc(SIZE,sizeof(float));
  if(data == NULL) fprintf(stderr,"Error Allocating Array Space\n");
  data--;                                 /*  data indexes from 1 to size  */

/*    *****   Read In Data From stdin.  Use 'fft < fft.data'  *****    */
  for(i=1;i<=SIZE;i+=2)
    scanf("%f %f",data+i,data+i+1);   /*  Read in Complex Valued Input  */

  TANGOalgoOp(fcn, "BEGIN");
  TANGOalgoOp(fcn, "Init", SIZE, data);
 TANGOalgoOp(fcn, "Flip"); 
  
/*    *****   Bit Reversal Done Here   *****    */
  j=1;
  n=nn << 1;
  for(i=1;i<n;i+=2){
    if(j > i){
      SWAP(data[j],data[i]);
      SWAP(data[j+1],data[i+1]);
      TANGOalgoOp(fcn, "Swap", (i-1)/2 , (j-1)/2);
    }  /*  IF  */
    m=n >> 1;
    while(m>=2 && j>m){
      j -= m;
      m >>= 1;
    }  /*  WHILE  */
    j += m;
  }  /*  FOR  */

  TANGOalgoOp(fcn, "Erase");  
  TANGOalgoOp(fcn, "Shift");

/*    *****   Fourier Transform Calculated Here   *****    */
  mmax=2;
  while (n > mmax){
    TANGOalgoOp(fcn, "Circle", mmax);
    istep=2*mmax;
    theta= -(2*PI)/mmax;
    wtemp=sin(0.5*theta);
    wpr= -2.0*wtemp*wtemp;
    wpi=sin(theta);
    wr=1.0;
    wi=0.0;
    for(m=1;m<mmax;m+=2){
      for(i=m;i<=n;i+=istep){	
	j=i+mmax;
	tempr=wr*data[j]-wi*data[j+1];
	tempi=wr*data[j+1]+wi*data[j];
	if(i == m && i > n-istep)
	  flag1=1;
	else if(i == m)
	  flag1=2;
	else if(i > n-istep)
	  flag1=3;
	else
	  flag1=4;
	TANGOalgoOp(fcn,"Twiddle",(m-1)/2,mmax,wr,wi,
		    (j-1)/2,tempr,tempi,flag1);
	if(i == 1)
	  flag2=1;
	else if(j == n-1)
	  flag2=3;
	else
	  flag2=2;
	TANGOalgoOp(fcn,"Butterfly",data[i]+tempr,data[i+1]+tempi,
		    data[i]-tempr,data[i+1]-tempi,data[i],data[i+1],data[j],
		    data[j+1],(i-1)/2,(j-1)/2,flag2);
	data[j]=data[i]-tempr;
	data[j+1]=data[i+1]-tempi;
	data[i]+=tempr;
	data[i+1]+=tempi;
      }  /*  FOR I  */
      wr=(wtemp=wr)*wpr-wi*wpi+wr;
      wi=wi*wpr+wtemp*wpi+wi;
    }  /*  FOR M  */

    TANGOalgoOp(fcn,"Update");
    mmax=istep;
  }  /*  WHILE  */

/*     *****   Print Out Results Here    *****    */
  printf("The Results Are:\n");
  printf("Real\tImag\n\n");
  for(i=1;i<SIZE;i+=2)
    printf("%7.4f\t%7.4f\n",data[i],data[i+1]);
  free(data+1);

  TANGOalgoOp(fcn, "END");
}  /*  MAIN  */
