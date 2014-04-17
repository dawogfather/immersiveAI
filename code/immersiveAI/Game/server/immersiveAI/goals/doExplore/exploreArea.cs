//-------------------------------------------------------------------
// Immersive AI :: Goal :: doExplore :: Solution :: exploreArea
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/// @fn exploreArea_evaluate(%agent)
/// @brief Called to determine whether to assign this solution.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function exploreArea_evaluate(%agent)
{
   // check if outside ranges to do explore
   if (%agent.getBoredom() <= 66)
   {
      return 0.0;
   }
   
   // calculate the weightings of each parameter
   %boredomWeight = (%agent.getBoredom() - 66) / 66;
   
   // average the weightings (div 10 to allow for futher weightings)
   %avgWeight = ((%boredomWeight) / 1) / 10;
   
   // multiply by agent weight for this goal
   switch$(%agent.getAgentType())
   {
      case $iAIAgentType_Entertainer: return (%avgWeight * 10); 
      case $iAIAgentType_Bandit: return (%avgWeight * 10);
      default : return 0.0;   
   }
}

//-------------------------------------------------------------------
/// @fn exploreArea_onEnter(%agent)
/// @brief Called on entry to this solution state.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function exploreArea_onEnter(%agent)
{
   iAIMessage("STATE: exploreArea being executed by " @ %agent);
   
   // generate a random number of paths to explore along
   %agent.exploreArea_totalPaths = getRandom(1, 20);
   %agent.exploreArea_paths = 0;
   
   // nothing to do when entering the state, just execute
   exploreArea_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn exploreArea_execute(%agent)
/// @brief Main execution state for the agent, called from onEnter.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function exploreArea_execute(%agent)
{
   // check if visited enough paths
   if (%agent.exploreArea_paths >= %agent.exploreArea_totalPaths)
   {
      // exit the state
      exploreArea_onExit(%agent);
      return;
   }
      
   // generate a path to a random position
   %agent.generatePath(getRandomPoint());
   
   // move along the path
   %agent.setMoveDestination(%agent.getCurrentPath().nextPosition());
   %agent.exploreArea_paths++;
}

//-------------------------------------------------------------------
/// @fn exploreArea_onReachDestination(%agent)
/// @brief Callback from onReachDestination. When agent reaches 
///        end point on path, function is called.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function exploreArea_onReachDestination(%agent)
{
   // agent has reached the end of the path, return to execute method
   exploreArea_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn exploreArea_onCheckVitals(%agent)
/// @brief Callback from checkVitals.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function exploreArea_onCheckVitals(%agent)
{
   %agent.setFatigue(%agent.getFatigue() + ($IAIAGENT_VITALS_FATIGUE * 1.5));
}

//-------------------------------------------------------------------
/// @fn exploreArea_onExit(%agent)
/// @brief Called on exiting from this solution, called from execute.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function exploreArea_onExit(%agent)
{
   %agent.setBoredom(%agent.getBoredom() - 20);  
   iAIGoalManager.completedSolution(%agent);
}