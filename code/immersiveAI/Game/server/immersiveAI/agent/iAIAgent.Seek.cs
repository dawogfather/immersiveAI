//-------------------------------------------------------------------
/// @fn iAIAgent::seekObject(%this, %objectName, %objectMask)
/// @brief Global function to seek an object in the game world. As
///        specified by the item name (as defined in the datablock)
///        and type mask.
///
/// @param %this Instance of iAIAgent.
/// @param %objectName Name of the object to find. Optional.
/// @param %objectMask Object type mask to search for.
/// @param %distanceAway Distance away to be close enough. Optional.
//-------------------------------------------------------------------
function iAIAgent::seekObject(%this, %objectName, %objectMask, %distanceAway)
{
   // setup seeking variables - seek logic happens in main think() loop
   %this.seeking = true;
   %this.seek_ObjectName = %objectName;
   %this.seek_ObjectMask = %objectMask;
   
   // if distance away specified, use it
   if (%distanceAway > 0)
      %this.seek_DistanceAway = %distanceAway;
   else
      %this.seek_DistanceAway = $IAISEEK_CLOSE_ENOUGH;
      
   %this.seek_ObjectPosition = "";
   %this.seek_Object = 0;
   %this.seek_OnObject = false;
   %this.seek_Found = false;
   %this.detecting = false;
}

//-------------------------------------------------------------------
/// @fn iAIAgent::detectObject(%this, %objectName, %objectMask)
/// @brief Global function to detect an object in the game world.
///
/// @param %this Instance of iAIAgent.
/// @param %objectName Name of the object to find. Optional.
/// @param %objectMask Object type mask to search for.
//-------------------------------------------------------------------
function iAIAgent::detectObject(%this, %objectName, %objectMask)
{
   // setup seeking variables - seek logic happens in main think() loop
   %this.detecting = true;
   %this.seeking = false;
   %this.seek_ObjectName = %objectName;
   %this.seek_ObjectMask = %objectMask;
}

//-------------------------------------------------------------------
/// @fn iAIAgent::think_seek(%this)
/// @brief Main looping logic for the SEEK module.
///
/// @param %this Datablock reference.
//-------------------------------------------------------------------
function iAIAgent::think_seek(%this)
{
   // check if searching for something   
   if (%this.seeking == true)
   {
      // check if still not found the object needed
      if (%this.seek_Found == false)
      {
         // find the closest object
         seek_findClosest(%this, %this.seek_ObjectMask, %this.seek_ObjectName);
      } else
      {
         // check if object is dead
         if (isDead(%this.seek_Object))
         {
             seek_cancelSeek(%this);
             return;
         } else
         {
            // check if the object has moved
            seek_checkMoved(%this, %this.seek_Object);
         }
      }
   }
   
   if (%this.detecting == true)
   {
      %closestObject = seek_closestObject(%this, %this.seek_ObjectMask, %this.seek_ObjectName);
      if ((%closestObject > 0) && (isObject(%closestObject)))
      {
         %this.detect_Found = true;
         %this.detect_Object = %closestObject;
         %this.detecting = false;
         eval(%this.currentSolution @ "_onReachDestination(\"" @ %this @ "\");");
      }
   }
}

function seek_generatePath(%agent, %object)
{
   %agent.seek_Found = true;
   %agent.seek_Object = %object;
   %agent.seek_ObjectPosition = $iAIPathMap.closestNode(%object.getPosition());

   // generate path and move to object location
   if (isObject(%agent.getCurrentPath()))
      %agent.getCurrentPath().delete();
   %agent.generatePath(%agent.seek_ObjectPosition);
   %agent.setMoveDestination(%agent.getCurrentPath().nextPosition());
}

function seek_foundObject(%agent, %object)
{
   %agent.seeking = false;
   %agent.seek_Found = true;
   %agent.seek_ObjectPosition = %object.getPosition();
   %agent.seek_OnObject = true;
   %agent.seek_Object = %object;
   
   // close enough, if object, warp to it
   if (%object.getType() & $TypeMasks::ItemObjectType)
   {
      %agent.setTransform(%object.getPosition());
   }
   
   // call solution onReachDestination
   eval(%agent.currentSolution @ "_onReachDestination(\"" @ %agent @ "\");");
}

function seek_cancelSeek(%agent)
{
   %agent.seeking = false;
   %agent.seek_Found = false;
   %agent.seek_ObjectPosition = "";
   %agent.seek_OnObject = false;
   %agent.seek_Object = 0;
}

function seek_closestObject(%agent, %objectMask, %objectName)
{
   %closestObject = 0;
   %closestDist = 10000;
   
   // search for objects
   InitContainerRadiusSearch(%agent.getPosition(), $IAISEEK_RADIUS, %objectMask);
   
   // loop through all results
   while ((%foundObject = containerSearchNext()) != 0)
   {
      // check if agent
      if (%foundObject == %agent)
         continue;
         
      // check if object dead
      if (isDead(%foundObject))
         continue;
      
      // check name if name is set..
      if (strlen(%objectName) > 0)
      {
         // check name is same
         if (strcmp(%foundObject.getDataBlock().getName(), %objectName) != 0)
         {
            continue;
         }
      }
      
      // check if new object closest
      %cDistance = VectorDist(%agent.getPosition(), %foundObject.getPosition());
      if (%cDistance < %closestDist)
      {
         %closestObject = %foundObject;
         %closestDist = %cDistance;
      }
   }
   
   return %closestObject;
}

