#version 150 

//Input variables for vertex shader
in  vec4 vPosition;
in  vec4 vColor;
//Output variable for fragment shader
//Flat turns off color interpolation
//see: http://stackoverflow.com/questions/15450996/opengl-shader-dont-interpolate-color
flat out vec4 fColor;

uniform mat4 viewMatrix;//View transformation
uniform mat4 projection;//Perspective projection - it's actually a frustum now.
uniform mat4 transformationMatrix;//Model transformation

void main() 
{
    gl_Position =  projection * viewMatrix * transformationMatrix * vPosition;
	gl_Position /= vPosition.w;
	fColor = vColor;
} 