#pragma once

#include <cmath>

#include "Command.h"

#define MIN_ARGS 5
#define PI 3.14159265

class Sphere : public Command
{
public:
	const std::string getName() const override;
	uint8_t execute(const map<string, string>& args) override;
private:
	int radius, slices, rings;
	Coordinates origin;
	string filepath;

	uint8_t parse(const map<string, string>&);
	const Triangles* createTriangles();
	Vertex createVertex(double, double);
};