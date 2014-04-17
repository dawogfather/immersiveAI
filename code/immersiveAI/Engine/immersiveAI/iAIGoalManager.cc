//-------------------------------------------------------------------
// Immersive AI :: iAIGoalManager
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/// @file iAIGoalManager.cc
//-------------------------------------------------------------------
/// @class iAIGoalManager
/// @author Gavin Bunney
/// @version 1.0
/// @brief Assigns goals to agents.
/// 
/// The Goal Manager checks its stack for agents requireing goals.
/// The goal descision network is implemented as a Bayesian Network.
//-------------------------------------------------------------------

#include "iAIGoalManager.h"
#include "util/safeDelete.h"
#include "console/consoleTypes.h"

IMPLEMENT_CONOBJECT(iAIGoalManager);

iAIGoalManager::iAIGoalManager()
{
	// nothing to do
}

iAIGoalManager::~iAIGoalManager()
{
	// nothing to do
}

bool iAIGoalManager::onAdd()
{
	if (!Parent::onAdd())
		return false;

	// get the goal library
	this->mGoalLibrary = dynamic_cast<iAIGoalLibrary*>(Sim::findObject(Con::getVariable("$iAIGoalLibrary")));

	// throw error if goal library not found
	if (!this->mGoalLibrary)
	{
		Con::errorf("Immersive AI :: GoalManager :: Unable to locate the goal library!");
		return false;
	}

	return true;
}

void iAIGoalManager::onRemove()
{
	Parent::onRemove();
}

bool iAIGoalManager::assignGoal(iAIAgent* agent, iAIGoal* goal, iAIGoalSolution* solution)
{
	// get the agent id
	char agentId[10];
	dSprintf(agentId, sizeof(agentId), "%d", agent->getId());

	// set previous variables to current goal & solution
	agent->setPreviousGoal(agent->getCurrentGoal());
	agent->setPreviousSolution(agent->getCurrentSolution());

	// set the current goal & solution to new goal and solution
	agent->setCurrentGoal(goal->mName);
	agent->setCurrentSolution(solution->mName);

	char solutionFunction[255];

	// build the solution function and execute
	dSprintf(solutionFunction, sizeof(solutionFunction), IAIGOALMANAGER_SOLUTION_FUNCTION_FORMAT, solution->mName);
	Con::executef(2, solutionFunction, agentId);
	
	return true;
}

bool iAIGoalManager::processRequest(iAIAgent* agent, bool getNewGoal, bool avoidCurrentGoal, bool avoidCurrentSolution)
{
	// check agent pointer valid
	if (!agent)
		return false;

	// get the agent id
	char agentId[10];
	dSprintf(agentId, sizeof(agentId), "%d", agent->getId());

	// the goal to change to
	iAIGoal* newGoal;

	// check if we want a new goal
	if (getNewGoal)
	{
		// need to find the best goal to assign...

		// get the list of goals for the agent type
		Vector<iAIGoal*> goalList;
		if (!this->mGoalLibrary->getGoals(agent->getAgentType(), goalList))
			return false;

		// execute each goal and find greatest match
		F32 max = 0.00;
		S8 goalIdx = 0;
		char functionName[255];

		// iterate over all goals
		for (S8 i = 0; i < goalList.size(); ++i)
		{
			// check if we need to avoid this goal, as current goal
			if ((avoidCurrentGoal) && (dStrcmp(goalList[i]->mName, agent->getCurrentGoal()) == 0))
				continue;

			// build the condition function to execute
			dSprintf(functionName, sizeof(functionName), IAIGOALMANAGER_EVALUATION_FORMAT, goalList[i]->mName);

			// execute the condition function
			const char *returnValueStr = Con::executef(2, functionName, agentId);
			F32 returnValue = dAtof(returnValueStr);

			// check for current goal being greater than last
			if (returnValue > max)
			{
				max = returnValue;
				goalIdx = i;
			}
		}

		// check the goalIdx is within bounds
		if (!(goalIdx < goalList.size()))
			return false;

		// set the new goal
		newGoal = goalList[goalIdx];
	} else
	{
		// don't want a new goal, get the current goal
		newGoal = this->mGoalLibrary->getGoal(agent->getAgentType(), agent->getCurrentGoal());
	}

	// check the newGoal is valid
	if (!newGoal)
		return false;

	// find the greatest solution match
	S8 solutionIdx = 0;
	F32 max = 0.00;
	char functionName[255];

	// iterate over all solutions for the new goal
	for (S8 i = 0; i < newGoal->mSolutions.size(); ++i)
	{
		// check if we need to avoid this solution, as current solution
		if ((avoidCurrentSolution) && (dStrcmp(newGoal->mSolutions[i]->mName, agent->getCurrentSolution()) == 0))
			continue;
		
		// build the condition function to execute
		dSprintf(functionName, sizeof(functionName), IAIGOALMANAGER_EVALUATION_FORMAT, newGoal->mSolutions[i]->mName);

		// execute the condition function
		const char *returnValueStr = Con::executef(2, functionName, agentId);
		F32 returnValue = dAtof(returnValueStr);

		// check for current solution being greater than last
		if (returnValue > max)
		{
			max = returnValue;
			solutionIdx = i;
		}
	}

	// check the solutionIdx is within bounds
	if (!(solutionIdx < newGoal->mSolutions.size()))
		return false;

	// assign the new goal to agent
	return (this->assignGoal(agent, newGoal, newGoal->mSolutions[solutionIdx]));
}

