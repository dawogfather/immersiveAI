//-------------------------------------------------------------------
// Immersive AI :: Seek :: iAIPathNode
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

#include "sceneGraph/sceneGraph.h"
#include "sceneGraph/sceneState.h"

#include "iAIPathGlobal.h"
#include "iAIPathNode.h"
#include "iAIPathMap.h"

iAIPathNode::iAIPathNode(const Point3F position, iAIPathGrid* pathGrid, const U16 idX, const U16 idY)
{
	this->mPosition = position;
	this->mIdX = idX;
	this->mIdY = idY;
	this->mParentGrid = pathGrid;

	this->mMoveModifier = 0.0f;
	this->mFitness = 0.0f;
	this->mLowestCostFromStart = 0.0f;
	this->mHeuristicCostToGoal = 0.0f;
	this->mParent = 0;
	this->mOpen = false;
	this->mClosed = false;
}

iAIPathNode::~iAIPathNode()
{
	while (!this->mNeighbours.empty())
	{
		this->mNeighbours.erase((U32)0);
	}
	this->mNeighbours.clear();
}

bool iAIPathNode::isClear()
{
	Point3F start = this->mPosition;
	Point3F end = this->mPosition + Point3F(0, 0, IAIPATHGLOBAL_NODE_CLEARANCE.z);

	RayInfo dummy;

	// if collided with something, isn't clear
	if (gServerContainer.castRay(start, end, IAIPATHGLOBAL_COLLISION_MASK, &dummy))
	{
		return false;
	}

	// must be valid
	return true;
}

bool iAIPathNode::isNeighbourValid(const Point3F neighbourPosition)
{
	// calculate vector in z
	Point3F vec = this->mPosition - neighbourPosition;
	F32 zSq = vec.z * vec.z;

	// ensure difference in node height is valid
	if (zSq > IAIPATHGLOBAL_MAX_SLOPE)
		return false;

	RayInfo dummy;
	
	// quick check from node to neighboour position
	if (gServerContainer.castRay(this->mPosition, neighbourPosition, IAIPATHGLOBAL_COLLISION_MASK, &dummy))
		return false;

	// check 4 points around clearance box
	Point3F offset = -Point3F(IAIPATHGLOBAL_NODE_CLEARANCE.x/2, 0, 0);
	if (gServerContainer.castRay(this->mPosition + offset, neighbourPosition + offset, IAIPATHGLOBAL_COLLISION_MASK, &dummy))
		return false;
	offset = Point3F(IAIPATHGLOBAL_NODE_CLEARANCE.x/2, 0, 0);
	if (gServerContainer.castRay(this->mPosition + offset, neighbourPosition + offset, IAIPATHGLOBAL_COLLISION_MASK, &dummy))
		return false;
	offset = Point3F(0, 0, IAIPATHGLOBAL_NODE_CLEARANCE.z);
	if (gServerContainer.castRay(this->mPosition + offset, neighbourPosition + offset, IAIPATHGLOBAL_COLLISION_MASK, &dummy))
		return false;
	offset = Point3F(IAIPATHGLOBAL_NODE_CLEARANCE.x/2, 0, IAIPATHGLOBAL_NODE_CLEARANCE.z);
	if (gServerContainer.castRay(this->mPosition + offset, neighbourPosition + offset, IAIPATHGLOBAL_COLLISION_MASK, &dummy))
		return false;

	// must be valid
	return true;
}

bool iAIPathNode::addNeighbour(iAIPathNode* neighbour)
{
	if (neighbour)
	{
		// check that neighbour is valid before adding
		if (isNeighbourValid(neighbour->mPosition))
		{
			this->mNeighbours.push_back(neighbour);
			return true;
		}
	}
	return false;
}

bool iAIPathNode::hasNeighbour(iAIPathNode* neighbour)
{
	if (neighbour)
	{
		// iterate over all neighbours
		for (U32 i = 0; i < this->mNeighbours.size(); i++)
		{
			// if position match, then it has that neighbour!
			if (this->mNeighbours[i]->mPosition == neighbour->mPosition)
				return true;
		}
	}
	return false;
}

bool iAIPathNode::removeNeighbour(iAIPathNode* neighbour)
{
	if (neighbour)
	{
		// iterate over all neighbours
		for (U32 i = 0; i < this->mNeighbours.size(); i++)
		{
			// if position match, then it has that neighbour!
			if (this->mNeighbours[i]->mPosition == neighbour->mPosition)
			{
				// remove the neighbour
				this->mNeighbours.erase(i);
				return true;
			}
		}
	}
	return false;
}

void iAIPathNode::updateMoveModifier()
{
	RayInfo dummy;
	// check if the node is in water
	if (gServerContainer.castRay(this->mPosition + Point3F(0,0,1000.0f), this->mPosition - Point3F(0, 0, IAIPATHGLOBAL_NODE_CLEARANCE.z / 2), WaterObjectType, &dummy))
		this->mMoveModifier = IAIPATHGLOBAL_MOVE_MODIFIER_WATER;
	else
		this->mMoveModifier = 0.0f;
}