#include "Split.h"

Split::Split() {
	//Инициализировать поля класса
	slice = Polygon();
}

const string Split::getName() const {
	//Задать имя команды
	string name = "Split";

	//Вернуть имя команды
	return name;
}

uint8_t Split::parse(const map<string, string>& args) {	
	//Проверить количество аргументов
	if (args.size() < MIN_ARGS) std::exit(LESS_ARGS_ERR_CODE);

	string key, value;	//переменные для хранения ключа и значения (парсинг аргументов)
	size_t position;	//Позиция (временная переменная, хранящая позицию искомого символа в строке)
	vector<size_t> positions;	//Массив позиций

	//Распарсить и записать в переменные значения аргументов
	for (pair<string, string> pair : args) {
		key = pair.first;
		value = pair.second;

		//Если рассматриваем нормаль
		if (key == "direction") {
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

			//Записать прочитанные значения нормали
			normal.x = stod(value.substr(1, positions[0]));
			normal.y = stod(value.substr(positions[0] + 1, positions[1]));
			normal.z = stod(value.substr(positions[1] + 1, value.size() - 1));

			//Если нормаль имеет длину 0
			if ((normal.x == 0) && (normal.y == 0) && (normal.z == 0)) std::exit(ZERO_NORMAL);
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
		else if (key == "input") input = value;

		//Записать имя первого файла вывода
		else if (key == "output1") first_output = value;

		//Записать имя второго файла вывода
		else if (key == "output2") second_output = value;
	}

	return 0;
}

uint8_t Split::execute(const map<string, string>& args) {
	//Распарсить аргументы для работы с ними
	parse(args);

	//Создать треугольники и записать их в массив, записать фигуру в файл
	STLParser parser = STLParser();

	vector<const Triangles*>* triangles_vector = createSplit(parser.read(input));

	if ((triangles_vector->at(0) == 0) || (triangles_vector->at(1) == 0)) std::exit(DOESNT_INTERSECT);

	parser.write(triangles_vector->at(0), first_output);
	parser.write(triangles_vector->at(1), second_output);

	return 0;
}

vector<const Triangles*>* Split::createSplit(Triangles* triangles) {	
	//Обьявить и инициализироать локальные переменные
	Triangles *positive = new Triangles(), \
		*negative = new Triangles(), \
		*filling; //Части исходной фигуры

	vector<const Triangles*>* split_figure = new vector<const Triangles*>();	//Разрезанная фигура

	//Счетчик вершин по одну сторону плоскости
	uint8_t side_counter = 0;

	//Добавить части фигуры в общий массив
	split_figure->push_back(positive);
	split_figure->push_back(negative);

	//Для каждого треугольника фигуры
	for (Triangle triangle : *triangles) {
		//Для каждой вершины треугольника
		for (Vertex vertex : triangle) {
			//Проверить положение относительно плоскости разреза
			if (checkSide(vertex)) ++side_counter;
		}
		
		if (side_counter == 3) positive->push_back(triangle);	//Записать треугольник в "позитивные"
		else if (side_counter == 0) negative->push_back(triangle);	//Записать треугольник в "негативные"
		else if ((side_counter == 2) || (side_counter == 1)){
			cutTriangle(&triangle, positive, negative);	//Если плоскость пересекает треуольник, разрезать его
		}

		//Обнулить значение счетчика сторон по одну сторону
		side_counter = 0;
	}

	//Заполнить дыру
	filling = triangulate(slice);

	//Заполнение является частью обоих фигур
	positive->insert(positive->end(), filling->begin(), filling->end());
	negative->insert(negative->end(), filling->begin(), filling->end());

	//Вернуть массив разрезанных фигур
	return split_figure;
}

void Split::cutTriangle(const Triangle* triangle, Triangles* positive, Triangles* negative) {
	//Инициализировать локальные переменные
	Triangles cutted_polygon;	//Разрезанный получившийся полигон
	Polygon polygon;	//Полигон, образованный в результате разреза исходного треугольника
	Triangle small_triangle;	//Треугольник, образованный в результате разреза исходного треугольника

	vector<Vertex>* intersections = findIntersections(triangle), \
		positive_vertexes = vector<Vertex>(), \
		negative_vertexes = vector<Vertex>();

	//Найти позитивные и негативные точки
	for (Vertex vertex : *triangle) {
		if (checkSide(vertex)) positive_vertexes.push_back(vertex);
		else negative_vertexes.push_back(vertex);
	}

	//Присвоить фигурам общие точки (точки пересечения)
	small_triangle.insert(small_triangle.end(), intersections->begin(), intersections->end());
	polygon.insert(polygon.end(), intersections->begin(), intersections->end());

	delete intersections;

	//Выполнить действия, в зависимости от расположения фигур
	if (positive_vertexes.size() == 1) {
		//Присвоить треугольнику одну точку (всегда с другой стороны разреза) и поместить его в правильный массив
		small_triangle.push_back(positive_vertexes[0]);
		positive->push_back(small_triangle);

		//Присвоить полигону оставшиеся от треугольнка точки, триангулировать его и поместить его в правильный массив
		polygon.push_back(negative_vertexes[1]);
		polygon.push_back(negative_vertexes[0]);
		cutted_polygon = *(triangulate(polygon));
		negative->insert(negative->end(), cutted_polygon.begin(), cutted_polygon.end());
	}

	else if (negative_vertexes.size() == 1) {
		//Присвоить треугольнику одну точку (всегда с другой стороны разреза) и поместить его в правильный массив
		small_triangle.push_back(negative_vertexes[0]);
		negative->push_back(small_triangle);

		//Присвоить полигону оставшиеся от треугольнка точки, триангулировать его и поместить его в правильный массив
		polygon.push_back(positive_vertexes[1]);
		polygon.push_back(positive_vertexes[0]);
		cutted_polygon = *(triangulate(polygon));
		positive->insert(positive->end(), cutted_polygon.begin(), cutted_polygon.end());
	}
}

bool Split::checkSide(Vertex vertex) {	
	//Проверить каждую вершину треугольника
	for (uint8_t counter = 0; counter < 3; ++counter) \
		//Если нормаль по оси не пуста, но позиция точки по оси меньше, чем у точки основания, то тока с "негативной" стороны
		if ((normal[counter] != 0) && (origin[counter] >= (vertex.position)[counter])) \
			return false;

	//Если работа функции не завершилась до этого, то точка на "позитивной" стороне
	return true;
}

Triangles* Split::triangulate(Polygon polygon) {
	//Инициализировать локальные переменные
	Triangles* triangles = new Triangles();
	Triangle triangle = Triangle();

	//Соеденить все точки с первой, создавая треугольники
	for (auto iterator = (polygon.begin() + 1); iterator != (polygon.end() - 1); ++iterator) {
		//Поместить три точки в треугольник 
		triangle.push_back(polygon[0]);
		triangle.push_back(*(iterator));
		triangle.push_back(*(iterator + 1));

		//Запимать треугольник в массив треугольников
		triangles->push_back(triangle);

		//Очистить треугольник
		triangle.clear();
	}
	//Вернуть массив созданных треугольников
	return triangles;
}

vector<Vertex>* Split::findIntersections(const Triangle* triangle) {
	//Инициализировать локальные переменные
	vector<Vertex>* intersections = new vector<Vertex>(), \
		positive_vertexes = vector<Vertex>(), \
		negative_vertexes = vector<Vertex>(), \
		* wrapper_vertexes;

	Vertex intersection, * wrapper_vertex;

	Coordinates side, distance, direction;
	
	double factor;

	//Найти позитивные и негативные точки
	for (Vertex vertex : *triangle) {
		if (checkSide(vertex)) positive_vertexes.push_back(vertex);
		else negative_vertexes.push_back(vertex);
	}

	//Унифицировать цикл, используя указатели-обертки
	if (positive_vertexes.size() == 1) {
		wrapper_vertex = &positive_vertexes[0];
		wrapper_vertexes = &negative_vertexes;
	}

	else {
		wrapper_vertex = &negative_vertexes[0];
		wrapper_vertexes = &positive_vertexes;
	}

	//Вычислить точки пересечения и записать их в массив
	for (Vertex vertex : *wrapper_vertexes) {
		side = wrapper_vertex->position - vertex.position;
		distance = origin - vertex.position;
		factor = (distance * normal) / (side * normal);
		direction = side * factor;

		intersection.position = direction + vertex.position;
		intersections->push_back(intersection);
	}

	//Добавить точки пересечения треугольника в общий массив точек пересечения
	slice.insert(slice.end(), intersections->begin(), intersections->end());

	return intersections;
}