/*
 * Copyright 2018 Illia Shvarov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <cmath>
#include "Command.hpp"

/** 
 * Constants
 */
#define MIN_ARGS 5
#define PI 3.14159265

/**
 * Creating a sphere out of the input
 */
class Sphere : public Command {
public:
	/**
	 * Return the name of the command
	 * @return the name of the command
	 */
	const std::string getName() const override;

	/**
	 * Start execution and create a sphere
	 * @return error code
	 */
	uint8_t execute(const map<string, string>& args) override;
protected:
	int radius, slices, rings;			// Features of a sphere
	Coordinates origin;					// Origin of the sphere
	string filepath;					// The output file

	/**
	 * Parsing the arguments
	 * @param args data about the sphere
	 * @return error code
	 */
	uint8_t parse(const map<string, string>&);

	/**
	 * Create a shape of a sphere out of vertexes
	 * @return a sphere as an array of triangles
	 */
	const Triangles* createTriangles();

	/**
	 * Create a vertex which is a part of the sphere
	 * @param phi angle of rotation in the spherical coordinate system
	 * @param theta angle of rotation in the spherical coordinate system
	 * @return a vertex
	 */
	Vertex createVertex(double, double);
};