#pragma once

#include <glm/glm.hpp>

class Color
{
public:
	Color() 
	{
		color[0] = 0;
		color[1] = 0;
		color[2] = 0;
	}

	Color(glm::vec3 color_)
	{
		color[0] = color_.x;
		color[1] = color_.y;
		color[2] = color_.z;
	}
	
	Color(float r, float g, float b) 
	{
		color[0] = r;
		color[1] = g;
		color[2] = b;
	}

	Color(float c)
	{
		color[0] = c;
		color[1] = c;
		color[2] = c;
	}

	float* getColor()
	{
		return color;
	}


public:
	operator glm::vec3()
	{
		return glm::vec3(color[0], color[1], color[2]);
	}



private:
	float color[3];
};
