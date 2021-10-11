R""(
#version 330 core
in vec2 ourPos;

out vec4 FragColor;

void main()
{
	float LIMIT = 300;
	float THRESH = 5;
	
	float a = ourPos.x;
	float b = ourPos.y;
	float a2 = a * a;
	float b2 = b * b;
	float d = (a + b) * (a + b);

	float iterations = 0;

	while(iterations < LIMIT)
	{
		a = a2 - b2 + ourPos.x;
		b = d - a2 - b2 + ourPos.y;
		a2 = a * a;
		b2 = b * b;
		d = (a + b) * (a + b);

		iterations += 1;
	}

	float val = sqrt(a2 + b2);
	float intensity = val*pow(exp(1), floor(log(1/val)));
	FragColor = vec4(intensity, intensity, intensity, 1.0f);
} 
)""