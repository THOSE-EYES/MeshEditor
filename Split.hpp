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
 * Error codes and constants
 */
#define MIN_ARGS 5
#define DOESNT_INTERSECT 4
#define ZERO_NORMAL 1

using Polygon = vector<Vertex>;

/**
 * Cutting objects into two using coordinates from the input
 */
class Split : public Command  {
public:
	/**
	 * Constructor
	 */
	Split();
	
	/**
	 * Getting command's title
	 * @return title
	 */
	const string getName() const override;

	/**
	 * Split the object
	 * @param args arguments for the command
	 * @return error code or 0
	 */
	uint8_t execute(const map<string, string>& args) override;

protected:
	Polygon slice;									// Common vertexes of the shape and the object
	Coordinates origin, normal;						// Plane characteristics
	string input, first_output, second_output;		// Names of the files with data
	
	/**
	 * Parsing the arguments
	 * @param args the arguments
	 * @return error code or 0
	 */
	uint8_t parse(const map<string, string>&);	//Парсинг аргументов и их запись во внутренние структуры обьекта

	/**
	 * Split the object into two
	 * @param triangles the object to cut
	 * @return parts of the object
	 */
	vector<const Triangles*>* createSplit(Triangles*);	//Создание разреза фигуры

	/**
	 * Cut the triangle
	 * @param triangle the triangle to cut
	 * @param positive the "positive" triangles
	 * @param negative the "negative" triangles
	 */
	void cutTriangle(const Triangle*, Triangles*, Triangles*);	//Создание разреза треугольника

	/**
	 * Check in which side from the plane the vertex is
	 * @param vertex the vertex to check
	 * @return if the vertex is on the "positive" side
	 */
	bool checkSide(Vertex);	//Возвращает true, если вершина на "позитивной стороне"

	/**
	 * Polygon triangulation
	 * @param polygon the polygon to triangulate
	 * @return result of triangulation
	 */
	Triangles* triangulate(Polygon);	//Триангуляция полигона

	/**
	 * Find intersection points of the plane and the object
	 */
	vector<Vertex>* findIntersections(const Triangle*);	//Поиск точек пересечения треугольника и плоскости
};