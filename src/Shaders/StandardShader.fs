#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

//Fog
uniform vec3 _FogColor;
uniform float near; 
uniform float far; 
uniform float _FogDensity; 
 
struct Material
{
    vec4 diffuse;
    vec3 ambient;
    vec3 specularColor;
    float shininess;

    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

struct Light
{
    //vec3 position;  
    vec3 direction;
    vec3 color;
};


uniform Material material;
uniform Light light;
uniform vec3 viewPos;


float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

void main()
{
    vec4 texColor = texture(material.texture_diffuse1, TexCoord) * material.diffuse;
        if(texColor.a < 0.1f)
            discard;

    //Fog
     float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    // FragColor = vec4(vec3(depth), 1.0); // Debug depth buffer
     vec4 fogColor = vec4(_FogColor.x, _FogColor.y, _FogColor.z, 1.0f);

    // ambient
    vec3 ambient = vec3(texColor) * material.ambient;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    //vec3 lightDir = normalize(light.position - FragPos);
    vec3 lightDir = normalize(-light.direction);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = (diff * (vec3(texColor))) * light.color;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    //vec3 specular = light.color * (spec * material.specularColor);  
    vec3 specular = light.color * spec * vec3(texture(material.texture_specular1, TexCoord)) * material.specularColor;

    //Result
    vec4 result = vec4((ambient + diffuse + specular), 1.0f);
    float fogStrength = min(depth * _FogDensity, 1.0f);
    FragColor = result + (fogColor - result) * fogStrength;
} 