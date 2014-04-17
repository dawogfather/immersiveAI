//-------------------------------------------------------------------
// Immersive AI :: Goal :: doHunt
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Load all the solution files
//-------------------------------------------------------------------
exec("./seekAndDestroy.cs");
exec("./patrolArea.cs");

//-------------------------------------------------------------------
// Add goal and solutions to library for each agent
//-------------------------------------------------------------------
$iAIGoalLibrary.addGoal($iAIAgentType_Bandit, "doHunt");
$iAIGoalLibrary.addSolution($iAIAgentType_Bandit, "doHunt", "seekAndDestroy");
$iAIGoalLibrary.addSolution($iAIAgentType_Bandit, "doHunt", "patrolArea");

$iAIGoalLibrary.addGoal($iAIAgentType_Soldier, "doHunt");
$iAIGoalLibrary.addSolution($iAIAgentType_Soldier, "doHunt", "seekAndDestroy");
$iAIGoalLibrary.addSolution($iAIAgentType_Soldier, "doHunt", "patrolArea");

//-------------------------------------------------------------------
/// @fn doHunt_evaluate(%agent)
/// @brief Called to determine whether to assign this goal.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function doHunt_evaluate(%agent)
{
   // check if outside ranges to do hunt
   if ((%agent.getHealth() <= 50) || (%agent.getFatigue() >= 33) || (%agent.getHappiness() <= 33))
   {
      return 0.0;
   }
   
   // calculate the weightings of each parameter
   %fatigueWeight = (33 - %agent.getFatigue()) / 33;
   %happinessWeight = (%agent.getHappiness() - 33) / 33;
   %healthWeight = (%agent.getHealth() - 50) / 50;
   
   // average the weightings (div 10 to allow for futher weightings)
   %avgWeight = ((%fatigueWeight + %happinessWeight + %healthWeight) / 3) / 10;
   
   // multiply by agent weight for this goal
   switch$(%agent.getAgentType())
   {
      case $iAIAgentType_Soldier: return (%avgWeight * 4); 
      case $iAIAgentType_Bandit: return (%avgWeight * 2);
      default : return 0.0;   
   }
}