//-------------------------------------------------------------------
// Immersive AI :: iAIGoalLibrary
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/// @file iAIGoalLibrary.cc
//-------------------------------------------------------------------
/// @class iAIGoalLibrary
/// @author Gavin Bunney
/// @version 1.0
/// @brief Collection of goals.
///
/// Holds a library of all goals for the agents in the game world.
//-------------------------------------------------------------------

#include "iAIGoalLibrary.h"
#include "console/consoleTypes.h"
#include "platform/platform.h"

IMPLEMENT_CONOBJECT(iAIGoalLibrary);

iAIGoalLibrary::iAIGoalLibrary()
{
	this->mSorted = false;
}

iAIGoalLibrary::~iAIGoalLibrary()
{
	this->clear();
}

bool iAIGoalLibrary::addGoal(const char* agentType, const char* goalName)
{
	iAIGoal *newGoal = new iAIGoal();

	// check string sizes valid
	if ((dStrlen(agentType) <= sizeof(newGoal->mAgentType)) &&
		(dStrlen(goalName) <= sizeof(newGoal->mName)))
	{
		// create the new goal
		dSprintf(newGoal->mAgentType, sizeof(newGoal->mAgentType), "%s", agentType);
		dSprintf(newGoal->mName, sizeof(newGoal->mName), "%s", goalName);

		// add to library
		this->mGoalLibrary.push_back(newGoal);

		return true;
	} else
	{
		Con::errorf("Immersive AI :: Goal Library :: Add goal failed - Invalid parameter lengths.");
		return false;
	}
}

bool iAIGoalLibrary::addSolution(const char* agentType, const char* goalName, const char* solutionName)
{
	iAIGoalSolution *newSolution = new iAIGoalSolution();

	// check string sizes valid
	if (dStrlen(solutionName) <= sizeof(newSolution->mName))
	{
		// create the new solution
		dSprintf(newSolution->mName, sizeof(newSolution->mName), "%s", solutionName);

		// find the goal in the library
		for (U32 i = 0; i < this->mGoalLibrary.size(); ++i)
		{
			// compare the name and agent type names
			if ((dStrcmp(this->mGoalLibrary[i]->mAgentType, agentType) == 0) &&
				(dStrcmp(this->mGoalLibrary[i]->mName, goalName) == 0))
			{
				// found, add to library
				this->mGoalLibrary[i]->mSolutions.push_back(newSolution);
				return true;
			}
		}

		// reached here, means goal/agent type didnt exist!
		Con::errorf("Immersive AI :: Goal Library :: Add solution failed - goal/agent combination doesn't exist.");
		return false;
	} else
	{
		Con::errorf("Immersive AI :: Goal Library :: Add solution failed - Invalid parameter lengths.");
		return false;
	}
}

bool iAIGoalLibrary::removeGoal(const char* goalName, const char* agentType)
{
	bool removedGoal = false;

	// iterate over all goals
	for (U32 i = 0; i < this->mGoalLibrary.size(); ++i)
	{
		// compare the name and agent type names
		if ((dStrcmp(this->mGoalLibrary[i]->mName, goalName) == 0) && (dStrcmp(this->mGoalLibrary[i]->mAgentType, agentType) == 0))
		{
			// clear any solutions
			this->mGoalLibrary[i]->mSolutions.clear();

			// erase the actual goal
			this->mGoalLibrary.erase(i);
			--i;
			removedGoal = true;
		}
	}

	return removedGoal;
}

bool iAIGoalLibrary::removeGoal(const char* goalName)
{
	bool removedGoal = false;

	// iterate over all goals
	for (U32 i = 0; i < this->mGoalLibrary.size(); ++i)
	{
		// compare the name and agent type names
		if (dStrcmp(this->mGoalLibrary[i]->mName, goalName) == 0)
		{
			// clear any solutions
			this->mGoalLibrary[i]->mSolutions.clear();

			// erase the actual goal
			this->mGoalLibrary.erase(i);
			--i;
			removedGoal = true;
		}
	}

	return removedGoal;
}

void iAIGoalLibrary::clear()
{
	// keep looping until the goal library is empty
	while (!this->mGoalLibrary.empty())
	{
		// loop while the current solutiosn isn't empty
		while (!this->mGoalLibrary.front()->mSolutions.empty())
		{
			// delete the front solution
			this->mGoalLibrary.front()->mSolutions.erase((U32)0);
		}

		// delete the front goal
		this->mGoalLibrary.erase((U32)0);
	}
}

