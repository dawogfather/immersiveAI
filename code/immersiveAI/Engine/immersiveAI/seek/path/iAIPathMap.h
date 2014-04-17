//-------------------------------------------------------------------
// Immersive AI :: Seek :: iAIPathMap
// Copyright (c) 2006 Gavin Bunney & Tom Romano
//-------------------------------------------------------------------

//-------------------------------------------------------------------
/// @file iAIPathMap.h
//-------------------------------------------------------------------
/// @class iAIPathMap
/// @author Gavin Bunney
/// @version 1.0
/// @brief Map of all nodes within the mission.
/// 
/// Holds a collection of all grids (collections of nodes) for the
/// current server map. The map creates grids and maps links between
/// them allowing for A* pathfinding.
//-------------------------------------------------------------------
#ifndef _IAIPATHMAP_H_
#define _IAIPATHMAP_H_

#include "iAIPathMap.h"
#include "iAIPathGrid.h"
#include "iAIPathNode.h"

class iAIPathMap : public SimObject
{
	typedef SimObject Parent;
	friend class iAIPath;

public:
	
	//-------------------------------------------------------------------
	/// @var DECLARE_CONOBJECT(iAIPathMap)
	/// @brief TorqueScript object.
	//-------------------------------------------------------------------
	DECLARE_CONOBJECT(iAIPathMap);

	//-------------------------------------------------------------------
	/// @fn iAIPathMap() 
	/// @brief Default constructor.
	//-------------------------------------------------------------------
	iAIPathMap();

	//-------------------------------------------------------------------
	/// @fn ~iAIPathMap() 
	/// @brief Deconstructor which clears all the grids.
	//-------------------------------------------------------------------
	~iAIPathMap();

	//-------------------------------------------------------------------
	/// @fn bool initialize()
	/// @brief Initializes the pathmap for the current server map.
	///
	/// @return initialization success.
	//-------------------------------------------------------------------
	bool initialize();

	//-------------------------------------------------------------------
	/// @fn bool createPathMap()
	/// @brief Creates the pathmap for the current server map.
	///
	/// @return creation success.
	//-------------------------------------------------------------------
	bool createPathMap();

	//-------------------------------------------------------------------
	/// @fn void clearMap()
	/// @brief Clears the map.
	//-------------------------------------------------------------------
	void clearMap();

	//-------------------------------------------------------------------
	/// @fn void toggleDisplay()
	/// @brief Toggle displaying of the pathmap.
	//-------------------------------------------------------------------
	void toggleDisplay();

	//-------------------------------------------------------------------
	/// @fn iAIPathNode* getClosestNode(const Point3F position)
	/// @brief Retrieves the closest node to the parsed position
	///
	/// @param position world point of node to find
	/// @return pointer to closest node
	//-------------------------------------------------------------------
	iAIPathNode* getClosestNode(const Point3F position);

	//-------------------------------------------------------------------
	/// @fn static U32 smNodeCount
	/// @brief Total count of nodes in the Path Map.
	//-------------------------------------------------------------------
	static U32 smNodeCount;
	
protected:

	//-------------------------------------------------------------------
	/// @var Vector<iAIPathGrid*> mGrids
	/// @brief Vector of all grids in the world.
	//-------------------------------------------------------------------
	Vector<iAIPathGrid*> mGrids;

	//-------------------------------------------------------------------
	/// @var bool mCompiled
	/// @brief Flag for when a pathmap has been compiled successfully.
	//-------------------------------------------------------------------
	bool mCompiled;

	//-------------------------------------------------------------------
	/// @var U32 mTerrainGridIndex
	/// @brief Holds the index of the terrain grid within mGrids.
	//-------------------------------------------------------------------
	U32 mTerrainGridIndex;
};

#endif