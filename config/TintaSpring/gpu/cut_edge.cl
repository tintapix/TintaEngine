



unsigned char getA(  __global unsigned char *A, int xPos, int yPos, int w, int h, int channels ){
	
	int indexByte = ( yPos * w + xPos ) * channels;
	
	return ( xPos < 0 || xPos >= w || yPos < 0 || yPos  >= h ) ? 0 : A[indexByte + 3];	
}

__kernel void image(  __global unsigned char *write,__global unsigned char *read, int w, int h ) {  
	
	
		
	int index = get_global_id(0);

	int y = (index / w);
	int x = (index  - y * w);
	
	int channels = 4;
	
	int pixel = get_global_id(0) * channels;
	
	
	unsigned char ACur = read[pixel + 3];
	
	if( ACur > 0 ) {		
			
		float len = 0;	
		int count	= 0;
		
		int xl = x - 1;
		int yl = y;
		
		
		unsigned char colorL_a = getA( read, xl, yl, w, h, channels );		
		
		
		int xlu = x - 1;
		int ylu = y - 1;		
		
		unsigned char colorLU_a = getA( read, xlu, ylu, w, h, channels );	
		
		
		int xu = x;
		int yu = y - 1;
		
			
		
		unsigned char colorU_a = getA( read, xu, yu, w, h, channels );
		
		
		int xur = x + 1;
		int yur = y - 1;
		
		
		
		unsigned char colorUR_a = getA( read, xur, yur, w, h, channels );
				
		int xr = x + 1;
		int yr = y;
		
		
		unsigned char colorR_a = getA( read, xr, yr, w, h, channels );
				
		int xrd = x + 1;
		int yrd = y + 1;
				
		
		unsigned char colorRD_a = getA( read, xrd, yrd, w, h, channels );
		
		
		int xd = x;
		int yd = y + 1;
				
		
		unsigned char colorD_a = getA( read, xd, yd, w, h, channels );
		
		
		int xld = x - 1;
		int yld = y + 1;
				
		
		unsigned char colorLD_a = getA( read, xld, yld, w, h, channels );
		
		
		if( colorL_a == 0 ||  colorLU_a == 0 || colorU_a == 0 || colorUR_a == 0 || colorR_a == 0 || colorRD_a == 0 || colorD_a == 0 || colorLD_a == 0 ) 
		{					
			write[pixel + 3] = 0; 			
		}			
	}    		
}
