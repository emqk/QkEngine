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
    vec3 specularColor;
    float shininess;

    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

struct DirLight {
    vec3 direction;
    vec3 color;
};  

struct PointLight
{
    vec3 position;  
    vec3 color;
    float intensity;
	
    float constant;
    float linear;
    float quadratic;
};


#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirLight;

uniform Material material;

uniform vec3 viewPos;
uniform vec3 ambientColor;


float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near * far) / (far + near - z * (far - near));	
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);  
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  


vec4 texColor;

void main()
{
    texColor = texture(material.texture_diffuse1, TexCoord) * material.diffuse;
        if(texColor.a < 0.1f)
            discard;

    //Fog
     float depth = LinearizeDepth(gl_FragCoord.z) / far; // divide by far for demonstration
    // FragColor = vec4(vec3(depth), 1.0); // Debug depth buffer
     vec4 fogColor = vec4(_FogColor.x, _FogColor.y, _FogColor.z, 1.0f);

    // Properties
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    // Directional lighting
    vec3 lightResult = CalcDirLight(dirLight, norm, viewDir);
    // Point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        lightResult += CalcPointLight(pointLights[i], norm, FragPos, viewDir);     
    
    vec4 result = vec4(lightResult, 1.0);
    float fogStrength = min(depth * _FogDensity, 1.0f);
    FragColor = result + (fogColor - result) * fogStrength;
} 

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // combine results
    vec3 ambient  = ambientColor * vec3(texColor);
    vec3 diffuse  = light.color  * diff * vec3(texColor);
    vec3 specular = light.color * spec * vec3(texture(material.texture_specular1, TexCoord)) * material.specularColor;
    return (ambient + diffuse + specular);
}  

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = light.intensity / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    //float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient  = ambientColor  * vec3(texColor);
    light.color *= light.intensity;
    vec3 diffuse  = light.color  * diff * vec3(texColor);
    vec3 specular = light.color * spec * vec3(texture(material.texture_specular1, TexCoord)) * material.specularColor;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}