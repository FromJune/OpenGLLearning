#version 430

in layout(location = 0) vec3 position;
in layout(location = 1) vec3 vertexColor;

uniform vec3 triOffset;

out vec3 outColor;

void main()
{
	gl_Position = vec4(position + triOffset, 1.0);
	outColor = vertexColor;
}