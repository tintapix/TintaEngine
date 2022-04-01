#version 330 

uniform sampler2D image;

in vec2 vTexCoord;
out vec4 fragColor;

const int A = 48271;
const int M = 2147483647;
const int Q = M / A;
const int R = M % A;
const int accuracy = 10000;
const float seedTrunc = 100000000.0;
float randomVal( vec2 texCoord ) {
	
	int seedVal = int(texCoord.s * texCoord.t * seedTrunc);
	int tmpState = A * ( seedVal % Q ) - R * ( seedVal / Q ); 
	
	if ( tmpState < 0 ){ 		
		tmpState = tmpState + M; 	
	}
	
	tmpState = tmpState % accuracy;
	
	
	return float( tmpState )/float( accuracy );
}

void main ( void )  
{   		
   fragColor =  texture2D( image,  vTexCoord.st );	   
   
   float offsetX = 0.01;
   float offsetY = 0.01;
   
   /*
   float alphaLeft =  texture2D( image,  vec2(vTexCoord.s - offset, vTexCoord.t) ).w;	   
   float alphaRight =  texture2D( image,  vec2(vTexCoord.s + offset, vTexCoord.t) ).w;

   float alphaLeftUp =  texture2D( image,  vec2(vTexCoord.s - offset, vTexCoord.t+ offset) ).w;	   
   float alphaRightUp =  texture2D( image,  vec2(vTexCoord.s + offset, vTexCoord.t+ offset) ).w;	   
   
   float alphaUp =  texture2D( image,  vec2(vTexCoord.s, vTexCoord.t + offset) ).w;	   
   float alphaDown =  texture2D( image,  vec2(vTexCoord.s, vTexCoord.t - offset) ).w;	

   float alphaLeftDown =  texture2D( image,  vec2(vTexCoord.s - offset, vTexCoord.t - offset) ).w;	   
   float alphaRightDown =  texture2D( image,  vec2(vTexCoord.s+ offset, vTexCoord.t - offset) ).w;	    
   */
   
   float rnd = randomVal( vTexCoord )/90.0;
   //offsetX += rnd ;
   //offsetY += rnd ;
   float aFactors[] = float[]( 
						texture2D( image,  vec2(vTexCoord.s - offsetX, vTexCoord.t) ).w, 
					    texture2D( image,  vec2(vTexCoord.s + offsetX, vTexCoord.t) ).w,
						texture2D( image,  vec2(vTexCoord.s - offsetX, vTexCoord.t+ offsetY) ).w,
						texture2D( image,  vec2(vTexCoord.s + offsetX, vTexCoord.t+ offsetY) ).w,
						texture2D( image,  vec2(vTexCoord.s, vTexCoord.t + offsetY) ).w,
						texture2D( image,  vec2(vTexCoord.s, vTexCoord.t - offsetY) ).w,
						texture2D( image,  vec2(vTexCoord.s - offsetX, vTexCoord.t - offsetY) ).w,
						texture2D( image,  vec2(vTexCoord.s+ offsetX, vTexCoord.t - offsetY) ).w );
   float fac = ( fragColor.a + aFactors[0] + aFactors[1] + aFactors[2] + aFactors[3] + aFactors[4] + aFactors[5] + aFactors[6] + aFactors[7] ) / (    aFactors.length() + 1);
   //fragColor = mix( vec4(1.0, 1.0, 1.0, fragColor.a ), fragColor , fac)); // smooth
    
	fragColor =  mix( vec4(1.0, 1.0, 1.0, 0.0 ), fragColor , fac * (1 - rnd)); // edge

		
   
   //fragColor.a = ( fragColor.a + aFactors[0] + aFactors[1] + aFactors[2] + aFactors[3] + aFactors[4] + aFactors[5] + aFactors[6] + aFactors[7] ) / (aFactors.length() + 1);
   
}    