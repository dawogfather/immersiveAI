//-------------------------------------------------------------------
// Immersive AI :: iAIGoalLibrary
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/// @file iAIGoalLibrary.h
//-------------------------------------------------------------------
/// @class iAIGoalLibrary
/// @author Gavin Bunney
/// @version 1.0
/// @brief Collection of goals.
/// 
/// Holds a library of all goals for the agents in the game world.
//-------------------------------------------------------------------

#ifndef _IAIGOALLIBRARY_H_
#define _IAIGOALLIBRARY_H_

#include "immersiveAI/agent/iAIAgent.h"
#include "console/simBase.h"

//-------------------------------------------------------------------
/// @struct iAIGoalSolution
/// @brief Represents a solution for a goal.
///
/// Holds the solution name and a condition function which is
/// executed, in script, to determine the weighting for the solution
/// at a particular point in time. The condition function must return
/// 0.00 <= return <= 1.00.
//-------------------------------------------------------------------
struct iAIGoalSolution
{
	//-------------------------------------------------------------------
	/// @var char mName[50]
	/// @brief Name of the solution.
	//-------------------------------------------------------------------
	char mName[50];
};

//-------------------------------------------------------------------
/// @struct iAIGoal
/// @brief Represents a goal for an agent type.
///
/// Holds the goal name, agent type the goal is for, and the name of
/// the condition function which is executed, in script, to determine
/// the weighting for the goal at a particular point in time. The 
/// condition function must return 0.00 <= return <= 1.00.
//-------------------------------------------------------------------
struct iAIGoal
{
	//-------------------------------------------------------------------
	/// @var char mName[50]
	/// @brief Name of the goal.
	//-------------------------------------------------------------------
	char mName[50];

	//-------------------------------------------------------------------
	/// @var char mAgentType[50]
	/// @brief Name of the agent type that goal is for.
	//-------------------------------------------------------------------
	char mAgentType[50];

	//-------------------------------------------------------------------
	/// @var Vector<iAIGoalSolution*> mSolutions
	/// @brief Collection of solution goals to solve this goal.
	//-------------------------------------------------------------------
	Vector<iAIGoalSolution*> mSolutions;
};

class iAIGoalLibrary : public SimObject
{
	typedef SimObject Parent;
	friend class iAIGoalManager;

public:
	
	//-------------------------------------------------------------------
	/// @var DECLARE_CONOBJECT(iAIGoalLibrary)
	/// @brief TorqueScript object.
	//-------------------------------------------------------------------
	DECLARE_CONOBJECT(iAIGoalLibrary);

	//-------------------------------------------------------------------
	/// @fn iAIGoalLibrary() 
	/// @brief Default constructor.
	//-------------------------------------------------------------------
	iAIGoalLibrary();

	//-------------------------------------------------------------------
	/// @fn ~iAIGoalManager() 
	/// @brief Deconstructor.
	//-------------------------------------------------------------------
	~iAIGoalLibrary();

	//-------------------------------------------------------------------
	/// @fn void addGoal(const char* agentType, const char* goalName)
	/// @brief Add a goal to the library.
	///
	/// @param agentType Type of agent the goal is for.
	/// @param goalName Name of the goal to add.
	/// @returns bool Add success.
	//-------------------------------------------------------------------
	bool addGoal(const char* agentType, const char* goalName);

	//-------------------------------------------------------------------
	/// @fn void addSolution(const char* agentType, const char* goalName,
	///                      const char* solutionName)
	/// @brief Add a solution for a goal to the library.
	///
	/// @param agentType Type of agent the goal is for.
	/// @param goalName Name of the goal to add the solution against.
	/// @param solutionName Name of the solution goal.
	/// @returns bool Add success.
	//-------------------------------------------------------------------
	bool addSolution(const char* agentType, const char* goalName, const char* solutionName);

	//-------------------------------------------------------------------
	/// @fn bool removeGoal(const char* goalName, const char* agentType)
	/// @brief Remove a goal from the library.
	///
	/// @param goalName Name of the goal to add.
	/// @param agentType Type of agent the goal is for.
	/// @returns Remove success.
	//-------------------------------------------------------------------
	bool removeGoal(const char* goalName, const char* agentType);

	//-------------------------------------------------------------------
	/// @fn bool removeGoal(const char* goalName)
	/// @brief Remove all goals that match the passed goal name.
	///
	/// @param goalName Name of all the goals to remove.
	/// @returns Remove success.
	//-------------------------------------------------------------------
	bool removeGoal(const char* goalName);

	//-------------------------------------------------------------------
	/// @fn void clear()
	/// @brief Empty the goal library.
	//-------------------------------------------------------------------
	void clear();

	//-------------------------------------------------------------------
	/// @fn void sort()
	/// @brief Sorts the library.
	//-------------------------------------------------------------------
	void sort();

	//-------------------------------------------------------------------
	/// @fn void printGoals()
	/// @brief Output all goals in the library to the console.
	//-------------------------------------------------------------------
	void printGoals();

	//-------------------------------------------------------------------
	/// @fn void printGoals(const char* agentType)
	/// @brief Output all goals in the library for the specified agent
	///        type to the console.
	///
	/// @param agentType Type of agent to output the goals for.
	//-------------------------------------------------------------------
	void printGoals(const char* agentType);

	//-------------------------------------------------------------------
	/// @fn bool getGoals(const char* agentType,
	///                   Vector<iAIGoal*> &returnList)
	/// @brief Retrieve all goals for the specified agent type.
	///
	/// @param agentType Type of agent to retrieve goals for.
	/// @param returnList Vector where the retrieved goals are placed.
	/// @returns bool returnList is not empty.
	//-------------------------------------------------------------------
	bool getGoals(const char* agentType, Vector<iAIGoal*> &returnList);

	//-------------------------------------------------------------------
	/// @fn iAIGoal* iAIGoalLibrary::getGoal(const char* agentType,
	///                                      const char* goalName)
	/// @brief Retrieve the specified goal.
	///
	/// @param agentType Type of agent to retrieve goal for.
	/// @param goalName Name of the goal to retrieve.
	/// @returns iAIGoal* Pointer to the found goal.
	//-------------------------------------------------------------------
	iAIGoal* getGoal(const char* agentType, const char* goalName);

protected:

	//-------------------------------------------------------------------
	/// @var Vector<iAIGoal*> mGoalLibrary
	/// @brief Collection of goals in the library.
	//-------------------------------------------------------------------
	Vector<iAIGoal*> mGoalLibrary;

	//-------------------------------------------------------------------
	/// @var bool mSorted
	/// @brief Flag set when library is sorted.
	//-------------------------------------------------------------------
	bool mSorted;
};

#endif