#version 330 core

out vec4 FragColor;
in vec3 NewPosition; 
in vec3 Normal;         
in vec2 TextureCoord;
in vec3 FragPos;
uniform sampler2D Diffuse0;
uniform sampler2D Specular0;
uniform sampler2D RoughnessMap; 
uniform sampler2D MetalnessMap;
uniform vec3 CameraPosition; 

void main()
{
    FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
}
