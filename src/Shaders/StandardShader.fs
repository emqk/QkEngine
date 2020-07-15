#version 330 core

out vec4 FragColor;
uniform vec4 _FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);
in vec2 TexCoord;

uniform sampler2D texture_diffuse1;
vec4 texColor;

float near = 0.1; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);

    float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
   // FragColor = vec4(vec3(depth), 1.0); // Debug depth buffer
    vec4 fogColor = vec4(1, 1, 1, depth);

    texColor = texture(texture_diffuse1, TexCoord) * _FragColor;
       if(texColor.a < 0.1f)
           discard;
    FragColor = texColor + (fogColor - texColor) * depth;
} 