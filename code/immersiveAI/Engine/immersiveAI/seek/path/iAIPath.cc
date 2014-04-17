//-------------------------------------------------------------------
// Immersive AI :: Seek :: iAIPath
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

#include "sceneGraph/sceneGraph.h"
#include "game/cameraSpline.h"

#include "iAIPath.h"
#include "iAIPathMap.h"
#include "iAIPathFind.h"
#include "iAIPathGlobal.h"

IMPLEMENT_CO_NETOBJECT_V1(iAIPath);

iAIPath::iAIPath()
{
	this->mTypeMask |= iAIPathObjectType;
	this->setPosition(Point3F(0,0,0));

	this->mTraversing = false;
	this->mShow = false;
	this->mRenderSpline = true;
	this->mLastNode = 0;

	// default path colour is orangey
	this->mPathColour = ColorI(157, 93, 31, 255);

	// default node colour is redish
	this->mPathNodeColour = ColorI(157, 31, 60, 255);
}

bool iAIPath::createPath(iAIPathMap* pathMap, Point3F start, Point3F end, const bool smoothPath)
{
	iAIPathNode* startNode = pathMap->getClosestNode(start);
	iAIPathNode* endNode = pathMap->getClosestNode(end);

	// check if start and end nodes in the same position
	if (startNode->mPosition == endNode->mPosition)
	{
		// just push on the end node
		this->mPathNodes.push_back(endNode);
		return true;
	}

	// get an instance of the singleton pathfinder
	iAIPathFind* pathFinder = iAIPathFind::getInstance();
	this->mTraversing = false;

	// find the path; if unable to find a path, loop until IAIPATHGLOBAL_PATH_RETRY_COUNT is reached
	U32 retryCount = 0;
	while ((!(pathFinder->generatePath(startNode, endNode, this->mPathNodes, smoothPath))) && (retryCount <= IAIPATHGLOBAL_PATH_RETRY_COUNT))
		++retryCount;

	// check that a path was found
	if (this->mPathNodes.size() > 0)
	{
		// update the world box, so that path will render
		this->updateWorldBox();
		return true;
	} else
	{
		Con::errorf("Immersive AI :: Seek :: Unable to find a valid path from %f, %f, %f to %f, %f, %f", start.x, start.y, start.z, end.x, end.y, end.z);
		return false;
	}
}

Point3F iAIPath::getNextPosition()
{
	// only pop previous node if we are already traversing!
	if (this->mTraversing)
	{
		// check that this isnt the last node
		if (this->mPathNodes.size() > 1)
		{
			// update the last node
			this->mLastNode = this->mPathNodes.front();

			// remove the previous node from the list
			this->mPathNodes.pop_front();
		}
	} else
	{
		this->mTraversing = true;
	}

	// return the node, if any left
	if (this->mPathNodes.size() > 0)
	{
		iAIPathNode* returnNode = this->mPathNodes.front();
		
		// if we are going to return the last node, clear the list
		if (this->mPathNodes.size() == 1)
		{
			this->mPathNodes.clear();
			this->mLastNode = 0;
		}

		return returnNode->mPosition;
	} else
	{
		return IAIPATHGLOBAL_INVALID_POSITION;
	}
}

bool iAIPath::hasNextNode()
{
	return (this->mPathNodes.size() > 0);
}

U32 iAIPath::nodeCount()
{
	return this->mPathNodes.size();
}

void iAIPath::updateWorldBox()
{
	// only need a both if there is a path ;)
	if (this->mPathNodes.size() > 0)
	{
		Point3F min = Point3F(this->mPathNodes.front()->mPosition);
		Point3F max = Point3F(this->mPathNodes.front()->mPosition);

		// iterate over all nodes and find the min & max
		for (U32 i = 0; i < this->mPathNodes.size(); ++i)
		{
			if (this->mPathNodes[i]->mPosition.x < min.x)
				min.x = this->mPathNodes[i]->mPosition.x;
			if (this->mPathNodes[i]->mPosition.y < min.y)
				min.y = this->mPathNodes[i]->mPosition.y;
			if (this->mPathNodes[i]->mPosition.z < min.z)
				min.z = this->mPathNodes[i]->mPosition.z;


			if (this->mPathNodes[i]->mPosition.x > max.x)
				max.x = this->mPathNodes[i]->mPosition.x;
			if (this->mPathNodes[i]->mPosition.y > max.y)
				max.y = this->mPathNodes[i]->mPosition.y;
			if (this->mPathNodes[i]->mPosition.z > max.z)
				max.z = this->mPathNodes[i]->mPosition.z;
		}

		// set position as halfway point
		this->setPosition(min + ((max - min) / 2));
		
		// create a box to encompass the entire path
		this->mObjBox.min.set(-(this->getPosition() - min));
		this->mObjBox.max.set(max - this->getPosition());

		// must reset world box & transform when changing object box
		this->resetWorldBox();
		this->setRenderTransform(mObjToWorld);
	}
}

