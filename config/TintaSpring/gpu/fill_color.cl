

__kernel void image( __global unsigned char *A, unsigned int r, unsigned  int g, unsigned  int b, float factor, int channels ) {  
	
	int pixel = get_global_id(0) * channels;
		
		

       
   A[pixel] 	= (unsigned char) mix(   (float)(A[pixel]),     (float)(r), factor );
   A[pixel + 1] = (unsigned char) mix(   (float)(A[pixel + 1]), (float)(g), factor );
   A[pixel + 2] = (unsigned char) mix(   (float)(A[pixel + 2]), (float)(b), factor );
   
}
