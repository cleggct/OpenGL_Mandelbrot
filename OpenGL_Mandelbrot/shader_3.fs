#version 330 core
in vec2 ourPos;

out vec4 FragColor;

void main()
{
	float LIMIT = 100;
	float THRESH = 5;
	//float inc = 1.0f/LIMIT;
	
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

	//float val = d;

	//float prime = 13;

	//if(d < 1)
	//{
	//	val = val*pow(10, ceil(log(1/d)/log(10))+1);
	//}

	//float intensity = 1.0f - mod(val, prime)/prime;

	//if(a2 + b2 >= THRESH)
	//{
	//	FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	//}
	//else
	//{
	//	float val = a2 + b2;
	//	float intensity = val*pow(exp(1), floor(log(1/val)));
	//	FragColor = vec4(intensity, intensity, intensity, 1.0f);
	//}

	float val = sqrt(a2 + b2);
	float intensity = val*pow(exp(1), floor(log(1/val)));
	FragColor = vec4(intensity, intensity, intensity, 1.0f);
} 