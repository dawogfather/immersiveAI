//-------------------------------------------------------------------
// Immersive AI :: Goal :: doExplore
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Load all the solution files
//-------------------------------------------------------------------
exec("./exploreArea.cs");

//-------------------------------------------------------------------
// Add goal and solutions to library for each agent
//-------------------------------------------------------------------
$iAIGoalLibrary.addGoal($iAIAgentType_Bandit, "doExplore");
$iAIGoalLibrary.addSolution($iAIAgentType_Bandit, "doExplore", "exploreArea");

$iAIGoalLibrary.addGoal($iAIAgentType_Entertainer, "doExplore");
$iAIGoalLibrary.addSolution($iAIAgentType_Entertainer, "doExplore", "exploreArea");

//-------------------------------------------------------------------
/// @fn doExplore_evaluate(%agent)
/// @brief Called to determine whether to assign this goal.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function doExplore_evaluate(%agent)
{
   // check if outside ranges to do explore
   if ((%agent.getFatigue() >= 33) || (%agent.getHappiness() <= 50) ||
      (%agent.getBoredom() <= 50))
   {
      return 0.0;
   }
   
   // calculate the weightings of each parameter
   %fatigueWeight = (33 - %agent.getFatigue()) / 33;
   %happinessWeight = (%agent.getHappiness() - 50) / 50;
   %boredomWeight = (%agent.getBoredom() - 50) / 50;
   
   // average the weightings (div 10 to allow for futher weightings)
   %avgWeight = ((%fatigueWeight + %happinessWeight + %boredomWeight) / 3) / 10;
   
   // multiply by agent weight for this goal
   switch$(%agent.getAgentType())
   {
      case $iAIAgentType_Entertainer: return (%avgWeight * 1); 
      case $iAIAgentType_Bandit: return (%avgWeight * 2);
      default : return 0.0;   
   }
}