#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in unsigned int aNormal;
layout (location = 2) in unsigned int aColor;

out vec3 FragPos;
out vec3 Normal;
out vec3 Color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const unsigned int GRASS_TYPE = 1;
const unsigned int STONE_TYPE = 2;
const unsigned int SNOW_TYPE = 3;
const unsigned int DIRT_TYPE = 4;

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

    Color = vec3(1.0, 1.0, 1.0);
    switch (aColor) {
    case GRASS_TYPE:
        Color = vec3(0.34, 0.49, 0.28);
        break;
    case STONE_TYPE:
        Color = vec3(0.57, 0.56, 0.52);
        break;
    case SNOW_TYPE:
        Color = vec3(0.95, 0.95, 0.95);
        break;
    case DIRT_TYPE:
        Color = vec3(.71, .40, .12);
        break;
    default:
        Color = vec3(1.0, 0.0, 0.0);
        break;
    }

    FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}