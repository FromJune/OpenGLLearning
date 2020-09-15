#version 430

in vec3 outColor;
out vec4 daColor;

void main()
{
	daColor = vec4(outColor, 1.0);
}
