// FRAGMENT SHADER

#version 440 core

out vec4 FragColor;

in float fTemperature;

void main() {
	FragColor = vec4(fTemperature, fTemperature, fTemperature, 1.0);
}