/*
	Created 	Elinor Townsend 2011
	
	Name		FSM
	Brief			Struct declarations and function prototypes
*/

#ifndef _FSM_H_
#define _FSM_H_

#include <stdio.h>
#include <stdlib.h>

// Holds name data
struct fsmNameEntry
{
	char* name;
	char* uppercaseName;
};

// Holds state data
struct fsmStateEntry
{
   char* name;
   struct fsmStateEntry* next;
};

// Holds event data
struct fsmEventEntry
{
   char* name;
   struct fsmEventEntry* next;
};

// Holds transition data
struct transitionEntry
{
   char* from_state;
   char* event;
   char* to_state;
   struct transitionEntry* next;
};

// Typedefs for structs
typedef struct fsmNameEntry fsmName;
typedef struct fsmStateEntry fsmState;
typedef struct fsmEventEntry fsmEvent;
typedef struct transitionEntry fsmTransition;

// Function prototypes
int setName(char* name);
int createState(char* state);
int setInitialState(char* initial_state);
int createEvent(char* event);
int addTransition(char* from_state, char* event, char* to_state);
char* getName(void);
char* getNameUppercase(void);
void printStates(FILE* outstream);
void printEvents(FILE* outstream);
char* getInitialState(void);
int statesCount(void);
int eventsCount(void);
void printPopulateLookupTable(FILE* outstream);
void clearName(void);
void clearStates(void);
void clearEvents(void);
void clearTransitions(void);
void clearAll(void);

#endif // _FSM_H_