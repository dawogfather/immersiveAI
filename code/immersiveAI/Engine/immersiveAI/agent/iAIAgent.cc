//-------------------------------------------------------------------
// Immersive AI :: Agent
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------
#include "iAIAgent.h"
#include "console/consoleInternal.h"
#include "core/realComp.h"
#include "math/mMatrix.h"
#include "game/moveManager.h"
#include "game/gameConnection.h"

IMPLEMENT_CO_NETOBJECT_V1(iAIAgent);

iAIAgent::iAIAgent()
{
	mTypeMask |= iAIAgentObjectType;
	this->setMoveTolerance(1.00f);

	this->mDamage = 0.00f;
	this->mHappiness = 100;
	this->mFatigue = 0;
	this->mMoney = 100.0;
	this->mBoredom = 0;
	this->mCurrentPath = 0;
}

iAIAgent::~iAIAgent()
{
	// nothing to destruct.. yet
}

void iAIAgent::initPersistFields()
{
	Parent::initPersistFields();
}

void iAIAgent::executeFunction(const char *name)
{
	Con::executef(2, name);
}

void iAIAgent::executeDatablockFunction(const char *name)
{
	Con::executef(this->getDataBlock(), 2, name, this->scriptThis());
}

void iAIAgent::setHealth(const S32 health)
{
	if ((health >= 0) && (health <= 100))
	{
		this->setDamageLevel(100 - health);
	} else
	{
		if (health < 0)
			this->setDamageLevel(100);
		else
			this->setDamageLevel(0);
	}
}

const S32 iAIAgent::getHealth()
{
	return (100 - this->getDamageLevel());
}

void iAIAgent::setHappiness(const S32 happiness)
{
	if ((happiness >= 0) && (happiness <= 100))
	{
		this->mHappiness = happiness;
	} else
	{
		if (happiness < 0)
			this->mHappiness = 0;
		else
			this->mHappiness = 100;
	}
}

const S32 iAIAgent::getHappiness()
{
	return this->mHappiness;
}

void iAIAgent::setFatigue(const S32 fatigue)
{
	if ((fatigue >= 0) && (fatigue <= 100))
	{
		this->mFatigue = fatigue;
	} else
	{
		if (fatigue < 0)
			this->mFatigue = 0;
		else
			this->mFatigue = 100;
	}
}

const S32 iAIAgent::getFatigue()
{
	return this->mFatigue;
}

void iAIAgent::setMoney(const F32 money)
{
	if ((money >= 0) && (money <= 10000))
	{
		this->mMoney = money;
	} else
	{
		if (money < 0)
			this->mMoney = 0;
		else
			this->mMoney = 10000;
	}
}

const F32 iAIAgent::getMoney()
{
	return this->mMoney;
}

void iAIAgent::setBoredom(const S32 boredom)
{
	if ((boredom >= 0) && (boredom <= 100))
	{
		this->mBoredom = boredom;
	} else
	{
		if (boredom < 0)
			this->mBoredom = 0;
		else
			this->mBoredom = 100;
	}
}

const S32 iAIAgent::getBoredom()
{
	return this->mBoredom;
}

void iAIAgent::setLevel(const S32 level)
{
	if ((level >= 1) && (level <= 100))
	{
		this->mLevel = level;
	} else
	{
		if (level < 1)
			this->mLevel = 1;
		else
			this->mLevel = 100;
	}
}

const S32 iAIAgent::getLevel()
{
	return this->mLevel;
}

ConsoleMethodGroupBegin(iAIAgent, ScriptFunctions, "iAIAgent Script Functions");

ConsoleMethod( iAIAgent, getCurrentPath, S32, 2, 2,
			  "S32 iAIAgent.getCurrentPath() - Retrieves the current path for the agent.")
{
	iAIPath *currentPath = object->getCurrentPath();
	if (currentPath)
		return (currentPath->getId());
	else
		return 0;
}

