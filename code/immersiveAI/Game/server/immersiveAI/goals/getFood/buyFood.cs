//-------------------------------------------------------------------
// Immersive AI :: Goal :: getFood :: Solution :: buyFood
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

$FoodVendorLocation = "420.406 329.79 218.504";

//-------------------------------------------------------------------
/// @fn buyFood_evaluate(%agent)
/// @brief Called to determine whether to assign this solution.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function buyFood_evaluate(%agent)
{
   // check if outside ranges to buy food
   if (%agent.getMoney() <= 100)
   {
      return 0.0;
   }
   
   // calculate the weightings of each parameter
   %moneyWeight = (%agent.getMoney() - 100) / 100;
   
   // average the weightings (div 10 to allow for futher weightings)
   %avgWeight = ((%moneyWeight) / 1) / 10;
   
   // multiply by agent weight for this goal
   switch$(%agent.getAgentType())
   {
      case $iAIAgentType_Entertainer: return (%avgWeight * 7); 
      case $iAIAgentType_Soldier: return (%avgWeight * 5);
      default : return 0.0;   
   }
}

//-------------------------------------------------------------------
/// @fn buyFood_onEnter(%agent)
/// @brief Called on entry to this solution state.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function buyFood_onEnter(%agent)
{
   iAIMessage("STATE: BuyFood being executed by " @ %agent);
   
   // generate a path to the vendor
   %agent.generatePath($FoodVendorLocation);
   %agent.setMoveDestination(%agent.getCurrentPath().nextPosition());
   %agent.atFoodVendor = false;
   
   buyFood_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn buyFood_execute(%agent)
/// @brief Main execution state for the agent, called from onEnter.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function buyFood_execute(%agent)
{
   // check if at the health vendor yet
   if (%agent.atFoodVendor == false)
      return;
      
   // agent is at the vendor wait a bit
   // then buy it!
   schedule(5000, 0, "buyFood_onExit", %agent);
}

//-------------------------------------------------------------------
/// @fn buyFood_onReachDestination(%agent)
/// @brief Callback from onReachDestination. When agent reaches 
///        end point on path, function is called.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function buyFood_onReachDestination(%agent)
{
   // arrived at the food vendor
   %agent.atFoodVendor = true;
   
   // execute the buying of food
   buyFood_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn buyFood_onCheckVitals(%agent)
/// @brief Callback from checkVitals.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function buyFood_onCheckVitals(%agent)
{
    // check if at the food Vendor
   if (%agent.atFoodVendor == true)
   {
      //do nothing
   }
}

//-------------------------------------------------------------------
/// @fn buyFood_onExit(%agent)
/// @brief Called on exiting from this solution, called from execute.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function buyFood_onExit(%agent)
{
   %agent.atFoodVendor = false;
   
   %agent.setMoney(%agent.getMoney()-50);
   // adding item...
   %agent.incInventory("FoodKit",1);
   // use item...
   %agent.use("FoodKit");
   // finished so request new goal
   iAIGoalManager.completedSolution(%agent);
}