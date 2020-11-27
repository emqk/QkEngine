#version 330 core

out vec4 FragColor;
uniform vec4 _FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

uniform sampler2D texture_diffuse1;

void main()
{
	FragColor = _FragColor;
} 