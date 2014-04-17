//-------------------------------------------------------------------
// Immersive AI :: Goal :: getHealth :: Solution :: buyHealth
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

$HealthVendorLocation = "323.09 347.599 218.99";

//-------------------------------------------------------------------
/// @fn buyHealth_evaluate(%agent)
/// @brief Called to determine whether to assign this solution.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function buyHealth_evaluate(%agent)
{
   // check if outside ranges to buy health
   if (%agent.getMoney() < 100)
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
/// @fn buyHealth_onEnter(%agent)
/// @brief Called on entry to this solution state.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function buyHealth_onEnter(%agent)
{
   iAIMessage("STATE: buyHealth being executed by " @ %agent);
   
   // generate a path to the club
   %agent.generatePath($HealthVendorLocation);
   %agent.setMoveDestination(%agent.getCurrentPath().nextPosition());
   %agent.atVendor = false;
   
   buyHealth_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn buyHealth_execute(%agent)
/// @brief Main execution state for the agent, called from onEnter.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function buyHealth_execute(%agent)
{
   // check if at the health vendor yet
   if (%agent.atVendor == false)
      return;
      
   // agent is at the vendor wait a bit
   // then buy it!
   schedule(5000, 0, "buyHealth_onExit", %agent);
}

//-------------------------------------------------------------------
/// @fn buyHealth_onReachDestination(%agent)
/// @brief Callback from onReachDestination. When agent reaches 
///        end point on path, function is called.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function buyHealth_onReachDestination(%agent)
{
   // arrived at the health vendor
   %agent.atVendor = true;
   
   // execute the buying of health
   buyHealth_execute(%agent);
}

//-------------------------------------------------------------------
/// @fn buyHealth_onCheckVitals(%agent)
/// @brief Callback from checkVitals.
///
/// @param %agent Agent throwing the callback.
//-------------------------------------------------------------------
function buyHealth_onCheckVitals(%agent)
{
   // check if at the health Vendor
   if (%agent.atVendor == true)
   {
      //do nothing for now
   }
}

//-------------------------------------------------------------------
/// @fn buyHealth_onExit(%agent)
/// @brief Called on exiting from this solution, called from execute.
///
/// @param %agent Agent state is assigned to.
//-------------------------------------------------------------------
function buyHealth_onExit(%agent)
{  
   %agent.atVendor = false;
   
   //removing monetary cost
   %agent.setMoney(%agent.getMoney() - 60);
   
   //adding item...
   %agent.incInventory("HealthKit",1);
   
   //use item...
   %agent.use("HealthKit");
   
   //finished so request new goal
   iAIGoalManager.completedSolution(%agent);
}