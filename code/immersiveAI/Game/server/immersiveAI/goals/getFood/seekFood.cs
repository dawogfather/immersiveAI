//-------------------------------------------------------------------
// Immersive AI :: Goal :: getFood :: Solution :: seekFood
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/// @fn seekFood_evaluate(%agent)
/// @brief Called to determine whether to assign this solution.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function seekFood_evaluate(%agent)
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
/// @fn seekFood_onEnter(%agent)
/// @brief Called on entry to this solution state.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function seekFood_onEnter(%agent)
{
   iAIMessage("STATE: seekFood being executed by " @ %agent);

   seekFood_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn seekFood_execute(%agent)
/// @brief Main execution state for the agent, called from onEnter.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function seekFood_execute(%agent)
{
   // seek for the health patch
   %agent.seekObject("FoodItem", $TypeMasks::ItemObjectType);
   %agent.seek_OnObject = false;
}

//-------------------------------------------------------------------
/// @fn seekFood_onReachDestination(%agent)
/// @brief Callback from onReachDestination. When agent reaches 
///        end point on path, function is called.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function seekFood_onReachDestination(%agent)
{
   // check if reached the food
   if (%agent.seek_OnObject == true)
   {
      seekFood_onExit(%agent);
   }
}

//-------------------------------------------------------------------
/// @fn seekFood_onCheckVitals(%agent)
/// @brief Callback from checkVitals.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function seekFood_onCheckVitals(%agent)
{
   %agent.setFatigue(%agent.getFatigue() + ($IAIAGENT_VITALS_FATIGUE*2));
}

//-------------------------------------------------------------------
/// @fn seekFood_onExit(%agent)
/// @brief Called on exiting from this solution, called from execute.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function seekFood_onExit(%agent)
{
   iAIGoalManager.completedSolution(%agent);
}