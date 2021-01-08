#include "Cube.h"

const string Cube::getName() const{
	string name = "Cube";

	return name;
}

uint8_t Cube::execute(const map<string, string>& args) {
	//Распарсить аргументы для работы с ними
	parse(args);

	//Создать треугольники и записать их в массив, записать фигуру в файл
	STLParser parser = STLParser();
	const Triangles* triangles = createTriangles();

	parser.write(triangles, filepath);

	return 0;
}

uint8_t Cube::parse(const map<string, string>& args) {
	//Проверить количество аргументов
	if (args.size() < MIN_ARGS) {
		return LESS_ARGS_ERR_CODE;
	}

	string key, value;	//
	size_t position;	//
	vector<size_t> positions;	//

	//Распарсить и записать в переменные значения аргументов
	for (pair<string, string> pair : args) {
		key = pair.first;
		value = pair.second;

		//Если рассматриваем длину стороны куба
		if (key == "L") {
			length = stoi(value);
			
			//Вернуть заданное значение, если длина меньше единицы
			if (length <= 0) std::exit(INCORR_LENGTH_ERR_CODE);
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
		else if (key == "filepath") {
			filepath = value;
		}
	}
}

const Triangles* Cube::createTriangles() {
	//Создать массив (вектор) треугольников для записи в файл
	Triangles* triangles = new Triangles();

	//Создать временные переменные для хранения промежуточных результатов вычислений
	Vertex origin_vertex, f_common, s_common;
	Triangle triangle;

	//Просчитать точки куба и создать на их основе треугольники (половина куба)
	for (int counter = 0, second_counter = 1; counter < 3 && second_counter < 4; ++counter, ++second_counter) {
		//second counter служит для перемещения общих точек (одна смещена по оси X, другая - по Y, одна по Y и так далее)
		if (second_counter == 3) {
			second_counter = 0;
		}
		
		//Очистить временные файлы вершины
		f_common.position = { origin.x, origin.y, origin.z };
		s_common.position = { origin.x, origin.y, origin.z };
		origin_vertex.position = origin;

		//Сместить две диагональные общие точки грани
		f_common.position[counter] += length;
		s_common.position[second_counter] += length;

		//Записать точки в треугольник
		triangle.push_back(f_common);
		triangle.push_back(s_common);
		triangle.push_back(origin_vertex);

		//Записать треугольник в вектор треугольников
		triangles->push_back(triangle);

		//Сместить осташуюся вершину (которая не является общей)
		triangle[2].position[counter] += length;
		triangle[2].position[second_counter] += length;

		//Записать новый треугольник в вектор
		triangles->push_back(triangle);

		//Очистить временный треугольник
		triangle.clear();
	}
	
	//Преместить точку отсчета по диагонали (для отрисовки второй половины куба)
	origin.x += length;
	origin.y += length;
	origin.z += length;

	//Просчитать точки куба и создать на их основе треугольники (вторая половина куба)
	for (int counter = 0, second_counter = 1; counter < 3 && second_counter < 4; ++counter, ++second_counter) {
		if (second_counter == 3) {
			second_counter = 0;
		}

		//Очистить временные файлы вершины
		f_common.position = { origin.x, origin.y, origin.z };
		s_common.position = { origin.x, origin.y, origin.z };
		origin_vertex.position = origin;

		//Сместить две диагональные общие точки грани
		f_common.position[counter] -= length;
		s_common.position[second_counter] -= length;

		//Записать точки в треугольник
		triangle.push_back(f_common);
		triangle.push_back(s_common);
		triangle.push_back(origin_vertex);

		//Записать треугольник в вектор треугольников
		triangles->push_back(triangle);

		//Сместить осташуюся вершину (которая не является общей)
		triangle[2].position[counter] -= length;
		triangle[2].position[second_counter] -= length;

		//Записать новый треугольник в вектор
		triangles->push_back(triangle);

		//Очистить временный треугольник
		triangle.clear();
	}

	//Вернуть заполненный массив (вектор) треугольников
	return triangles;
}