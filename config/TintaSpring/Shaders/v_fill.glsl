#version 330


layout (location = 0) in vec3 in_position; 

uniform mat4 cam;


void main(void)
{   
 
//mat4 P = WorldMatrix * ViewMatrix * ProjectMatrix;
	//pos = ProjectMatrix * P;	
	vec4 pos = cam * vec4(in_position,1.0) ; //vec3      ( gl_ModelViewMatrix * gl_Vertex );             // transformed point to world space	
    //n   = normalize ( gl_NormalMatrix * gl_Normal );                // transformed n
   
	//vec4 cameraPosition = vec4( 0.0, 0.0, -50.0, 1.0);
	gl_Position = pos;	
	
	
}