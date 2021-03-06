//-------------------------------------------------------------------
// Immersive AI :: iAIAgent_Bandit
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------


datablock PlayerData(iAIAgent_Bandit : iAIAgentData)
{
   shapeFile = "client/data/shapes/avatars/orc/orc.dts";
};

//-------------------------------------------------------------------
/// @fn iAIAgent_Bandit::spawn(%name, %spawnPoint)
/// @brief Creates and spawns a new Bandit into the game world, with
///        the parsed name and at the spawn point.
///
/// @param %name Name of the agent.
/// @param %spawnPoint Position in the world to spawn.
//-------------------------------------------------------------------
function iAIAgent_Bandit::spawn(%name, %spawnPoint)
{
   // create the new agent
   %newAgent = new iAIAgent() {
      dataBlock = iAIAgent_Bandit;
   };
   MissionCleanup.add(%newAgent);
   
   // set the agent type
   %newAgent.setAgentType($iAIAgentType_Bandit);
   
   // set the agents name and place at spawn point
   %newAgent.setShapeName(%name SPC "[" @ %newAgent.getAgentType() SPC %newAgent.getId() @ "]");
   %newAgent.setTransform(%spawnPoint);

   // set the agents inventory and mount a weapon
   %newAgent.setInventory(CrossbowAmmo, 1000);
   %newAgent.mountImage(CrossbowImage, 0);
   
   // retrieve the goal list for the agent type
   %newAgent.goalList = $iAIGoalLibrary.getGoalList($iAIAgentType_Bandit);
   
   return %newAgent;
}

//-------------------------------------------------------------------
/// @fn iAIAgent_Bandit::onMoveStuck(%this, %obj)
/// @brief Called on agent being stuck in the game world.
///
/// @param %this Datablock reference.
/// @param %obj Instance of the agent.
//-------------------------------------------------------------------
function iAIAgent_Bandit::onMoveStuck(%this, %obj)
{
   // no special actions required, call the global onMoveStuck
   iAIAgent::onMoveStuck(%this, %obj);
}

//-------------------------------------------------------------------
/// @fn iAIAgent_Bandit::onReachDestination(%this, %obj)
/// @brief Called when an agent reaches their destination.
///
/// @param %this Datablock reference.
/// @param %obj Instance of the agent.
//-------------------------------------------------------------------
function iAIAgent_Bandit::onReachDestination(%this, %obj)
{
   // no special actions required, call the global onReachDestination
   iAIAgent::onReachDestination(%this, %obj);
}