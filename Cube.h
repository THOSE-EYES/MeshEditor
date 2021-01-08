#pragma once

#include "Command.h"


#define MIN_ARGS 3

//Наследуемый от класса "Команда" класс Куб для рисования фигуры
class Cube : public Command
{
public:
	const std::string getName() const override;
	uint8_t execute(const map<string, string>&) override;	//Выполнение команды рисования

private:
	int length;
	Coordinates origin;
	string filepath;

	uint8_t parse(const map<string, string>&);
	const Triangles* createTriangles();
};