//-------------------------------------------------------------------
// Immersive AI :: Goal :: doDefend :: Solution :: fleeArea
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/// @fn fleeArea_evaluate(%agent)
/// @brief Called to determine whether to assign this solution.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function fleeArea_evaluate(%agent)
{
   if (%agent.getAgentType() $= $iAIAgentType_Entertainer)
      return 1.0;
   
   // check if outside ranges to flee area
   if ((%agent.getHealth() >= 50) && (%agent.getFatigue() <= 50))
   {
      return 0.0;
   }
   
   // calculate the weightings of each parameter
   %healthWeight = (50 - %agent.getHealth()) / 50;
   %fatigueWeight = (%agent.getFatigue() - 50) / 50;
   
   // average the weightings (div 10 to allow for futher weightings)
   %avgWeight = ((%healthWeight + %fatigueWeight) / 2) / 10;
   
   // multiply by agent weight for this goal
   switch$(%agent.getAgentType())
   {
      case $iAIAgentType_Soldier: return (%avgWeight * 10);
      default : return 0.0;   
   }
}

//-------------------------------------------------------------------
/// @fn fleeArea_onEnter(%agent)
/// @brief Called on entry to this solution state.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function fleeArea_onEnter(%agent)
{
   iAIMessage("STATE: fleeArea being executed by " @ %agent);
   
   fleeArea_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn fleeArea_execute(%agent)
/// @brief Main execution state for the agent, called from onEnter.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function fleeArea_execute(%agent)
{
   // delete any current path
   if (isObject(%agent.getCurrentPath()))
      %agent.getCurrentPath().delete();
      
   // generate a path to a random position
   %agent.generatePath(getRandomPoint());
   
   // move along the path
   %agent.setMoveDestination(%agent.getCurrentPath().nextPosition());
}

//-------------------------------------------------------------------
/// @fn fleeArea_onReachDestination(%agent)
/// @brief Callback from onReachDestination. When agent reaches 
///        end point on path, function is called.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function fleeArea_onReachDestination(%agent)
{
   // reached the end of the flee path
   fleeArea_onExit(%agent);
}

//-------------------------------------------------------------------
/// @fn fleeArea_onCheckVitals(%agent)
/// @brief Callback from checkVitals.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function fleeArea_onCheckVitals(%agent)
{
   %agent.setFatigue(%agent.getFatigue() + ($IAIAGENT_VITALS_FATIGUE * 1.5));
}

//-------------------------------------------------------------------
/// @fn fleeArea_onExit(%agent)
/// @brief Called on exiting from this solution, called from execute.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function fleeArea_onExit(%agent)
{
   // update vitals
   %agent.setHappiness(%agent.getHappiness() - 20);
   %agent.setFatigue(%agent.getFatigue() + 20);
   
   iAIGoalManager.completedSolution(%agent);
}