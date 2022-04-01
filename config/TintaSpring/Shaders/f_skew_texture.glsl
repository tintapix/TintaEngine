#version 330

uniform sampler2D name;
uniform float factor;


in vec2 vTexCoord;
out vec4 fragColor;

void main ( void )  
{   
   float tiles = 1.0;
	   
   float offset = 1.0 / tiles;
   float tileX = floor( vTexCoord.x * tiles ); 
	  
   vec4 color =  texture2D( name,  vec2(  mix(  vTexCoord.x / offset - tileX + factor,  vTexCoord.x , 1.0 - vTexCoord.y ) , vTexCoord.y ) );	   
  
   
   
   fragColor = color;
}    