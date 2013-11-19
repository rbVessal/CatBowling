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

void main() 
{
	vec4 offset = vec4(newX, newY, newZ, 0.0);
<<<<<<< HEAD
    gl_Position = projection * model_view * transformationMatrix * ((vPosition + offset));
	gl_Position /= vPosition.w;
=======
    gl_Position =  projection * model_view * (vPosition + offset)/vPosition.w;
>>>>>>> 479d05018c1419c1500cdaaef51e5ca4402f25fa
	fColor = vColor;
} 