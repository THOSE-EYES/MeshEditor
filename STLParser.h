#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream> 
#include <iterator>
#include <tuple>

using namespace std;

#define NOT_OPENED 2

//Структура, хранящая координаты
struct Coordinates { 
	double x, y, z;

	double& operator[] (int);
	Coordinates& operator-(Coordinates&);
	Coordinates& operator+(Coordinates&);
	double operator*(Coordinates&);
	Coordinates& operator*(double);
};

//Структура, хранящая вершину треугольника и его нормаль
struct Vertex {
	Coordinates position;
	Coordinates normal;
};

//Аналог #define
using Triangle = vector<Vertex>;	//Вектор вершин (треугольник)
using Triangles = vector<vector<Vertex>>;	//Вектор треугольников

class STLParser {
public:
	Triangles* read(const string);	//Чтение данных фигуры из файла
	void write(const Triangles*, const string);	//Запись данных фигуры в файл
};