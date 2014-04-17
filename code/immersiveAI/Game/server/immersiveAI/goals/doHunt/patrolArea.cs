//-------------------------------------------------------------------
// Immersive AI :: Goal :: doHunt :: Solution :: patrolArea
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/// @fn patrolArea_evaluate(%agent)
/// @brief Called to determine whether to assign this solution.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function patrolArea_evaluate(%agent)
{
   // check if outside ranges to patrol area
   if (%agent.getBoredom() >= 66)
   {
      return 0.0;
   }
   
   // calculate the weightings of each parameter
   %boredomWeight = (66 - %agent.getBoredom()) / 66;
   
   // average the weightings (div 10 to allow for futher weightings)
   %avgWeight = ((%boredomWeight) / 1) / 10;
   
   // multiply by agent weight for this goal
   switch$(%agent.getAgentType())
   {
      case $iAIAgentType_Soldier: return (%avgWeight * 7); 
      case $iAIAgentType_Bandit: return (%avgWeight * 3);
      default : return 0.0;   
   }
}

//-------------------------------------------------------------------
/// @fn patrolArea_onEnter(%agent)
/// @brief Called on entry to this solution state.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function patrolArea_onEnter(%agent)
{
   iAIMessage("STATE: patrolArea being executed by " @ %agent);
   
   %agent.detect_Found = false;
   
   // pick 4 way points to run between in the patrol loop
   if (%agent.patrolArea_WP1 $= "")
      %agent.patrolArea_WP1 = getRandomPoint();
   if (%agent.patrolArea_WP2 $= "")
      %agent.patrolArea_WP2 = getRandomPoint();
   if (%agent.patrolArea_WP3 $= "")
      %agent.patrolArea_WP3 = getRandomPoint();
   if (%agent.patrolArea_WP4 $= "")
      %agent.patrolArea_WP4 = getRandomPoint();
      
   %nextWP = "";
   switch (%agent.patrolArea_currentWP)
   {
      case 0: %nextWP = %agent.patrolArea_WP1;
      case 1: %nextWP = %agent.patrolArea_WP2;
      case 2: %nextWP = %agent.patrolArea_WP3;
      case 3: %nextWP = %agent.patrolArea_WP4;
      default: %nextWP = %agent.patrolArea_WP1;
   }
   
   // generate a path to the next way point
   %agent.generatePath(%nextWP);
   %agent.setMoveDestination(%agent.getCurrentPath().nextPosition());
   
   // setup detecting of specific agent types
   switch$(%agent.getAgentType())
   {
      case $iAIAgentType_Soldier: %agent.detectObject("iAIAgent_Bandit", $TypeMasks::iAIAgentObjectType);
      case $iAIAgentType_Bandit: %agent.detectObject("iAIAgent_Soldier", $TypeMasks::iAIAgentObjectType);
      default : patrolArea_onExit(%agent);
   }
}

//-------------------------------------------------------------------
/// @fn patrolArea_execute(%agent)
/// @brief Main execution state for the agent, called from onEnter.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function patrolArea_execute(%agent)
{
   %nextWP = "";
   switch (%agent.patrolArea_currentWP)
   {
      case 1: %nextWP = %agent.patrolArea_WP2;
      case 2: %nextWP = %agent.patrolArea_WP3;
      case 3: %nextWP = %agent.patrolArea_WP4;
      case 4: %nextWP = %agent.patrolArea_WP1;
   }
   
   // generate a path to the next way point
   %agent.generatePath(%nextWP);

   // move to next way point
   %agent.setMoveDestination(%agent.getCurrentPath().nextPosition());
}

//-------------------------------------------------------------------
/// @fn patrolArea_onReachDestination(%agent)
/// @brief Callback from onReachDestination. When agent reaches 
///        end point on path, function is called.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function patrolArea_onReachDestination(%agent)
{
   // check if found an object
   if (%agent.detect_Found == true)
   {
      // go into seekAndDestroy mode...
      %agent.goalInterrupt = true;
      iAIGoalManager.assignGoal(%agent, "doHunt", "seekAndDestroy");
      return;
   }
   
   %agent.patrolArea_currentWP++;
   if (%agent.patrolArea_currentWP > 4)
      %agent.patrolArea_currentWP = 1;
   patrolArea_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn patrolArea_onCheckVitals(%agent)
/// @brief Callback from checkVitals.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function patrolArea_onCheckVitals(%agent)
{
   %agent.setFatigue(%agent.getFatigue() + ($IAIAGENT_VITALS_FATIGUE * 1.5));
   %agent.setBoredom(%agent.getBoredom() + ($IAIAGENT_VITALS_BOREDOM * 1.2));
}

//-------------------------------------------------------------------
/// @fn patrolArea_onExit(%agent)
/// @brief Called on exiting from this solution, called from execute.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function patrolArea_onExit(%agent)
{
   // gets $50 for patrolling
   %agent.setMoney(%agent.getMoney() + 50);
   
   %agent.detect_Found = false;
   %agent.detecting = false;
   %agent.detect_Object = 0;
   iAIGoalManager.completedSolution(%agent);
}