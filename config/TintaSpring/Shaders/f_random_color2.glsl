#version 440 


uniform sampler2D textureSun;
//uniform int seed;

in vec2 vTexCoord;
out vec4 fragColor;

/*
const vec2 	randVec = vec2( 770.9833, 30.233 );
const float randConst = 44400.11199;

float random ( vec2 st ) 
{ 
	return fract( sin( dot( st,randVec ) ) * randConst ); 
}
*/

const int A = 48271;
const int M = 2147483647;
const int Q = M / A;
const int R = M % A;
const int accuracy = 10000;
const float seedTrunc = 100000000.0;

float randomVal( vec2 texCoord, float bottom, float top ) {
	
	int seedVal = int(texCoord.s * texCoord.t * seedTrunc);
	int tmpState = A * ( seedVal % Q ) - R * ( seedVal / Q ); 
	
	if ( tmpState < 0 ){ 		
		tmpState = tmpState + M; 	
	}
	
	int up = int( top * float(accuracy) );
	
	int down = int( bottom * float(accuracy) );
	
	tmpState = ( tmpState % ( up  - down ) ) + down;
	
	
	return float( tmpState )/float( accuracy );
}

void main( void ) {	
	
	
	vec4 color =  texture2D( textureSun,  vTexCoord.st );	   
	//color.r = random( position.xy );
	
	
	color.r = mix( color.r, color.r * randomVal( vTexCoord.st, 0.6, 1.4 ), color.g);
	color.g = mix( color.g, color.g * randomVal( vTexCoord.st, 0.6, 1.4 ), color.g);
	color.b = mix( color.b, color.b * randomVal( vTexCoord.st, 0.6, 1.4 ), color.g);
	
    fragColor = color;  
	 
}