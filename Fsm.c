/*
	Created 	Elinor Townsend 2011
	
	Name		FSM
	Brief		Functions for storing data from scripts provided
*/

#include <ctype.h>
#include "Fsm.h"
#include "SymbolTable.h"

// Pointers for singly linked lists used to store data
fsmName* fsm_name = NULL;
fsmState* fsm_state_list = NULL;
fsmEvent* fsm_event_list = NULL;
fsmTransition* fsm_transition_list = NULL;

// Variables
char* fsm_initial_state = "";
int num_states = 0;
int num_events = 0;

/*
	Name		setName
	Syntax		setName(char* name)
	Param		char* name - The name to set
	Return		int - 0 if name is not set, 1 if it is
	Brief		Sets the name for the Finite State Machine
	Details		This name will be prefixed to the generated file names, 
				the FSM class, the States enum and the Events 
				enum produced
*/
int setName(char* name)
{
	if (addSymbol(name, NAME))
	{
		fsmName* newName;

		// Set the new name
		newName = (fsmName*) malloc (sizeof(fsmName));
		newName->name = name;
/*		
		// Commented out as this was changing the name to all caps. 
		// Need to rewrite.
		while(*name != '\0')
		{
			*name = toupper((unsigned char)*name);
			++name;
		}
*/
		newName->uppercaseName = name;
		
		clearName();
		fsm_name = newName;
		
		return 1;
	}
	return 0;
}

/*
	Name		createState
	Syntax		createState(char* state)
	Param		char* state - The name of the state to be created
	Return		int - 0 if state is not created, 1 if it is
	Brief		Creates a state 
*/
int createState(char* state)
{
	if (addSymbol(state, STATE))
	{
		fsmState* newState;

		// Add new state to the state list
		newState = (fsmState*) malloc (sizeof(fsmState));
		newState->name = state;
		newState->next = fsm_state_list;
		fsm_state_list = newState;	
		num_states++;
		return 1;
	}
	return 0;
}

/*
	Name		setInitialState
	Syntax		setInitialState(char* initial_state)
	Param		char* initial_state - The state to be set as the intial state
	Return		int - 0 if state is not set as initial state, 1 if it is
	Brief		Sets initial_state as the initial state for the FSM. 
	Details		This will overwrite the existing initial state. If no initial state 
				is set, the last declared state will be used.
*/
int setInitialState(char* initial_state)
{
	fsmState* state = fsm_state_list;
	// Compare name with the name of each item in the symbol table
	while (state != NULL && strcmp(initial_state, state->name)) 
	{
		state = state->next;
	}
	if (state)
	{
		fsm_initial_state = state->name;
		return 1;
	}
	else
		return 0;
}

/*
	Name		createEvent
	Syntax		createEvent(char* event)
	Param		char* event - The name of the event to create
	Return		int - 0 if event is not created, 1 if it is
	Brief		Creates a event for the FSM - Adds it to the symbol list 
				and fsm_event_list
*/
int createEvent(char* event)
{
	if (addSymbol(event, EVENT))
	{
		fsmEvent* newEvent;

		// Add new event to the event list
		newEvent = (fsmEvent*) malloc (sizeof(fsmEvent));
		newEvent->name = event;
		newEvent->next = fsm_event_list;
		fsm_event_list = newEvent;
		num_events++;
		return 1;
	}
	return 0;
}

/*
	Name		addTransition
	Syntax		addTransition(char* from_state, char* event, char* to_state)
	Param		char* from_state - The name of the state to transition from
	Param		char* event - The name of the event that triggers the transition
	Param		char* to_state - The name of the state to transition to
	Return		int - 0 if event is not created, 1 if it is
	Brief		Creates a event for the FSM - Adds it to the symbol list 
				and fsm_event_list
*/
int addTransition(char* from_state, char* event, char* to_state)
{
	fsmTransition* newTransition;
	
	// Check if to and from states are the same
	if (!strcmp(from_state, to_state))
	{
		printf("Illegal transition: from %s to %s on event %s.\n", from_state, to_state, event);
		return 0;
	}
	
	// Check that from state exists 
	if ((isDeclared(from_state) == 0) || (getSymbolType(from_state) != STATE))
	{
		printf("Illegal transition: from state %s has not been declared.\n", from_state);
		return 0;
	}
	
	// Check if event exists and that name is not being used as a state		
	if ((isDeclared(event) == 0) || (getSymbolType(event) != EVENT))
	{
		if (createEvent(event))
		{
			printf("Event %s created.\n", event); 
		}
		else
		{
			printf("Event %s could not be created.\n", event);
			return 0;
		}
	}
	// Create the to_state if it has not been declared yet
	createState(to_state);

	// Add new transition to the transition list
	newTransition = (fsmTransition*) malloc (sizeof(fsmTransition));		
	newTransition->from_state = from_state;
	newTransition->event = event;
	newTransition->to_state = to_state;	
	newTransition->next = fsm_transition_list;
	fsm_transition_list = newTransition;	
	return 1;	
}

