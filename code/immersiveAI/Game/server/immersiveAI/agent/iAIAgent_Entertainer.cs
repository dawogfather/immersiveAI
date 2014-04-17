//-------------------------------------------------------------------
// Immersive AI :: iAIAgent_Entertainer
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------
exec("client/data/shapes/avatars/armygirl/armygirl.cs");

datablock PlayerData(iAIAgent_Entertainer : iAIAgentData)
{
   shapeFile = "client/data/shapes/avatars/armygirl/armygirl.dts";
};

//-------------------------------------------------------------------
/// @fn iAIAgent_Entertainer::spawn(%name, %spawnPoint)
/// @brief Creates and spawns a new Entertainer into the game world,
///        with the parsed name and at the spawn point.
///
/// @param %name Name of the agent.
/// @param %spawnPoint Position in the world to spawn.
//-------------------------------------------------------------------
function iAIAgent_Entertainer::spawn(%name, %spawnPoint)
{
   // create the new agent
   %newAgent = new iAIAgent() {
      dataBlock = iAIAgent_Entertainer;
   };
   MissionCleanup.add(%newAgent);
   
   // set the agent type
   %newAgent.setAgentType($iAIAgentType_Entertainer);

   // set the agents name and place at spawn point
   %newAgent.setShapeName(%name SPC "[" @ %newAgent.getAgentType() SPC %newAgent.getId() @ "]");
   %newAgent.setTransform(%spawnPoint);
   
   // set the agents inventory and mount a weapon
   %newAgent.setInventory(Sword, 1);
   %newAgent.mountImage(SwordImage, 0);

   // retrieve the goal list for the agent type
   %newAgent.goalList = $iAIGoalLibrary.getGoalList($iAIAgentType_Entertainer);

   return %newAgent;
}

//-------------------------------------------------------------------
/// @fn iAIAgent_Entertainer::onMoveStuck(%this, %obj)
/// @brief Called on agent being stuck in the game world.
///
/// @param %this Datablock reference.
/// @param %obj Instance of the agent.
//-------------------------------------------------------------------
function iAIAgent_Entertainer::onMoveStuck(%this, %obj)
{
   // no special actions required, call the global onMoveStuck function
   iAIAgent::onMoveStuck(%this, %obj);
}

//-------------------------------------------------------------------
/// @fn iAIAgent_Entertainer::onReachDestination(%this, %obj)
/// @brief Called when an agent reaches their destination.
///
/// @param %this Datablock reference.
/// @param %obj Instance of the agent.
//-------------------------------------------------------------------
function iAIAgent_Entertainer::onReachDestination(%this, %obj)
{
   // no special actions required, call the global onReachDestination
   iAIAgent::onReachDestination(%this, %obj);
}