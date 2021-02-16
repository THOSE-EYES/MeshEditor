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

#include "Application.hpp"

void Application::registerCommand(Command* command) {
	// Adding the command to the list
	commands.push_back(command);
}

int Application::execute(int argc, char** argv) {
	int result = -1;
	string argument;							// Temporary variable for arguments
	map<string, string> arguments_map;

	// Everything except the name of the executable and the command is an argument
	for (char** argument_ptr = &argv[2]; argument_ptr != &argv[argc]; ++argument_ptr) {
		argument = string(*argument_ptr);

		// Workaround for Windows
		//replace(argument.begin(), argument.end(), "\\", "/");

		// Save the arguments (key=value format) into the map
		arguments_map.insert(make_pair(argument.substr(0, argument.find("=")), argument.substr((argument.find("=")) + 1, argument.size())));
	}

	for (Command* command : commands) {
		if (command->getName() == argv[1]) {
			result = command->execute(arguments_map);
		}
	}
	
	return result;
}