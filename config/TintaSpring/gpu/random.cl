

int randomVal( float2 pos , float bottom, float top, __local int * seed ) {
	
	const int A = 48271;
	const int M = 2147483647;
	const int Q = M / A;
	const int R = M % A;
	const float accuracy = 1000000.0;
	//const float seedTrunc = 333333.0;


	int seedVal = (int)( pos.x * pos.y * (float)(seed[0])) ;
	
	int tmpState = A * ( seedVal % Q ) - R * ( seedVal / Q ); 
	
	if ( tmpState < 0 ){ 		
		tmpState = tmpState + M; 	
	}
	
	
	int up = (int)( top * accuracy );
	
	int down = (int)( bottom * accuracy );
	
	tmpState = ( tmpState % ( up  - down ) ) + down;	
	
	atomic_xchg( seed, tmpState );
	
	//seed[0] = tmpState;
	
	
	return (float) tmpState / accuracy;	
	
}


__kernel void image( __global unsigned char *A, int w, int h, unsigned int r, unsigned  int g, unsigned  int b, int channels, int seed ) {  
	
	int pixel = get_global_id(0) * channels;
	
	//__global int * seedGlob = {0};
	
	__local float seedGlob;
	
	atomic_xchg( &seedGlob, seed );
	 
	unsigned char rCh = (unsigned char) r;
	unsigned char gCh = (unsigned char) g;
	unsigned char bCh = (unsigned char) b;		
		 		
	int y = get_global_id(0) / w;
	int x = get_global_id(0)  - y * w;
	
	float2 pos = {(float)(x)/(float)(w),  (float)(y)/(float)(h) };	
	
	A[pixel] = floor(randomVal( pos, 0.0, 255.0, &seedGlob ) * 255.0);	
	A[pixel+1] = 255; //floor(randomVal(pos, 0.0, 255.0, seedGlob )* 255.0);	
	A[pixel+2] = 255;//floor(randomVal(pos, 0.0, 255.0, seedGlob )* 255.0);
	
}
	
	
