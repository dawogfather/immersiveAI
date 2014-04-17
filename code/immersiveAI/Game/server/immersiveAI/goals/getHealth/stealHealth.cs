//-------------------------------------------------------------------
// Immersive AI :: Goal :: getHealth :: Solution :: stealHealth
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

$HealthVendorLocation = "323.09 347.599 218.99";

//-------------------------------------------------------------------
/// @fn stealHealth_evaluate(%agent)
/// @brief Called to determine whether to assign this solution.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function stealHealth_evaluate(%agent)
{
   // check if outside ranges to steal
   if (%agent.getHappiness() >= 50)
   {
      return 0.0;
   }
   
   // calcaulte weightings of each parameters
   %happinessWeight = (50 - %agent.getHappiness()) / 50;
   
   // average the weightings (div 10 to allow for futher weightings)
   %avgWeight = ((%happinessWeight) / 1) / 10;
   
   // multiply by agent weight for this solution
   switch$(%agent.getAgentType())
   {
      case $iAIAgentType_Bandit: return (%avgWeight * 5); 
      case $iAIAgentType_Entertainer: return (%avgWeight * 3);
      default : return 0.0;   
   }
}

//-------------------------------------------------------------------
/// @fn stealHealth_onEnter(%agent)
/// @brief Called on entry to this solution state.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function stealHealth_onEnter(%agent)
{
   iAIMessage("STATE: stealHealth being executed by " @ %agent);
   
   // generate a path to the club
   %agent.generatePath($HealthVendorLocation);
   %agent.setMoveDestination(%agent.getCurrentPath().nextPosition());
   %agent.atHealthVendor = false;
   
   stealHealth_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn stealHealth_execute(%agent)
/// @brief Main execution state for the agent, called from onEnter.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function stealHealth_execute(%agent)
{
   // check if at the health vendor yet
   if (%agent.atHealthVendor == false)
      return;
      
   // agent is at the vendor shoot and steal  from it!
   
   //set the agents aim
   %agent.setAimObject(HealthVendor.getId());
   
   //set the agent to shoot the vendor
   %agent.setImageTrigger(0, true);
   
   //set the agent to stop shooting
   %agent.schedule(1500, setImageTrigger, 0, false);
   
   schedule(5000, 0, "stealHealth_onExit", %agent);
}

//-------------------------------------------------------------------
/// @fn stealHealth_onReachDestination(%agent)
/// @brief Callback from onReachDestination. When agent reaches 
///        end point on path, function is called.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function stealHealth_onReachDestination(%agent)
{
   // arrived at the health vendor
   %agent.atHealthVendor = true;
   
   // execute the buying of food
   stealHealth_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn stealHealth_onCheckVitals(%agent)
/// @brief Callback from checkVitals.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function stealHealth_onCheckVitals(%agent)
{
   // check if at the health Vendor
   if (%agent.atHealthVendor == true)
   {
      //do nothing
   }
}

//-------------------------------------------------------------------
/// @fn stealHealth_onExit(%agent)
/// @brief Called on exiting from this solution, called from execute.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function stealHealth_onExit(%agent)
{
   %agent.atHealthVendor = false;
   
   //clear the aim
   %agent.clearAim();
   
   //add the taken item...
   %agent.incInventory("HealthKit",1);
   
   //use item...
   %agent.use("HealthKit");
   
   //finished so request new goal
   iAIGoalManager.completedSolution(%agent);
}