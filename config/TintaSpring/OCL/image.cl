

__kernel void image(__global unsigned char *A,  __global unsigned char *B ) {  
	
	int pixel = get_global_id(0) * 4;
	
	unsigned char r =  	A[pixel];
	unsigned char g =  	A[pixel+1];
	unsigned char b =  	A[pixel+2];	
	
   if( (r -  g) > 50  || ( r - b) > 50 ){
		B[pixel+3] = 0;
		B[pixel+1] = 250;
	    B[pixel+2] = 10;
   }
   else {   
	   B[pixel]   = 0;
	   B[pixel+1] = 0;
	   B[pixel+2] = 0;
   }
   
   
   B[pixel+3] = 255;
	
	
}