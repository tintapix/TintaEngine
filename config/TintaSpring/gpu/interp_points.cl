
/*
	channel(rgb) - 0,1,2
*/



int findNearest( __global int *array, int arraySize, float x, float y){

	int halfSQ = arraySize/3;
	
	int iter = 0;
	float minLen = 99999999999.0;
	
	int index = -1;
	
	for(int i = 0; i < halfSQ; i++){
	
		float xArr = (float)(array[iter]);
		float yArr = (float)(array[iter+1]);
		
		float len = distance( (float2)(xArr, yArr), (float2)(x , y) );
		
		index = minLen > len ? iter : index;
		
		minLen = min(minLen,len);
		
		iter += 3;
	}
	
	
	return index;
}


int findFar( __global int *array, int arraySize, float x, float y){

	int halfSQ = arraySize/3;
	
	int iter = 0;
	float minLen = 0.0;
	
	int index = -1;
	
	for(int i = 0; i < halfSQ; i++){
	
		float xArr = (float)(array[iter]);
		float yArr = (float)(array[iter+1]);
		
		float len = distance( (float2)(xArr, yArr), (float2)(x , y) );
		
		index = minLen < len ? iter : index;
		
		minLen = max(minLen,len);
		
		iter += 3;
	}
	
	
	return index;
}


int  mixColors( int color1, float minLen, float x, float y, __global int *array, int arraySize  ) {

	
	int rez = color1;
	
	float r1 =  (float)( ( color1 >> 16 ) & 255 );
	float g1 =  (float)( ( color1 >> 8  ) & 255 ); 
	float b1 =  (float)( ( color1 >> 0  ) & 255 );
	
	
	int halfSQ = arraySize/3;	
	int pos = 0;
	
	for( int i = 0; i < halfSQ; i++ ){
		
		float2 p1 = (float2) ( (float)( array[pos] ), (float)( array[pos + 1] ) );
		float2 p2 = (float2)( x, y) ;
		
		float len = distance( p1, p2 );
				
		int color2 = array[pos + 2];
		
		int r2 =  ( ( color2 >> 16 ) & 255 );
		int g2 =  ( ( color2 >> 8  ) & 255 ); 
		int b2 =  ( ( color2 >> 0  ) & 255 );
		
		//float factor = minLen/ max( len, 1.0f );
		float factor = minLen/ max( len  , 1.0f ); //near
		//float factor = max( len , 1.0f )/minLen * 0.2; //far
		
		r1 = mix( r1,(float)(r2),  factor ) ;
		g1 = mix( g1,(float)(g2),  factor ) ;
		b1 = mix( b1,(float)(b2),  factor ) ;
		
		
		pos += 3;
	}
	
	/*Color rez;// = (Color)(r1,g1,b1);
	rez.r = r1;
	rez.g = g1;
	rez.b = b1;*/
	int rRez = (int)(r1);
	int gRez = (int)(g1);
	int bRez = (int)(b1);
	
	rez =  ( rRez << 16 ) | ( gRez << 8 ) | ( bRez << 0 );
	
	return rez;
}

__kernel void image( __global unsigned char *A, int w, int h, __global int *array,  int arraySize, int channels  ) 
{  
	
	
	int pixel = get_global_id(0) * channels;
		
	int y = get_global_id(0) / w;
	int x = get_global_id(0)  - y * w;
	
	int indexMin = findNearest(array ,arraySize, (float)(x), (float)(y) );
	
	//int indexMin = findFar(array ,arraySize, (fl	oat)(x), (float)(y) );
	
	
	float minLen = distance( (float2)( (float)(array[indexMin]), (float)(array[indexMin + 1] )), (float2)(x , y) );
	
	int color = array[indexMin + 2];
	
	color = mixColors( color, minLen, (float)(x),(float)(y), array, arraySize );
	
	
	int r =  ( ( color >> 16 ) & 255 );
	int g =  ( ( color >> 8 ) & 255 ); 
	int b =  ( ( color >> 0 ) & 255 );
	
	A[pixel]   = r;
	A[pixel+1] = g;
	A[pixel+2] = b; 
	
	if( channels > 3 )
		A[pixel+3] = 255; //  (int)( (1.0 - min(len/maxLen,(float)(1.0))) * 255.0 );	
}