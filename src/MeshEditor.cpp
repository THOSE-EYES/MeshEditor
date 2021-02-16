#include "Application.hpp"
#include "Cube.hpp"
#include "Sphere.hpp"
#include "Split.hpp"


#include <iostream>

int main(int argc, char* argv[]) {
	
	//Создаем обьект приложения, которым оперируем
	Application app;

	//Создаем обьект команды для выполнения (рисование сферы, куба и т.д.)
	Cube* cube = new Cube();
	Sphere* sphere = new Sphere();
	Split* split = new Split();

	//Регистрируем команды
	app.registerCommand(sphere);
	app.registerCommand(cube);
	app.registerCommand(split);

	//Выполняем команды, введенные с клавиатуры и возвращаем результат выполнения
	return app.execute(argc, argv);
}
