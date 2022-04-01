#version 330 

uniform sampler2D image;
uniform float factor;

in vec2 vTexCoord;
out vec4 fragColor;

void main ( void )  
{   		
   float tiles = 2.0;
   float offset = 1.0 / tiles;
   float tileX = floor( vTexCoord.x * tiles ); 
   // vec4 color =  texture2D( name,  vec2( ( vTexCoord.x / offset - tileX ) + factor , vTexCoord.y ) );	   
   
   
	vec2 vTexCoordPolar = vTexCoord;
    //vTexCoordPolar.s = sqrt(pow(vTexCoord.s  - 0.707,2) + pow(vTexCoord.t - 0.707,2));
	
	vTexCoordPolar.s = sqrt(pow(vTexCoord.s  - 0.66,2) + pow(vTexCoord.t - 0.66,2));
	
	
	vTexCoordPolar.t = atan((vTexCoord.s - 0.67)/(vTexCoord.t- 0.67));
	fragColor =  texture2D( image,  vec2( ( vTexCoordPolar.x / offset - tileX ) - factor , vTexCoordPolar.y ) );	 
    //fragColor =  texture2D( image,  vTexCoordPolar.st );	   
   
}    