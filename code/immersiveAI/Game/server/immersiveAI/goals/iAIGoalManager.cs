//-------------------------------------------------------------------
// Immersive AI :: iAIGoalManager
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/// @fn initialize(%this)
/// @brief Initializes the GoalManager. Called when a new game is
///        created.
///
/// @param %this Instance of iAIGoalManager.
//-------------------------------------------------------------------
function iAIGoalManager::initialize(%this)
{
   //
}

//-------------------------------------------------------------------
/// @fn assignGoal(%this, %agent, %goal, %solution)
/// @brief Assigns the parsed goal & solution to the specific agent.
///        Executes the 'onEnter' method of the selected solution.
///
/// @param %this Instance of iAIGoalManager.
/// @param %agent Agent to assign to.
/// @param %goal Goal to assign to the agent.
/// @param %solution Solution for the goal.
/// @returns bool Assignment success.
//-------------------------------------------------------------------
function iAIGoalManager::assignGoal(%this, %agent, %goal, %solution)
{
   // check valid parameters
   if (((!isObject(%agent)) || (%agent.getState() $= "Dead")) ||
       ((strlen(%goal) == 0) || (strlen(%solution) == 0)))
      return false;
   
   //%agent.setActionThread("look");//should reset their animations
      
   // check for a goal interrupt
   if (%agent.goalInterrupt == true)
   {
      %agent.goalInterrupt = false;
      %agent.resumePrevious = true;
   } else
   {
      if (%agent.resumePrevious == true)
      {
         %agent.resumePrevious = false;
         
         // resume previous only if previous is a differnt goal/solution!
         if (!((%agent.previousGoal $= %agent.currentGoal) &&
            (%agent.previousSolution $= %agent.currentSolution)))
         {
            iAIGoalManager.resumePreviousSolution(%agent);
            return;
         }
      }
   }

   // check if agent has a current goal
   if (strlen(%agent.currentGoal) != 0)
   {
      // set the previous goal/solution to the current ones
      %agent.previousGoal = %agent.currentGoal;
      %agent.previousSolution = %agent.currentSolution;
   }

   // set the new goal & solution
   %agent.currentGoal = %goal;
   %agent.currentSolution = %solution;

   // execute the solution
   %current = eval(%solution @ "_onEnter(\"" @ %agent @ "\");");

   return true;
}

//-------------------------------------------------------------------
/// @fn evaluateList(%this, %agent, %wordList)
/// @brief Evaluates the list of words in the parsed space
///        deliminated list. Executes each words '_evaluate' method
///        passing the %agent variable.
///
/// @param %this Instance of iAIGoalManager.
/// @param %agent Agent to pass into the evaluate method.
/// @param %wordList String of space deliminated words to execute.
/// @param %avoidWord Word to avoid in the list.
/// @returns string Highest evaluated word.
//-------------------------------------------------------------------
function iAIGoalManager::evaluateList(%this, %agent, %wordList, %avoidWord)
{
   // get number of words in the wordList
   %wordCount = getWordCount(%wordList);

   // hold the max value & max string found
   %maxValue = -1;
   %maxName = "";

   // iterate over all goals
   for (%i = 0; %i < %wordCount; %i++)
   {
      // get the current goal name from the goalList
      %currentWord = getWord(%wordList, %i);

      // check if to avoid the current word
      if (%currentWord $= %avoidWord)
         continue;
      
      // execute the goal evaluate method
      %current = eval(%currentWord @ "_evaluate(\"" @ %agent @ "\");");

      // check for less than 0, as not allowed into this goal
      if (%current < 0)
         continue;
      
      // check if best goal found
      if (%current > %maxValue)
      {
         // set best goal values
         %maxValue = %current;
         %maxName = %currentWord;
      }
   }

   return %maxName;
}

