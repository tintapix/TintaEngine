

__kernel void image( __global unsigned char *A, int channel, float factor, int channels ) {  
	
	int pixel = get_global_id(0) * channels;
	
	/*
		unsigned char r =  	A[pixel];
		unsigned char g =  	A[pixel+1];
		unsigned char b =  	A[pixel+2];	
	*/
	
   
   if( (float)A[pixel + channel]/255 > factor ){
   
		A[pixel] = 255;
		A[pixel+1] = 255;
	    A[pixel+2] = 255;
		
   }
   else {   
	   A[pixel]   = 0;
	   A[pixel+1] = 0;
	   A[pixel+2] = 0;
   }   
   
	
	
}