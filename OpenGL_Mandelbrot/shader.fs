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

	int thresh1 = 30;
	int thresh2 = 90;

	if (iterations <= thresh1)
	{
		float intensity = iterations/thresh1;
		FragColor = vec4(intensity, intensity, 0.0f, 1.0f);
	}
	else if (thresh1 < iterations && iterations <= thresh2)
	{
		float val = mod(iterations, 2.0f);
		if(val == 0.0f)
		{
			FragColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
		}
		else
		{
			FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}
	else
	{
		float intensity = 0.5*(iterations-thresh2)/(LIMIT-thresh2);
		FragColor = vec4(1.0f-intensity+0.5, 1.0f-intensity+0.45, 0.0f, 1.0f);
	}

	//float intensity = 1.0f - iterations*inc;
	//FragColor = vec4(intensity, intensity, intensity, intensity);
} 