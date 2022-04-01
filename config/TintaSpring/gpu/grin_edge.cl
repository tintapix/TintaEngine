


unsigned char getA(  __global unsigned char *A, int xPos, int yPos, int w, int h, int channels, unsigned char def ){
	
	int indexByte = ( yPos * w + xPos ) * channels;
	
	return ( xPos < 0 || xPos >= w || yPos < 0 || yPos  >= h ) ? def : A[indexByte + 3];	
}

__kernel void fillholes( __global unsigned char *write, __global unsigned char *read, int w, int h, int channels, int minBase ) {  
	
	
		
	int index = get_global_id(0);

	int y = index / w;
	int x = index  - y * w;
	
	int pixel = get_global_id(0) * channels;
	
	
	if(  read[ pixel + 3 ] == 0 ) {	
		
		int sumA = 0;
		int countA = 0;
		
		int xl = x - 1;
		int yl = y;			
		
		unsigned char colorL_a = getA( read, xl, yl, w, h, channels, 255 );		
		
		if( colorL_a > 0 ){
			countA++;		
			sumA += colorL_a;
		}
		
		int xlu = x - 1;
		int ylu = y - 1;
		
		unsigned char colorLU_a = getA( read, xlu, ylu, w, h, channels, 255 );
		
		if( colorLU_a > 0 ){
			countA++;		
			sumA += colorLU_a;
		}
					
		int xu = x;
		int yu = y - 1;
		
		unsigned char colorU_a = getA( read, xu, yu, w, h, channels, 255 );
		
		if( colorU_a > 0 ){
			countA++;		
			sumA += colorU_a;
		}
				
		int xur = x + 1;
		int yur = y - 1;
		
		unsigned char colorUR_a = getA( read, xur, yur, w, h, channels, 255 );
		
		if( colorUR_a > 0 ){
			countA++;		
			sumA += colorUR_a;
		}
				
		int xr = x + 1;
		int yr = y;
		
		unsigned char colorR_a = getA( read, xr, yr, w, h, channels, 255 );
		
		if( colorR_a > 0 ){
			countA++;		
			sumA += colorR_a;				
		}
		
		int xrd = x + 1;
		int yrd = y + 1;
		
		unsigned char colorRD_a = getA( read, xrd, yrd, w, h, channels, 255 );
		
		if( colorRD_a > 0 ) {
			countA++;		
			sumA += colorRD_a;
		}
				
		
		int xd = x;
		int yd = y + 1;
		
		unsigned char colorD_a = getA( read, xd, yd, w, h, channels, 255 );
		
		if( colorD_a > 0 ){
			countA++;		
			sumA += colorD_a;
		}
				
		
		int xld = x - 1;
		int yld = y + 1;
		
		unsigned char colorlD_a = getA( read, xld, yld, w, h, channels, 255 );
		
		if( colorlD_a > 0 ){
			countA++;		
			sumA += colorlD_a;
		}
		
		if( countA >= minBase ){
			//write[ pixel ] = 100;		
			//write[ pixel + 1 ] = 150;		
			write[ pixel + 3 ] = (unsigned char)( sumA / countA );		
		}
				
	}
       
	
}


__kernel void grinbarb( __global unsigned char *write, __global unsigned char *read, int w, int h, int channels, int minBase ) {  
	
	
		
	int index = get_global_id(0);

	int y = index / w;
	int x = index  - y * w;
	
	int pixel = get_global_id(0) * channels;
	
	
	if(  read[ pixel + 3 ] > 0 ) {	
		
		int sumA = 0;
		int countA = 0;
		
		int xl = x - 1;
		int yl = y;			
		
		unsigned char colorL_a = getA( read, xl, yl, w, h, channels, 0 );		
		
		if( colorL_a == 0 ){
			countA++;		
			sumA += colorL_a;
		}
		
		int xlu = x - 1;
		int ylu = y - 1;
		
		unsigned char colorLU_a = getA( read, xlu, ylu, w, h, channels, 0 );
		
		if( colorLU_a == 0 ){
			countA++;		
			sumA += colorLU_a;
		}
					
		int xu = x;
		int yu = y - 1;
		
		unsigned char colorU_a = getA( read, xu, yu, w, h, channels, 0 );
		
		if( colorU_a == 0 ){
			countA++;		
			sumA += colorU_a;
		}
				
		int xur = x + 1;
		int yur = y - 1;
		
		unsigned char colorUR_a = getA( read, xur, yur, w, h, channels, 0 );
		
		if( colorUR_a == 0 ){
			countA++;		
			sumA += colorUR_a;
		}
				
		int xr = x + 1;
		int yr = y;
		
		unsigned char colorR_a = getA( read, xr, yr, w, h, channels, 0 );
		
		if( colorR_a == 0 ){
			countA++;		
			sumA += colorR_a;				
		}
		
		int xrd = x + 1;
		int yrd = y + 1;
		
		unsigned char colorRD_a = getA( read, xrd, yrd, w, h, channels, 0 );
		
		if( colorRD_a == 0 ) {
			countA++;		
			sumA += colorRD_a;
		}
				
		
		int xd = x;
		int yd = y + 1;
		
		unsigned char colorD_a = getA( read, xd, yd, w, h, channels, 0 );
		
		if( colorD_a == 0 ){
			countA++;		
			sumA += colorD_a;
		}
				
		
		int xld = x - 1;
		int yld = y + 1;
		
		unsigned char colorlD_a = getA( read, xld, yld, w, h, channels, 0 );
		
		if( colorlD_a == 0 ){
			countA++;		
			sumA += colorlD_a;
		}
		
		if( countA >=  minBase ){
			//write[ pixel ] = 100;		
			//write[ pixel + 1 ] = 150;		
			write[ pixel + 3 ] = 100; //(unsigned char)( sumA / countA );		
		}
				
	}
       
	
}
