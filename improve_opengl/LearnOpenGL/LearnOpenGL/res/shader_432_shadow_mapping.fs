
#version 330 core

out vec4 color;

struct Material{
	//(通常等同于漫反射光照) vec3 ambient; // 环境光照
	// 漫反射贴图
	sampler2D texture_diffuse1;
	sampler2D texture_diffuse2;
	sampler2D texture_diffuse3;
	sampler2D texture_diffuse4;
	sampler2D texture_diffuse5;

	// 镜面光照
	sampler2D texture_specular1;
	sampler2D texture_specular2;
	sampler2D texture_specular3;
	sampler2D texture_specular4;
	sampler2D texture_specular5;

	float shininess; // 反光度
};

// 平行光
struct DirectionLight{
	// vec3 position;
	vec3 direction; // 
	vec3 ambient; // 环境光照
	vec3 diffuse; // 漫反射光照
	vec3 specular; // 镜面光照
};

// 点光源
struct PointLight{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;

	float radius;
	samplerCube shadowTexture;
};

// 聚光灯
struct SpotLight{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
};

// 平行光
vec3 calcDirectionLight(DirectionLight light, vec3 normal, vec3 viewDir);
// 点光源
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos);
// 聚光灯
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos);

// 阴影深度计算
float ShadowCalculation(PointLight light, vec3 fragPos);



in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords; 



uniform vec3 viewPos;

uniform Material material;
uniform DirectionLight directionLight; 

#define POINT_LIGHT_COUNT 3
uniform PointLight pointLights[POINT_LIGHT_COUNT];


void main()
{
	vec3 result = vec3(0.0f);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 normal = normalize(Normal);
	
	// result += calcDirectionLight(directionLight, normal, viewDir);
	// result += calcPointLight(pointLight, normal, viewDir, FragPos);

	for(int i = 0; i < POINT_LIGHT_COUNT; i ++)
	{
	 	result += calcPointLight(pointLights[i], normal, viewDir, FragPos);
	}
	// result += calcSpotLight(spotLight, normal, viewDir, FragPos);

	color = vec4(result, 1.0f);
}

float ShadowCalculation(PointLight light, vec3 fragPos)
{
	vec3 fragToLight = fragPos - light.position;

	float currentDepth = length(fragToLight);
	// float shadowValue = currentDepth > closestDepth ? 1.0 : 0.0;

	float shadow = 0.0;
	float samples = 4.0;
	float offset = 0.1;

	for(float x = -offset; x < offset; x += offset / (samples * 0.5))
	{
		for(float y = -offset; y < offset; y += offset / (samples * 0.5))
		{
			for(float z = -offset; z < offset; z += offset / (samples * 0.5))
			{
				float closestDepth = texture(light.shadowTexture, fragToLight + vec3(x,y,z)).r;
				closestDepth *= light.radius;
				shadow += currentDepth > closestDepth ? 1.0 : 0.0;
			}
		}
	}

	shadow /= (samples * samples * samples);

	// vec2 texelSize = 1.0 / textureSize(ShadowTexture, 0);

	// float shadowValue = 0.0;
	// float currentDepth = length(fragToLight);
	// float closestDepth = 0.0;
	// for(int i = -1; i <= 1; i++)
	// {
	// 	for(int j = -1; j <= 1; j++)
	// 	{
	// 		closestDepth = texture(ShadowTexture, fragToLight).r;
	// 		closestDepth *= far_plane;
	// 		shadowValue += currentDepth > closestDepth ? 1.0 : 0.0;
	// 	}
	// }
	// shadowValue /= 9;

	return shadow;
}

// 平行光
vec3 calcDirectionLight(DirectionLight light, vec3 normal, vec3 viewDir)
{
	// 贴图
	vec3 diffuseTexture = vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 specularTexture = vec3(texture(material.texture_specular1, TexCoords));
	// ambient
	vec3 ambient = light.ambient * diffuseTexture;
	
	// diffuse
	vec3 lightDir = normalize(-light.direction);
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * (diff * diffuseTexture);
	
	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (spec * specularTexture); 

	// float shadow = ShadowCalculation(FragPosLightSpace);

	float shadow = 0.0f;

	return ambient + (1.0 - shadow) * (diffuse + specular);
}

// 点光源
vec3 calcPointLight(PointLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
	// 贴图
	vec3 diffuseTexture = vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 specularTexture = vec3(texture(material.texture_diffuse1, TexCoords));
	// vec3 diffuseTexture = vec3(1.0f);
	// vec3 specularTexture = vec3(1.0f);

	float distance = length(light.position - fragPos);
	float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * distance * distance);
	// ambient
	vec3 ambient = light.ambient * diffuseTexture * attenuation;
	
	vec3 lightDir = normalize(light.position - fragPos);
	vec3 halfwayDir = normalize(viewDir + lightDir);

	// diffuse
	float diff = max(dot(normal, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * (diff * diffuseTexture) * attenuation;

	// specular
	float spec = 0.0f;
	spec = pow(max(dot(normal, -halfwayDir), 0.0), material.shininess);

	vec3 specular = light.specular * (spec * specularTexture);//  * attenuation; 
	// vec3 specular = vec3(0.0f);

	float shadow = ShadowCalculation(light, fragPos);

	return ambient + (1.0 - shadow) * (diffuse + specular);
}

// 聚光灯
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos)
{
	// 贴图
	vec3 diffuseTexture = vec3(texture(material.texture_diffuse1, TexCoords));
	vec3 specularTexture = vec3(texture(material.texture_specular1, TexCoords));

	// ambient
	vec3 result = light.ambient * diffuseTexture;
	vec3 ambient = light.ambient * diffuseTexture;

	vec3 lightDir = normalize(light.position - fragPos);
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	if (theta > light.outerCutOff)
	{
		// diffuse
		float diff = max(dot(normal, lightDir), 0.0f);
		vec3 diffuse = light.diffuse * (diff * diffuseTexture);
		diffuse *= intensity;

		// specular
		vec3 reflectDir = reflect(-lightDir, normal);
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
		vec3 specular = light.specular * (spec * specularTexture); 
		specular *= intensity;

		result = ambient + diffuse + specular;
	}
	
	return result;
}

