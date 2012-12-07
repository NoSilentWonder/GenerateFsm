/*
	Created 	Elinor Townsend 2011
	
	Name		Create Files
	Brief			Functions for creating the FSM header and source files using
					information from a GenerateFSM script.
*/

#include <stdio.h>
#include <stdlib.h>
#include "CreateFiles.h"
#include "Fsm.h"

/*
	Name		createHeader
	Syntax		createHeader(void)
	Brief			Opens/creates a file and writes the FSM header file to it
*/
void createHeader(void)
{
	FILE* outstream;
	char name[256];
	char filename[256];
	char headerguard[256];
	
	sprintf(name, "%s", getName());
	
	sprintf(filename, "%sFsm.h", name);	
	sprintf(headerguard, "%s_FSM_H", getNameUppercase());
	outstream = fopen(filename, "w");
	
	if (!outstream)
	{
		printf("Failed to open file for outputting FSM header.\n");
	}
	fprintf(outstream, "// Automatically generated using the GenerateFSM compiler.\n// Copyright Elinor Townsend 2011.\n\n");	
	fprintf(outstream, "#ifndef %s\n#define %s\n\n", headerguard, headerguard);

	// Write states enum to file
	fprintf(outstream, "// The %s FSM states.\n", name);
	fprintf(outstream, "enum %sState\n", name);
	fprintf(outstream, "{\n");
	printStates(outstream);
	fprintf(outstream, "};\n\n");

	// Write events enum to file
	fprintf(outstream, "// The %s FSM events.\n", name);
	fprintf(outstream, "enum %sEvent\n", name);
	fprintf(outstream, "{\n");
	printEvents(outstream);
	fprintf(outstream, "};\n\n");

	// Write FSM class to file
	fprintf(outstream, "// The %s FSM class.\n", name);
	fprintf(outstream, "class %sFsm\n", name);
	fprintf(outstream, "{\n");
	fprintf(outstream, "public:\n");
	fprintf(outstream, "\t%sFsm();\n", name);
	fprintf(outstream, "\t%sFsm(%sState _initialState);\n", name, name);
	fprintf(outstream, "\t~%sFsm();\n\n", name);
	fprintf(outstream, "\tvoid applyEvent(%sEvent _event);\n", name);
	fprintf(outstream, "\tvoid reset();\n");
	fprintf(outstream, "\t%sState getCurrentState();\n", name);
	fprintf(outstream, "\tvoid setCurrentState(%sState _state);\n", name);
	fprintf(outstream, "private:\n");
	fprintf(outstream, "\t%sState currentState;\n", name);
	fprintf(outstream, "\t%sState initialState;\n", name);	
	fprintf(outstream, "\t%sState lookUpTable[%d][%d];\n", name, statesCount(), eventsCount());
	fprintf(outstream, "};\n\n");	
	fprintf(outstream, "#endif /*%s*/\n", headerguard);
	fclose(outstream);
	
	printf("Header file created.\n");
}

