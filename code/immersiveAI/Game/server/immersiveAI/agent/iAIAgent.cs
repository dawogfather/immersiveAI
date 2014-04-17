//-------------------------------------------------------------------
// Immersive AI :: iAIAgent
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

// global datablock for all agents
datablock PlayerData(iAIAgentData : PlayerBody)
{
   category = "iAIAgent";
};

// load all modules scripts
exec("./iAIAgent.Seek.cs");
exec("./iAIAgent.Combat.cs");

// load all agent types
exec("./iAIAgent_Bandit.cs");
exec("./iAIAgent_Entertainer.cs");
exec("./iAIAgent_Soldier.cs");


//-------------------------------------------------------------------
/// @fn iAIAgent::think(%this)
/// @brief Main looping logic for an agent.
///
/// @param %this Datablock reference.
//-------------------------------------------------------------------
function iAIAgent::think(%this)
{
   // check valid agent
   if ((!isObject(%this)) || (%this.getState() $= "Dead"))
      return;

   // run each modules think methods
   %this.think_seek();
   %this.think_combat();
   
   // increment ticks in current goal
   if (%this.currentGoal $= %this.lastTick_Goal)
   {
      %this.lastTick_ticks++;
   } else
   {
      // goal not the same as last think tick; reset tick counter
      %this.lastTick_ticks = 0;
   }
   
   %this.lastTick_Goal = %this.currentGoal;
   
   // check if ticks over limit
   if (%this.lastTick_ticks > $IAIAGENT_THINK_TICK_LIMIT)
   {
      %this.lastTick_ticks = 0;
      eval(%this.currentSolution @ "_onExit(\"" @ %this @ "\");");
   }
   
   // check if agent doesn't have a goal/solution
   if ((%this.currentGoal $= "") || (%this.currentSolution $= ""))
   {
      // request new goal and solution
      iAIGoalManager.requestNewGoal(%this);
   }
   
   // reschedule the think
   %this.schedule($IAIAGENT_THINK, think);
}

//-------------------------------------------------------------------
/// @fn iAIAgent::checkVitals(%this)
/// @brief Updates agent vitals and throws a callback to the current
///        solutions _onCheckVitals to update as necessary.
///
/// @param %this Datablock reference.
//-------------------------------------------------------------------
function iAIAgent::checkVitals(%this)
{
   // update the agents vitals
   %this.setHappiness(%this.getHappiness() + $IAIAGENT_VITALS_HAPPINESS);
   %this.setFatigue(%this.getFatigue() + $IAIAGENT_VITALS_FATIGUE);
   %this.setBoredom(%this.getBoredom() + $IAIAGENT_VITALS_BOREDOM);

   // callback to current solution onCheckVitals
   if (strlen(%this.currentSolution) > 0)
      eval(%this.currentSolution @ "_onCheckVitals(\"" @ %this @ "\");");

   // reschedule check vitals
   %this.schedule($IAIAGENT_CHECK_VITALS, checkVitals);
}

//-------------------------------------------------------------------
/// @fn iAIAgent::onReachDestination(%this, %agent)
/// @brief Global function called when an agent reaches their
///        destination.
///
/// @param %this Datablock reference.
/// @param %agent Instance of the agent.
//-------------------------------------------------------------------
function iAIAgent::onReachDestination(%this, %agent)
{
   if (isObject(%agent) && !(%agent.getState() $= "Dead"))
   {
      if (%agent.combat_IsSideStepping == true)
      {
         %agent.combat_IsSideStepping = false;
      }
      if ((%agent.getCurrentPath() > 0) && (isObject(%agent.getCurrentPath())))
      {
         // if there is still nodes on the path, get the next one
         if ((isObject(%agent.getCurrentPath())) && (%agent.getCurrentPath().hasNextNode()))
         {
            %agent.setMoveDestination(%agent.getCurrentPath().nextPosition());
         }
         else
         {
            // no next node, delete the path 
            %agent.getCurrentPath().delete();
            
            // call solution onReachDestination
            eval(%agent.currentSolution @ "_onReachDestination(\"" @ %agent @ "\");");
         }
      } else
      {
         // no current path, throw call back to solution onReachDestination
         eval(%agent.currentSolution @ "_onReachDestination(\"" @ %agent @ "\");");
      }
   }
}

//-------------------------------------------------------------------
/// @fn iAIAgent::generatePath(%this, %destination)
/// @brief Generates a path from two world points to another. Adds
///        the created path to the game world and assigns to the
///        parsed agent.
///
/// @param %this Agent to generate the path for.
/// @param %destination Point3F destination location.
//-------------------------------------------------------------------
function iAIAgent::generatePath(%this, %destination)
{
   // check if have current path
   if (isObject(%this.getCurrentPath()))
      %this.getCurrentPath().delete();
   
   // create the new path
   %newPath = new iAIPath();
   
   // check path able to be created
   if (%newPath.createPath(%this.getPosition(), %destination) == true)
   {
      // set the show variables according to agents settings
      %newPath.showPath = %this.path_show;
      %newPath.renderSpline = %this.path_spline;

      // add path to the world
      MissionCleanup.add(%newPath);

      // set the agent's current path to the new one
      %this.setCurrentPath(%newPath);
   } else
   {
      // warp a little bit towards destination, hopefully make a path
      //%this.setTransform($iAIPathMap.closestNode(%this.getPosition()));
      //%this.generatePath(%destination);
   }
}

//-------------------------------------------------------------------
/// @fn iAIAgent::onDeath(%this)
/// @brief Called when an agent dies.
///
/// @param %this Agent which died.
//-------------------------------------------------------------------
function iAIAgent::onDeath(%this)
{
   // delete any current path of the agent's
   if (isObject(%this.getCurrentPath()))
      %this.getCurrentPath().delete();

   // remove from control centre list
   iAICC_AgentList.removeRowById(%this.getId());
   iAIAgentManager.agentCount--;
}

//-------------------------------------------------------------------
/// @fn iAIAgent::onMoveStuck(%this, %agent)
/// @brief Global function to call when an agent is stuck. Attempts
///        to unstick the agent!
///
/// @param %this Datablock reference.
/// @param %agent Instance of the agent.
//-------------------------------------------------------------------
function iAIAgent::onMoveStuck(%this, %agent)
{
   if (isObject(%agent) && !(%agent.getState() $= "Dead"))
   {
      if (isObject(%agent.getCurrentPath()))
      {
         iAIMessage(%agent.getId() @ " is stuck");
         // check if able to warp to next node
         if (%agent.getCurrentPath().hasNextNode())
         {
            iAIMessage(%agent.getId() @ " going to try warping");
         
            // warp to next node
            %agent.setTransform(%agent.getCurrentPath().nextPosition());
            
            // set move destination to next node in path
            %agent.setMoveDestination(%agent.getCurrentPath().nextPosition());
         } else
         {
            // no next node, check distance to destination
            %distance = VectorLen(%agent.getMoveDestination() - %agent.getTransform());
            if ((%distance < 10) && (%distance > -10))
            {
               iAIMessage(%agent.getId() @ " is close enough");
               // must be near enough to destination, finish moving
               %agent.setMoveDestination(%agent.getTransform());
            } else
            {
               // still too far away, generate a new path
               iAIMessage(%agent.getId() @ " is too far... new path");
               
               // delete current path
               %agent.getCurrentPath().delete();
               
               // generate a new path
               %agent.generatePath(%agent.getMoveDestination());
            }
         }
      } else
      {
         iAIMessage(%agent.getId() @ " is stuck with no where to go!");
      }
   }
}