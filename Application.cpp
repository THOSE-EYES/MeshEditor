#include "Application.h"

void Application::registerCommand(Command* command) {
	//Добавляем команду в список команд
	commands.push_back(command);
}

int Application::execute(int argc, char** argv) {
	int result = -1;	//Переменная, хранящая результат выполнения команды

	string argument;	//Переменная, хранящая конвертированные в тип "строка" значения аргументов
	map<string, string> arguments_map;	//Сбор аргументов в типе "map"

	//Каждый аргумент (кроме первого (сам исполняемый файл) и второго (название фигуры))
	for (char** argument_ptr = &argv[2]; argument_ptr != &argv[argc]; ++argument_ptr) {
		//Конвертировать в тип "строка"
		argument = string(*argument_ptr);

		//Заменить символы слэша в путях Windows
		//std::replace(итератор начала строки, итератор конца строки, заменяемый символ, замена) - заменяет все найденные символы в строке на указанные
		replace(argument.begin(), argument.end(), "\\", "/");

		//Вставить в сбор аргументов строку, но разбитую разделителем "="
		//строка.substr(начало, конец) - возвращает подстроку в заданном интервале
		//строка.find(элемент) - возвращает индекс элемента в строке, если найден
		arguments_map.insert(make_pair(argument.substr(0, argument.find("=")), argument.substr((argument.find("=")) + 1, argument.size())));
	}

	//Перебрать каждую команду
	for (Command* command : commands) {
		//Если имя команды совпадает с желаемым
		if (command->getName() == argv[1]) {
			//Выполнить данную команду, записать результат ее выполнения
			result = command->execute(arguments_map);
		}
	}

	//Возвращаем результат выполнения программы
	return result;
}