function seek_findClosest(%agent, %objectMask, %objectName)
{
   %closestObject = 0;
   %closestDist = 10000;
   
   // search for objects
   InitContainerRadiusSearch(%agent.getPosition(), $IAISEEK_RADIUS, %objectMask);
   
   // loop through all results
   while ((%foundObject = containerSearchNext()) != 0)
   {
      // check if agent
      if (%foundObject == %agent)
         continue;
         
      // check if object dead
      if (isDead(%foundObject))
         continue;
      
      // check name if name is set..
      if (strlen(%objectName) > 0)
      {
         // check name is same
         if (strcmp(%foundObject.getDataBlock().getName(), %objectName) != 0)
         {
            continue;
         }
      }
      
      // check if new object closest
      %cDistance = VectorDist(%agent.getPosition(), %foundObject.getPosition());
      if (%cDistance < %closestDist)
      {
         %closestObject = %foundObject;
         %closestDist = %cDistance;
      }
   }
   
   // check if found an object
   if (isObject(%closestObject))
   {
      // check if close enough
      if (%closestDist < %agent.seek_DistanceAway)
      {
         // if seeking an agent, check the LOS
         if (isAgent(%closestObject))
         {
            if (haveLOS(%agent, %closestObject))
            {
               seek_foundObject(%agent, %closestObject);
            } else
            {
               seek_generatePath(%agent, %closestObject);
            }
         } else
         {
            // not an agent, must of found the object
            seek_foundObject(%agent, %closestObject);
         }
      } else
      {
         // not close enough, generate a path to the object
         seek_generatePath(%agent, %closestObject);
      }
   } else
   {
      // still didn't find the object
      // goto some random point if agent not already moving somewhere
      if (!((isObject(%agent.getCurrentPath())) && (%agent.getCurrentPath().hasNextNode())))
      {
         // generate a path to a random position
         if ((%agent.getCurrentPath() > 0) && (isObject(%agent.getCurrentPath())))
            %agent.getCurrentPath().delete();
         %agent.generatePath(getRandomPoint());
         
         // start moving along the path
         %agent.setMoveDestination(%agent.getCurrentPath().nextPosition());
      }
   }
   
   return %closestObject;
}

function seek_CheckMoved(%agent, %object)
{
   // get all objects in radius
   InitContainerRadiusSearch(%agent.getPosition(), $IAISEEK_RADIUS, %object.getType());

   // find the one we are after
   %foundItAgain = false;
   while ((%foundObject = containerSearchNext()) != 0)
   {
      // check if found the object
      if (%foundObject == %object)
      {
         %foundItAgain = true;
         break;
      }
   }
   
   // check if didn't find it
   if (%foundItAgain == false)
   {
      // set object as not found
      %this.seek_Found = false;
      return;
   }
   
   %distanceToObject = VectorDist(%agent.getPosition(), %object.getPosition());
   %objectPosition = $iAIPathMap.closestNode(%object.getPosition());
   
   // check if close enough
   if (%distanceToObject < %agent.seek_DistanceAway)
   {
      // if seeking an agent, check the LOS
      if (isAgent(%object))
      {
         if (haveLOS(%agent, %object))
         {
            seek_foundObject(%agent, %object);
         } else
         {
            seek_generatePath(%agent, %object);
         }
      } else
      {
         // not an agent, must of found the object
         seek_foundObject(%agent, %object);
      }
   } else
   {
      // not close enough, check if object moved
      if (%objectPosition !$= %agent.seek_ObjectPosition)
      {
         // object moved; need to make a new path to its new position
         %agent.seek_ObjectPosition = %objectPosition;
         
         // delete current path
         if (isObject(%agent.getCurrentPath()))
            %agent.getCurrentPath().delete();
            
         // generate a path to new position
         %agent.generatePath(%agent.seek_ObjectPosition);
         
         // start moving along the path
         %agent.setMoveDestination(%agent.getCurrentPath().nextPosition());
      } else
      {
         // hasn't moved, don't need to do anything :)
      }
   }
}

function isDead(%agent)
{
   if ((!isObject(%agent)) || (%agent == 0))
   {
      return true;
   }

   // if corpse or debris, must be dead
   if ((%agent.getType() & $TypeMasks::CorpseObjectType) |
      (%agent.getType() & $TypeMasks::DebrisObjectType))
   {
      return true;
   }
      
   // check if player/agent and state is dead
   if (((%agent.getType() & $TypeMasks::iAIAgentObjectType) ||
        (%agent.getType() & $TypeMasks::PlayerObjectType))
       && (%agent.getState() $= "Dead"))
   {
       return true;
   }
   
   // must be alive (or something that cannot be dead)
   return false;
}

function isAgent(%agent)
{
   if (%agent == 0)
   {
      return false;
   }
   
   if ((isObject(%agent)) && (
                     (%agent.getType() & $TypeMasks::iAIAgentObjectType) ||
                     (%agent.getType() & $TypeMasks::PlayerObjectType)))
      return true;
   else
      return false;
}

function haveLOS(%agent, %object)
{  
   // check valid objects
   if (!((isObject(%agent)) && (isObject(%object))))
      return;

   // start at the agents eye level
   %start = %agent.getEyeTransform();
   
   // end at objects object box center
   %end = %object.getWorldBoxCenter();
   
   // cast the ray
   %result = containerRayCast(%start, %end,  $TypeMasks::TerrainObjectType |
                                             $TypeMasks::InteriorObjectType |
                                             $TypeMasks::ItemObjectType |
                                             $TypeMasks::PlayerObjectType |
                                             $TypeMasks::iAIAgentObjectType |
                                             $TypeMasks::StaticObjectType,
                                             %agent);
                                             
   // get the object collided with
	%collidedWith = getWord(%result, 0);
   
   // check if object looking for
	if (%collidedWith == %object)
	   return true;
	else
		return false;
}