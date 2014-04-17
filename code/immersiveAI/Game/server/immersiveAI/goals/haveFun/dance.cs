//-------------------------------------------------------------------
// Immersive AI :: Goal :: haveFun :: Solution :: dance
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

$ClubLocation = "405 331 200";

//-------------------------------------------------------------------
/// @fn dance_evaluate(%agent)
/// @brief Called to determine whether to assign this solution.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function dance_evaluate(%agent)
{
   // check if outside ranges to dance
   if (%agent.getFatigue() <= 33)
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
      case $iAIAgentType_Entertainer: return (%avgWeight * 10); 
      case $iAIAgentType_Bandit: return (%avgWeight * 10);
      default : return 0.0;   
   }
}

//-------------------------------------------------------------------
/// @fn dance_onEnter(%agent)
/// @brief Called on entry to this solution state.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function dance_onEnter(%agent)
{
   iAIMessage("STATE: dance being executed by " @ %agent);
   
   // generate a path to the club
   %agent.setActionThread("look");
   %agent.generatePath($ClubLocation);
   %agent.setMoveDestination(%agent.getCurrentPath().nextPosition());
   %agent.atClub = false;
   
   dance_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn dance_execute(%agent)
/// @brief Main execution state for the agent, called from onEnter.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function dance_execute(%agent)
{
   // check if at the club yet
   if (%agent.atClub == false)
      return;
      
   // agent is at the club
   // dance baby, dance!
   %agent.setActionThread("dance");
   
   schedule(10000, 0, "dance_onExit", %agent);
   %agent.schedule(9000, setActionThread, "look");
}

//-------------------------------------------------------------------
/// @fn dance_onReachDestination(%agent)
/// @brief Callback from onReachDestination. When agent reaches 
///        end point on path, function is called.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function dance_onReachDestination(%agent)
{
   // arrived at the club
   %agent.atClub = true;
   
   // execute the dancing
   dance_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn dance_onCheckVitals(%agent)
/// @brief Callback from checkVitals.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function dance_onCheckVitals(%agent)
{
   // check if at the club
   if (%agent.atClub == true)
   {
      %agent.setFatigue(%agent.getFatigue() + ($IAIAGENT_VITALS_FATIGUE * 2));
      %agent.setHappiness(%agent.getHappiness() - ($IAIAGENT_VITALS_HAPPINESS * 2));
   }
}

//-------------------------------------------------------------------
/// @fn dance_onExit(%agent)
/// @brief Called on exiting from this solution, called from execute.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function dance_onExit(%agent)
{
   %agent.atClub = false;
   
   %agent.setHappiness(%agent.getHappiness() + 33);
   %agent.setFatigue(%agent.getFatigue() - 33);
   %agent.setBoredom(%agent.getBoredom() - 33);

   // stop dancing
   %agent.setActionThread("look");
   iAIGoalManager.completedSolution(%agent);
}
