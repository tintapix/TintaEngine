#version 330 

uniform sampler2D image;
uniform float factor;
uniform vec3 color;

in vec2 vTexCoord;
out vec4 fragColor;

void main ( void )  
{   		
   fragColor =  texture2D( image,  vTexCoord.st );	   
   
   fragColor.a = step( factor, 1.0 - distance( fragColor.rgb, color / 1.732050 ));
   //fragColor.a = 1.0 - distance( fragColor.rgb, color / 1.732050 );
   
   //fragColor.a = 0.1;
}    