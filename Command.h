#pragma once

#include <string>
#include <map>

#include "STLParser.h"

#include <iostream>

using namespace std;

#define INCORR_LENGTH_ERR_CODE 1
#define FILES_ERR_CODE 2
#define LESS_ARGS_ERR_CODE 3

class Command
{
public:
	virtual ~Command() {}
	virtual const std::string getName() const = 0;
	virtual uint8_t execute(const std::map<std::string, std::string>& args) = 0;
};