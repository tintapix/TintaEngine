
/*
	Replace channels with condition
	replace rgba - 0,1,2,3
	replaceWith rgba - 0,1,2,3
	invertValue = 0 default or 255
*/
__kernel void image( __global unsigned char *A, int replace, int replaceWith , int channels ) {  
	
	int pixel = get_global_id(0) * channels;	    
	A[pixel + replace ] = 255 - A[pixel + replaceWith ];		
    
}