/*
	Name		createSource
	Syntax		createSource(void)
	Brief			Opens/creates a file and writes the FSM source file to it
*/
void createSource(void)
{
	FILE* outstream;
	char name[256];
	char filename[256];
	
	sprintf(name, "%s", getName());
	
	sprintf(filename, "%sFsm.cpp", name);
	outstream = fopen(filename, "w");
	
	if (!outstream)
	{
		printf("Failed to open file for outputting FSM header.\n");
	}
	
	fprintf(outstream, "// Automatically generated using the GenerateFSM compiler.\n// Copyright Elinor Townsend 2011.\n\n");	
	
	fprintf(outstream, "#include \"%sFsm.h\"\n\n", name);

	// Write default constructor to file
	fprintf(outstream, "// %sFsm default constructor.\n", name);
	fprintf(outstream, "%sFsm::%sFsm()\n", name, name);
	fprintf(outstream, "{\n");
	fprintf(outstream, "\tinitialState = %s;\n", getInitialState());
	fprintf(outstream, "\tcurrentState = initialState;\n\n");
	fprintf(outstream, "\tfor (int i = 0; i < %d; ++i) {\n", statesCount());
	fprintf(outstream, "\t\tfor (int j = 0, j < %d; ++j) {\n", eventsCount());
	fprintf(outstream, "\t\t\tlookUpTable[i][j] = i;\n");
	fprintf(outstream, "\t\t}\n");
	fprintf(outstream, "\t}\n");
	printPopulateLookupTable(outstream);
	fprintf(outstream, "}\n\n");

	// Write constructor to file
	fprintf(outstream, "// %sFsm constructor - sets initial state.\n", name);
	fprintf(outstream, "%sFsm::%sFsm(%sState _initialState)\n", name, name, name);
	fprintf(outstream, "{\n");
	fprintf(outstream, "\tinitialState = _initialState;\n");
	fprintf(outstream, "\tcurrentState = initialState;\n\n");
	fprintf(outstream, "\tfor (int i = 0; i < %d; ++i) {\n", statesCount());
	fprintf(outstream, "\t\tfor (int j = 0, j < %d; ++j) {\n", eventsCount());
	fprintf(outstream, "\t\t\tlookUpTable[i][j] = i;\n");
	fprintf(outstream, "\t\t}\n");
	fprintf(outstream, "\t}\n");
	printPopulateLookupTable(outstream);
	fprintf(outstream, "}\n\n");
	
	// Write destructor to file
	fprintf(outstream, "// %sFsm destructor.\n", name);
	fprintf(outstream, "%sFsm::~%sFsm()\n", name, name);
	fprintf(outstream, "{\n");
	fprintf(outstream, "}\n\n");
	
	// Write applyEvent method to file
	fprintf(outstream, "// %sFsm applyEvent:\n", name);
	fprintf(outstream, "// Param - %sEvent _event - The event to be applied to the FSM.\n", name);
	fprintf(outstream, "// Description - Applies a event to the FSM and transitions to a new state if required.\n");
	fprintf(outstream, "void %sFsm::applyEvent(%sEvent _event)\n", name, name);
	fprintf(outstream, "{\n");
	fprintf(outstream, "\tcurrentState = lookUpTable[currentState][_event];\n");
	fprintf(outstream, "}\n\n");
	
	// Write reset method to file
	fprintf(outstream, "// %sFsm reset:\n", name);
	fprintf(outstream, "// Description - Resets the FSM to its initial state.\n");
	fprintf(outstream, "void %sFsm::reset()\n", name);
	fprintf(outstream, "{\n");
	fprintf(outstream, "\tcurrentState = initialState;\n");
	fprintf(outstream, "}\n\n");
	
	// Write getCurrentState method to file
	fprintf(outstream, "// %sFsm getCurrentState:\n", name);
	fprintf(outstream, "// Return - %sState - Returns the current state.\n", name);
	fprintf(outstream, "// Description - Returns the current state of the FSM.\n");
	fprintf(outstream, "%sState %sFsm::getCurrentState()\n", name, name);
	fprintf(outstream, "{\n");
	fprintf(outstream, "\treturn currentState;\n");
	fprintf(outstream, "}\n\n");
	
	// Write setCurrentState method to file
	fprintf(outstream, "// %sFsm setCurrentState:\n", name);
	fprintf(outstream, "// Param - %sState _state - State to set.\n", name);
	fprintf(outstream, "// Description - Sets the current state of the FSM.\n");
	fprintf(outstream, "void %sFsm::setCurrentState(%sState _state)\n", name, name);
	fprintf(outstream, "{\n");
	fprintf(outstream, "\tcurrentState = _state;\n");
	fprintf(outstream, "}\n\n");

	fclose(outstream);
	
	printf("Source file created.\n");
}

