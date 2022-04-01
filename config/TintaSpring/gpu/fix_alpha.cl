
__kernel void image( __global unsigned char *A, int minimal, int channels  ) {  
	
	int pixel = get_global_id(0) * channels;	
    
	if ( A[ pixel + 3 ]  < minimal )
		 A[ pixel + 3 ] = 0;		
    
}
