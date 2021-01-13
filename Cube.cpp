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

#include "Cube.hpp"

const string Cube::getName() const{
	string name = "Cube";

	return name;
}

uint8_t Cube::execute(const map<string, string>& args) {
	// Parse the arguments
	parse(args);

	// Split the object
	STLParser parser = STLParser();
	const Triangles* triangles = createTriangles();

	parser.write(triangles, filepath);

	return 0;
}

uint8_t Cube::parse(const map<string, string>& args) {
	string key, value;			// Temporary values of map iteration
	size_t position;			// Temporary variable for storing a position inside of a string
	vector<size_t> positions;	// Array of positions

	// Check arguments size
	if (args.size() < MIN_ARGS) std::exit(LESS_ARGS_ERR_CODE);

	for (pair<string, string> pair : args) {
		key = pair.first;
		value = pair.second;

		// If the argument is a length of the cube's side
		if (key == "L") {
			length = stoi(value);
			
			// terminate the execution, if the length is incorrect
			if (length <= 0) std::exit(INCORR_LENGTH_ERR_CODE);
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

		// Save the output file name
		else if (key == "filepath") {
			filepath = value;
		}
	}
}

const Triangles* Cube::createTriangles() {
	Triangles* triangles = new Triangles();
	Vertex origin_vertex, f_common, s_common;
	Triangle triangle;

	// Calculate the vertexes of the cube and create triangles based on them (half of the cube)
	for (int counter = 0, second_counter = 1; counter < 3 && second_counter < 4; ++counter, ++second_counter) {
		// second counter is used to move common points (one is offset along the X axis, the other - along Y, one along Y, and so on)
		if (second_counter == 3) {
			second_counter = 0;
		}
		
		// Clear vertex temporary files
		f_common.position = { origin.x, origin.y, origin.z };
		s_common.position = { origin.x, origin.y, origin.z };
		origin_vertex.position = origin;

		// Offset two diagonal common points of a face
		f_common.position[counter] += length;
		s_common.position[second_counter] += length;

		// Write points to the triangle
		triangle.push_back(f_common);
		triangle.push_back(s_common);
		triangle.push_back(origin_vertex);

		// Write the triangle to the triangle vector
		triangles->push_back(triangle);

		//Offset Remaining Vertex (which is not shared)
		triangle[2].position[counter] += length;
		triangle[2].position[second_counter] += length;

		// Write the new triangle to the vector
		triangles->push_back(triangle);

		// Clear the temporary triangle
		triangle.clear();
	}
	
	// Move the origin diagonally (to draw the second half of the cube)
	origin.x += length;
	origin.y += length;
	origin.z += length;

	//Просчитать точки куба и создать на их основе треугольники (вторая половина куба)
	for (int counter = 0, second_counter = 1; counter < 3 && second_counter < 4; ++counter, ++second_counter) {
		// second counter is used to move common points (one is offset along the X axis, the other - along Y, one along Y, and so on)
		if (second_counter == 3) {
			second_counter = 0;
		}

		// Clear vertex temporary files
		f_common.position = { origin.x, origin.y, origin.z };
		s_common.position = { origin.x, origin.y, origin.z };
		origin_vertex.position = origin;

		// Offset two diagonal common points of a face
		f_common.position[counter] -= length;
		s_common.position[second_counter] -= length;

		// Write points to the triangle
		triangle.push_back(f_common);
		triangle.push_back(s_common);
		triangle.push_back(origin_vertex);

		// Write the triangle to the triangle vector
		triangles->push_back(triangle);

		//Offset Remaining Vertex (which is not shared)
		triangle[2].position[counter] -= length;
		triangle[2].position[second_counter] -= length;

		// Write the new triangle to the vector
		triangles->push_back(triangle);

		// Clear the temporary triangle
		triangle.clear();
	}

	return triangles;
}