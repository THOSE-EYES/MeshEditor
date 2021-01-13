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

#include <vector>
#include <string>
#include <fstream>
#include <sstream> 
#include <iterator>

using namespace std;

/** 
 * Error codes
 */
#define NOT_OPENED 2

using Triangle = vector<Vertex>;			// Vector of vertexes
using Triangles = vector<vector<Vertex>>;	// Vector of triangles

/**
 * 
 */
struct Coordinates { 
	double x, y, z;

	/**
	 * Get a coordinate
	 * @param index index of the coordinate to get
	 * @return a coordinate
	 */
	double& operator[] (int);

	/**
	 * Subtract coordinates
	 * @param subtrahend
	 * @return result
	 */
	Coordinates& operator-(Coordinates&);

	/**
	 * Sum the coordinates
	 * @param addition
	 * @return result
	 */
	Coordinates& operator+(Coordinates&);

	/**
	 * Multiply the coordinates
	 * @param multiplier
	 * @return result
	 */
	double operator*(Coordinates&);

	/**
	 * Multiply the coordinates
	 * @param multiplier
	 * @return result
	 */
	Coordinates& operator*(double);
};

/**
 * Storing positions and normals of vertexes
 */
struct Vertex {
	Coordinates position;
	Coordinates normal;
};

/**
 * Class for file I/O
 */
class STLParser {
public:
	/**
 	* Reading from *.stl files
	* @param filename
	* @return a 3D object read from the file
 	*/
	Triangles* read(const string);

	/**
	 * Writing the object to a file
	 * @param triangles array of triangles to store into the file
	 * @param filename writing the object to this file
	 */
	void write(const Triangles*, const string);	
};