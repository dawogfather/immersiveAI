//-------------------------------------------------------------------
// Immersive AI :: Agent
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/// @file iAIAgent.h
//-------------------------------------------------------------------
/// @class iAIAgent
/// @author Gavin Bunney
/// @version 1.0
/// @brief The AI Agent within the game world.
/// 
/// The iAIAgent class holds all information about an agent within
/// the game world. It maintains its current goal list, state and
/// vital statistics.<br><br>
/// It is an extension to the standard Torque AIPlayer class. The
/// AIPlayer class handles the movement within the game world and
/// the process of aiming towards an object. All other agent logic is
/// contained within this iAIAgent class.<br><br>
///
/// TypeMask |= iAIAgentObjectType
//-------------------------------------------------------------------

#ifndef _IAIAGENT_H_
#define _IAIAGENT_H_

#include "game/aiPlayer.h"
#include "immersiveAI/seek/path/iAIPath.h"

class iAIAgent : public AIPlayer
{
	typedef AIPlayer Parent;
	friend class iAIGoalManager;

public:

	//-------------------------------------------------------------------
	/// @var DECLARE_CONOBJECT(iAIAgent)
	/// @brief TorqueScript object.
	//-------------------------------------------------------------------
	DECLARE_CONOBJECT(iAIAgent);
	
	//-------------------------------------------------------------------
	/// @fn iAIAgent() 
	/// @brief Default constructor.
	//-------------------------------------------------------------------
	iAIAgent();

	//-------------------------------------------------------------------
	/// @fn ~iAIAgent() 
	/// @brief Destructor.
	//-------------------------------------------------------------------
	~iAIAgent();

	//-------------------------------------------------------------------
	/// @fn static void initPersistFields()
	/// @brief Exposes some variables to TorqueScript.
	//-------------------------------------------------------------------
	static void initPersistFields();

	//-------------------------------------------------------------------
	/// @fn void setCurrentPath(iAIPath* path)
	/// @brief Sets the Agent's path to the parsed path.
	///
	/// @param path Pointer to the iAIPath for this agent.
	//-------------------------------------------------------------------
	void setCurrentPath(iAIPath* path) { this->mCurrentPath = path; }

	//-------------------------------------------------------------------
	/// @fn iAIPath* getCurrentPath()
	/// @brief Returns the Agent's current path.
	///
	/// @return iAIPath Pointer to the Agent's current path.
	//-------------------------------------------------------------------
	iAIPath* getCurrentPath() { return this->mCurrentPath; }

	//-------------------------------------------------------------------
	/// @fn void setAgentType(const char* agentType)
	/// @brief Set the type of agent.
	///
	/// @param agentType Type of the agent.
	//-------------------------------------------------------------------
	void setAgentType(const char* agentType) { dSprintf(this->mAgentType, sizeof(this->mAgentType), "%s", agentType); }
	
	//-------------------------------------------------------------------
	/// @fn const char* getAgentType()
	/// @brief Retrieve the type of agent.
	///
	/// @return char* Type of the agent.
	//-------------------------------------------------------------------
	const char* getAgentType() const { return this->mAgentType; }

	//-------------------------------------------------------------------
	/// @fn void setHealth(const S32 health)
	/// @brief Set the current health value.
	///
	/// @param health Health value to set to.
	//-------------------------------------------------------------------
	void setHealth(const S32 health);

	//-------------------------------------------------------------------
	/// @fn const S32 getHealth()
	/// @brief Retrieve the current health value.
	///
	/// @return S32 Health value.
	//-------------------------------------------------------------------
	const S32 getHealth();

	//-------------------------------------------------------------------
	/// @fn void setHappiness(const S32 happiness)
	/// @brief Set the current happiness value.
	///
	/// @param happiness Happiness value to set to.
	//-------------------------------------------------------------------
	void setHappiness(const S32 happiness);

	//-------------------------------------------------------------------
	/// @fn const S32 getHappiness()
	/// @brief Retrieve the current happiness value.
	///
	/// @return S8 Happiness value.
	//-------------------------------------------------------------------
	const S32 getHappiness();

