#version 460 core
   
in vec3 Normal;
in vec3 Color;
in vec2 UV;

uniform sampler2D TextureSampler;
uniform sampler2D CloudTexture;

uniform float Time;

uniform vec2 CloudRotationSpeed = vec2(0.003, 0.008);

uniform vec3 LightDirection = vec3(0.0, 0.0, 1.0);
uniform float LightIntensity;

out vec4 OutColor;

void main()
{
	vec3 N = normalize(Normal);

	vec3 L = -normalize(LightDirection);

	float Lambertian = max(dot(N, L), 0.0f);

	vec3 ViewDirection = vec3(0.0f, 0.0f, 1.0);
	vec3 V = -ViewDirection;

	vec3 R = reflect(-L, N);

	float Alpha = 55.0;
	float Specular = pow(max(dot(R, -V), 0.0), Alpha);

	vec3 EarthColor = texture(TextureSampler, UV).rgb;
	vec3 CloudColor = texture(CloudTexture, UV + Time * CloudRotationSpeed).rgb;
	vec3 FinalColor = (EarthColor + CloudColor) * LightIntensity * Lambertian + Specular;
	
	OutColor = vec4(FinalColor, 1.0);

}
