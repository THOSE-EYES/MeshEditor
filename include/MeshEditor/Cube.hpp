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

#include "Command.hpp"

/**
 * Constants
 */
#define MIN_ARGS 3

/**
 * A command to create a cube in STL format
 */
class Cube : public Command {
public:
	/**
	 * Getting command's title
	 * @return title
	 */
	const std::string getName() const override;

	/**
	 * Start execution
	 * @param args arguments for the command
	 * @return error code or 0
	 */
	uint8_t execute(const map<string, string>&) override;
protected:
	int length;				// Length of a side
	Coordinates origin;		// Origin of the cube
	string filepath;		// File to save the cube

	/**
	 * Parsing the arguments
	 * @param args the arguments
	 * @return error code or 0
	 */
	uint8_t parse(const map<string, string>&);

	/**
	 * Create a shape of a cube out of vertexes
	 * @return a cube as an array of triangles
	 */
	const Triangles* createTriangles();
};