	//-------------------------------------------------------------------
	/// @fn void setFatigue(const S32 fatigue)
	/// @brief Set the current fatigue value.
	///
	/// @param fatigue Fatigue value to set to.
	//-------------------------------------------------------------------
	void setFatigue(const S32 fatigue);

	//-------------------------------------------------------------------
	/// @fn const S32 getFatigue()
	/// @brief Retrieve the current fatigue value.
	///
	/// @return S8 Fatigue value.
	//-------------------------------------------------------------------
	const S32 getFatigue();

	//-------------------------------------------------------------------
	/// @fn void setMoney(const F32 money)
	/// @brief Set the current money value.
	///
	/// @param money Money value to set to.
	//-------------------------------------------------------------------
	void setMoney(const F32 money);
	
	//-------------------------------------------------------------------
	/// @fn const F32 getMoney()
	/// @brief Retrieve the current money value.
	///
	/// @return F32 Money value.
	//-------------------------------------------------------------------
	const F32 getMoney();

	//-------------------------------------------------------------------
	/// @fn void setBoredom(const S32 boredom)
	/// @brief Set the current boredom value.
	///
	/// @param boredom Boredom value to set to.
	//-------------------------------------------------------------------
	void setBoredom(const S32 boredom);
	
	//-------------------------------------------------------------------
	/// @fn const S32 getBoredom()
	/// @brief Retrieve the current boredom value.
	///
	/// @return S32 Boredom value.
	//-------------------------------------------------------------------
	const S32 getBoredom();

	//-------------------------------------------------------------------
	/// @fn void setLevel(const S32 level)
	/// @brief Set the current level value.
	///
	/// @param level Level value to set to.
	//-------------------------------------------------------------------
	void setLevel(const S32 level);
	
	//-------------------------------------------------------------------
	/// @fn const S32 getLevel()
	/// @brief Retrieve the current level value.
	///
	/// @return S32 Level value.
	//-------------------------------------------------------------------
	const S32 getLevel();

protected:

	//-------------------------------------------------------------------
	/// @var char mAgentType[255]
	/// @brief Name of the agent's type.
	//-------------------------------------------------------------------
	char mAgentType[255];

	//-------------------------------------------------------------------
	/// @var iAIPath* mCurrentPath
	/// @brief Pointer to the Agent's current path.
	//-------------------------------------------------------------------
	iAIPath* mCurrentPath;

	//-------------------------------------------------------------------
	/// @var S32 mHappiness
	/// @brief Happiness level of the agent. 0 is really angry, 100 is
	///        really happy.
	//-------------------------------------------------------------------
	S32 mHappiness;

	//-------------------------------------------------------------------
	/// @var S32 mFatigue
	/// @brief Fatigue level of the agent. 0 is really awake, 100 is 
	///        really tired.
	//-------------------------------------------------------------------
	S32 mFatigue;

	//-------------------------------------------------------------------
	/// @var F32 mMoney
	/// @brief Amount of money the agent has. 0 is really poor, 10 000 is
	///        really rich.
	//-------------------------------------------------------------------
	F32 mMoney;

	//-------------------------------------------------------------------
	/// @var S32 mBoredom
	/// @brief Boredom level of the agent. 0 is really excited, 100 is 
	///        really bored.
	//-------------------------------------------------------------------
	S32 mBoredom;

	//-------------------------------------------------------------------
	/// @var S32 mLevel
	/// @brief Level of the agent. 1 is really weak, 100 is really
	///        strong.
	//-------------------------------------------------------------------
	S32 mLevel;

private:
	
	//-------------------------------------------------------------------
	/// @fn void executeFunction(const char *name)
	/// @brief Executes a TorqueScript function
	///
	/// @param name The name of the function to execute.
	//-------------------------------------------------------------------
	void executeFunction(const char *name);

	//-------------------------------------------------------------------
	/// @fn void executeDatablockFunction(const char *name)
	/// @brief Executes a TorqueScript function contained within the
	///        agent's datablock.
	///
	/// @param name The name of the function to execute.
	//-------------------------------------------------------------------
	void executeDatablockFunction(const char *name);
};

#endif
