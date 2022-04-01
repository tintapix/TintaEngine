#version 330 


uniform sampler2D textureSun;
uniform float rand;
uniform float timeElapsed;

in vec2 vTexCoord;
out vec4 fragColor;


const vec2 randVec = vec2( 770.9833, 30.233 );

const float randConst1 = 3333.11199;
const float randConst2 = 76.91199;

const float speed = 6000000.0;


float random ( vec2 st, in float rand ){ 
	return fract( sin( dot( st,randVec ) ) * rand  ); 
}

void main( void ) {	

	vec2 position =  vTexCoord.st;
	
	vec4 color =  texture2D( textureSun,  vTexCoord.st );	   
	color.r = 0.0;
	//color.a = color.r;
	color.g = random( position, mix( randConst1, randConst2, fract( timeElapsed /speed ) ) ); 
	color.b = 0.0;
	 
    fragColor = color;  	 
}