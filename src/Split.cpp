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

#include "Split.hpp"

Split::Split() {
	slice = Polygon();
}

const string Split::getName() const {
	string name = "Split";

	return name;
}

uint8_t Split::parse(const map<string, string>& args) {	
	string key, value;			// Temporary values of map iteration
	size_t position;			// Temporary variable for storing a position inside of a string
	vector<size_t> positions;	// Array of positions

	// Check the size of argument array
	if (args.size() < MIN_ARGS) {
		std::exit(LESS_ARGS_ERR_CODE);
	}

	/*
		TODO : 
			- Create a static method in the class Application to split strings into tokens 
			- Refactor this method and remove duplicating
	*/

	for (pair<string, string> pair : args) {
		key = pair.first;
		value = pair.second;

		// If the data is a normal vector
		if (key == "direction") {
			position = value.find(",");
			positions = vector<size_t>();

			// Fetching positions of commas
			while (position != string::npos) {
				positions.push_back(position);

				// Find the next comma
				position = value.find(",", position + string(",").size());
			}

			// Save the normal vector
			normal.x = stod(value.substr(1, positions[0]));
			normal.y = stod(value.substr(positions[0] + 1, positions[1]));
			normal.z = stod(value.substr(positions[1] + 1, value.size() - 1));

			// Exit if the normal has zero length
			if ((normal.x == 0) && (normal.y == 0) && (normal.z == 0)) std::exit(ZERO_NORMAL);
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

		// Save the input file name
		else if (key == "input") input = value;

		// Save the output file name
		else if (key == "output1") first_output = value;

		// Save the output file name
		else if (key == "output2") second_output = value;
	}

	return 0;
}

uint8_t Split::execute(const map<string, string>& args) {
	STLParser parser = STLParser();
	vector<const Triangles*>* triangles_vector;

	// Parse the arguments
	parse(args);

	// Split the object
	triangles_vector = createSplit(parser.read(input));

	// Check if the object was cut
	if ((triangles_vector->at(0) == 0) || (triangles_vector->at(1) == 0)) std::exit(DOESNT_INTERSECT);

	// Write the object to the files
	parser.write(triangles_vector->at(0), first_output);
	parser.write(triangles_vector->at(1), second_output);

	return 0;
}

vector<const Triangles*>* Split::createSplit(Triangles* triangles) {	
	Triangles *positive = new Triangles(),
		*negative = new Triangles(),
		*filling; 
	vector<const Triangles*>* split_figure = new vector<const Triangles*>();
	uint8_t side_counter = 0; 														// Counter of the "positive" triangles

	// Initialize the storage for the cut object
	split_figure->push_back(positive);
	split_figure->push_back(negative);

	for (Triangle triangle : *triangles) {
		for (Vertex vertex : triangle) {
			// Check the side
			if (checkSide(vertex)) ++side_counter;
		}
		
		// Save the "positive" triangle
		if (side_counter == 3) positive->push_back(triangle);

		// Save the "negative" triangle
		else if (side_counter == 0) negative->push_back(triangle);

		// Cut the triangle
		else cutTriangle(&triangle, positive, negative);

		// Zeroing the counter for the next triangle
		side_counter = 0;
	}

	// Fill the hole in the object
	filling = triangulate(slice);
	positive->insert(positive->end(), filling->begin(), filling->end());
	negative->insert(negative->end(), filling->begin(), filling->end());

	return split_figure;
}

void Split::cutTriangle(const Triangle* triangle, Triangles* positive, Triangles* negative) {
	Triangles cutted_polygon;
	Polygon polygon;
	Triangle small_triangle;	// Triangle, created by cuting a polygon
	vector<Vertex>* intersections = findIntersections(triangle),
		positive_vertexes = vector<Vertex>(),
		negative_vertexes = vector<Vertex>();

	// Find positive and negative vertexes
	for (Vertex vertex : *triangle) {
		if (checkSide(vertex)) positive_vertexes.push_back(vertex);
		else negative_vertexes.push_back(vertex);
	}

	// Save shared points
	small_triangle.insert(small_triangle.end(), intersections->begin(), intersections->end());
	polygon.insert(polygon.end(), intersections->begin(), intersections->end());

	delete intersections;

	// Make a cut based on the position of the triangle 
	if (positive_vertexes.size() == 1) {
		small_triangle.push_back(positive_vertexes[0]);
		positive->push_back(small_triangle);

		polygon.push_back(negative_vertexes[1]);
		polygon.push_back(negative_vertexes[0]);
		cutted_polygon = *(triangulate(polygon));
		negative->insert(negative->end(), cutted_polygon.begin(), cutted_polygon.end());
	}

	else if (negative_vertexes.size() == 1) {
		small_triangle.push_back(negative_vertexes[0]);
		negative->push_back(small_triangle);

		polygon.push_back(positive_vertexes[1]);
		polygon.push_back(positive_vertexes[0]);
		cutted_polygon = *(triangulate(polygon));
		positive->insert(positive->end(), cutted_polygon.begin(), cutted_polygon.end());
	}
}

bool Split::checkSide(Vertex vertex) {	
	for (uint8_t counter = 0; counter < 3; ++counter)
		//If the normal along the axis is not empty, but the position of the point along the axis is less than that of \
			the base point, then the point is on the "negative" side
		if ((normal[counter] != 0) && (origin[counter] >= (vertex.position)[counter]))
			return false;

	return true;
}

Triangles* Split::triangulate(Polygon polygon) {
	Triangles* triangles = new Triangles();
	Triangle triangle = Triangle();

	// Connect all the vertexes with the first one
	for (auto iterator = (polygon.begin() + 1); iterator != (polygon.end() - 1); ++iterator) {
		// Create a triangle
		triangle.push_back(polygon[0]);
		triangle.push_back(*(iterator));
		triangle.push_back(*(iterator + 1));

		// Save the triangle
		triangles->push_back(triangle);

		// Clear the triangle
		triangle.clear();
	}

	return triangles;
}

vector<Vertex>* Split::findIntersections(const Triangle* triangle) {
	vector<Vertex>* intersections = new vector<Vertex>(),
		positive_vertexes = vector<Vertex>(),
		negative_vertexes = vector<Vertex>(),
		* wrapper_vertexes;
	Vertex intersection, * wrapper_vertex;
	Coordinates side, distance, direction;
	
	double factor;

	// Find positive and negative points
	for (Vertex vertex : *triangle) {
		if (checkSide(vertex)) positive_vertexes.push_back(vertex);
		else negative_vertexes.push_back(vertex);
	}

	// Unify the loop for positive and negative vertexes
	if (positive_vertexes.size() == 1) {
		wrapper_vertex = &positive_vertexes[0];
		wrapper_vertexes = &negative_vertexes;
	}

	else {
		wrapper_vertex = &negative_vertexes[0];
		wrapper_vertexes = &positive_vertexes;
	}

	// Calculate the intersection and store it inside of the array
	for (Vertex vertex : *wrapper_vertexes) {
		side = wrapper_vertex->position - vertex.position;
		distance = origin - vertex.position;
		factor = (distance * normal) / (side * normal);
		direction = side * factor;

		intersection.position = direction + vertex.position;
		intersections->push_back(intersection);
	}

	// Add the intersection points of the triangle to the general array of intersection points
	slice.insert(slice.end(), intersections->begin(), intersections->end());

	return intersections;
}