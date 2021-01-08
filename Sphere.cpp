#include "Sphere.h"

const string Sphere::getName() const {
	string name = "Sphere";

	return name;
}

uint8_t Sphere::execute(const map<string, string>& args) {	//Сделать методом класса Command
	//Распарсить аргументы для работы с ними
	int result = parse(args);

	//Создать треугольники и записать их в массив, записать фигуру в файл
	STLParser parser = STLParser();
	const Triangles* triangles = createTriangles();

	parser.write(triangles, filepath);

	return result;
} 

uint8_t Sphere::parse(const map<string, string>& args) {
	//Проверить количество аргументов
	if (args.size() < MIN_ARGS) std::exit(LESS_ARGS_ERR_CODE);

	string key, value;	//
	size_t position;	//
	vector<size_t> positions;	//

	//Распарсить и записать в переменные значения аргументов
	for (pair<string, string> pair : args) {
		key = pair.first;
		value = pair.second;

		//Если рассматриваем длину стороны куба
		if (key == "R") {
			radius = stoi(value);

			//Вернуть заданное значение, если длина меньше единицы
			if (radius <= 0) return INCORR_LENGTH_ERR_CODE;
		}

		//Если рассматриваем точку отсчета
		else if (key == "origin") {
			//Находим положение первой разделяющей запятой
			position = value.find(",");

			//Вектор позиций запятых для разбивки
			positions = vector<size_t>();

			//Разбить строку координат
			while (position != string::npos) {
				//Добавить позицию запятой
				positions.push_back(position);

				//Получить следующую позицию запятой
				position = value.find(",", position + string(",").size());
			}

			//Заполнить структуру, хранящую позицию точки отсчета
			origin.x = stod(value.substr(1, positions[0]));
			origin.y = stod(value.substr(positions[0] + 1, positions[1]));
			origin.z = stod(value.substr(positions[1] + 1, value.size() - 1));
		}

		//Записать имя файла
		else if (key == "slices") {
			slices = stoi(value);
		}

		else if (key == "rings") {
			rings = stoi(value);
		}

		//Записать имя файла
		else if (key == "filepath") {
			filepath = value;
		}
	}
}

const Triangles* Sphere::createTriangles() {
	//Создать массив (вектор) треугольников для записи в файл
	Triangles* triangles = new Triangles();

	//Создать временные переменные для хранения промежуточных результатов вычислений
	Triangle triangle;
	
	//Шаг угла (определяется количеством разрезов)
	double phi_step = (360.0 / slices);
	double theta_step = (180.0 / rings);

	for (double phi = 0; phi < 360.0; phi += phi_step) {
		for (double theta = 0; theta < 180.0; theta += theta_step) {
			//Записать в треугольник три вершины 
			triangle.push_back(createVertex(phi, theta));	//Первая вершина
			triangle.push_back(createVertex(phi + phi_step, theta));	//Следующая вершина по горизонтали
			triangle.push_back(createVertex(phi, theta + theta_step));	//Следующая вершина по вертикали

			triangles->push_back(triangle);

			//Поменять первую вершину на следующую по диагонали (создаем развернутый треугольник для вормирования полигона)
			triangle[0] = createVertex(phi + phi_step, theta + theta_step);

			triangles->push_back(triangle);

			//Очистить массив вершин для нового использования
			triangle.clear();
		}
	}

	//Вернуть заполненный массив (вектор) треугольников
	return triangles;
}

Vertex Sphere::createVertex(double phi, double theta) {
	//Создать вершину
	Vertex vertex = Vertex();

	//Конвертировать значения углов в радианы
	phi = phi * PI / 180;
	theta = theta * PI / 180;

	//Вычислить координаты вершины
	vertex.position.x = origin.x + (cos(phi) * radius * sin(theta));
	vertex.position.y = origin.y + (sin(phi) * radius * sin(theta));
	vertex.position.z = origin.z + (cos(theta) * radius);

	//Вернуть вершину
	return vertex;
}