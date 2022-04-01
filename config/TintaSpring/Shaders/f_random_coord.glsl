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

float randomVal( vec2 texCoord ) {
	
	int seedVal = int(texCoord.s * texCoord.t * seedTrunc);
	int tmpState = A * ( seedVal % Q ) - R * ( seedVal / Q ); 
	
	if ( tmpState < 0 ){ 		
		tmpState = tmpState + M; 	
	}
	
	tmpState = tmpState % accuracy;
	
	
	return float( tmpState )/float( accuracy );
}

void main( void ) {	
		
		
	vec4 color =  texture2D( textureSun,  vec2( vTexCoord.s + mix(0.0,  randomVal( vTexCoord ), 0.01 ), vTexCoord.t ) );	   	
		
    fragColor = color;  
	 
}