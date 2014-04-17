//-------------------------------------------------------------------
// Immersive AI :: Goal :: goHome :: Solution :: seekHome
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

$HomeLocation = "120 280 250";

//-------------------------------------------------------------------
/// @fn seekHome_evaluate(%agent)
/// @brief Called to determine whether to assign this solution.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function seekHome_evaluate(%agent)
{
   // check if outside ranges to go home
   if (%agent.getHealth() >= 66)
   {
      return 0.0;
   }
   
   // calculate the weightings of each parameter
   %healthWeight = (66 - %agent.getHealth()) / 66;
   
   // average the weightings (div 10 to allow for futher weightings)
   %avgWeight = ((%healthWeight) / 1) / 10;
   
   // multiply by agent weight for this goal
   switch$(%agent.getAgentType())
   {
      case $iAIAgentType_Entertainer: return (%avgWeight * 10); 
      case $iAIAgentType_Bandit: return (%avgWeight * 10);
      case $iAIAgentType_Soldier: return (%avgWeight * 10);
      default : return 0.0;
   }
}

//-------------------------------------------------------------------
/// @fn seekHome_onEnter(%agent)
/// @brief Called on entry to this solution state.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function seekHome_onEnter(%agent)
{
   iAIMessage("STATE: seekHome being executed by " @ %agent);
   
   // generate a path to home
   %agent.generatePath($HomeLocation);
   %agent.setMoveDestination(%agent.getCurrentPath().nextPosition());
   %agent.atHome = false;
   
   seekHome_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn seekHome_execute(%agent)
/// @brief Main execution state for the agent, called from onEnter.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function seekHome_execute(%agent)
{
   // check if home yet
   if (%agent.atHome == false)
      return;
      
   // agent home... refresh stats
   %agent.setHealth(100);
   %agent.setMoney(%agent.getMoney() + 200);
   
   schedule(10000, 0, "seekHome_onExit", %agent);
}

//-------------------------------------------------------------------
/// @fn seekHome_onReachDestination(%agent)
/// @brief Callback from onReachDestination. When agent reaches 
///        end point on path, function is called.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function seekHome_onReachDestination(%agent)
{
   // arrived home
   %agent.atHome = true;
   
   // execute home functions
   seekHome_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn seekHome_onCheckVitals(%agent)
/// @brief Callback from checkVitals.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function seekHome_onCheckVitals(%agent)
{
   // check if home
   if (%agent.atHome == true)
   {
      %agent.setFatigue(%agent.getFatigue() - ($IAIAGENT_VITALS_FATIGUE*1.5));
      %agent.setBoredom(%agent.getBoredom() - ($IAIAGENT_VITALS_BOREDOM*1.5));
   }
}

//-------------------------------------------------------------------
/// @fn seekHome_onExit(%agent)
/// @brief Called on exiting from this solution, called from execute.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function seekHome_onExit(%agent)
{
   %agent.atHome = false;
   iAIGoalManager.completedSolution(%agent);
}