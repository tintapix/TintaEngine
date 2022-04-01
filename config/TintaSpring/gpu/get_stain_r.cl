
// monochrome rough
__kernel void image( __global unsigned char *A, unsigned int r, unsigned  int g, unsigned  int b, float factor, int channels ) {  
	
	int pixel = get_global_id(0) * channels;
	
	unsigned char rCh = (unsigned char) r;
	unsigned char gCh = (unsigned char) g;
	unsigned char bCh = (unsigned char) b;					
	
   float3 v1 = (float3)( (float)A[pixel],(float)A[pixel+1],(float)A[pixel+2] );
   
   float3 v2 = (float3)( (float)rCh,(float)gCh,(float)bCh );    
   
   unsigned char color = distance(v1,v2) < factor ? 255 : 0;
   
   if ( channels > 3 ) 
		A[pixel+3] = color;
	
}
