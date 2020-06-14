#version 330 core

out vec4 FragColor;
uniform vec4 _FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

void main()
{
	FragColor = vec4(1.0f, 0.5f, 0.5f, 1.0f);
} 