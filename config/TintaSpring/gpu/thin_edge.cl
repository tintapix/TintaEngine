


int packColor( __global unsigned char *A, int xPos, int yPos, int w, int h, int channels ) {
	
	int indexByte = (yPos * w + xPos) * channels;
	
	int rez =   ( A[indexByte] << 24 ) | ( A[indexByte+1] << 16 ) | ( A[indexByte + 2] << 8 ) | ( A[indexByte + 3] << 0 );
	
	return rez;
}


bool getColor(  __global unsigned char *A, int xPos, int yPos, int w, int h, int channels ){
	
	return !(xPos < 0 || xPos >= w || yPos < 0 || yPos  >= h ); //? -1 : packColor( A, xPos, yPos, w, h, channels );	
}

__kernel void image( __global unsigned char *read, __global unsigned char *write,int w, int h ) {  
	
	
		
	int index = get_global_id(0);

	int y = index / w;
	int x = index  - y * w;
	int channels = 4;
	int pixel = get_global_id(0) * channels;
	
	
	unsigned char ACur = read[pixel + 3];
	
		
	if( ACur > 0 ) {
	
		
		
		
		float len = 0;	
		int count	= 0;
		
		int xl = x - 1;
		int yl = y;
		
		
		unsigned char colorLA_a = 255;		
		
		if(getColor( read, xl, yl, w, h, channels ) )
		{
		
			int colorL = packColor( read, xl, yl, w, h, channels );					
			colorLA_a =  colorL & 255;								
		}
		
		int xlu = x - 1;
		int ylu = y - 1;		
		
		unsigned char colorLU_a = 255;	
		
		if( getColor( read, xlu, ylu, w, h, channels ) )
		{			
			int colorLU = packColor( read, xlu, ylu, w, h, channels );
			colorLU_a =   colorLU  & 255;			
		}
		
		int xu = x;
		int yu = y - 1;
		
			
		
		unsigned char colorU_a = 255;
		if( getColor( read, xu, yu, w, h, channels ) )
		{
			int colorU = packColor( read, xu, yu, w, h, channels );
			colorU_a =   colorU  & 255;
		}
		
		int xur = x + 1;
		int yur = y - 1;
		
		
		
		unsigned char colorUR_a = 255;
		if( getColor( read, xur, yur, w, h, channels ) )
		{
			int colorUR = packColor( read, xur, yur, w, h, channels );
			colorUR_a =   colorUR  & 255;
		}
		
		int xr = x + 1;
		int yr = y;
		
		
		unsigned char colorR_a = 255;
		if( getColor( read, xr, yr, w, h, channels ) )
		{
			int colorR = packColor( read, xr, yr, w, h, channels );
			colorR_a =   colorR & 255;
		}
		
		int xrd = x + 1;
		int yrd = y + 1;
		
		
		
		unsigned char colorRD_a = 255;
		if( getColor( read, xrd, yrd, w, h, channels ) )
		{
			int colorRD = packColor( read, xrd, yrd, w, h, channels );
			colorRD_a =  colorRD  & 255;
		}
		
		int xd = x;
		int yd = y + 1;
		
		
		
		unsigned char colorD_a = 255;
		if( getColor( read, xd, yd, w, h, channels ) )
		{
			int colorD = packColor( read, xd, yd, w, h, channels );
			colorD_a =  colorD & 255;
		}
		
		int xld = x - 1;
		int yld = y + 1;
		
		
		
		unsigned char colorLD_a = 255;
		if( getColor( read, xld, yld, w, h, channels ) )
		{
			int colorlD = packColor( read, xld, yld, w, h, channels );
			colorLD_a = colorlD & 255;
		}
		
		if( colorLA_a == 0 ||  colorLU_a == 0 || colorU_a == 0 || colorUR_a == 0 || colorR_a == 0 || colorRD_a == 0 || colorD_a == 0 || colorLD_a == 0 ) 
		{				
			write[pixel] = 0; 
			write[pixel+3] = 0; 
						
		}	
		
		
	}    
	
}
