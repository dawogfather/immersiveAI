//-------------------------------------------------------------------
/// @fn iAIAgent::think_combat(%this)
/// @brief Main looping logic for the COMBAT module.
///
/// @param %this Datablock reference.
//-------------------------------------------------------------------
function iAIAgent::think_combat(%this)
{
   // clear last attacked
   %this.lastAttackedBy = 0;
   
   if (%this.combat_InCombat == true)
   {
      // agent is in combat; randomly sidestep
      if (getRandom(0,1) == 1)
         %this.sideStep();
         
      // check for LOS
      if (haveLOS(%this, %this.hunt_Object))
      {
         // if not sidestepping, stop moving as have LOS now
         if (%this.combat_IsSideStepping == false)
            %this.stop();
         
         // fire
         %this.setImageTrigger(0, true);
         
         // increase inventory (never ending ammo)
         %this.incInventory("CrossbowAmmo", 1);
         
         // schedule the trigger stop
         %this.schedule(1500, setImageTrigger, 0, false);
      } else
      {
         // don't have LOS start moving towards player
         %this.setMoveDestination(%this.hunt_Object.getPosition());
      }
   }
}

//-------------------------------------------------------------------
/// @fn iAIAgent::sideStep(%this)
/// @brief Makes an agent side step in a random direction.
///
/// @param %this Datablock reference.
//-------------------------------------------------------------------
function iAIAgent::sideStep(%this)
{
   // set that are side stepping
   %this.combat_IsSideStepping = true;
   
   %sideStepLimit = 50;
   
   // get a random X&Y
   %xrand = getRandom(1, %sideStepLimit / 2);
   %yrand = getRandom(1, %sideStepLimit / 2);
   
   // start destination at current position
   %destination = %this.getTransform();
   
   // Word(0) is the x value
   %destination = setWord(%destination, 0, (getWord(%destination, 0) + (%xrand)));
   
   // Word(1) is the y value
   %destination = setWord(%destination, 1, (getWord(%destination, 1) + (%yrand)));

   // start moving
   %this.setMoveDestination(%destination);
}

//-------------------------------------------------------------------
/// @fn iAIAgent::onDamage(%this, %attacker)
/// @brief Called when damage is applied to the agent.
///
/// @param %this Datablock reference.
/// @param %attacker Person attacking this agent.
//-------------------------------------------------------------------
function iAIAgent::onDamage(%this, %attacker)
{
   if ((%this.getState() $= "Dead") || (!isObject(%attacker)))
      return;

   // dont care about self
   if (%attacker.getId() == %this.getId())
      return;
      
   // only interested if damage was from another agent/player
   if (!(
      (%attacker.getType() & $TypeMasks::iAIAgentObjectType) || 
      (%attacker.getType() & $TypeMasks::PlayerObjectType)))
      return;

   // check if already done interrupt checking for this attacker
   if (%this.lastAttackedBy == %attacker)
      return;
   
   %this.lastAttackedBy = %attacker;
   
   // evaluate to flee area
   %fleeArea = fleeArea_evaluate(%this);
   if (%fleeArea > 0)
   {
      %this.goalInterrupt = true;
      iAIGoalManager.assignGoal(%this, "doDefend", "fleeArea");
      return;
   }
   
   // check if not in combat
   if (%this.combat_InCombat == false)
   {
      // not in combat, so go and attack this player!
      // go into seekAndDestroy mode...
      %this.seekAndDestroy_Object = %attacker;
      %this.goalInterrupt = true;
      iAIGoalManager.assignGoal(%this, "doHunt", "seekAndDestroy");
      return;
   } else
   {
      // in combat already; check if new attacker is closer
      %currentTargetDist = VectorDist(%this.getPosition(), %this.hunt_Object.getPosition());
      %newTargetDist = VectorDist(%this.getPosition(), %attacker.getPosition());
      
      if (%newTargetDist < %currentTargetDist)
      {
         // new target is closer, throw interrupt and hunt new attacker
         %this.seekAndDestroy_Object = %attacker;
         %this.goalInterrupt = true;
         iAIGoalManager.assignGoal(%this, "doHunt", "seekAndDestroy");
         return;
      } else
      {
         // new attacker is further away, just ignore it
      }
   }
}