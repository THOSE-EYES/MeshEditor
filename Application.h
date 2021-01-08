#pragma once

#include <vector>
#include <algorithm>

#include "Command.h"


class Application {
public:
	void registerCommand(Command*);
	int execute(int, char**);

private:
	std::vector<Command*> commands;
};
