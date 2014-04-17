//-------------------------------------------------------------------
// Immersive AI :: Goal :: doDefend
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
// Load all the solution files
//-------------------------------------------------------------------
exec("./fleeArea.cs");

//-------------------------------------------------------------------
// Add goal and solutions to library for each agent
//-------------------------------------------------------------------
// the defend goals are started from when an agent is attacked
$iAIGoalLibrary.addGoal($iAIAgentType_Entertainer, "doDefend");
$iAIGoalLibrary.addSolution($iAIAgentType_Entertainer, "doDefend", "fleeArea");

$iAIGoalLibrary.addGoal($iAIAgentType_Soldier, "doDefend");
$iAIGoalLibrary.addSolution($iAIAgentType_Soldier, "doDefend", "fleeArea");


//-------------------------------------------------------------------
/// @fn doDefend_evaluate(%agent)
/// @brief Called to determine whether to assign this goal.
///
/// @param %agent Agent to determine for.
/// @returns F32 between 0.00 and 1.00
//-------------------------------------------------------------------
function doDefend_evaluate(%agent)
{
   // always return -1 to ensure never use this goal
   return -1;
}