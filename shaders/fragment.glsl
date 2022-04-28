#version 440 core

// Time for a fragment shader re-write

struct Material {
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};


struct Light {
	vec3 lightPos;
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specualar;

	float constant;
	float linear;
	float quadratic;
};

struct Tex{
	sampler2D diffuse;
	float shine;
};

// Uniform values
// For now, lets max out at 255 lights just for simplicity
uniform Material material;
uniform Light light[255];
uniform vec3 cameraPos;
uniform sampler2D diffuse_mat;
uniform int lightCount;

uniform float brightness;

// Our in values from vertex
in vec2 textcord;
in vec3 Normal;
in vec3 FragPos;

// Output values
out vec4 FragColor;

// Our local variables
// Light direction
vec3 lightDir = vec3(0.0f);
// Ambient value
vec3 ambient = vec3(0.0f);
// Normals Calculation
vec3 norm = vec3(0.0f);

// Our diffuse
float diff = 0.0f;
vec3 diffuse = vec3(0.0f);
// (TODO) Maybe make this a uniform
float specularStrength = 0.0f;

vec3 viewDir = vec3(0.0f);
vec3 reflectDir = vec3(0.0f);

// Our specular values
float spec = 0.0f;
vec3 specular = vec3(0.0f);

// Our values for distance
float cal_dist = 0.0f;
float attenuation = 0.0f;

// Our result
vec4 result = vec4(0.0f);

void calculateAmbient(int y){
	int x = y;
	ambient += light[x].ambient * material.ambient * vec3(texture(diffuse_mat, textcord));
}

void findLightDirection(int y){
	int x = y;
	norm = normalize(Normal);
	if (light[x].direction == vec3(0.0)){
		lightDir = normalize(light[x].lightPos - FragPos);
	}
	else{
		lightDir = normalize(-light[x].direction);
	}
}

void calculateDiffuse(int y){
	int x = y;
	if (light[x].direction == vec3(0.0)){
        diff = max(dot(norm, lightDir), 0.0);
        diffuse = material.diffuse * light[x].diffuse * diff * vec3(texture(diffuse_mat, textcord));
	}
	else{
        diffuse = material.diffuse * light[x].diffuse * vec3(texture(diffuse_mat, textcord));
	}
}

void calculateReflect(){
	specularStrength = 0.5;
	viewDir = normalize(cameraPos - FragPos);
	reflectDir = reflect(-lightDir, norm);
}

void calculateSpecular(int y){
	int x = y;
	//float spec = pow(max(dot(cameraPos, reflectDir), 0.0), material.shininess);
	spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	specular = material.specular * light[x].specualar * spec * vec3(texture(diffuse_mat, textcord));
}

void calculateAttenuation(int y){
	int x = y;
	if (light[x].constant != 0.0){
		cal_dist    = length(light[x].lightPos - FragPos);
  		attenuation = 1.0 / (light[x].constant + light[x].linear * cal_dist + light[x].quadratic * (cal_dist * cal_dist));

  		ambient *= attenuation;
  		diffuse *= attenuation;
  		specular *= attenuation;
	}
}
void main(){
	for (int i = 0; i < lightCount; i++){
		calculateAmbient(i);

		findLightDirection(i);

		calculateDiffuse(i);
        if (light[i].direction == vec3(0.0)){
            calculateReflect();

            calculateSpecular(i);
        }
        else{
            specular = vec3(0.0f);
        }

		calculateAttenuation(i);

		result += vec4(ambient+diffuse+specular, 1.0f);
		//result += vec4(i/lightCount);
	}

        FragColor = (result * brightness);
        //FragColor = vec4(cameraPos, 0.5f);
}
