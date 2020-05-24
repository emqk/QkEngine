#version 330 core

out vec4 FragColor;
uniform vec4 _FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
in vec2 TexCoord;

uniform sampler2D texture1;
vec4 texColor;

void main()
{
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);

    texColor = texture(texture1, TexCoord) + _FragColor;
       if(texColor.a < 0.6f)
           discard;
       FragColor = texColor;
} 