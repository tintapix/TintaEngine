
// smooth
__kernel void image( __global unsigned char *A, unsigned int r, unsigned  int g, unsigned  int b, float factor, int channels, int threshold ) {  
	
	int pixel = get_global_id(0) * channels;
		
	unsigned char rCh = (unsigned char) r;
	unsigned char gCh = (unsigned char) g;
	unsigned char bCh = (unsigned char) b;	
	
    float3 v1 = (float3)( (float)A[pixel],(float)A[pixel+1],(float)A[pixel+2] );
   
    float3 v2 = (float3)( (float)rCh,(float)gCh,(float)bCh );
   		
	unsigned char v = (unsigned char)( mix(  0.f, (float)A[pixel+3], min( distance(v1,v2)/factor, 1.0f ) ) );
	 
	A[pixel+3] = v < threshold ? 0 : v;
   
}
