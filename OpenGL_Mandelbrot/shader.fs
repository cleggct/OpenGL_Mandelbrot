#version 330 core
in vec2 ourPos;

out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	float THRESH = 100.0f;
	int LIMIT = 50;
	float INC = 1.0f/LIMIT;
	//int ITERATIONS = 100;
	//float COLORS = 5;

	vec2 num = vec2((ourPos.x*ourPos.x)-(ourPos.y*ourPos.y), 2*ourPos.x*ourPos.y);
	num = vec2((num.x*num.x) - (num.y*num.y) + ourPos.x, (2*num.x*num.y) + ourPos.y);

	//for (int i = 0; i < ITERATIONS; ++i)
	//{
	//	num = vec2((num.x*num.x) - (num.y*num.y) + ourPos.x, (2*num.x*num.y) + ourPos.y);
	//}

	//float mag = length(num);
	//float y = ceil(log2(1/mag));
	//float val = mod(mag*(exp2(y)), COLORS)/COLORS;

	//FragColor = vec4(0.0f, 0.0f, val, 1.0f);

	int i = 0;

	while((length(num) < THRESH) && (i < LIMIT))
	{
		i += 1;
		//LIMIT *= 10;
		num = vec2((num.x*num.x) - (num.y*num.y) + ourPos.x, (2*num.x*num.y) + ourPos.y);
		FragColor = vec4(FragColor.x - INC, FragColor.y - INC, FragColor.z - INC, FragColor.w);
	}
} 