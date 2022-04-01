#version 330 

uniform sampler2D image;
uniform float ratio;
uniform float factor;

in vec2 vTexCoord;
out vec4 fragColor;

void main ( void )  
{     
   float revRatio = ( 1.0 - ratio );
   float offset = max( ratio > 1.0 ? 0.0 : ( revRatio - mix( 0.0, revRatio, factor ) ), 0.0  );
   vec2 tex = vec2( vTexCoord.s, min( vTexCoord.t * min( ratio,  1.0 ) + offset , 1.0 ) );
   fragColor =  texture2D( image,  tex );	         
}    