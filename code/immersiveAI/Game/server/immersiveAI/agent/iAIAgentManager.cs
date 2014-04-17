//-------------------------------------------------------------------
// Immersive AI :: iAIAgentManager
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/// @fn initialize(%this)
/// @brief Initializes the iAIAgentManager. Called when a new game is
///        created.
///
/// @param %this Instance of iAIAgentManager.
//-------------------------------------------------------------------
function iAIAgentManager::initialize(%this)
{
   // set initial agent count to 0
   %this.agentCount = 0;
   
   // waits 15 seconds after game creation to start the thinking loop
   %this.schedule(15000, think);
}

//-------------------------------------------------------------------
/// @fn think(%this)
/// @brief Main thinking loop. Called every 2seconds.
///        Detects if new agents need to be spawned.
///
/// @param %this Instance of iAIManager.
//-------------------------------------------------------------------
function iAIAgentManager::think(%this)
{
   // check if need to spawn more agents
   if (%this.agentCount < $IAIAGENT_COUNT)
   {
      // spawn an agent!
      %this.agentCount++;
      %spawnedAgent = %this.spawn();
      iAIMessage("Spawned agent " @ %spawnedAgent.getId() @ " #" @ %this.agentCount @ "/" @ $IAIAGENT_COUNT);
   }
   
   // reschedule the thinking loop
   %this.schedule(2000, think);
}

//-------------------------------------------------------------------
/// @fn spawn(%this, %agentTypeCode = random)
/// @brief Spawns a new agent into the game world. If %agentTypeCode
///        is not specified, a random agent type is chosen.
///
/// @param %this Instance of iAIManager.
/// @param %agentType Agent type to spawn in the world.
/// @returns iAIAgent Agent spawned.
//-------------------------------------------------------------------
function iAIAgentManager::spawn(%this, %agentTypeCode)
{
   // check if agent type code not passed
   if (!(%agentTypeCode > 0))
   {
      // get a random agent type
      %agentTypeCode = getRandom(1, 3);
   }
   
   // create a new agent, based on type code
   switch(%agentTypeCode)
   {
      case $iAIAgentType_Bandit_Code: %newAgent = iAIAgent_Bandit::spawn(createRandomName("client/data/names.nam"), pickSpawnPoint());
      case $iAIAgentType_Entertainer_Code: %newAgent = iAIAgent_Entertainer::spawn(createRandomName("client/data/names.nam"), pickSpawnPoint());
      case $iAIAgentType_Soldier_Code: %newAgent = iAIAgent_Soldier::spawn(createRandomName("client/data/names.nam"), pickSpawnPoint());
   }
   
   // set the agent vitals
   %newAgent.setHappiness(100);
   %newAgent.setMoney(200.0);
   %newAgent.setHealth(100);
   %newAgent.setFatigue(0);
   %newAgent.setBoredom(0);
   
   // set default path options
   %newAgent.path_spline = false;
   %newAgent.path_show = true;
   
   // set as not in combat
   %newAgent.combat_InCombat = false;
   
   // schedule a check vitals
   %newAgent.schedule($IAIAGENT_CHECK_VITALS, checkVitals);

   // add to gui agent list
   iAICC_AgentList.addRow(%newAgent.getId(), %newAgent.getShapeName());
   
   // request a goal
   %newAgent.goalInterrupt = false;
   %newAgent.resumePrevious = false;
   %newAgent.lastTick_ticks = 0;
   %newAgent.lastTick_Goal = "";
   iAIGoalManager.requestNewGoal(%newAgent);

   // start the thinking loop
   %newAgent.schedule($IAIAGENT_THINK, think);

   return %newAgent;
}