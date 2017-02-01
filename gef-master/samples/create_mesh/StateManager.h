#pragma once

#include <vector>




class State;

class StateManager
{
#define MAINMENU 0
#define APPLICATION_OPTIONS 1
#define SANDBOX_CALIBRATION 2
#define TD_GAME_MENU 3



protected:
	std::vector<State*> stateList;

	
public:
	inline void AddState(State* state) { stateList.push_back(state); }
	inline State* GetCurrentState() { return stateList.back(); }
	inline void GotoPreviousState() 
	{ 
		stateList.back();
		stateList.back() = NULL;
		stateList.pop_back();
	}
	inline int GetNumStates() { return stateList.size(); }

	//std::vector<int> statelist;

	

};
