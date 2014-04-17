//-------------------------------------------------------------------
// Immersive AI :: Seek :: iAIPathGrid
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------
#include "terrain/terrData.h"
#include "interior/interiorInstance.h"
#include "sceneGraph/sceneGraph.h"
#include "sceneGraph/sceneState.h"
#include "platform/profiler.h"

#include "iAIPathNode.h"
#include "iAIPathGrid.h"
#include "iAIPathMap.h"
#include "iAIPathGlobal.h"

IMPLEMENT_CONOBJECT(iAIPathGrid);

iAIPathGrid::iAIPathGrid()
{
	this->setPosition(Point3F(0,0,0));
	this->mTypeMask |= iAIPathGridObjectType;

	this->mCompiled = false;
	this->mDensity = 0.0f;
	this->mGridBox = Box3F(0,0,0, 0,0,0);
	this->mShow = false;
	this->mNodesCountX = 0;
	this->mNodesCountY = 0;
}

iAIPathGrid::~iAIPathGrid()
{
	while (!this->mNodes.empty())
	{
		this->mNodes.erase((U32)0);
	}
	this->mNodes.clear();
}

bool iAIPathGrid::onAdd()
{
	// call Parent, ensure worked
	if (!Parent::onAdd())
	   return false;

	// create object box
	this->updateWorldBox();

	// add to scene
    gClientContainer.addObject(this);
    gClientSceneGraph->addObjectToScene(this);
	
	return true;
}

void iAIPathGrid::onRemove()
{
	// remove from scene
	removeFromScene();
	Parent::onRemove();
}

