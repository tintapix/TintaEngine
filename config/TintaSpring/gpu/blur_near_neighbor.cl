


int packColor( const __global unsigned char *A, int xPos, int yPos, int w, int h, int channels ) {
	
	int indexByte = (yPos * w + xPos) * channels;
	
	int rez =  ( A[indexByte] << 16 ) | ( A[indexByte + 1] << 8 ) | ( A[indexByte + 2] << 0 );
	
	return rez;
}


int getColor( const  __global unsigned char *A, int xPos, int yPos, int w, int h, int channels ){
	
	return (xPos < 0 || xPos >= w || yPos < 0 || yPos  >= h ) ? -1 : packColor( A, xPos, yPos, w, h, channels );	
}

__kernel void image( __global unsigned char *write, const __global unsigned char *read, int w, int h, int channels ) {  
	
	
		
	int index = get_global_id(0);

	int y = index / w;
	int x = index  - y * w;
	
	int pixel = get_global_id(0) * channels;
	
	int RCur = read[pixel];
	int GCur = read[pixel + 1];
	int BCur = read[pixel + 2];
	
	
	int xl = x - 1;
	int yl = y;
	
	
	int count = 0;
	
	int rSum = RCur;
	int gSum = GCur;	
	int bSum = BCur;
	count++;
	
	
	int colorL = getColor( read, xl, yl, w, h, channels );	
	
	if( colorL > -1 )
	{
		unsigned char r1 =  ( colorL >> 16 ) & 255;
		unsigned char g1 =  ( colorL >> 8  ) & 255; 
		unsigned char b1 =  ( colorL >> 0  ) & 255;
	
		rSum += r1;
		gSum += g1;
		bSum += b1;
		
		count++;
	}
	
	int xlu = x - 1;
	int ylu = y - 1;
	
	int colorLU = getColor( read, xlu, ylu, w, h, channels );
	
	if( colorLU > -1 )
	{
		unsigned char r1 =  ( colorLU >> 16 ) & 255;
		unsigned char g1 =  ( colorLU >> 8  ) & 255; 
		unsigned char b1 =  ( colorLU >> 0  ) & 255;
	
		rSum += r1;
		gSum += g1;
		bSum += b1;
		
		count++;
	}
	
	int xu = x;
	int yu = y - 1;
	
	int colorU = getColor( read, xu, yu, w, h, channels );
	
	
	if( colorU > -1 )
	{
		unsigned char r1 =  ( colorU >> 16 ) & 255;
		unsigned char g1 =  ( colorU >> 8  ) & 255; 
		unsigned char b1 =  ( colorU >> 0  ) & 255;
	
		rSum += r1;
		gSum += g1;
		bSum += b1;
		
		count++;
	}
	
	int xur = x + 1;
	int yur = y - 1;
	
	int colorUR = getColor( read, xur, yur, w, h, channels );
	
	if( colorUR > -1 )
	{
		unsigned char r1 =  ( colorUR >> 16 ) & 255;
		unsigned char g1 =  ( colorUR >> 8  ) & 255; 
		unsigned char b1 =  ( colorUR >> 0  ) & 255;
	
		rSum += r1;
		gSum += g1;
		bSum += b1;
		
		count++;
	}
	
	int xr = x + 1;
	int yr = y;
	
	int colorR = getColor( read, xr, yr, w, h, channels );
	
	
	if( colorR > -1 )
	{
		unsigned char r1 =  ( colorR >> 16 ) & 255;
		unsigned char g1 =  ( colorR >> 8  ) & 255; 
		unsigned char b1 =  ( colorR >> 0  ) & 255;
	
		rSum += r1;
		gSum += g1;
		bSum += b1;
		
		count++;
	}
	
	int xrd = x + 1;
	int yrd = y + 1;
	
	int colorRD = getColor( read, xrd, yrd, w, h, channels );
	
	if( colorRD > -1 )
	{
		unsigned char r1 =  ( colorRD >> 16 ) & 255;
		unsigned char g1 =  ( colorRD >> 8  ) & 255; 
		unsigned char b1 =  ( colorRD >> 0  ) & 255;
	
		rSum += r1;
		gSum += g1;
		bSum += b1;
		
		count++;
	}
	
	int xd = x;
	int yd = y + 1;
	
	int colorD = getColor( read, xd, yd, w, h, channels );
	
	if( colorD > -1 )
	{
		unsigned char r1 =  ( colorD >> 16 ) & 255;
		unsigned char g1 =  ( colorD >> 8  ) & 255; 
		unsigned char b1 =  ( colorD >> 0  ) & 255;
	
		rSum += r1;
		gSum += g1;
		bSum += b1;
		
		count++;
	}
	
	int xld = x - 1;
	int yld = y + 1;
	
	int colorlD = getColor( read, xld, yld, w, h, channels );
	
	if( colorlD > -1 )
	{
		unsigned char r1 =  ( colorlD >> 16 ) & 255;
		unsigned char g1 =  ( colorlD >> 8  ) & 255; 
		unsigned char b1 =  ( colorlD >> 0  ) & 255;
	
		rSum += r1;
		gSum += g1;
		bSum += b1;
		
		count++;
	}	
	
	write[pixel] = rSum/count;
	write[pixel + 1] = gSum/count;
	write[pixel + 2] = bSum/count;
	
   if( channels == 4 ){
		write[pixel+3] = read[pixel+3];
   }     
	
}
