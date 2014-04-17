//-------------------------------------------------------------------
// Immersive AI :: Goal :: getHealth :: Solution :: seekHealth
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/// @fn seekHealth_evaluate(%agent)
/// @brief Called to determine whether to assign this solution.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function seekHealth_evaluate(%agent)
{
   // check if outside ranges to seek
   if (%agent.getFatigue() >= 50)
   {
      return 0.0;
   }
   
   // calcaulte weightings of each parameters
   %fatigueWeight = (50 - %agent.getFatigue()) / 50;
   
   // average the weightings (div 10 to allow for futher weightings)
   %avgWeight = ((%fatigueWeight) / 1) / 10;
   
   // multiply by agent weight for this solution
   switch$(%agent.getAgentType())
   {
      case $iAIAgentType_Bandit: return (%avgWeight * 5); 
      case $iAIAgentType_Soldier: return (%avgWeight * 5);
      default : return 0.0;   
   }
}

//-------------------------------------------------------------------
/// @fn seekHealth_onEnter(%agent)
/// @brief Called on entry to this solution state.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function seekHealth_onEnter(%agent)
{
   iAIMessage("STATE: seekHealth being executed by " @ %agent);
   
   seekHealth_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn seekHealth_execute(%agent)
/// @brief Main execution state for the agent, called from onEnter.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function seekHealth_execute(%agent)
{
   // seek for the health patch
   %agent.seekObject("HealthPatch", $TypeMasks::ItemObjectType);
   %agent.reachedHealthPatch = false;
}

//-------------------------------------------------------------------
/// @fn seekHealth_onReachDestination(%agent)
/// @brief Callback from onReachDestination. When agent reaches 
///        end point on path, function is called.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function seekHealth_onReachDestination(%agent)
{
   // check if reached health
   if (%agent.reachedHealthPatch == true)
   {
      seekHealth_onExit(%agent);
   }
}

//-------------------------------------------------------------------
/// @fn seekHealth_onCheckVitals(%agent)
/// @brief Callback from checkVitals.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function seekHealth_onCheckVitals(%agent)
{
   %agent.setFatigue(%agent.getFatigue() + ($IAIAGENT_VITALS_FATIGUE*2));
}

//-------------------------------------------------------------------
/// @fn seekHealth_onExit(%agent)
/// @brief Called on exiting from this solution, called from execute.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function seekHealth_onExit(%agent)
{
   iAIGoalManager.completedSolution(%agent);
}