/*
	Name		getName
	Syntax		getName(void)
	Return		char* - The name of the FSM
	Brief		Returns the name provided from the script. If no name
				has been set then the string "default" is returned.
*/
char* getName(void)
{
	if (fsm_name)
		return fsm_name->name;
	else
		return "default";
}

/*
	Name		getNameUppercase
	Syntax		getNameUppercase(void)
	Return		char* - The name of the FSM in uppercase
	Brief		Returns the name provided from the script in uppercase. 
				If no name has been set then the string "DEFAULT" is returned.
	Details		Currently, this will return the lowercase name, as the uppercase
				transform is not included - see setName function.
*/
char* getNameUppercase()
{
	if (fsm_name)
		return fsm_name->uppercaseName;
	else
		return "DEFAULT";
}

/*
	Name		printStates
	Syntax		printStates(FILE* outstream)
	Param		FILE* outstream - The file to print to
	Brief		Prints each state in the fsm_state_list followed
				by a comma. Each state is indented once.
	Details		The states are printed are the contents of an enum
				in the header file produced by GenerateFSM.
*/
void printStates(FILE* outstream)
{
	fsmState* state = fsm_state_list;
	// Print the names of all the states
	while (state) 
	{
		fprintf(outstream, "\t%s,\n", state->name);
		state = state->next;
	}
}

/*
	Name		printEvents
	Syntax		printEvents(FILE* outstream)
	Param		FILE* outstream - The file to print to
	Brief		Prints each event in the fsm_event_list followed
				by a comma. Each event is indented once.
	Details		The events are printed are the contents of an enum
				in the header file produced by GenerateFSM.
*/
void printEvents(FILE* outstream)
{
	fsmEvent* event = fsm_event_list;
	// Print the names of all the states
	while (event) 
	{
		fprintf(outstream, "\t%s,\n", event->name);
		event = event->next;
	}
}

/*
	Name		getInitialState
	Syntax		getInitialState(void)
	Return		char* - Name of the initial state
	Brief		Returns the name of the initial state or the
				last declared state if no initial state specified
	Details		The FSM class that is produced provides a constructor
				that can be used to set a specific initial state if the 
				default is not wanted
*/
char* getInitialState(void)
{
	if (strcmp(fsm_initial_state, ""))
		return fsm_initial_state;
	else
		return fsm_state_list->name;
}

/*
	Name		statesCount
	Syntax		statesCount(void)
	Return		int - Number of states 
	Brief		Returns the number of declared states
*/
int statesCount(void)
{
	return num_states;
}

/*
	Name		eventsCount
	Syntax		eventsCount(void)
	Return		int - Number of events 
	Brief		Returns the number of declared events
*/
int eventsCount(void)
{
	return num_events;
}

/*
	Name		printPopulateLookupTable
	Syntax		printPopulateLookupTable(FILE* outstream)
	Param		FILE* outstream - The file to print to
	Brief		Prints the transitions for the FSM as entries to
				an array.
*/
void printPopulateLookupTable(FILE* outstream)
{
	fsmTransition* trans = fsm_transition_list;
	while (trans)
	{
		fprintf(outstream, "\tlookUpTable[%s][%s] = \"%s\";\n", trans->from_state, trans->event, trans->to_state);	
		trans = trans->next;
	}
}

/*
	Name		clearName
	Syntax		clearName(void)
	Brief		Frees the memory being taken up by the FSM name
	Details		This does not remove the name from the symbol list
*/
void clearName(void)
{
	if (fsm_name)
		free(fsm_name);
}

/*
	Name		clearStates
	Syntax		clearStates(void)
	Brief		Frees the memory being taken up by the FSM states
	Details		This does not remove the states from the symbol list
*/
void clearStates(void)
{
	fsmState* nextState;
	
	while (fsm_state_list)
	{
		nextState = fsm_state_list->next;
		free(fsm_state_list);
		fsm_state_list = nextState;		
	}
}

/*
	Name		clearEvents
	Syntax		clearEvents(void)
	Brief		Frees the memory being taken up by the FSM events
	Details		This does not remove the events from the symbol list
*/
void clearEvents(void)
{
	fsmEvent* nextEvent;
	
	while (fsm_event_list)
	{
		nextEvent = fsm_event_list->next;
		free(fsm_event_list);
		fsm_event_list = nextEvent;
	}
}

/*
	Name		clearTransitions
	Syntax		clearTransitions(void)
	Brief		Frees the memory being taken up by the FSM transitions
*/
void clearTransitions(void)
{
	fsmTransition* nextTransition;
	
	while (fsm_transition_list)
	{
		nextTransition = fsm_transition_list->next;
		free(fsm_transition_list);
		fsm_transition_list = nextTransition;
	}
}

/*
	Name		clearAll
	Syntax		clearAll(void)
	Brief		Frees up memory that FSM data is stored in
*/
void clearAll(void)
{
	clearName();
	clearStates();
	clearEvents();
	clearTransitions();
	clearSymbolTable();
}
