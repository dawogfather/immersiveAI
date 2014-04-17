//-------------------------------------------------------------------
// Immersive AI :: iAIGoalManager
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/// @file iAIGoalManager.h
//-------------------------------------------------------------------
/// @class iAIGoalManager
/// @author Gavin Bunney
/// @version 1.0
/// @brief Assigns goals to agents.
/// 
/// The Goal Manager checks its stack for agents requireing goals.
/// The goal descision network is implemented as a Bayesian Network.
//-------------------------------------------------------------------

#ifndef _IAIGOALMANAGER_H_
#define _IAIGOALMANAGER_H_

#include "immersiveAI/agent/iAIAgent.h"
#include "platform/platformThread.h"
#include "console/simBase.h"
#include "iAIGoalLibrary.h"

//-------------------------------------------------------------------
/// @def IAIGOALMANAGER_EVALUATION_FORMAT
/// @brief Format of the evaluation functions. %s is the goal or
///        solution name.
//-------------------------------------------------------------------
#define IAIGOALMANAGER_EVALUATION_FORMAT		"%s_evaluate"

//-------------------------------------------------------------------
/// @def IAIGOALMANAGER_SOLUTION_FUNCTION_FORMAT
/// @brief Format of the solution function to execute. %s is the
///        solution name.
//-------------------------------------------------------------------
#define IAIGOALMANAGER_SOLUTION_FUNCTION_FORMAT	"%s_onEnter"

class iAIGoalManager : public SimObject
{
	typedef SimObject Parent;

public:
	
	//-------------------------------------------------------------------
	/// @var DECLARE_CONOBJECT(iAIGoalManager)
	/// @brief TorqueScript object.
	//-------------------------------------------------------------------
	DECLARE_CONOBJECT(iAIGoalManager);

	//-------------------------------------------------------------------
	/// @fn iAIGoalManager() 
	/// @brief Default constructor.
	//-------------------------------------------------------------------
	iAIGoalManager();

	//-------------------------------------------------------------------
	/// @fn ~iAIGoalManager() 
	/// @brief Deconstructor.
	//-------------------------------------------------------------------
	~iAIGoalManager();

	//-------------------------------------------------------------------
	/// @fn bool onAdd()
	/// @brief Called on adding to Sim.
	///
	/// @return Add success.
	//-------------------------------------------------------------------
	bool onAdd();

	//-------------------------------------------------------------------
	/// @fn bool onRemove()
	/// @brief Called on removal from Sim.
	//-------------------------------------------------------------------
	void onRemove();

	//-------------------------------------------------------------------
	/// @fn void requestNewGoal(iAIAgent* agent,
	///                         bool avoidCurrentGoal = false)
	/// @brief Request a goal for the parsed agent.
	///
	/// @param agent Pointer to the agent to request a goal for.
	/// @param avoidCurrentGoal Flag to avoid the current goal or not.
	///                         Default false.
	//-------------------------------------------------------------------
	void requestNewGoal(iAIAgent* agent, bool avoidCurrentGoal = false);

	//-------------------------------------------------------------------
	/// @fn void requestNewSolution(iAIAgent* agent)
	/// @brief Request a new solution for the agents current goal.
	///
	/// @param agent Pointer to the agent to request a solution for.
	//-------------------------------------------------------------------
	void requestNewSolution(iAIAgent* agent);

	void processRequestBatch();
	bool isEmpty();

protected:

	//-------------------------------------------------------------------
	/// @fn bool assignGoal(iAIAgent* agent, iAIGoal* goal,
	///	                           iAIGoalSolution* solution)
	/// @brief Assign a goal & solution to the specified agent. Executes
	///        the solution once it is assigned to the agent.
	///
	/// @param agent Pointer to the agent to assign to.
	/// @param goal Pointer to the goal to assign.
	/// @param solution Pointer to the solution to assign.
	/// @returns bool Assignment success.
	//-------------------------------------------------------------------
	bool assignGoal(iAIAgent* agent, iAIGoal* goal, iAIGoalSolution* solution);

	//-------------------------------------------------------------------
	/// @fn bool processRequest(iAIAgent* agent,
	///                                bool getNewGoal = true,
	///                                bool avoidCurrentGoal = false, 
	///                                bool avoidCurrentSolution = false)
	/// @brief Processes a goal change request for the specified agent.
	/// 
	/// Determines the best goal to assign for the specified agent, based
	/// on the various condition functions stored in the iAIGoalLibrary. 
	/// <br><br>
	/// By default, a new goal is assigned for the agent, possibly being
	/// the same goal they are currently in. The optional parameters can
	/// alter this default behaviour however.
	/// <br><br>
	/// <b>getNewGoal</b> if set will not retrieve a new goal for the 
	/// agent but will instead locate another solution for the current
	/// goal.
	/// <br>
	/// <b>avoidCurrentGoal</b> if set will avoid the current goal that
	/// the agent has assigned, generating a new <i>different</i> goal.
	/// <br>
	/// <b>avoidCurrentSolution</b> if set will avoid the current
	/// solution that the agent has assigned, generating a new
	/// <i>different</i> solution.
	///
	/// @param agent Pointer to the agent to determine the best goal for.
	/// @param getNewGoal Flag to retrieve another goal or not.
	/// @param avoidCurrentGoal Flag to avoid the current goal and get
	///        another one.
	/// @param avoidCurrentSolution Flag to avoid the current solution
	///        and get another one.
	/// @returns bool Request processing success.
	//-------------------------------------------------------------------
	bool processRequest(iAIAgent* agent, bool getNewGoal = true, bool avoidCurrentGoal = false, bool avoidCurrentSolution = false);

	//-------------------------------------------------------------------
	/// @var Vector<iAIAgent*> mRequestNewGoalList
	/// @brief List of agents which request new goals.
	//-------------------------------------------------------------------
	Vector<iAIAgent*> mRequestNewGoalList;

	//-------------------------------------------------------------------
	/// @var Vector<iAIAgent*> mRequestNewGoalAvoidList
	/// @brief List of agents which request new goals, avoiding the 
	///        current goal.
	//-------------------------------------------------------------------
	Vector<iAIAgent*> mRequestNewGoalAvoidList;

	//-------------------------------------------------------------------
	/// @var Vector<iAIAgent*> mRequestNewSolutionList
	/// @brief List of agents which request new solutions.
	//-------------------------------------------------------------------
	Vector<iAIAgent*> mRequestNewSolutionList;

	//-------------------------------------------------------------------
	/// @var iAIGoalLibrary* mGoalLibrary
	/// @brief Pointer to the goal library instance.
	//-------------------------------------------------------------------
	iAIGoalLibrary* mGoalLibrary;
};

#endif