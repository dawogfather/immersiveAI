//-------------------------------------------------------------------
// Immersive AI :: Goal :: doRest :: Solution :: relax
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/// @fn relax_evaluate(%agent)
/// @brief Called to determine whether to assign this solution.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function relax_evaluate(%agent)
{
   // check if outside ranges to relax
   if ((%agent.getFatigue() <= 33))
   {
      return 0.0;
   }
   
   // calculate the weightings of each parameter
   %fatigueWeight = (%agent.getFatigue() - 33) / 33;
   
   // average the weightings (div 10 to allow for futher weightings)
   %avgWeight = ((%fatigueWeight) / 1) / 10;
   
   // multiply by agent weight for this goal
   switch$(%agent.getAgentType())
   {
      case $iAIAgentType_Entertainer: return (%avgWeight * 5); 
      case $iAIAgentType_Bandit: return (%avgWeight * 4);
      case $iAIAgentType_Soldier: return (%avgWeight * 6);
      default : return 0.0;   
   }
}

//-------------------------------------------------------------------
/// @fn relax_onEnter(%agent)
/// @brief Called on entry to this solution state.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function relax_onEnter(%agent)
{
   iAIMessage("STATE: relax being executed by " @ %agent);
   relax_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn relax_execute(%agent)
/// @brief Main execution state for the agent, called from onEnter.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function relax_execute(%agent)
{
   // delete any current path
   if (isObject(%agent.getCurrentPath()))
      %agent.getCurrentPath().delete();
      
   // stop moving
   %agent.stop();
      
   // a nice relax animation
   %agent.setActionThread("Death8", true);
   
   // relax for half a minute
   schedule(30000, 0, "relax_onExit", %agent);
}

//-------------------------------------------------------------------
/// @fn relax_onReachDestination(%agent)
/// @brief Callback from onReachDestination. When agent reaches 
///        end point on path, function is called.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function relax_onReachDestination(%agent)
{
   //
}

//-------------------------------------------------------------------
/// @fn relax_onCheckVitals(%agent)
/// @brief Callback from checkVitals.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function relax_onCheckVitals(%agent)
{
   %agent.setFatigue(%agent.getFatigue() - ($IAIAGENT_VITALS_FATIGUE*2));
   %agent.setBoredom(%agent.getBoredom() - ($IAIAGENT_VITALS_BOREDOM*2));
}

//-------------------------------------------------------------------
/// @fn relax_onExit(%agent)
/// @brief Called on exiting from this solution, called from execute.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function relax_onExit(%agent)
{
   // cancel the relax animation
   %agent.setActionThread("look");
   %agent.setFatigue(%agent.getFatigue() - 50);
   %agent.setBoredom(%agent.getBoredom() - 50);
   %agent.setHappiness(%agent.getHappiness() + 15);

   iAIGoalManager.completedSolution(%agent);
}