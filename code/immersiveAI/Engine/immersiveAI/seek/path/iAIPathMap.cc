//-------------------------------------------------------------------
// Immersive AI :: Seek :: iAIPathMap
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

#include "game/missionArea.h"
#include "platform/profiler.h"
#include "terrain/terrData.h"
#include "game/gameConnection.h"
#include "interior/interiorInstance.h"

#include "iAIPathMap.h"
#include "iAIPathGlobal.h"
#include "iAIPathGrid.h"
#include "iAIPathNode.h"

IMPLEMENT_CONOBJECT(iAIPathMap);

U32 iAIPathMap::smNodeCount = 0;

iAIPathMap::iAIPathMap()
{
	this->mCompiled = false;
	this->mTerrainGridIndex = 0;
}

iAIPathMap::~iAIPathMap()
{
	this->clearMap();
}

bool iAIPathMap::initialize()
{
	// if not compiled, create the path map
	if (!this->mCompiled)
	{
		// create a new pathmap
		this->mCompiled = this->createPathMap();
	}
	return this->mCompiled;
}

bool iAIPathMap::createPathMap()
{
	Con::iAIMessagef("Immersive AI :: Seek :: Building PathMap...");

	// clear any current path map
	this->clearMap();

	// generate an avoid list of all the grids
	Vector<Box3F> avoidList;
	for (U32 i = 0; i < this->mGrids.size(); ++i)
		avoidList.push_back(this->mGrids[i]->getWorldBox());

	// calculate the entire mission area
	MissionArea *missionAreaPtr = dynamic_cast<MissionArea*>(Sim::findObject("MissionArea"));
	if (!missionAreaPtr)
		return false;

	// set grid points are the initial mission area points in x&y to the extent of the mission area
	Point3F gridStart = Point3F(missionAreaPtr->getArea().point.x, missionAreaPtr->getArea().point.y, 100.0);
	Point3F gridEnd = Point3F(missionAreaPtr->getArea().point.x + missionAreaPtr->getArea().extent.x, missionAreaPtr->getArea().point.y + missionAreaPtr->getArea().extent.y, 100.0);

	// pointer for the terrain grid
	iAIPathGrid *terrainGrid = new iAIPathGrid();

	// generate a grid for the terrain, avoiding the avoid list
	if (terrainGrid->createTerrainGrid(gridStart, gridEnd, avoidList, IAIPATHGLOBAL_GRID_DENSITY_TERRAIN))
	{
		// add to pathmap collection
		this->mGrids.push_back(terrainGrid);

		// set the type of the terrain grid to just a zone type;
		// needed to optimise the getClosestNode function
		// (so we don't always get the terrain grid as the collided grid!)
		terrainGrid->mTypeMask |= PhysicalZoneObjectType;

		// add grid to the scene
		terrainGrid->registerObject();

		// set the terrain grid index variable
		this->mTerrainGridIndex = this->mGrids.size() - 1;
	}

	// iterate over all grids to calculate total node count
	for (U32 i = 0; i < this->mGrids.size(); ++i)
		iAIPathMap::smNodeCount += this->mGrids[i]->mNodes.size();

	Con::iAIMessagef("Immersive AI :: Seek :: PathMap Built!");
	return true;
}

void iAIPathMap::clearMap()
{
	// iterate over nodes and delete all
	for (U32 i = 0; i < this->mGrids.size(); ++i)
	{
		if ((this->mGrids[i]) && (!this->mGrids[i]->isDeleted()))
			this->mGrids[i]->deleteObject();
		this->mGrids[i] = 0;
	}

	// set as uncompiled
	this->mCompiled = false;
	this->mTerrainGridIndex = 0;
	iAIPathMap::smNodeCount = 0;
}

void iAIPathMap::toggleDisplay()
{
	// iterate over all grids
	for (U32 i = 0; i < this->mGrids.size(); ++i)
	{
		// toggle the display
		this->mGrids[i]->toggleDisplay();
	}
}

iAIPathNode* iAIPathMap::getClosestNode(const Point3F position)
{
	PROFILE_SCOPE(iAIPathMap_getClosestNode);
	iAIPathNode* closestNode = 0;

	RayInfo rInfo;

	// set the ray well above & below terrain
	Point3F start = Point3F(position.x, position.y, position.z + 1000.0f);
	Point3F end = Point3F(position.x, position.y, position.z - 1000.0f);

	// see what grid node is in
	if (gServerContainer.castRay(position, end, iAIPathGridObjectType, &rInfo))
	{
		// cast the found grid and find its closest node
		if (iAIPathGrid* collidedGrid = dynamic_cast<iAIPathGrid*>(rInfo.object))
			closestNode = collidedGrid->getClosestNode(position);
	} else
	{
		// not in a grid, must be in terrain grid!
		closestNode = this->mGrids[this->mTerrainGridIndex]->getClosestNode(position);
	}

	return closestNode;
}

ConsoleMethodGroupBegin(iAIPathMap, ScriptFunctions, "iAIPathMap Script Functions");

ConsoleMethod( iAIPathMap, initialize, bool, 2, 2,
			  "bool iAIPathMap.initialize() - Initializes the PathMap for the current mission.")
{
	return object->initialize();
}

ConsoleMethod( iAIPathMap, toggleDisplay, void, 2, 2,
			  "void iAIPathMap.toggleDisplay() - Toggles displaying of the pathmap.")
{
	object->toggleDisplay();
}

ConsoleMethod( iAIPathMap, closestNode, const char *, 3, 3,
			  "Point3F iAIPathMap.closestNode(Point3F pos) - Get the closest node to the supplied position.")
{
	// ensure pos passed
	if (dStrlen(argv[2]) != 0) {

		// pass the args into a Point3F
		Point3F position;
		dSscanf(argv[2], "%f %f %f", &position.x, &position.y, &position.z);

		// get the closest node
		iAIPathNode* closestNode = object->getClosestNode(position);

		// ensure found a closest index
		if (closestNode)
		{
			char *returnBuffer = Con::getReturnBuffer(256);
			dSprintf(returnBuffer, 256, "%f %f %f", closestNode->mPosition.x, closestNode->mPosition.y, closestNode->mPosition.z);

			return returnBuffer;
		} else
		{
			Con::errorf("Immersive AI :: Seek :: PathMap - no node found near %f %f %f", &position.x, &position.y, &position.z);
			return "";
		}
	} else
	{
		Con::errorf("Immersive AI :: Seek :: PathMap - no Point3F parsed to ClosestNode!");
		return "";
	}
}

ConsoleMethodGroupEnd(iAIPathMap, ScriptFunctions);