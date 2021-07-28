#version 330 core


layout (location = 0) in vec2 aPos;

out vec2 ourPos;

uniform vec2 dims;
uniform vec3 settings;

void main()
{
	float WIDTH = dims.x;
	float HEIGHT = dims.y;
	float SCALE = settings.z;
	float OFFSET_X = settings.x;
	float OFFSET_Y = settings.y;

	float w = WIDTH/2;
	float h = HEIGHT/2;

	float x_pos = aPos.x;
	float y_pos = (aPos.y * h)/w;

    gl_Position = vec4(aPos.x, aPos.y, 0.0f, 1.0f);
	ourPos = vec2((x_pos/SCALE)+OFFSET_X, (y_pos/SCALE)+OFFSET_Y);
}