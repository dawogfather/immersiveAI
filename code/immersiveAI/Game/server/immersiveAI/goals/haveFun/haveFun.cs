//-------------------------------------------------------------------
// Immersive AI :: Goal :: haveFun
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Load all the solution files
//-------------------------------------------------------------------
exec("./dance.cs");

//-------------------------------------------------------------------
// Add goal and solutions to library for each agent
//-------------------------------------------------------------------
$iAIGoalLibrary.addGoal($iAIAgentType_Bandit, "haveFun");
$iAIGoalLibrary.addSolution($iAIAgentType_Bandit, "haveFun", "dance");

$iAIGoalLibrary.addGoal($iAIAgentType_Entertainer, "haveFun");
$iAIGoalLibrary.addSolution($iAIAgentType_Entertainer, "haveFun", "dance");

//-------------------------------------------------------------------
/// @fn haveFun_evaluate(%agent)
/// @brief Called to determine whether to assign this goal.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function haveFun_evaluate(%agent)
{
   // check if outside ranges to have fun
   if ((%agent.getFatigue() <= 33) || (%agent.getHappiness() >= 33) ||
      (%agent.getBoredom() <= 33))
   {
      return (getRandom(0, 2) / 100);
   }
   
   // calculate the weightings of each parameter
   %fatigueWeight = (%agent.getFatigue() - 33) / 33;
   %happinessWeight = (33 - %agent.getHappiness()) / 33;
   %boredomWeight = (%agent.getBoredom() - 33) / 33;
   
   // average the weightings (div 10 to allow for futher weightings)
   %avgWeight = ((%fatigueWeight + %happinessWeight + %boredomWeight) / 3) / 10;
   
   // multiply by agent weight for this goal
   switch$(%agent.getAgentType())
   {
      case $iAIAgentType_Entertainer: return (%avgWeight * 3); 
      case $iAIAgentType_Bandit: return (%avgWeight * 1);
      default : return 0.0;   
   }
}
