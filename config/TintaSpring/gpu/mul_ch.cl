
__kernel void image( __global unsigned char *A, int channel, float factor , int channels ) {  
	
	int pixel = get_global_id(0) * channels;	
    int posCh  = pixel + channel;
	
	A[ posCh ] =  (unsigned char) ( (float)A[ posCh ]  * factor );   
}
