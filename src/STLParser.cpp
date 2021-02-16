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

#include "STLParser.hpp"

double& Coordinates::operator[] (int index) {
	switch (index) {
		case 0 :
			return x;
			break;

		case 1:
			return y;
			break;

		case 2:
			return z;
			break;

		default:
			// Throw an exception 

			break;
	}
}

Coordinates& Coordinates::operator-(Coordinates& subtrahend) {
	Coordinates result = { (x - subtrahend.x), (y - subtrahend.y), (z - subtrahend.z) };

	return result;
}

Coordinates& Coordinates::operator+(Coordinates& addition) {
	Coordinates result = { (x + addition.x), (y + addition.y), (z + addition.z) };

	return result;
}

double Coordinates::operator*(Coordinates& multiplier) {
	double result = (x * multiplier.x) + (y * multiplier.y) + (z * multiplier.z);

	return result;
}

Coordinates& Coordinates::operator*(double multiplier) {
	Coordinates result = {(x * multiplier) , (y * multiplier), (z * multiplier)};

	return result;
}

Triangles* STLParser::read(const string filename) {
	Triangles* triangles = new Triangles();
	Triangle  triangle;								// Temporary triangle
	Vertex vertex;									// Temporary 
	string line;									// Current line in the file
	istringstream iss;								// Stream used to split the line
	vector<string> tokens;							// Temporary variable for tokens
	fstream in;										// Stream of the current file

	// Open the file
	in.open(filename, fstream::in);

	// Quit if the file was not opened
	if (!(in.is_open())) std::exit(NOT_OPENED) ;

	// Read the whole file
	while (!in.eof()) {
		// Getting a line
		getline(in, line);

		// Don't do anything if the line is empty
		if (line.size() == 0) continue;

		// Split the line
		iss = istringstream(line);
		tokens = vector<string>((istream_iterator<string>(iss)), istream_iterator<string>());

		// Saving triangle's normal
		if (tokens[0] == "facet") {
			vertex.normal = { stod(tokens[2]), stod(tokens[3]), stod(tokens[4]) };
		}

		// Saving triangle's vertex
		else if (tokens[0] == "vertex") {
			vertex.position = { stod(tokens[1]), stod(tokens[2]), stod(tokens[3]) };

			triangle.push_back(vertex);
		}

		// Save the triangle if the description is ended
		else if(tokens[0] == "endfacet") {
			triangles->push_back(triangle);

			// Clear the temporary triangle
			triangle.clear();
		}
	}

	// Close the file after reading
	in.close();

	return triangles;
}

void STLParser::write(const Triangles* triangles, const string filename) {
	fstream out;	// Output file stream

	// Open the file
	out.open(filename, fstream::out);

	// Writing a header
	out << "solid test" << endl;

	// Save every triangle
	for (Triangle triangle : *triangles) {
		// Don't use normals
		out << "facet normal 0 0 0" << endl;
		out << "outer loop" << endl;
		
		// Saving vertexes of the triangle
		for (Vertex vertex : triangle) {
			out << "vertex " << vertex.position.x << " " << vertex.position.y << " " << vertex.position.z << endl;
		}

		// End triangle's section
		out << "endloop" << endl;
		out << "endfacet" << endl << endl;
	}

	// End the operation
	out << "endsolid test" << endl;
	out.close();

	delete triangles;
}