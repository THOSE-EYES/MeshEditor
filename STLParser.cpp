#include "STLParser.h"

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
			//TODO
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
	Triangle  triangle;	//Треугольник
	Vertex vertex;	//Вершина
	
	string line;	//Переменная, хранящая текущую прочитанную строку
	istringstream iss;	//Строковый поток для разбития строки на токены
	vector<string> tokens;	//Временная переменная, хранящая разбитую на токены прочитанную строку

	fstream in;	//Файловый поток (для чтения из файла)
	in.open(filename, fstream::in);

	//Если файл некорректен
	if (!(in.is_open())) std::exit(NOT_OPENED) ;

	while (!in.eof() && in.is_open()) {
		//Получаем строку из файла
		getline(in, line);

		//Пропускаем итерацию, если строка пуста
		if (line.size() == 0) continue;

		//Разбиваем строку на части (токены)
		iss = istringstream(line);
		tokens = vector<string>((istream_iterator<string>(iss)), istream_iterator<string>());

		//Парсим файл, заполняем структуры
		if (tokens[0] == "facet") {
			vertex.normal = { stod(tokens[2]), stod(tokens[3]), stod(tokens[4]) };
		}

		//Если найдена вершина, записываем ее в массив вершин
		else if (tokens[0] == "vertex") {
			vertex.position = { stod(tokens[1]), stod(tokens[2]), stod(tokens[3]) };

			triangle.push_back(vertex);
		}

		//Если заканчивается описание треугольника, записываем его в вектор треугольников
		else if(tokens[0] == "endfacet") {
			triangles->push_back(triangle);

			//Очищаем текущий треугольник
			triangle.clear();
		}
	}

	in.close();

	return triangles;
}

void STLParser::write(const Triangles* triangles, const string filename) {
	fstream out;	//Файловый поток (для чтения из файла)
	out.open(filename, fstream::out);	//Открываем файл для записи

	//Записываем заголовок файла
	out << "solid test" << endl;

	//Перебираем каждый треугольник в векторе треугольников
	for (Triangle triangle : *triangles) {
		out << "facet normal 0 0 0" << endl;
		out << "outer loop" << endl;
		
		//Перебираем каждую вершину текущего треугольника
		for (Vertex vertex : triangle) {
			//Записываем координаты каждой вершины треугольника
			out << "vertex " << vertex.position.x << " " << vertex.position.y << " " << vertex.position.z << endl;
		}

		out << "endloop" << endl;
		out << "endfacet" << endl << endl;
	}

	//Записываем признак конца фигуры
	out << "endsolid test" << endl;

	delete triangles;
}