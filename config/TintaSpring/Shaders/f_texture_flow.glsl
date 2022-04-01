
#version 330

uniform sampler2D textureSun;
uniform float timeElapsed;

in vec2 vTexCoord;
out vec4 fragColor;

const vec2 randVec = vec2( 0.0, 17.9833 );


float random ( vec2 st, in float rand ){ 
	return fract( sin( dot( st,randVec ) ) * rand  ); 
}

void main( void ) {	

	vec2 position =  vTexCoord.st;
	
	vec4 color1 =  texture2D( textureSun,  position );	   
	
	// v2
	// v2 vec4 color2 =  texture2D( textureSun,  vec2( 0.12 * fract( timeElapsed /15000.0 ), position.y ) );	
	
	// v1
	//vec4 color2 =  texture2D( textureSun,  vec2( position.x * random ( position, fract( timeElapsed / 30000.0 ) ), position.y * random ( position, fract( timeElapsed / 30000.0 ) ) ) );
	
	//v3
	vec4 color2 =  texture2D( textureSun,  vec2( position.x * random ( position, fract( timeElapsed / 10000.0 ) ), position.y  ) );
	
	vec4 color;
	
	color.r = mix(color2.r, color1.r, 0.0);
	color.g = mix(color2.g, color1.g, 0.0);
	color.b = mix(color2.b, color1.b, 0.0);
	color.a = 1.0;
		
	 
    fragColor = color;  	 
}