#version 330 core
in vec2 ourPos;

out vec4 FragColor;

void main()
{
	float LIMIT = 100;
	float THRESH = 5;
	float inc = 1.0f/LIMIT;

    //FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	
	float a = ourPos.x;
	float b = ourPos.y;
	float a2 = a * a;
	float b2 = b * b;
	float d = (a + b) * (a + b);

	float iterations = 0;

	while((a2 + b2 < THRESH) && (iterations < LIMIT))
	{
		a = a2 - b2 + ourPos.x;
		b = d - a2 - b2 + ourPos.y;
		a2 = a * a;
		b2 = b * b;
		d = (a + b) * (a + b);

		iterations += 1;
	}

	float intensity = 1.0f - iterations*inc;
	FragColor = vec4(intensity, intensity, intensity, intensity);
} 