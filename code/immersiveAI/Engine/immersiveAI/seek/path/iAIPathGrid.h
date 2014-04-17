//-------------------------------------------------------------------
// Immersive AI :: Seek :: iAIPathGrid
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/// @file iAIPathGrid.h
//-------------------------------------------------------------------
/// @class iAIPathGrid
/// @author Gavin Bunney
/// @version 1.0
/// @brief Represents a small collection of nodes.
/// 
/// Holds a collection of nodes based around a set grid position.
/// Used in combination with other grids to form the iAIPathMap.
/// <br><br>
/// TypeMask |= iAIPathGridObjectType;
//-------------------------------------------------------------------
#ifndef _IAIPATHGRID_H_
#define _IAIPATHGRID_H_

#include "iAIPathNode.h"
#include "iAIPathGrid.h"
#include "iAIPathMap.h"

class iAIPathGrid : public SceneObject
{
	friend class iAIPathMap;

	typedef SceneObject Parent;

public:
	
	//-------------------------------------------------------------------
	/// @var DECLARE_CONOBJECT(iAIPathGrid)
	/// @brief TorqueScript object.
	//-------------------------------------------------------------------
	DECLARE_CONOBJECT(iAIPathGrid);

	//-------------------------------------------------------------------
	/// @fn iAIPathGrid() 
	/// @brief Default constructor.
	//-------------------------------------------------------------------
	iAIPathGrid();

	//-------------------------------------------------------------------
	/// @fn ~iAIPathGrid() 
	/// @brief Default deconstructor.
	//-------------------------------------------------------------------
	~iAIPathGrid();

	//-------------------------------------------------------------------
	/// @fn bool onAdd()
	/// @brief Called on adding to Sim.
	//-------------------------------------------------------------------
	bool onAdd();

	//-------------------------------------------------------------------
	/// @fn bool onRemove()
	/// @brief Called on removal from Sim.
	//-------------------------------------------------------------------
	void onRemove();

	//-------------------------------------------------------------------
	/// @fn bool prepRenderImage(SceneState *state, const U32 stateKey,
	///     const U32 startZone, const bool modifyBaseZoneState = false)
	/// @brief Called on scene render. Detects if the path needs to be
	///        rendered, calls renderObject if need to render the path.
	///
	/// @param state the current SceneState.
	/// @param stateKey key for the state.
	/// @param startZone zone for scene start.
	/// @param modifyBaseZoneState flag to modify the base zone state.
	/// @return scene state change.
	//-------------------------------------------------------------------
	bool prepRenderImage(SceneState *state, const U32 stateKey, const U32 startZone, const bool modifyBaseZoneState = false);

	//-------------------------------------------------------------------
	/// @fn void renderObject(SceneState *state, SceneRenderImage *image)
	/// @brief Renders the path.
	///
	/// @param state the current SceneState.
	/// @param image scene to render in.
	//-------------------------------------------------------------------
	void renderObject(SceneState *state, SceneRenderImage *image);

	//-------------------------------------------------------------------
	/// @fn void toggleDisplay()
	/// @brief Toggles displaying of the grid.
	//-------------------------------------------------------------------
	void toggleDisplay();

	//-------------------------------------------------------------------
	/// @fn bool createTerrainGrid(const Point3F worldStart,
	///						       const Point3F worldEnd,
	///                            Vector<Box3F> &avoidList,
	///                            const F32 density = 1.0f)
	/// @brief Creates a grid for the parsed area and density, avoiding
	///        the locations within the avoidList.
	///
	/// @param worldStart The starting point in world coords for the grid
	/// @param worldEnd The ending point in world coords for the grid
	/// @param avoidList Vector of boxes (in world points) to avoid
	/// @param density Density of node coverage. Default 1.0f.
	/// @return creation success.
	//-------------------------------------------------------------------
	bool createTerrainGrid(const Point3F worldStart, const Point3F worldEnd, Vector<Box3F> &avoidList, const F32 density = 1.0f);

	//-------------------------------------------------------------------
	/// @fn void clearGrid()
	/// @brief Clears the grid.
	//-------------------------------------------------------------------
	void clearGrid();

	//-------------------------------------------------------------------
	/// @fn iAIPathNode* getClosestNode(const Point3F position)
	/// @brief Retrieves the closest node to the parsed position
	///
	/// @param position world point of node to find
	/// @return pointer to closest node
	//-------------------------------------------------------------------
	iAIPathNode* getClosestNode(const Point3F position);

	//-------------------------------------------------------------------
	/// @var Box3F mGridBox
	/// @brief Box encompassing the whole grid.
	//-------------------------------------------------------------------
	Box3F mGridBox;

private:
	
	//-------------------------------------------------------------------
	/// @fn bool isInAvoidList(const iAIPathNode *node,
	///          const Vector<Box3F> &avoidList)
	/// @brief Checks if the given node is within the avoid list.
	///
	/// @param node Node to check.
	/// @param avoidList Vector of Boxes to check within.
	/// @return True if node is in the avoid list space.
	//-------------------------------------------------------------------
	bool isInAvoidList(const iAIPathNode *node, const Vector<Box3F> &avoidList);

	//-------------------------------------------------------------------
	/// @fn void updateWorldBox()
	/// @brief Updates the grids worldbox to encompass the entire grid.
	//-------------------------------------------------------------------
	void updateWorldBox();

protected:
	
	//-------------------------------------------------------------------
	/// @var Vector<iAIPathNode*> mNodes
	/// @brief Vector of all nodes in the grid.
	//-------------------------------------------------------------------
	Vector<iAIPathNode*> mNodes;

	//-------------------------------------------------------------------
	/// @var F32 mDensity
	/// @brief Density of nodes per unit of worldspace.
	//-------------------------------------------------------------------
	F32 mDensity;

	//-------------------------------------------------------------------
	/// @var U16 mNodesCountX
	/// @brief Count of nodes of pathmap grid, in X direction.
	//-------------------------------------------------------------------
	U16 mNodesCountX;

	//-------------------------------------------------------------------
	/// @var U16 mNodesCountY
	/// @brief Count of nodes of pathmap grid, in Y direction.
	//-------------------------------------------------------------------
	U16 mNodesCountY;

	//-------------------------------------------------------------------
	/// @var bool mCompiled
	/// @brief Flag for when a pathmap has been compiled successfully.
	//-------------------------------------------------------------------
	bool mCompiled;

	//-------------------------------------------------------------------
	/// @var bool mShow
	/// @brief Flag to render the grid.
	//-------------------------------------------------------------------
	bool mShow;
};

#endif