#version 330 


uniform sampler2D textureSun;
uniform float rand;

in vec2 vTexCoord;
out vec4 fragColor;

const vec2 	randVec = vec2( 770.9833, 30.233 );
const float randConst = 44400.11199;

float random ( vec2 st ) 
{ 
	return fract( sin( dot( st,randVec ) ) * randConst ); 
}


void main( void ) {	

	vec2 position =  vTexCoord.st;
	
	position.x *= rand;	

	vec4 color =  texture2D( textureSun,  vTexCoord.st );	   
	color.r = random( position.xy );
	color.g = random( position.xy );
	color.b = random( position.xy );
	 
    fragColor = color;  
	 
}