//-------------------------------------------------------------------
// Immersive AI :: Goal :: getFood
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Load all the solution files
//-------------------------------------------------------------------
exec("./buyFood.cs");
exec("./seekFood.cs");
exec("./stealFood.cs");

//-------------------------------------------------------------------
// Add goal and solutions to library for each agent
//-------------------------------------------------------------------
$iAIGoalLibrary.addGoal($iAIAgentType_Bandit, "getFood");
$iAIGoalLibrary.addSolution($iAIAgentType_Bandit, "getFood", "seekFood");
$iAIGoalLibrary.addSolution($iAIAgentType_Bandit, "getFood", "stealFood");

$iAIGoalLibrary.addGoal($iAIAgentType_Entertainer, "getFood");
$iAIGoalLibrary.addSolution($iAIAgentType_Entertainer, "getFood", "buyFood");
$iAIGoalLibrary.addSolution($iAIAgentType_Entertainer, "getFood", "stealFood");

$iAIGoalLibrary.addGoal($iAIAgentType_Soldier, "getFood");
$iAIGoalLibrary.addSolution($iAIAgentType_Soldier, "getFood", "buyFood");
$iAIGoalLibrary.addSolution($iAIAgentType_Soldier, "getFood", "seekFood");

//-------------------------------------------------------------------
/// @fn getFood_evaluate(%agent)
/// @brief Called to determine whether to assign this goal.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function getFood_evaluate(%agent)
{
   // check if outside ranges to get food
   if ((%agent.getHealth() >= 80) || (%agent.getHappiness() >= 80))
   {
      return 0.0;
   }
   
   // calculate the weightings of each parameter
   %healthWeight = (80 - %agent.getHealth()) / 50;
   %happinessWeight = (80 - %agent.getHappiness()) / 80;
   
   // average the weightings (div 10 to allow for futher weightings)
   %avgWeight = ((%healthWeight + %happinessWeight) / 2) / 10;
   
   // multiply by agent weight for this goal
   switch$(%agent.getAgentType())
   {
      case $iAIAgentType_Bandit: return (%avgWeight * 2);
      case $iAIAgentType_Entertainer: return (%avgWeight * 1); 
      case $iAIAgentType_Soldier: return (%avgWeight * 1);
      default : return 0.0;   
   }
}