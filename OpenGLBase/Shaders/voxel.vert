#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in unsigned int aNormal;

out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    switch (aNormal) {
        case 0: // Front
            Normal = mat3(transpose(inverse(model))) * vec3(0, 0, 1);
            break;
        case 1: // Back
            Normal = mat3(transpose(inverse(model))) * vec3(0, 0, -1);
            break;
        case 2: // Right
            Normal = mat3(transpose(inverse(model))) * vec3(1, 0, 0);
            break;
        case 3: // Left
            Normal = mat3(transpose(inverse(model))) * vec3(-1, 0, 0);
            break;
        case 4: // Top
            Normal = mat3(transpose(inverse(model))) * vec3(0, 1, 0);
            break;
        case 5: // Bottom
            Normal = mat3(transpose(inverse(model))) * vec3(0, -1, 0);
            break;
        default:
            break;
    }
    FragPos = vec3(model * vec4(aPos, 1.0));
    //Normal = mat3(transpose(inverse(model))) * aNormal;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}