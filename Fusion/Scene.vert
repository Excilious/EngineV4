layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
layout (location = 3) in vec2 aTex;

out vec3 Normal;
out vec3 NormalAmbience;
out vec3 Colour;
out vec2 TextureCoord;
out vec3 FragPos;

uniform mat4 CameraMatrix;
uniform mat4 Translation;
uniform mat4 Rotation;
uniform mat4 Scale;

void main()
{ 
    mat4 Model = Translation * Rotation * Scale;
    Colour = aColor;
    // Transform normal properly and normalize
    Normal = aNormal;
    NormalAmbience = normalize(mat3(transpose(inverse(Model))) * aNormal);
    
    // Correct texture coordinate handling
    TextureCoord = aTex;

    // Get world-space fragment position (fixed offset issue)
    FragPos = vec3(Model * vec4(aPos, 1.0f));

    // Final position in clip space
    gl_Position = CameraMatrix * Model * vec4(aPos, 1.0);
}
