//-------------------------------------------------------------------
// Immersive AI :: Goal :: doRest :: Solution :: sleep
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/// @fn sleep_evaluate(%agent)
/// @brief Called to determine whether to assign this solution.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function sleep_evaluate(%agent)
{
   // check if outside ranges to do sleep
   if ((%agent.getFatigue() <= 66) || (%agent.getBoredom() <= 50))
   {
      return 0.0;
   }
   
   // calculate the weightings of each parameter
   %fatigueWeight = (%agent.getFatigue() - 66) / 66;
   %boredomWeight = (%agent.getBoredom() - 50) / 50;
   
   // average the weightings (div 10 to allow for futher weightings)
   %avgWeight = ((%fatigueWeight + %boredomWeight) / 2) / 10;
   
   // multiply by agent weight for this goal
   switch$(%agent.getAgentType())
   {
      case $iAIAgentType_Entertainer: return (%avgWeight * 5); 
      case $iAIAgentType_Bandit: return (%avgWeight * 6);
      case $iAIAgentType_Soldier: return (%avgWeight * 4);
      default : return 0.0;   
   }
}

//-------------------------------------------------------------------
/// @fn sleep_onEnter(%agent)
/// @brief Called on entry to this solution state.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function sleep_onEnter(%agent)
{
   iAIMessage("STATE: sleep being executed by " @ %agent);
   
   // generate path to the home
   %agent.generatePath($HomeLocation);
   %agent.setMoveDestination(%agent.getCurrentPath().nextPosition());
   %agent.atHouse = false;
   sleep_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn sleep_execute(%agent)
/// @brief Main execution state for the agent, called from onEnter.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function sleep_execute(%agent)
{
   // check if at the house yet
   if (%agent.atHouse == false)
      return;
      
   // agent is at the house, so sleep
   %agent.setActionThread("Death8", true);
   
   // sleep for half a minute
   schedule(30000, 0, "sleep_onExit", %agent);
}

//-------------------------------------------------------------------
/// @fn sleep_onReachDestination(%agent)
/// @brief Callback from onReachDestination. When agent reaches 
///        end point on path, function is called.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function sleep_onReachDestination(%agent)
{
   // check if still on a path
   // arrived at the house
   %agent.atHouse = true;
   sleep_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn sleep_onCheckVitals(%agent)
/// @brief Callback from checkVitals.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function sleep_onCheckVitals(%agent)
{
   // check if at the house
   if (%agent.atHouse == true)
   {
      %agent.setFatigue(%agent.getFatigue() - ($IAIAGENT_VITALS_FATIGUE*4));
      %agent.setBoredom(%agent.getBoredom() - ($IAIAGENT_VITALS_BOREDOM*4));
   }
}

//-------------------------------------------------------------------
/// @fn sleep_oExit(%agent)
/// @brief Called on exiting from this solution, called from execute.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function sleep_onExit(%agent)
{
   %agent.atHouse = false;
   %agent.setActionThread("look");
   %agent.setHealth(%agent.getHealth() + 15);
   %agent.setFatigue(0);
   %agent.setBoredom(0);
   iAIGoalManager.completedSolution(%agent);
}