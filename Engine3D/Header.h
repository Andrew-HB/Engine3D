#pragma once
#include <Windows.h>
#include <string>
#include <vector>
#include <fstream>
#include <strstream>
#include <algorithm>
#include <cmath>
#include <list>

using namespace std;

float fz(float x, float y, float a, float b, float c);

struct vec3d {
	float x = 0;
	float y = 0;
	float z = 0;
	float w = 1;
};

struct triangle {
	vec3d p[3];

	float gryscale;
};

struct mesh {
	std::vector<triangle> tris;

	bool LoadFromObjectFile(string sFilename);
	void UseMathFormula(); 
};

struct mat4x4 {
	float m[4][4] = { 0 };
};

void DrawTriangle(HDC hdc, float x1, float y1, float x2, float y2, float x3, float y3);

COLORREF GetGrayscaleColour(float lum);