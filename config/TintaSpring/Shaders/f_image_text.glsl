#version 330 

uniform sampler2D image;

in vec2 vTexCoord;
out vec4 fragColor;

void main ( void )  
{   		
   fragColor =  texture2D( image,  vTexCoord.st );	   
     
}    