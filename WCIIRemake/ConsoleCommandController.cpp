#include "pch.h"
#include "ConsoleCommandController.h"


extern Console* defaultConsole;
extern Controller* gameController;
extern ThreadDescriptor* gameThreads;

ConsoleCommandController::ConsoleCommandController(Console* ioconsole, Controller* mainController) {
	if (ioconsole != NULL) {
		this->console = ioconsole;
	}
	else {
		this->console = defaultConsole;
	}

	if (mainController != NULL) {
		this->mainController = mainController;
	}
	else {
		this-> mainController = gameController;
	}

	this->CommandHandlerRunning = false;

	ConsoleCommandControllerTHREAD* ConComConTHRD = new ConsoleCommandControllerTHREAD(this);

	if (ConComConTHRD) {
		this->ConComConTHRDescriptor  = ConComConTHRD->getDescriptor();
	}
	else {
		cout << "Error allocating memory" << endl;
	}
	ConComConTHRD->startThread();
}


ConsoleCommandController::~ConsoleCommandController() {
}

bool ConsoleCommandController::readCondition(char character) {
	if ((character != ' ') && character != 0) {
		return true;
	}
	return false;
}

string ConsoleCommandController::commandType(char character) {
	if (character == '-') {
		return "flag";
	}
	if (isalpha(character)) {
		return "command";
	}
	if (isdigit(character)) {
		return "number";
	}
	return "unknown";
}

pair <string, string> ConsoleCommandController::nextToken(string command) {
	bool readBegin = true;
	pair <string, string> temp;
	while (!readCondition(command[ParserPosition])) {
		if (command[ParserPosition] == 0) {
			temp.first = "\0";
			temp.second = "end";
			return temp;
		}
		ParserPosition++;
	}


	while (readCondition(command[ParserPosition])) {
		temp.first += command[ParserPosition];
		if (readBegin) {
			temp.second = commandType(temp.first[0]);
		}
		readBegin = false;
		ParserPosition++;
	}
	return temp;
}


void ConsoleCommandController::initParser() {
	ParserPosition = 0;
}

Command_c ConsoleCommandController::parseCommand(string command)
{
	initParser();
	if (command.length() == 0) {
		return Command_c("empty");
	}
	pair <string, string> temp;
	Command_c command_c;
	temp = nextToken(command);
	for (int i = 0; temp.second != "end"; i++) {
		command_c.args.push_back(temp);
		temp = nextToken(command);
	}
	return command_c;
}

Command_c ConsoleCommandController::getCommand() {
	string command;
	command = console->getLine();
	return parseCommand(command);
}

void ConsoleCommandController::throwCommand(Command_c command) {
	mainController->addEventToQueue(command);
}

void ConsoleCommandController::operateEvent(Command_c command) {
	if (command == "exitgame") {
		exitGame(command);
	}
}

//CONSOLE COMMAND CONTROLLER COMMANDS(EVENTS)

bool ConsoleCommandController::exitGame(Command_c command) {
	if (command.args.size() == 1) {
		return gameThreads->stopThread(ConComConTHRDescriptor);
	}
	return false;
}