ConsoleMethod( iAIAgent, setCurrentPath, bool, 3, 3,
			  "bool iAIAgent.setCurrentPath(iAIPath path) - Sets the current path for the agent.")
{
	// ensure pos passed
	if (dStrlen(argv[2]) != 0)
	{
		// find the path in the Sim and set the agent path
		iAIPath *path;
		if (Sim::findObject(dAtoi(argv[2]), path))
		{
			object->setCurrentPath(path);
			return true;
		} else
		{
			Con::errorf("Immersive AI :: Agent :: Path [%d] not found.", dAtoi(argv[2]));
			return false;
		}
	} else
	{
		Con::errorf("Immersive AI :: Agent :: path not passed!");
		return false;
	}
}

//-------------------------------------------------------------------
// Variable accessors/mutators
//-------------------------------------------------------------------

ConsoleMethod( iAIAgent, setAgentType, void, 3, 3,
			  "void iAIAgent.setAgentType(string agentType) - Set the type of agent.")
{
	object->setAgentType(argv[2]);
}

ConsoleMethod( iAIAgent, getAgentType, const char*, 2, 2,
			  "string iAIAgent.getAgentType() - Get the type of agent.")
{
	const char *agentType = object->getAgentType();
	char *returnBuffer = Con::getReturnBuffer(dStrlen(agentType) + 1);
	dSprintf(returnBuffer, sizeof(returnBuffer), "%s", agentType);

	return returnBuffer;
}

ConsoleMethod( iAIAgent, setHealth, void, 3, 3,
			  "void iAIAgent.setHealth(S8 health) - Set the health level.")
{
	object->setHealth(dAtoi(argv[2]));
}

ConsoleMethod( iAIAgent, getHealth, S32, 2, 2,
			  "S8 iAIAgent.getHealth() - Get the health level.")
{
	return object->getHealth();
}

ConsoleMethod( iAIAgent, setHappiness, void, 3, 3,
			  "void iAIAgent.setHappiness(S32 happiness) - Set the happiness level.")
{
	object->setHappiness(dAtoi(argv[2]));
}

ConsoleMethod( iAIAgent, getHappiness, S32, 2, 2,
			  "S8 iAIAgent.getHappiness() - Get the happiness level.")
{
	return object->getHappiness();
}

ConsoleMethod( iAIAgent, setFatigue, void, 3, 3,
			  "void iAIAgent.setFatigue(S32 fatigue) - Set the fatigue level.")
{
	object->setFatigue(dAtoi(argv[2]));
}

ConsoleMethod( iAIAgent, getFatigue, S32, 2, 2,
			  "S8 iAIAgent.getFatigue() - Get the fatigue level.")
{
	return object->getFatigue();
}

ConsoleMethod( iAIAgent, setMoney, void, 3, 3,
			  "void iAIAgent.setMoney(F32 money) - Set the money level.")
{
	object->setMoney(dAtof(argv[2]));
}

ConsoleMethod( iAIAgent, getMoney, S32, 2, 2,
			  "F32 iAIAgent.getMoney() - Get the money level.")
{
	return object->getMoney();
}

ConsoleMethod( iAIAgent, setBoredom, void, 3, 3,
			  "void iAIAgent.setBoredom(S32 boredom) - Set the boredom level.")
{
	object->setBoredom(dAtoi(argv[2]));
}

ConsoleMethod( iAIAgent, getBoredom, S32, 2, 2,
			  "S8 iAIAgent.getBoredom() - Get the boredom level.")
{
	return object->getBoredom();
}

ConsoleMethod( iAIAgent, setLevel, void, 3, 3,
			  "void iAIAgent.setLevel(S32 level) - Set the agent level.")
{
	object->setLevel(dAtoi(argv[2]));
}

ConsoleMethod( iAIAgent, getLevel, S32, 2, 2,
			  "S8 iAIAgent.getLevel() - Get the agent level.")
{
	return object->getLevel();
}

ConsoleMethodGroupEnd(iAIAgent, ScriptFunctions);