bool iAIPathGrid::prepRenderImage(SceneState *state, const U32 stateKey, const U32 startZone, const bool modifyBaseZoneState)
{
	// render if showing the node
	if (this->mShow)
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

void iAIPathGrid::renderObject(SceneState *state, SceneRenderImage *image)
{
	// save matrix to restore canonical state
	glPushMatrix();
	
	// going to render some lines!
	glBegin(GL_LINES);
	
	// render the nodes
	for (U32 i = 0; i < this->mNodes.size(); ++i)
	{
		glColor4ub(IAIPATHGLOBAL_GRID_RENDER_NODE_COLOUR);
		glVertex3fv(this->mNodes[i]->mPosition + IAIPATHGLOBAL_GRID_RENDER_CLEARANCE);
		glVertex3fv(this->mNodes[i]->mPosition + IAIPATHGLOBAL_GRID_RENDER_CLEARANCE + IAIPATHGLOBAL_GRID_RENDER_NODE_HEIGHT);

		// render neighbour links
		glColor4ub(IAIPATHGLOBAL_GRID_RENDER_COLOUR);
		for (U32 j = 0; j < this->mNodes[i]->mNeighbours.size(); ++j)
		{
			if (this->mNodes[i]->mNeighbours[j])
			{
				glVertex3fv(this->mNodes[i]->mPosition + IAIPATHGLOBAL_GRID_RENDER_CLEARANCE);
				glVertex3fv(this->mNodes[i]->mNeighbours[j]->mPosition + IAIPATHGLOBAL_GRID_RENDER_CLEARANCE);
			}
		}
	}

	// render the gridbox
	glColor4ub(IAIPATHGLOBAL_GRID_RENDER_BOX_COLOUR);
	dglWireCube(Point3F(this->mObjBox.len_x()/2, this->mObjBox.len_y()/2, this->mObjBox.len_z()/2), this->getBoxCenter());

	// end of line drawing
	glEnd();

	// restore canonical rendering state
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	// restore canonical maxtrix state
	glPopMatrix();

	// ensure canonical state is restored
	AssertFatal(dglIsInCanonicalState(), "Error, GL not in canonical state on exit");
}

void iAIPathGrid::toggleDisplay()
{
	this->mShow = !this->mShow;
}

void iAIPathGrid::clearGrid()
{
	this->mNodes.clear();
	this->mCompiled = false;
	this->mDensity = 0.0f;
	this->mGridBox = Box3F(0,0,0, 0,0,0);
	this->mNodesCountX = 0;
	this->mNodesCountY = 0;
}

bool iAIPathGrid::createTerrainGrid(const Point3F worldStart, const Point3F worldEnd, Vector<Box3F> &avoidList, const F32 density)
{
	// grab the terrain & ensure valid
	TerrainBlock *terrain = dynamic_cast<TerrainBlock*>(Sim::findObject("Terrain"));
	if (!terrain)
		return false;

	// clear all the nodes
	this->clearGrid();

	// set the grid start & end bounds
	this->mGridBox = Box3F(worldStart, worldEnd);
	
	// check the gridbox is actually valid
	if (!this->mGridBox.isValidBox())
	{
		Con::errorf("Immersive AI :: Seek :: Grid build failed - grid box not valid!");
		return false;
	}

	// the density is nodes per gridsize; default gridSize to 10.0f if none found
	this->mDensity = density / Con::getFloatVariable("Server::gridSize", 10.0f);

	// density step needs to be the squareroot, as operates in both X & Y
	F32 densityStep = 1 / mSqrt(this->mDensity);

	// calculate the count of nodes in x & y
	this->mNodesCountX = this->mGridBox.len_x() * mSqrt(this->mDensity);
	this->mNodesCountY = this->mGridBox.len_y() * mSqrt(this->mDensity);

	// create all nodes
	for (U16 iterX = 0; iterX < this->mNodesCountX; ++iterX)
	{
		for (U16 iterY = 0; iterY < this->mNodesCountY; ++iterY)
		{
			Point3F nodePos = this->mGridBox.min;
			nodePos.x += densityStep * iterX;
			nodePos.y += densityStep * iterY;

			// transform point to terrain transform
			terrain->getWorldTransform().mulP(nodePos);
			nodePos.convolveInverse(terrain->getScale());
			F32 height;
			if (terrain->getHeight(Point2F(nodePos.x, nodePos.y), &height))
			{
				nodePos.z = height;
				nodePos.convolve(terrain->getScale());
				terrain->getTransform().mulP(nodePos);
			}

			// create the node
			iAIPathNode *newNode = new iAIPathNode(nodePos, this, iterX, iterY);
			this->mNodes.push_back(newNode);
		}
	}

	// join all the node neighbours
	for (U32 iter = 0; iter < this->mNodes.size(); ++iter)
	{
		U16 currentX = this->mNodes[iter]->mIdX;
		U16 currentY = this->mNodes[iter]->mIdY;
		
		// north
		if (currentY < (this->mNodesCountY - 1))
			this->mNodes[iter]->addNeighbour(this->mNodes[iter+1]);

		// south
		if (currentY > 0)
			this->mNodes[iter]->addNeighbour(this->mNodes[iter-1]);

		// east
		if (currentX < (this->mNodesCountX-1))
			this->mNodes[iter]->addNeighbour(this->mNodes[iter + this->mNodesCountY]);

		// west
		if (currentX > 0)
			this->mNodes[iter]->addNeighbour(this->mNodes[iter - this->mNodesCountY]);

		// north-east
		if (currentX < (this->mNodesCountX - 1) && (currentY < (this->mNodesCountY - 1)))
			this->mNodes[iter]->addNeighbour(this->mNodes[ iter + this->mNodesCountY + 1]);

		// south-east
		if (currentX < (this->mNodesCountX - 1) && (currentY > 0))
			this->mNodes[iter]->addNeighbour(this->mNodes[ iter+this->mNodesCountY-1]);

		// south-west
		if ((currentX > 0) && (currentY > 0))
			this->mNodes[iter]->addNeighbour(this->mNodes[ iter - this->mNodesCountY - 1 ]);

		// north-west
		if ((currentX > 0) && (currentY < (this->mNodesCountY - 1)))
			this->mNodes[iter]->addNeighbour(this->mNodes[ iter - this->mNodesCountY + 1 ]);
	}

	// cull invalid and alone nodes
	for (U32 gridIter = 0; gridIter < this->mNodes.size(); ++gridIter)
	{
		// remove the node if in an invalid position or has no neighbours or in avoid list
		if ((!this->mNodes[gridIter]->isClear()) || (this->mNodes[gridIter]->mNeighbours.size() == 0) ||
			this->isInAvoidList(this->mNodes[gridIter], avoidList))
		{
			// remove it from its neighbours first
			for (U32 j = 0; j < this->mNodes[gridIter]->mNeighbours.size(); ++j)
			{
				this->mNodes[gridIter]->mNeighbours[j]->removeNeighbour(this->mNodes[gridIter]);
			}
			
			// remove from grid list
			this->mNodes.erase(gridIter);
		}
	}

	// set as compiled if any nodes in the grid
	this->mCompiled = (this->mNodes.size() > 0);

	// update the world box so renders properly
	this->updateWorldBox();

	return this->mCompiled;
}

bool iAIPathGrid::isInAvoidList(const iAIPathNode *node, const Vector<Box3F> &avoidList)
{
	// iterate over all boxes in the avoid list
	for (U32 z = 0; z < avoidList.size(); ++z)
	{
		Box3F nodeBox = Box3F(node->mPosition - (IAIPATHGLOBAL_NODE_CLEARANCE/2), node->mPosition + (IAIPATHGLOBAL_NODE_CLEARANCE/2));

		// see if the avoid list box overlaps the nodes box
		if (avoidList[z].isOverlapped(nodeBox))
			return true;
	}

	// didn't overlap on any boxes
	return false;
}

void iAIPathGrid::updateWorldBox()
{
	if (this->mCompiled)
	{
		Point3F min = Point3F(this->mNodes.front()->mPosition);
		Point3F max = Point3F(this->mNodes.front()->mPosition);

		// iterate over all nodes and find the min & max
		for (U32 i = 0; i < this->mNodes.size(); ++i)
		{
			if (this->mNodes[i]->mPosition.x < min.x)
				min.x = this->mNodes[i]->mPosition.x;
			if (this->mNodes[i]->mPosition.y < min.y)
				min.y = this->mNodes[i]->mPosition.y;
			if (this->mNodes[i]->mPosition.z < min.z)
				min.z = this->mNodes[i]->mPosition.z;


			if (this->mNodes[i]->mPosition.x > max.x)
				max.x = this->mNodes[i]->mPosition.x;
			if (this->mNodes[i]->mPosition.y > max.y)
				max.y = this->mNodes[i]->mPosition.y;
			if (this->mNodes[i]->mPosition.z > max.z)
				max.z = this->mNodes[i]->mPosition.z;
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

iAIPathNode* iAIPathGrid::getClosestNode(const Point3F position)
{
	PROFILE_SCOPE(iAIPathGrid_getClosestNode);

	F32 closestVec = 1000.0f;
	F32 currentVec = 1000.0f;
	iAIPathNode* closestNode = 0;

	// iterate over all nodes
	for (U32 i = 0; i < this->mNodes.size(); ++i)
	{
		// calculate the vector length
		currentVec = Point3F(position - this->mNodes[i]->mPosition).len();

		// check if its closest one found so far
		if (currentVec < closestVec)
		{
			// save as closest so far
			closestVec = currentVec;
			closestNode = this->mNodes[i];
		}
	}

	return closestNode;
}