#version 330

uniform sampler2D name;
uniform float factor;
uniform float tiles;
//uniform float alpha;

in vec2 vTexCoord;
out vec4 fragColor;

void main ( void )  
{   
	   
   float offset = 1.0 / tiles;
   float tileX = floor( vTexCoord.x * tiles ); 
	  
   vec4 color =  texture2D( name,  vec2( ( vTexCoord.x / offset - tileX ) + factor , vTexCoord.y ) );	   
  
   
   
   fragColor = color;
}    