//-------------------------------------------------------------------
// Immersive AI :: Goal :: doRest
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Load all the solution files
//-------------------------------------------------------------------
exec("./relax.cs");
exec("./sleep.cs");

//-------------------------------------------------------------------
// Add goal and solutions to library for each agent
//-------------------------------------------------------------------
$iAIGoalLibrary.addGoal($iAIAgentType_Bandit, "doRest");
$iAIGoalLibrary.addSolution($iAIAgentType_Bandit, "doRest", "relax");
$iAIGoalLibrary.addSolution($iAIAgentType_Bandit, "doRest", "sleep");

$iAIGoalLibrary.addGoal($iAIAgentType_Entertainer, "doRest");
$iAIGoalLibrary.addSolution($iAIAgentType_Entertainer, "doRest", "relax");
$iAIGoalLibrary.addSolution($iAIAgentType_Entertainer, "doRest", "sleep");

$iAIGoalLibrary.addGoal($iAIAgentType_Soldier, "doRest");
$iAIGoalLibrary.addSolution($iAIAgentType_Soldier, "doRest", "relax");
$iAIGoalLibrary.addSolution($iAIAgentType_Soldier, "doRest", "sleep");

//-------------------------------------------------------------------
/// @fn doRest_evaluate(%agent)
/// @brief Called to determine whether to assign this goal.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function doRest_evaluate(%agent)
{
   // check if outside ranges to do rest
   if ((%agent.getFatigue() <= 33) || (%agent.getHealth() >= 50))
   {
      return 0.0;
   }
   
   // calculate the weightings of each parameter
   %fatigueWeight = (%agent.getFatigue() - 33) / 33;
   %healthWeight = (50 - %agent.getHealth()) / 50;
   
   // average the weightings (div 10 to allow for futher weightings)
   %avgWeight = ((%fatigueWeight + %healthWeight) / 2) / 10;
   
   // multiply by agent weight for this goal
   switch$(%agent.getAgentType())
   {
      case $iAIAgentType_Entertainer: return (%avgWeight * 2); 
      case $iAIAgentType_Bandit: return (%avgWeight * 1);
      case $iAIAgentType_Soldier: return (%avgWeight * 3);
      default : return 0.0;   
   }
}