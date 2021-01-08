#pragma once

#include "Command.h"

#define MIN_ARGS 5
#define DOESNT_INTERSECT 4
#define ZERO_NORMAL 1

using Polygon = vector<Vertex>;

class Split : public Command  {
public:
	Split();
	
	const string getName() const override;
	uint8_t execute(const map<string, string>& args) override;

private:
	Polygon slice;	//Общие точки плоскости и фигуры
	Coordinates origin, normal;	//Характеристика плоскости
	string input, first_output, second_output;	//Файлы для работы
	
	uint8_t parse(const map<string, string>&);	//Парсинг аргументов и их запись во внутренние структуры обьекта
	vector<const Triangles*>* createSplit(Triangles*);	//Создание разреза фигуры
	void cutTriangle(const Triangle*, Triangles*, Triangles*);	//Создание разреза треугольника
	bool checkSide(Vertex);	//Возвращает true, если вершина на "позитивной стороне"
	Triangles* triangulate(Polygon);	//Триангуляция полигона
	vector<Vertex>* findIntersections(const Triangle*);	//Поиск точек пересечения треугольника и плоскости
};