static S32 QSORT_CALLBACK goalCompare( const void* a, const void* b )
{
	const iAIGoal *goalA = *((iAIGoal **) a);
	const iAIGoal *goalB = *((iAIGoal **) b);

	// sort by agent type first
	S8 result = dStrcmp(goalA->mAgentType, goalB->mAgentType);
	if (result == 0)
	{
		// then by goal name
		return dStrcmp(goalA->mName, goalB->mName);
	} else
	{
		return result;
	}
	return 0;
}

static S32 QSORT_CALLBACK goalSolutionCompare( const void* a, const void* b )
{
	// sort by solution name
	return dStrcmp((*((iAIGoalSolution **) a))->mName, (*((iAIGoalSolution **) b))->mName);
}


void iAIGoalLibrary::sort()
{
	// sort the solutions within the goals first
	for (U32 i = 0; i < this->mGoalLibrary.size(); ++i)
	{
		dQsort((void *)&(this->mGoalLibrary[i]->mSolutions[0]), this->mGoalLibrary[i]->mSolutions.size(), sizeof(iAIGoalSolution*), goalSolutionCompare);
	}

	// sort the goals
	dQsort((void *)&(this->mGoalLibrary.front()), this->mGoalLibrary.size(), sizeof(iAIGoal*), goalCompare);

	this->mSorted = true;
}

void iAIGoalLibrary::printGoals()
{
	Con::iAIMessagef("iAIGoalLibrary -- Goals list:");
	for (U32 i = 0; i < this->mGoalLibrary.size(); ++i)
	{
		Con::iAIMessagef("[%d - %s] %s", i, this->mGoalLibrary[i]->mAgentType, this->mGoalLibrary[i]->mName);
		for (U32 j = 0; j < this->mGoalLibrary[i]->mSolutions.size(); ++j)
		{
			Con::iAIMessagef("--- [%d] %s", j, this->mGoalLibrary[i]->mSolutions[j]->mName);
		}
	}
}

void iAIGoalLibrary::printGoals(const char* agentType)
{
	Con::iAIMessagef("iAIGoalLibrary :: Goal List for '%s'", agentType);
	for (U32 i = 0; i < this->mGoalLibrary.size(); ++i)
	{
		// only output the desired agent type goals
		if (dStrcmp(this->mGoalLibrary[i]->mAgentType, agentType) == 0)
		{
			Con::iAIMessagef("[%d] %s", i, this->mGoalLibrary[i]->mName);
			for (U32 j = 0; j < this->mGoalLibrary[i]->mSolutions.size(); ++j)
			{
				Con::iAIMessagef("--- [%d] %s", j, this->mGoalLibrary[i]->mSolutions[j]->mName);
			}
		}
	}
}

bool iAIGoalLibrary::getGoals(const char* agentType, Vector<iAIGoal*> &returnList)
{
	// check if not sorted and sort if need be
	if (!this->mSorted)
		this->sort();

	returnList.clear();

	// iterate over all goals in the library
	for (U32 i = 0; i < this->mGoalLibrary.size(); ++i)
	{
		// check for agent type
		if (dStrcmp(this->mGoalLibrary[i]->mAgentType, agentType) == 0)
		{
			// add to return list
			returnList.push_back(this->mGoalLibrary[i]);
		} else
		{
			// if the return list is not empty, then must have gone beyond agent type
			if (!returnList.empty())
			{
				// break out
				break;
			}
		}
	}

	return (!returnList.empty());
}

iAIGoal* iAIGoalLibrary::getGoal(const char* agentType, const char* goalName)
{
	if ((dStrlen(agentType) == 0) || (dStrlen(goalName) == 0))
		return 0;

	// iterate over all goals in the library
	for (U32 i = 0; i < this->mGoalLibrary.size(); ++i)
	{
		// check for agent type and goalName
		if ((dStrcmp(this->mGoalLibrary[i]->mAgentType, agentType) == 0) &&
			(dStrcmp(this->mGoalLibrary[i]->mName, goalName) == 0))
		{
			return this->mGoalLibrary[i];
		}
	}

	// didn't find the goal
	return 0;
}

ConsoleMethodGroupBegin(iAIGoalLibrary, ScriptFunctions, "iAIGoalLibrary Script Functions");

