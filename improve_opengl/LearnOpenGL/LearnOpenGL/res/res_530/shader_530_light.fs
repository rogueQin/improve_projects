
#version 330 core

out vec4 color;

in VS_OUT{
	vec2 TexCoords;
	vec3 FragPos;
	vec3 Normal;
	mat3 TBN;
} fs_in;

uniform vec3 camPos;

#define POINT_LIGHT_COUNT 4
uniform vec3 lightPosition[POINT_LIGHT_COUNT];

uniform samplerCube image_irradiance_cube;
uniform samplerCube image_prefilter_cube;
uniform sampler2D image_brdf_LUT;

// uniform sampler2D image_albedo;
// uniform sampler2D image_normal;
// uniform sampler2D image_ao;
// uniform sampler2D image_metallic;
// uniform sampler2D image_roughness;
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;
uniform float ao;

const float PI = 3.14159265359;
const float MAX_REFLECTION_LOD = 4.0;

// 菲涅尔方程
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// 法线分布函数
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N,H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = NdotH2 * (a2 - 1.0) + 1.0;
	denom = PI * denom * denom;

	return nom / denom;
}

// 几何遮蔽函数
float GeometrySchlickGGX(float NdotV, float roughness)
{
	float nom = NdotV;
	float denom = NdotV * (1 - roughness) + roughness;

	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N,V), 0.0);
	float NdotL = max(dot(N,L), 0.0);
	float ggx1 = GeometrySchlickGGX(NdotV, roughness);
	float ggx2 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

void main()
{
	// vec3 albedo = pow(texture(image_albedo, fs_in.TexCoords).rgb, vec3(2.2));
	// vec3 normal_color = texture(image_normal, fs_in.TexCoords).rgb;
	// normal_color = normalize(normal_color * 2.0 - 1.0);
	// vec3 normal = fs_in.TBN * normal_color;
	// float metallic = texture(image_metallic, fs_in.TexCoords).r;
	// float roughness = texture(image_roughness, fs_in.TexCoords).r;
	// float ao = texture(image_ao, fs_in.TexCoords).r;

	vec3 normal = (fs_in.Normal);

	vec3 light_color = vec3(300.0);

	vec3 view_dir = normalize(camPos - fs_in.FragPos);
	vec3 reflect_dir = reflect(-view_dir, normal);

	vec3 Lo = vec3(0.0);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	// 点光源能量
	for(int i = 0; i < 4; i++)
	{
		vec3 light_dir = normalize(lightPosition[i] - fs_in.FragPos);
		vec3 half_dir = normalize(view_dir + light_dir);

		// 入射光能量
		float distance = length(lightPosition[i] - fs_in.FragPos);
		float attenuaction = 1.0 / (distance * distance);
		vec3 radiance = light_color * attenuaction;

		// 菲涅尔方程
		vec3 F = fresnelSchlick(max(dot(light_dir, view_dir), 0.0), F0);
		vec3 kD = (vec3(1.0) - F) * (1 - metallic);

		// 法线分布
		float D = DistributionGGX(normal, half_dir, roughness);

		// 几何遮蔽函数
		float G = GeometrySmith(normal, view_dir, light_dir, roughness);

		// BRDF(双向反射分布函数)
		vec3 nominator = D * G * F;
		float denominator = 4.0 * max(dot(normal, view_dir), 0.0) * max(dot(normal, light_dir), 0.0) + 0.001;
		vec3 specular = nominator / denominator;

		// 出射光线
		float NdotL = max(dot(normal, light_dir), 0.0);
		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
		// Lo += radiance * NdotL * G;
	}

	// 环境光漫反射
	vec3 F_env = fresnelSchlick(max(dot(fs_in.Normal, view_dir), 0.0), F0);
	vec3 kD_env = (vec3(1.0) - F_env) * (1 - metallic);

	vec3 irradiance = texture(image_irradiance_cube, fs_in.Normal).rgb;
	vec3 diffuse = irradiance * albedo;

	vec3 prefilteredColor = textureLod(image_prefilter_cube, reflect_dir, roughness * MAX_REFLECTION_LOD).rgb;
	vec2 envBRDF = texture(image_brdf_LUT, vec2(max(dot(normal, view_dir), 0.0), roughness)).rg;

	vec3 specular = prefilteredColor * (F_env * envBRDF.x + envBRDF.y);
	vec3 ambient = (kD_env * diffuse + specular) * ao;

	// vec3 ambient = vec3(0.03) * albedo * ao;
	vec3 out_color = ambient + Lo;
	out_color = out_color / (out_color + vec3(1.0));
	out_color = pow(out_color, vec3(1.0/2.2)); 

	color = vec4(out_color, 1.0);
}
