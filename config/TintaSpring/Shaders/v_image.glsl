#version 330
layout (location = 0) in vec3 in_position; 
layout (location = 1) in vec2 in_tex_coordinate; 

uniform mat4 cam;

out vec2 vTexCoord;


void main(void)
{   
	
    //mat4 P = WorldMatrix * ViewMatrix * ProjectMatrix;
	//pos = ProjectMatrix * P;	
	vec4 pos = cam * vec4(in_position,1.0) ; //vec3      ( gl_ModelViewMatrix * gl_Vertex );             // transformed point to world space	
    //n   = normalize ( gl_NormalMatrix * gl_Normal );                // transformed n
   
	//vec4 cameraPosition = vec4( 0.0, 0.0, -50.0, 1.0);
	gl_Position = pos;
	vTexCoord = in_tex_coordinate;
	 //gl_Position = vec4(-0.95,0.95,0.5,1);
	
}