bool iAIPath::onAdd()
{
	// call Parent, ensure worked
	if(!Parent::onAdd())
	   return false;

	// create object box
	this->updateWorldBox();

	// add to scene
    gClientContainer.addObject(this);
    gClientSceneGraph->addObjectToScene(this);
	
	return true;
}

void iAIPath::onRemove()
{
	// remove from scene
	removeFromScene();
	Parent::onRemove();
}

bool iAIPath::prepRenderImage(SceneState *state, const U32 stateKey, const U32 startZone, const bool modifyBaseZoneState)
{
	// render if there is a path to render and want to show it
	if ((this->mShow) && (this->mPathNodes.size() > 0))
	{
		// return if last state
		if (this->isLastState(state, stateKey)) return false;
		
		// set last state
		this->setLastState(state, stateKey);

		// see if object rendered
		if (state->isObjectRendered(this))
		{
			// get a SceneRenderImage to show on	
			SceneRenderImage* image = new SceneRenderImage;
			image->obj = this;

			// insert into scene image
			state->insertRenderImage(image);
		}
	}

	return false;
}

void iAIPath::renderObject(SceneState *state, SceneRenderImage *image)
{
	// save matrix to restore canonical state
	glPushMatrix();
	
	// enable blend
	glEnable(GL_BLEND);

	// always rendering lines
	glBegin(GL_LINES);

	// see if we want a linear or spline path
	if (this->mRenderSpline)
	{
		CameraSpline pathSpline;

		// add the lastNode to the spline
		if (this->mLastNode)
		{
			pathSpline.push_back(new CameraSpline::Knot(
					this->mLastNode->mPosition,
					QuatF(0, 0, 0, 0),
					1.0f,
					CameraSpline::Knot::NORMAL,
					CameraSpline::Knot::SPLINE));
		}

		// iterate over all the nodes: add to spline and draw the stick
		for (U32 j = 0; j < this->mPathNodes.size(); j++)
		{
			if (this->mPathNodes[j])
			{
				// add a new knot for each path node
				pathSpline.push_back(new CameraSpline::Knot(
						this->mPathNodes[j]->mPosition,
						QuatF(0, 0, 0, 0),
						1.0f,
						CameraSpline::Knot::NORMAL,
						CameraSpline::Knot::SPLINE));
			}

			// draw the path node
			glColor4ub(this->mPathNodeColour.red, this->mPathNodeColour.green, this->mPathNodeColour.blue, this->mPathNodeColour.alpha);
			glVertex3fv(this->mPathNodes[j]->mPosition + IAIPATHGLOBAL_PATH_RENDER_CLEARANCE);
			glVertex3fv(this->mPathNodes[j]->mPosition + IAIPATHGLOBAL_PATH_RENDER_CLEARANCE + IAIPATHGLOBAL_PATH_RENDER_NODE_HEIGHT);
		}

		F32 iter = 0.0f;
		Point3F lastPoint = Point3F(0,0,0);

		// draw the entire spline
		while (iter < (pathSpline.size()-1))
		{
			// get set k to the current knot value
			CameraSpline::Knot k;
			pathSpline.value(iter, &k);

			// advance the spline iter
			iter = pathSpline.advanceDist(iter, 2.0f);

			// get the knot point information
			Point3F newPoint;
			k.mRotation.mulP(Point3F(0,0,0), &newPoint);
			newPoint += k.mPosition;

			// check if there is a last point info stored
			if (lastPoint == Point3F(0,0,0))
			{
				lastPoint = newPoint;
			} else
			{
				glColor4ub(this->mPathColour.red, this->mPathColour.green, this->mPathColour.blue, this->mPathColour.alpha);

				// draw a line between the new point and the last point
				glVertex3fv(newPoint + IAIPATHGLOBAL_PATH_RENDER_CLEARANCE);
				glVertex3fv(lastPoint + IAIPATHGLOBAL_PATH_RENDER_CLEARANCE);

				// set the new last point to the new point
				lastPoint = newPoint;
			}
		}

	} else
	{
		// draw a path between the last node and the current start node
		if (this->mLastNode)
		{
			glColor4ub(this->mPathColour.red, this->mPathColour.green, this->mPathColour.blue, this->mPathColour.alpha);
			glVertex3fv(this->mLastNode->mPosition + IAIPATHGLOBAL_PATH_RENDER_CLEARANCE);
			glVertex3fv(this->mPathNodes[0]->mPosition+ IAIPATHGLOBAL_PATH_RENDER_CLEARANCE);
		}

		for (int j = 1; j < this->mPathNodes.size(); j++)
		{
			if (this->mPathNodes[j])
			{
				// draw the path line
				glColor4ub(this->mPathColour.red, this->mPathColour.green, this->mPathColour.blue, this->mPathColour.alpha);
				glVertex3fv(this->mPathNodes[j-1]->mPosition + IAIPATHGLOBAL_PATH_RENDER_CLEARANCE);
				glVertex3fv(this->mPathNodes[j]->mPosition + IAIPATHGLOBAL_PATH_RENDER_CLEARANCE);

				// draw the node stick
				glColor4ub(this->mPathNodeColour.red, this->mPathNodeColour.green, this->mPathNodeColour.blue, this->mPathNodeColour.alpha);
				glVertex3fv(this->mPathNodes[j]->mPosition + IAIPATHGLOBAL_PATH_RENDER_CLEARANCE);
				glVertex3fv(this->mPathNodes[j]->mPosition + IAIPATHGLOBAL_PATH_RENDER_CLEARANCE + IAIPATHGLOBAL_PATH_RENDER_NODE_HEIGHT);
			}
		}
	}

	// end of line drawing
	glEnd();

	// disable the blend
	glDisable(GL_BLEND);

	// restore canonical maxtrix state
	glPopMatrix();

	// ensure canonical state is restored
	AssertFatal(dglIsInCanonicalState(), "Error, GL not in canonical state on exit");
}


