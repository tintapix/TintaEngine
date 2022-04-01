#version 330 

uniform sampler2D image;

in vec2 vTexCoord;
out vec4 fragColor;

const int dir = 8;
const float offsetX = 0.00126;
const float offsetY = 0.00176;

void fillChannels( out float values[dir], int channel ) {
	values[0] = texture2D( image,  vec2(vTexCoord.s - offsetX, vTexCoord.t) )[channel];	
	values[1] = texture2D( image,  vec2(vTexCoord.s + offsetX, vTexCoord.t) )[channel];	
	values[2] = texture2D( image,  vec2(vTexCoord.s - offsetX, vTexCoord.t+ offsetY) )[channel];	
	values[3] = texture2D( image,  vec2(vTexCoord.s + offsetX, vTexCoord.t+ offsetY) )[channel];	
	values[4] = texture2D( image,  vec2(vTexCoord.s, vTexCoord.t + offsetY) )[channel];	
	values[5] = texture2D( image,  vec2(vTexCoord.s, vTexCoord.t - offsetY) )[channel];	
	values[6] = texture2D( image,  vec2(vTexCoord.s - offsetX, vTexCoord.t) )[channel];	
	values[7] = texture2D( image,  vec2(vTexCoord.s + offsetX, vTexCoord.t - offsetY) )[channel];	
}


void main ( void )  
{   		
	
	float rFactors[dir];
	float gFactors[dir];
	float bFactors[dir];
	
 	float aFactors[dir];
	
	fillChannels(rFactors, 0 );
	fillChannels(gFactors, 1 );
	fillChannels(bFactors, 2 );
	
	fillChannels(aFactors, 3 );
	
	/*float aFactors[] = float[]( 
						texture2D( image,  vec2(vTexCoord.s - offset, vTexCoord.t) )[3], 
					    texture2D( image,  vec2(vTexCoord.s + offset, vTexCoord.t) )[3],
						texture2D( image,  vec2(vTexCoord.s - offset, vTexCoord.t+ offset) ).w,
						texture2D( image,  vec2(vTexCoord.s + offset, vTexCoord.t+ offset) ).w,
						texture2D( image,  vec2(vTexCoord.s, vTexCoord.t + offset) )[3],
						texture2D( image,  vec2(vTexCoord.s, vTexCoord.t - offset) )[3],
						texture2D( image,  vec2(vTexCoord.s - offset, vTexCoord.t - offset) )[3],
						texture2D( image,  vec2(vTexCoord.s + offset, vTexCoord.t - offset) )[3] 
						);
   
   */
   fragColor =  texture2D( image,  vTexCoord.st );	   
  
/*  
if ( fragColor.a  == 1.0 )
		fragColor.g = 1.0;
else
		fragColor.g = 0.5;
	*/
float stp = 0.76814;
	
fragColor.r = step(  stp, ( fragColor.r + rFactors[0] + rFactors[1] + rFactors[2] + rFactors[3] 
								+ rFactors[4] + rFactors[5] + rFactors[6] + rFactors[7] 
								) / ( rFactors.length() + 1 ) );
								
fragColor.g = step(  stp, ( fragColor.g + gFactors[0] + gFactors[1] + gFactors[2] + gFactors[3] 
								+ gFactors[4] + gFactors[5] + gFactors[6] + gFactors[7] 
								) / ( gFactors.length() + 1 ) );
								
fragColor.b = step(  stp, ( fragColor.b + bFactors[0] + bFactors[1] + bFactors[2] + bFactors[3] 
								+ bFactors[4] + bFactors[5] + bFactors[6] + bFactors[7] 
								) / ( bFactors.length() + 1 ) );
								
								
fragColor.a = step(  stp, ( fragColor.a + aFactors[0] + aFactors[1] + aFactors[2] + aFactors[3] 
								+ aFactors[4] + aFactors[5] + aFactors[6] + aFactors[7] 
								) / ( aFactors.length() + 1 ) );

								
/*fragColor.a =  ( aFactors[0] + aFactors[1] + aFactors[2] + aFactors[3] 
								+ aFactors[4] + aFactors[5] + aFactors[6] + aFactors[7] 
								) / ( aFactors.length()  ) ;*/
								
								
								
//fragColor.a = ( fragColor.a + aFactors[0] + aFactors[1] + aFactors[2] + aFactors[3] 
								// *+ aFactors[4] + aFactors[5] + aFactors[6] + aFactors[7]*/ ) / ( aFactors.length() + 1 );
  
   
      
   
}    