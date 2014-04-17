//-------------------------------------------------------------------
// Immersive AI :: Goal :: getFood :: Solution :: stealFood
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

$FoodVendorLocation = "420.406 329.79 218.504";

//-------------------------------------------------------------------
/// @fn stealFood_evaluate(%agent)
/// @brief Called to determine whether to assign this solution.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function stealFood_evaluate(%agent)
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
/// @fn stealFood_onEnter(%agent)
/// @brief Called on entry to this solution state.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function stealFood_onEnter(%agent)
{
   iAIMessage("STATE: stealFood being executed by " @ %agent);
   
   // generate a path to the club
   %agent.generatePath($FoodVendorLocation);
   %agent.setMoveDestination(%agent.getCurrentPath().nextPosition());
   %agent.atFoodVendor = false;
   
   stealFood_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn stealFood_execute(%agent)
/// @brief Main execution state for the agent, called from onEnter.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function stealFood_execute(%agent)
{
   // check if at the food vendor yet
   if (%agent.atFoodVendor == false)
      return;
      
   // agent is at the vendor shoot and steal  from it!
   
   //set the agents aim
   %agent.setAimObject(FoodVendor.getId());
   //set the agent to shoot the vendor
   %agent.setImageTrigger(0,true);
   //set the agent to stop shooting
   %agent.schedule(1500, setImageTrigger, 0, false);
   
   schedule(5000, 0, "stealFood_onExit", %agent);
}

//-------------------------------------------------------------------
/// @fn stealFood_onReachDestination(%agent)
/// @brief Callback from onReachDestination. When agent reaches 
///        end point on path, function is called.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function stealFood_onReachDestination(%agent)
{
   // arrived at the food vendor
   %agent.atFoodVendor = true;
   
   // execute the buying of food
   stealFood_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn stealFood_onCheckVitals(%agent)
/// @brief Callback from checkVitals.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function stealFood_onCheckVitals(%agent)
{
   // check if at the food Vendor
   if (%agent.atFoodVendor == true)
   {
      //do nothing
   }
}

//-------------------------------------------------------------------
/// @fn stealFood_onExit(%agent)
/// @brief Called on exiting from this solution, called from execute.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function stealFood_onExit(%agent)
{
   %agent.atFoodVendor = false;
   
   //clear the aim
   %agent.clearAim();
   //add the taken item...
   %agent.incInventory("FoodKit",1);
   //use item...
   %agent.use("FoodKit");
   //finished so request new goal
   iAIGoalManager.completedSolution(%agent);
}