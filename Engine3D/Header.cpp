#include "Header.h"

float fz(float x, float y, float a, float b, float c) {
	return (x * x) / (a * a) - (y * y) / (b * b); // Hyperbolic paraboloid (5, 5, -)
}
	//return (x * x) / (a * a) + (y * y) / (b * b); // Elliptical paraboloid (3, 3, -)
	//return sqrtf(c * c * ((x * x) / (a * a) + (y * y) / (b * b))); // Conical surface (3, 3, 3)
	//return sqrtf(a * a - x * x - y * y);

void DrawTriangle(HDC hdc, float x1, float y1, float x2, float y2, float x3, float y3) {
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
	LineTo(hdc, x3, y3);
	LineTo(hdc, x1, y1);
}

COLORREF GetGrayscaleColour(float lum) {
	return RGB(lum * 255, lum * 255, lum * 255);
}

bool mesh::LoadFromObjectFile(string sFilename) {
	ifstream f(sFilename);
	if (!f.is_open())
		return false;

	// Local cache of verts
	vector<vec3d> verts;

	while (!f.eof())
	{
		char line[128];
		f.getline(line, 128);

		strstream s;
		s << line;

		char junk;

		if (line[0] == 'v')
		{
			vec3d v;
			s >> junk >> v.x >> v.y >> v.z;
			verts.push_back(v);
		}

		if (line[0] == 'f')
		{
			int f[3];
			s >> junk >> f[0] >> f[1] >> f[2];
			tris.push_back({ verts[f[0] - 1], verts[f[1] - 1], verts[f[2] - 1] });
		}
	}

	return true;
}

void mesh::UseMathFormula() {
	float a = 5.0f;
	float b = 5.0f;
	float c = 3.0f;

	for (float i = -10; i <= 10; i += 0.5f) {
		for (float j = -10; j <= 10; j += 0.5f) {
			triangle temp;
			temp.p[0] = { (float)i, fz(i, j, a, b, c) , (float)j};
			temp.p[1] = { (float)i, fz(i, j + 1, a, b, c) , (float)j + 1.0f};
			temp.p[2] = { (float)i + 1.0f, fz(i + 1, j, a, b, c) , (float)j};

			tris.push_back(temp);

			temp.p[0] = { (float)i, fz(i, j + 1, a, b, c) , (float)j + 1};
			temp.p[1] = { (float)i + 1, fz(i + 1, j + 1, a, b, c) , (float)j + 1.0f};
			temp.p[2] = { (float)i + 1.0f, fz(i + 1, j, a, b, c) , (float)j};

			tris.push_back(temp);
		}
	}
}
