
/*
	channel(rgb) - 0,1,2
*/


/*
	function findNearest( box, xPos , yPos )	

	local q = c_boxelements(box)
	
	local halfQ = q/2
	
	local iter = 0
	local minLen = 99999999999
	local pos = 0
	--util.msg(halfQ)
	for k = 0, halfQ - 1 do
		
		
		x = c_getvalboxi32( box, iter )
		y = c_getvalboxi32( box, iter + 1 )
		local len = s_get_length( x, y, xPos , yPos )		
		if  len < minLen then
		
			minLen = len
			pos = x
		end
		
		
		
		iter = iter + 2
	end	
	return minLen
end


		len = findNearest( box, h_ , w_ )	
		
		c_setpixelf( w_,h_, 0, 1.0 - math.min(len/maxLen,1.0), 0 )
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

__kernel void image( __global unsigned char *A, int w, int h, __global int *array,  int arraySize,  int channels, float maxLen  ) 
{  
	
	
	int pixel = get_global_id(0) * channels;
		
	int y = get_global_id(0) / w;
	int x = get_global_id(0)  - y * w;
	
	int indexMin = findNearest(array ,arraySize, (float)(x), (float)(y) );
	
	float minLen = distance( (float2)( (float)(array[indexMin]), (float)(array[indexMin + 1] )), (float2)(x , y) );
	
	int color = array[indexMin + 2];
	
	
	int r =  ( ( color >> 16 ) & 255 );
	int g =  ( ( color >> 8 ) & 255 ); 
	int b =  ( ( color >> 0 ) & 255 );
	
	A[pixel]   = (unsigned char)(mix( (float)(r), 0.0f, min(minLen/maxLen,1.0f))); //(int)( 1.0 - min(minLen/maxLen,(float)(1.0)) * r );
	A[pixel+1] = (unsigned char)(mix( (float)(g), 0.0f, min(minLen/maxLen,1.0f)));//(int)( 1.0 - min(minLen/maxLen,(float)(1.0)) * g );
	A[pixel+2] = (unsigned char)(mix( (float)(b), 0.0f, min(minLen/maxLen,1.0f)));//(int)( 1.0 - min(minLen/maxLen,(float)(1.0)) * b ); 
	//if(channels > 3)
	//	A[pixel+3] =  (int)( (1.0 - min(minLen/maxLen,(float)(1.0))) * 255.0 );
	
	
}