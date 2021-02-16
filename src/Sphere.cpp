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

#include "Sphere.hpp"

const string Sphere::getName() const {
	string name = "Sphere";

	return name;
}

uint8_t Sphere::execute(const map<string, string>& args) {
	// Parse the arguments
	int result = parse(args);

	// Create a shape and save it
	STLParser parser = STLParser();
	const Triangles* triangles = createTriangles();

	parser.write(triangles, filepath);

	return result;
} 

uint8_t Sphere::parse(const map<string, string>& args) {
	string key, value;			// Temporary values of map iteration
	size_t position;			// Temporary variable for storing a position inside of a string
	vector<size_t> positions;	// Array of positions

	// Check arguments size
	if (args.size() < MIN_ARGS) std::exit(LESS_ARGS_ERR_CODE);
	for (pair<string, string> pair : args) {
		key = pair.first;
		value = pair.second;

		// If the data is a sphere's radius
		if (key == "R") {
			radius = stoi(value);

			// Stop the parsing if the radius is incorrect
			if (radius <= 0) return INCORR_LENGTH_ERR_CODE;
		}

		// If the data is the origin coordinates
		else if (key == "origin") {
			position = value.find(",");
			positions = vector<size_t>();

			// Fetching positions of commas
			while (position != string::npos) {
				// Save the current comma position
				positions.push_back(position);

				// Get the next comma position
				position = value.find(",", position + string(",").size());
			}

			// Save the position of the origin
			origin.x = stod(value.substr(1, positions[0]));
			origin.y = stod(value.substr(positions[0] + 1, positions[1]));
			origin.z = stod(value.substr(positions[1] + 1, value.size() - 1));
		}

		// If the data is amount of slices of a sphere
		else if (key == "slices") {
			slices = stoi(value);
		}

		// If the data is amount of rings of a sphere
		else if (key == "rings") {
			rings = stoi(value);
		}

		// Save the output file name
		else if (key == "filepath") {
			filepath = value;
		}
	}
}

const Triangles* Sphere::createTriangles() {
	Triangles* triangles = new Triangles();		// The sphere as an array of triangles
	Triangle triangle;							// Temporary variable for triangles
	double phi_step = (360.0 / slices);			// Angle step
	double theta_step = (180.0 / rings);		// Angle step

	for (double phi = 0; phi < 360.0; phi += phi_step) {
		for (double theta = 0; theta < 180.0; theta += theta_step) {
			// Create vertexes of a triangle
			triangle.push_back(createVertex(phi, theta));
			triangle.push_back(createVertex(phi + phi_step, theta));
			triangle.push_back(createVertex(phi, theta + theta_step));

			// Store the triangle inside of the array
			triangles->push_back(triangle);

			// Creating an oposite triangle to create decrease amounts of steps inside of the loops
			triangle[0] = createVertex(phi + phi_step, theta + theta_step);

			// Store the triangle inside of the array
			triangles->push_back(triangle);

			// Delete the temporary triangle
			triangle.clear();
		}
	}

	return triangles;
}

Vertex Sphere::createVertex(double phi, double theta) {
	Vertex vertex = Vertex();

	// Convert the angles to radians
	phi = phi * PI / 180;
	theta = theta * PI / 180;

	// Calculate the vertex's coordinates  
	vertex.position.x = origin.x + (cos(phi) * radius * sin(theta));
	vertex.position.y = origin.y + (sin(phi) * radius * sin(theta));
	vertex.position.z = origin.z + (cos(theta) * radius);

	return vertex;
}