ConsoleMethod( iAIGoalLibrary, addGoal, bool, 4, 4,
			  "bool iAIGoalLibrary.addGoal(string agentType, string goalName) - Add a goal to the library.")
{
	// ensure all params passed
	if ((dStrlen(argv[2]) != 0) && (dStrlen(argv[3]) != 0))
	{
		object->addGoal(argv[2], argv[3]);
		return true;
	} else
	{
		Con::errorf("Immersive AI :: Goal Library :: Not all goal params passed to add to the library!");
		return false;
	}
}

ConsoleMethod( iAIGoalLibrary, addSolution, bool, 5, 5,
			  "bool iAIGoalLibrary.addSolution(string agentType, string goalName, string solutionName) - Add a solution for a goal to the library.")
{
	// ensure all params passed
	if ((dStrlen(argv[2]) != 0) && (dStrlen(argv[3]) != 0) && (dStrlen(argv[4]) != 0))
	{
		object->addSolution(argv[2], argv[3], argv[4]);
		return true;
	} else
	{
		Con::errorf("Immersive AI :: Goal Library :: Not all solution params passed to add to the library!");
		return false;
	}
}

ConsoleMethod( iAIGoalLibrary, printGoals, void, 2, 3,
			  "void iAIGoalLibrary.printGoals([optional] string agentType) - Output all the goals to the console.")
{
	// see if want just a single agent type
	if (dStrlen(argv[2]) != 0)
	{
		object->printGoals(argv[2]);
	} else
	{
		object->printGoals();
	}
}

ConsoleMethod( iAIGoalLibrary, clear, void, 2, 2,
			  "void iAIGoalLibrary.clear() - Clear all goals in the library.")
{
	object->clear();
}

ConsoleMethod( iAIGoalLibrary, removeGoal, bool, 3, 4,
			  "bool iAIGoalLibrary.removeGoal(string goalName, [optional] string agentType) - Remove a goal from the library.")
{
	// ensure at least 1 param passed
	if (dStrlen(argv[2]) != 0)
	{
		if (dStrlen(argv[3]) != 0)
			return (object->removeGoal(argv[2], argv[3]));
		else
			return (object->removeGoal(argv[2]));
	} else
	{
		Con::errorf("Immersive AI :: Goal Library :: Remove goal not given enough parameters.");
		return false;
	}
}

ConsoleMethod( iAIGoalLibrary, sort, void, 2, 2,
			  "void iAIGoalLibrary.sort() - Sort the goal library.")
{
	object->sort();
}

ConsoleMethod( iAIGoalLibrary, getGoalList, const char*, 3, 3,
			  "string iAIGoalLibrary.getGoalList(string agentType) - Get a space deliminated string of the goals for an agent type.")
{
	char *returnBuffer = Con::getReturnBuffer(512);
	dSprintf(returnBuffer, sizeof(returnBuffer), "");

	// ensure agent type parsed
	if (dStrlen(argv[2]) != 0)
	{
		// get the goals for the parsed agent type
		Vector<iAIGoal*> goalList;
		if (object->getGoals(argv[2], goalList))
		{
			// iterate over all the goals and add to return string
			for (U32 i = 0; i < goalList.size(); ++i)
			{
				if (i != 0)
					dStrcat(returnBuffer, " ");
				dStrcat(returnBuffer, goalList[i]->mName);
			}
		}
	}

	return returnBuffer;
}

ConsoleMethod( iAIGoalLibrary, getSolutionList, const char*, 4, 4,
			  "string iAIGoalLibrary.getSolutionList(string agentType, string goalName) - Get a space deliminated string of the solutions.")
{
	char *returnBuffer = Con::getReturnBuffer(512);
	dSprintf(returnBuffer, sizeof(returnBuffer), "");

	// ensure goal & agent type parsed
	if ((dStrlen(argv[2]) != 0) && (dStrlen(argv[3]) != 0))
	{
		// get a pointer to the goal
		iAIGoal* goal = object->getGoal(argv[2], argv[3]);

		// check valid goal found
		if (goal)
		{
			// iterate over all the goal solutions and add to return string
			for (U32 i = 0; i < goal->mSolutions.size(); ++i)
			{
				if (i != 0)
					dStrcat(returnBuffer, " ");
				dStrcat(returnBuffer, goal->mSolutions[i]->mName);
			}
		}
	}

	return returnBuffer;
}

ConsoleMethodGroupEnd(iAIGoalLibrary, ScriptFunctions);