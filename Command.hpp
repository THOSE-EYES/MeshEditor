/*
 * Copyright 2018 Illia Shvarov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include <string>
#include <map>
#include "STLParser.h"
#include <iostream>

using namespace std;

/**
 * Error codes
 */
#define INCORR_LENGTH_ERR_CODE 1
#define FILES_ERR_CODE 2
#define LESS_ARGS_ERR_CODE 3

/**
 * Abstract class dedicated to executing commands (shape creation)
 */
class Command {
public:
	/**
	 * Virtual destructor
	 */
	virtual ~Command() {}

	/**
	 * Getting the name of the command
	 */
	virtual const std::string getName() const = 0;

	/**
	 * Execute the command
	 * @param args arguments for the command
	 * @return error code or 0
	 */
	virtual uint8_t execute(const std::map<std::string, std::string>& args) = 0;
};