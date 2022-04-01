
#version 330 

uniform sampler2D textureSun;
uniform float tiles;

in vec2 vTexCoord;
//layout (location = 0) in float tiles;

//uniform float period;

/*
float hash( float n ){
    return fract( sin( n )*758.5453 );
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 4.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}



float random( in float seed ){
	seedLast =  fract(seedLast * seed);
	//seed = (a * seed + c) % m;
	return seedLast;
}

float noise2d( in vec2 x , float c){
    vec2 p = floor(x);
    vec2 f = smoothstep(0.0, 1.0, fract(x));
    float n = p.x + p.y*c;
    return mix(mix(hash(n+0.0),hash(n+1.0),f.x),mix(hash(n+c),hash(n+c+1.0),f.x),f.y);
}
*/
void main ( void )  
{     
   float offset = 1.0 / tiles;
   
   float tileX = floor( vTexCoord.x * tiles );
   float tileY = floor( vTexCoord.y * tiles );
   
   vec4 color =  texture2D( textureSun,  vec2( vTexCoord.x / offset - tileX, vTexCoord.y / offset - tileY) );	   
   
   gl_FragColor = color;
}    