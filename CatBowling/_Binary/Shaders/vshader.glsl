#version 150 

//Input variables for vertex shader
in  vec4 vPosition;
in  vec4 vColor;
//Output variable for fragment shader
//Flat turns off color interpolation
//see: http://stackoverflow.com/questions/15450996/opengl-shader-dont-interpolate-color
flat out vec4 fColor;

uniform float newX;
uniform float newY;
uniform float newZ;

uniform mat4 model_view;
uniform mat4 projection;
uniform mat4 transformationMatrix;

void main() 
{
	vec4 offset = vec4(newX, newY, newZ, 0.0);
    gl_Position =  projection * model_view * transformationMatrix * (vPosition + offset);
	gl_Position /= vPosition.w;
	fColor = vColor;
} 