//-------------------------------------------------------------------
/// @fn requestNewGoal(%this, %agent, %avoidCurrentGoal,
///                    %avoidCurrentSolution)
/// @brief Requests a new goal for the parsed agent.
///
/// @param %this Instance of iAIGoalManager.
/// @param %agent Agent to find a new goal for.
/// @param %avoidCurrentGoal Flag to avoid the current goal.
/// @param %avoidCurrentSolution Flag to avoid the current solution.
/// @returns bool Request success.
//-------------------------------------------------------------------
function iAIGoalManager::requestNewGoal(%this, %agent, %avoidCurrentGoal, %avoidCurrentSolution)
{
   // check valid parameters
   if ((!isObject(%agent)) || (%agent.getState() $= "Dead"))
      return false;

   // check if wanting to avoid the current goal
   %avoidWord = "";
   if (%avoidCurrentGoal == true)
      %avoidWord = %agent.currentGoal;
      
   // evaluate the agents goals
   %goal = %this.evaluateList(%agent, %agent.goalList, %avoidWord);

   // check goal found
   if (strlen(%goal) == 0)
      return false;

   // get a list of solutions for the goal
   %solutionList = $iAIGoalLibrary.getSolutionList(%agent.getAgentType(), %goal);

   // check solution list found
   if (strlen(%solutionList) == 0)
      return false;

   // check if wanting to avoid the current solution
   %avoidWord = "";
   if (%avoidCurrentSolution == true)
      %avoidWord = %agent.currentSolution;

   // evaluate the solutions
   %solution = %this.evaluateList(%agent, %solutionList, %avoidWord);

   // check solution found
   if (strlen(%solution) == 0)
      return false;

   // assign and execute the goal and solution to the agent
   return (%this.assignGoal(%agent, %goal, %solution));
}

//-------------------------------------------------------------------
/// @fn requestNewSolution(%this, %agent, %avoidCurrentSolution)
/// @brief Requests a new solution for the parsed agent; retaining
///        the current goal.
///
/// @param %this Instance of iAIGoalManager.
/// @param %agent Agent to find a new solution for.
/// @param %avoidCurrentSolution Flag to avoid the current solution.
/// @returns bool Request success.
//-------------------------------------------------------------------
function iAIGoalManager::requestNewSolution(%this, %agent, %avoidCurrentSolution)
{
   // check valid parameters
   if ((!isObject(%agent)) || (%agent.getState() $= "Dead") ||
       (strlen(%agent.currentGoal) == 0))
      return false;

   // get a list of solutions for the current goal
   %solutionList = $iAIGoalLibrary.getSolutionList(%agent.getAgentType(), %agent.currentGoal);

   // check solution list found
   if (strlen(%solutionList) == 0)
      return false;

   // check if wanting to avoid the current solution
   %avoidWord = "";
   if (%avoidCurrentSolution == true)
      %avoidWord = %agent.currentSolution;
      
   // evaluate the solutions
   %solution = %this.evaluateList(%agent, %solutionList, %avoidWord);

   // check solution found
   if (strlen(%solution) == 0)
      return false;

   // assign and execute the solution to the agent
   return (%this.assignGoal(%agent, %agent.currentGoal, %solution));
}

//-------------------------------------------------------------------
/// @fn resumePreviousSolution(%this, %agent)
/// @brief Resumes a previous solution for the given agent.
///
/// @param %this Instance of iAIGoalManager.
/// @param %agent Agent to resume for.
/// @returns bool Assignment success.
//-------------------------------------------------------------------
function iAIGoalManager::resumePreviousSolution(%this, %agent)
{
   // check valid parameters
   if ((!isObject(%agent)) || (%agent.getState() $= "Dead"))
      return false;

   // check if agent has previous
   if ((strlen(%agent.previousGoal) != 0) && (strlen(%agent.previousSolution) != 0))
   {
      // assign the previous goal/solution
      return (iAIGoalManager.assignGoal(%agent, %agent.previousGoal, %agent.previousSolution));
   } else
   {
      // didn't have a previous goal/solution
      return false;
   }
}

//-------------------------------------------------------------------
/// @fn completedSolution(%this, %agent)
/// @brief Completes and requests a new goal for the given agent.
///
/// @param %this Instance of iAIGoalManager.
/// @param %agent Agent to complete for.
//-------------------------------------------------------------------
function iAIGoalManager::completedSolution(%this, %agent)
{
   // check valid parameters
   if ((!isObject(%agent)) || (%agent.getState() $= "Dead"))
      return false;

   // some cleanup..
   %agent.setActionThread("look");
   if (isObject(%agent.getCurrentPath()))
      %agent.getCurrentPath().delete();
   
   // check if over think tick limit
   if (%this.lastTick_ticks > $IAIAGENT_THINK_TICK_LIMIT)
   {
      // request a different goal and solution
      %this.requestNewGoal(%agent, true, true);
   } else
   {
      // request a new goal and solution
      %this.requestNewGoal(%agent);
   }
}