//-------------------------------------------------------------------
// Immersive AI :: Goal :: getHealth
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Load all the solution files
//-------------------------------------------------------------------
exec("./buyHealth.cs");
exec("./seekHealth.cs");
exec("./stealHealth.cs");

//-------------------------------------------------------------------
// Add goal and solutions to library for each agent
//-------------------------------------------------------------------
$iAIGoalLibrary.addGoal($iAIAgentType_Bandit, "getHealth");
$iAIGoalLibrary.addSolution($iAIAgentType_Bandit, "getHealth", "seekHealth");
$iAIGoalLibrary.addSolution($iAIAgentType_Bandit, "getHealth", "stealHealth");

$iAIGoalLibrary.addGoal($iAIAgentType_Entertainer, "getHealth");
$iAIGoalLibrary.addSolution($iAIAgentType_Entertainer, "getHealth", "buyHealth");
$iAIGoalLibrary.addSolution($iAIAgentType_Entertainer, "getHealth", "stealHealth");

$iAIGoalLibrary.addGoal($iAIAgentType_Soldier, "getHealth");
$iAIGoalLibrary.addSolution($iAIAgentType_Soldier, "getHealth", "buyHealth");
$iAIGoalLibrary.addSolution($iAIAgentType_Soldier, "getHealth", "seekHealth");

//-------------------------------------------------------------------
/// @fn getHealth_evaluate(%agent)
/// @brief Called to determine whether to assign this goal.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function getHealth_evaluate(%agent)
{
   // check if outside ranges to get health
   if (%agent.getHealth() >= 50)
   {
      return 0.0;
   }
   
   // calculate the weightings of each parameter
   %healthWeight = (50 - %agent.getHealth()) / 50;
   
   // average the weightings (div 10 to allow for futher weightings)
   %avgWeight = (%healthWeight / 1) / 10;
   
   // multiply by agent weight for this goal
   switch$(%agent.getAgentType())
   {
      case $iAIAgentType_Bandit: return (%avgWeight * 1);
      case $iAIAgentType_Entertainer: return (%avgWeight * 1); 
      case $iAIAgentType_Soldier: return (%avgWeight * 1);
      default : return 0.0;   
   }
}