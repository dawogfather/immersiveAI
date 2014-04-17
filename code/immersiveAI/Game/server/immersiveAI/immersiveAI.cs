//-------------------------------------------------------------------
// Immersive AI
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

// agent types
$iAIAgentType_Bandit = "bandit";
$iAIAgentType_Bandit_Code = 1;
$iAIAgentType_Entertainer = "entertainer";
$iAIAgentType_Entertainer_Code = 2;
$iAIAgentType_Soldier = "soldier";
$iAIAgentType_Soldier_Code = 3;

// execute the main managers/libraries
exec("./agent/iAIAgent.cs");
exec("./agent/iAIAgentManager.cs");
exec("./goals/iAIGoalLibrary.cs");
exec("./goals/iAIGoalManager.cs");

// number of agents to spawn
$IAIAGENT_COUNT = 2;

// radius to search
$IAISEEK_RADIUS = 150;
$IAISEEK_CLOSE_ENOUGH = 10;

// time between check vitals loops
$IAIAGENT_CHECK_VITALS = 10000;

// vitals rates of decay
$IAIAGENT_VITALS_HAPPINESS = -10;
$IAIAGENT_VITALS_FATIGUE = 10;
$IAIAGENT_VITALS_BOREDOM = 10;

// time between thinking loops
$IAIAGENT_THINK = 2000;

// number of think ticks before an agent leaves a goal (if in same goal whole time)
$IAIAGENT_THINK_TICK_LIMIT = 60;

//-------------------------------------------------------------------
/// @fn immersiveAI_Initialize()
/// @brief Initializes the immersive AI system. Called when a game
///        is created.
///
//-------------------------------------------------------------------
function immersiveAI_Initialize()
{
   // create & init the goal library
   $iAIGoalLibrary = new iAIGoalLibrary();
   MissionCleanup.add($iAIGoalLibrary);
   $iAIGoalLibrary.initialize();

   // create & init the goal manager
   new ScriptObject(iAIGoalManager) {};
   MissionCleanup.add(iAIGoalManager);
   iAIGoalManager.initialize();

   // create the path map
   $iAIPathMap = new iAIPathMap();
   MissionCleanup.add($iAIPathMap);

   // init pathmap for the current mission
   if ($iAIPathMap.Initialize())
   {
      // start the iAIAgentManager
      new ScriptObject(iAIAgentManager) {};
      MissionCleanup.add(iAIAgentManager);
      iAIAgentManager.initialize();
   }
}

//-------------------------------------------------------------------
/// @fn getRandomPoint()
/// @brief Helper function to get a random position in the world.
///
/// @returns Point3F Random position in the world.
//-------------------------------------------------------------------
function getRandomPoint()
{
   %randX = getRandom(-150, 600);
   %randY = getRandom(-150, 600);
   %randZ = getTerrainHeight(%randX SPC %randY) + 50;
   return %randX SPC %randY SPC %randZ;
}

