#version 330 core


layout (location = 0) in vec2 aPos;

out vec2 ourPos;

uniform vec3 settings;

void main()
{
	float SCALE = settings.z;
	float OFFSET_X = settings.x;
	float OFFSET_Y = settings.y;

    gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
	ourPos = vec2((aPos.x/SCALE)+OFFSET_X, (aPos.y/SCALE)+OFFSET_Y);
}