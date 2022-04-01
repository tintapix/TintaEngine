#version 330 

uniform vec4 color;
//uniform vec4 lightColor;
//uniform vec3 lightPos;

//varying vec4 pos;
//varying vec3 n;
out vec4 fragColor;
void main (void)  
{   
	
	//gl_FragData [] = vec4 ( 0.5*n2 + vec3(0.5), 1.0 );	
   fragColor = color;  
}    