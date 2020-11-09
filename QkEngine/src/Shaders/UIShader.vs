#version 330 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 aTexCoord;
out vec2 TexCoords;

uniform vec3 topLeft;
uniform vec3 size;

void main()
{
    vec2 vert = vec2(vertex.x * size.x - (1.0f - size.x) + topLeft.x * 2.0f
                    ,2.0f+ vertex.y * size.y - (1.0f - size.y) - size.y * 2.0f - topLeft.y * 2.0f);
    gl_Position = vec4(vert.x, vert.y, 0.0f, 1.0);
    TexCoords = aTexCoord;
}