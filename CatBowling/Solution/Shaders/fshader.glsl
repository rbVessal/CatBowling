#version 150

flat in vec4  fColor;

uniform float randomNumberR;
uniform float randomNumberG;
uniform float randomNumberB;
uniform float randomNumberA;

void main()
{
	//vec4 fColor = vec4(randomNumberR, randomNumberG, randomNumberB, randomNumberA);

    gl_FragColor = fColor;
}
