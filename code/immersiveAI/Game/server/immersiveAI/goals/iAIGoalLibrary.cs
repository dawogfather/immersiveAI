//-------------------------------------------------------------------
// Immersive AI :: iAIGoalLibrary
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

function iAIGoalLibrary::initialize(%this)
{
   // execute all the goals
   exec("./doDefend/doDefend.cs");
   exec("./doExplore/doExplore.cs");
   exec("./doHunt/doHunt.cs");
   exec("./doRest/doRest.cs");
   exec("./getFood/getFood.cs");
   exec("./getHealth/getHealth.cs");
   exec("./goHome/goHome.cs");
   exec("./haveFun/haveFun.cs");
   
   // sort the library
   %this.sort();
}