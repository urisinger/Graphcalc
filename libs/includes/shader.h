#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <stack>

#include <glad.h>
#include "Parser.h"

struct vec2 {
	float x;
	float y;
	float ratio;
	vec2(float _x, float _y) {
		x = _x;
		y = _y;
		if (_x != 0.0) {
			ratio = _y / _x;
		}
		else {
			ratio = 0.0;
		}
	}
};


class Shader {
public:
	Shader(const std::string& vertexFilePath, const std::string& fragmentFilePath ,vec2 res);
	~Shader();

    unsigned int GetID();
	void SetCameraUniform(vec2 res, vec2 zoom, vec2 offset);
	void Bind();
	void UnBind();
    void BindTexture(int index);

private:
	unsigned int Renderer_ID;

	unsigned int CompileShader(unsigned int type, const std::string& source);
	std::string OpenShader(std::string filepath);
};