void iAIGoalManager::processRequestBatch()
{
	// process the new goal list if its not empty
	if (!this->mRequestNewGoalList.empty())
	{
		// process the new goal request, check if it failed
		if (!this->processRequest(this->mRequestNewGoalList.front()))
		{
			Con::errorf("Unable to process the new goal request for agent %s", this->mRequestNewGoalList.front());
		}

		// remove request
		this->mRequestNewGoalList.pop_front();
	}

	// process the new goal list, avoiding current if its not empty
	if (!this->mRequestNewGoalAvoidList.empty())
	{
		// process the new goal request, check if it failed
		if (!this->processRequest(this->mRequestNewGoalAvoidList.front(), true, true))
		{
			Con::errorf("Unable to process the new goal request for agent %s", this->mRequestNewGoalAvoidList.front());
		}

		// remove request
		this->mRequestNewGoalAvoidList.pop_front();
	}

	// process the new solution list if its not empty
	if (!this->mRequestNewSolutionList.empty())
	{
		// process the new goal request, check if it failed
		if (!this->processRequest(this->mRequestNewSolutionList.front(), false, false, true))
		{
			Con::errorf("Unable to process the new goal request for agent %s", this->mRequestNewSolutionList.front());
		}

		// remove request
		this->mRequestNewSolutionList.pop_front();
	}
}

bool iAIGoalManager::isEmpty()
{
	return (this->mRequestNewGoalList.empty() && this->mRequestNewGoalAvoidList.empty() && this->mRequestNewSolutionList.empty());
}

void iAIGoalManager::requestNewGoal(iAIAgent* agent, bool avoidCurrentGoal)
{
	if (avoidCurrentGoal)
	{
		// add the agent to the request list
		this->mRequestNewGoalAvoidList.push_back(agent);
	} else
	{
		// add the agent to the new request list
		this->mRequestNewGoalList.push_back(agent);
	}
}

void iAIGoalManager::requestNewSolution(iAIAgent* agent)
{
	// add the agent to the request list
	this->mRequestNewSolutionList.push_back(agent);
}


ConsoleMethodGroupBegin(iAIGoalManager, ScriptFunctions, "iAIGoalManager Script Functions");

ConsoleMethod( iAIGoalManager, processRequestBatch, void, 2, 2,
			  "void iAIGoalManager.processRequestBatch() - Processes the next request batch.")
{
	object->processRequestBatch();
}

ConsoleMethod( iAIGoalManager, isEmpty, bool, 2, 2,
			  "bool iAIGoalManager.isEmpty() - Returns true if the request list is empty.")
{
	return object->isEmpty();
}

ConsoleMethod( iAIGoalManager, requestGoal, bool, 3, 4,
			  "bool iAIGoalManager.requestGoal(iAIAgent agent, [optional] bool avoidCurrentGoal) - Request a new goal for the specified agent. Optionally avoiding the current goal.")
{
	// ensure agent parsed
	if (dStrlen(argv[2]) != 0)
	{
		// get the parsed agent & ensure valid
		iAIAgent *agent = dynamic_cast<iAIAgent*>(Sim::findObject(argv[2]));

		if (agent)
		{
			// see if avoidCurrentGoal is parsed
			if (dStrlen(argv[3]) != 0)
			{
				// request a goal for the agent
				object->requestNewGoal(agent, dAtob(argv[2]));
			} else
			{
				// request a goal for the agent
				object->requestNewGoal(agent);
			}
			return true;
		} else
		{
			Con::errorf("Immersive AI :: GoalManager :: Unable to locate parsed agent!");
			return false;
		}
	} else
	{
		Con::errorf("Immersive AI :: GoalManager :: No Agent parsed to add!");
		return false;
	}
}

ConsoleMethod( iAIGoalManager, requestSolution, bool, 3, 4,
			  "bool iAIGoalManager.requestSolution(iAIAgent agent) - Request a new solution for the agent's current goal.")
{
	// ensure agent parsed
	if (dStrlen(argv[2]) != 0)
	{
		// get the parsed agent & ensure valid
		iAIAgent *agent = dynamic_cast<iAIAgent*>(Sim::findObject(argv[2]));

		if (agent)
		{
			// request a solution for the agent
			object->requestNewSolution(agent);
			return true;
		} else
		{
			Con::errorf("Immersive AI :: GoalManager :: Unable to locate parsed agent!");
			return false;
		}
	} else
	{
		Con::errorf("Immersive AI :: GoalManager :: No Agent parsed to add!");
		return false;
	}
}

ConsoleMethodGroupEnd(iAIGoalManager, ScriptFunctions);