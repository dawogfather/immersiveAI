//-------------------------------------------------------------------
// Immersive AI :: Goal :: doHunt :: Solution :: seekAndDestroy
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/// @fn seekAndDestroy_evaluate(%agent)
/// @brief Called to determine whether to assign this solution.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function seekAndDestroy_evaluate(%agent)
{
   // check if outside ranges to patrol area
   if (%agent.getHappiness() >= 66)
   {
      return 0.0;
   }
   
   // calculate the weightings of each parameter
   %happinessWeight = (66 - %agent.getHappiness()) / 66;
   
   // average the weightings (div 10 to allow for futher weightings)
   %avgWeight = ((%happinessWeight) / 1) / 10;
   
   // multiply by agent weight for this goal
   switch$(%agent.getAgentType())
   {
      case $iAIAgentType_Soldier: return (%avgWeight * 3); 
      case $iAIAgentType_Bandit: return (%avgWeight * 7);
      default : return 0.0;   
   }
}

//-------------------------------------------------------------------
/// @fn seekAndDestroy_onEnter(%agent)
/// @brief Called on entry to this solution state.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function seekAndDestroy_onEnter(%agent)
{
   iAIMessage("STATE: seekAndDestroy being executed by " @ %agent);
   
   // find an agent to kill
   %agent.hunt_CloseFollowing = false;
   
   // check if need to explicity attack someone
   if (isObject(%agent.seekAndDestroy_Object))
   {
      %agent.seeking = true;
      %agent.seek_Found = true;
      %agent.seek_ObjectMask = %agent.seekAndDestroy_Object.getType();
      %agent.seek_DistanceAway = 100;
      seek_generatePath(%agent, %agent.seekAndDestroy_Object);
   } else
   {
      // set the seek depending on the agent type
      switch$(%agent.getAgentType())
      {
         case $iAIAgentType_Soldier: %agent.seekObject("iAIAgent_Bandit", $TypeMasks::iAIAgentObjectType, 100);
         case $iAIAgentType_Bandit: %agent.seekObject("iAIAgent_Soldier", $TypeMasks::iAIAgentObjectType, 100);
         default : seekAndDestroy_onExit(%agent);
      }
   }
}

//-------------------------------------------------------------------
/// @fn seekAndDestroy_execute(%agent)
/// @brief Main execution state for the agent, called from onEnter.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function seekAndDestroy_execute(%agent)
{
   if ((!isObject(%agent)) || (%agent.getState() $= "Dead"))
      return;
   
   // check if object is dead
   if (isDead(%agent.hunt_Object))
   {
      seekAndDestroy_onExit(%agent);
      return;
   }

   // schedule execute again after 2seconds   
   schedule(2000, 0, seekAndDestroy_execute, %agent);
}

//-------------------------------------------------------------------
/// @fn seekAndDestroy_onReachDestination(%agent)
/// @brief Callback from onReachDestination. When agent reaches 
///        end point on path, function is called.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function seekAndDestroy_onReachDestination(%agent)
{
   // check if finished seeking
   if ((%agent.seeking == false) && (%agent.hunt_CloseFollowing == false) && (%agent.seek_Found == true))
   {
      // now stay with the player..
      %agent.hunt_Object = %agent.seek_Object;
      %agent.hunt_CloseFollowing = true;
      
      // set as agent now in combat
      %agent.combat_InCombat = true;
      
      // stay aiming at the hunting agent
      %agent.setAimObject(%agent.hunt_Object, "0 0 2");
      
      // delete any current path
      if (isObject(%agent.getCurrentPath()))
         %agent.getCurrentPath().delete();

      seekAndDestroy_execute(%agent);
   }
}

//-------------------------------------------------------------------
/// @fn seekAndDestroy_onCheckVitals(%agent)
/// @brief Callback from checkVitals.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function seekAndDestroy_onCheckVitals(%agent)
{
   %agent.setFatigue(%agent.getFatigue() + ($IAIAGENT_VITALS_FATIGUE * 1.5));
   %agent.setBoredom(%agent.getBoredom() + ($IAIAGENT_VITALS_BOREDOM * 1.2));
}

//-------------------------------------------------------------------
/// @fn seekAndDestroy_onExit(%agent)
/// @brief Called on exiting from this solution, called from execute.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function seekAndDestroy_onExit(%agent)
{
   // stops fireing and clear the aim
   %agent.setImageTrigger(0, false);
   %agent.clearAim();
   
   %agent.hunt_CloseFollowing = false;
   %agent.hunt_Object = 0;
   %agent.combat_InCombat = false;
   %agent.seekAndDestroy_Object = 0;
   
   // gets $50 for destroying
   %agent.setMoney(%agent.getMoney() + 50);
   
   iAIGoalManager.completedSolution(%agent);
}