#version 330 core

out vec4 FragColor;
uniform vec4 _FragColor = vec4(0.0f, 0.0f, 0.0f, 0.0f);

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
vec4 texColor;

//Fog
uniform vec3 _FogColor;
uniform float near; 
uniform float far; 
uniform float _FogDensity; 
 
//Lighting
uniform vec3 lightColor;
uniform vec3 lightPos;  
uniform vec3 ambientLightColor;

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
    vec4 fogColor = vec4(_FogColor.x, _FogColor.y, _FogColor.z, 1.0f);

    texColor = texture(texture_diffuse1, TexCoord) * _FragColor;
       if(texColor.a < 0.1f)
           discard;

    //object color after lighting
    vec4 afterLighting = vec4(lightColor, 1.0f) * texColor;

    // ambient light color
    vec3 ambient = ambientLightColor * lightColor;
  	
    // Final ambient light color(light color + object color) 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
            
    vec4 ambientResult = vec4((ambient + diffuse), 1.0f) * afterLighting;
    FragColor = ambientResult + (fogColor - ambientResult) * (depth * _FogDensity);
} 