
/*
	channel(rgba) - 0,1,2,3
*/
__kernel void image( __global unsigned char *A, __global unsigned char *pallete, int palleteSize, int channel, int channels,int channelsPallete ) {  
	
	int pixel = get_global_id(0) * channels;
		
	unsigned char val =  A[ pixel + channel ];
	
	float valf = ((float)val)/255.f;
	
	int pos = (int)( valf * (float)(palleteSize  - 1)) * channelsPallete;
	
	
	A[pixel]   = pallete[pos];
	A[pixel+1] = pallete[pos + 1];
	A[pixel+2] = pallete[pos + 2]; 
	
	
}