void iAIPath::initPersistFields()
{
	Parent::initPersistFields();
 
	addGroup("Misc");
		addField("showPath", TypeBool, Offset(mShow, iAIPath), "Display the path on rendering.");
		addField("renderSpline", TypeBool, Offset(mRenderSpline, iAIPath), "Render the path as a spline. If set to false, will render as linear.");
		addField("pathColour", TypeColorI, Offset(mPathColour, iAIPath), "The colour of the rendered path.");
		addField("pathNodeColour", TypeColorI, Offset(mPathNodeColour, iAIPath), "The colour of the nodes on the rendered path.");
	endGroup("Misc");
}

ConsoleMethodGroupBegin(iAIPath, ScriptFunctions, "iAIPath Script Functions");

ConsoleMethod( iAIPath, createPath, bool, 4, 5,
			  "bool iAIPath.createPath(Point3F start, Point3F goal, bool smoothPath = true) - Create a path between the two points.")
{
	// ensure pos passed
	if ((dStrlen(argv[2]) != 0) && (dStrlen(argv[3]) != 0))
	{
		// parse arguments into points and create the path
		Point3F start;
		Point3F goal;
		dSscanf(argv[2], "%f %f %f", &start.x, &start.y, &start.z);
		dSscanf(argv[3], "%f %f %f", &goal.x, &goal.y, &goal.z);

		// get the path map variable
		iAIPathMap* pathMap = 0;
		if (Sim::findObject(dAtoi(Con::getVariable("$iAIPathMap")), pathMap))
		{
			// see of the smoothPath parameter is set
			if (dStrlen(argv[4]) != 0)
				return (object->createPath(pathMap, start, goal, dAtob(argv[4])));
			else
				return (object->createPath(pathMap, start, goal));
		} else
		{
			Con::errorf("Immersive AI :: Seek :: Path - unable to find the iAIPathMap");
			return false;
		}
	} else
	{
		Con::errorf("Immersive AI :: Seek :: Path- not enough nodes passed to CreatePath!");
		return false;
	}
}

ConsoleMethod( iAIPath, nextPosition, const char*, 2, 2,
			  "Point3F iAIPath.nextPosition() - Get the next position on the path.")
{
	char *returnBuffer = Con::getReturnBuffer(256);

	Point3F nextPosition = object->getNextPosition();
	if (nextPosition != IAIPATHGLOBAL_INVALID_POSITION)
	{
		dSprintf(returnBuffer, 256, "%f %f %f", nextPosition.x, nextPosition.y, nextPosition.z);
	} else
	{
		dSprintf(returnBuffer, 256, "");
	}

	return returnBuffer;
}

ConsoleMethod( iAIPath, hasNextNode, bool, 2, 2,
			  "bool iAIPath.hasNextNode() - Returns if the path has another node.")
{
	return (object->hasNextNode());
}

ConsoleMethod( iAIPath, nodeCount, S32, 2, 2,
			  "U32 iAIPath.nodeCount() - Returns number of nodes left in the path.")
{
	return (object->nodeCount());
}


ConsoleMethodGroupEnd(iAIPath, ScriptFunctions);