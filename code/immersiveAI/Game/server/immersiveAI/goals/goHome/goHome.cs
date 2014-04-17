//-------------------------------------------------------------------
// Immersive AI :: Goal :: goHome
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Load all the solution files
//-------------------------------------------------------------------
exec("./seekHome.cs");

//-------------------------------------------------------------------
// Add goal and solutions to library for each agent
//-------------------------------------------------------------------
$iAIGoalLibrary.addGoal($iAIAgentType_Bandit, "goHome");
$iAIGoalLibrary.addSolution($iAIAgentType_Bandit, "goHome", "seekHome");

$iAIGoalLibrary.addGoal($iAIAgentType_Entertainer, "goHome");
$iAIGoalLibrary.addSolution($iAIAgentType_Entertainer, "goHome", "seekHome");

$iAIGoalLibrary.addGoal($iAIAgentType_Soldier, "goHome");
$iAIGoalLibrary.addSolution($iAIAgentType_Soldier, "goHome", "seekHome");

//-------------------------------------------------------------------
/// @fn goHome_evaluate(%agent)
/// @brief Called to determine whether to assign this goal.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function goHome_evaluate(%agent)
{
   // check if outside ranges to go home
   if ((%agent.getHealth() >= 66) || (%agent.getFatigue() <= 33) ||
      (%agent.getMoney() >= 200) || (%agent.getBoredom() <= 33))
   {
      return (getRandom(0, 2) / 100);
   }
   
   // calculate the weightings of each parameter
   %healthWeight = (66 - %agent.getHealth()) / 66;
   %fatigueWeight = (%agent.getFatigue() - 33) / 33;
   %moneyWeight = (%agent.getMoney() - 200) / 200;
   %boredomWeight = (%agent.getBoredom() - 33) / 33;
   
   // average the weightings (div 10 to allow for futher weightings)
   %avgWeight = ((%healthWeight + %fatigueWeight + %moneyWeight + %boredomWeight) / 4) / 10;
   
   // multiply by agent weight for this goal
   switch$(%agent.getAgentType())
   {
      case $iAIAgentType_Entertainer: return (%avgWeight * 2); 
      case $iAIAgentType_Bandit: return (%avgWeight * 1);
      case $iAIAgentType_Soldier: return (%avgWeight * 1);
      default : return 0.0;
   }
}