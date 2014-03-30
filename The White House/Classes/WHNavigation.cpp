//
//  WHNavigation.cpp
//  The White House
//
//  Created by Sidney Just on 29/03/14.
//  Copyright (c) 2014 Überpixel. All rights reserved.
//

#include "WHNavigation.h"
#include "DetourNavMeshBuilder.h"
#include "RecastAlloc.h"
#include "Recast.h"
#include "RecastDump.h"

#include "DebugDraw.h"
#include "RecastDebugDraw.h"

namespace WH
{
	/*struct WHDebugDraw
	{
		virtual void depthMask(bool state)
		{}
		
		virtual void texture(bool state)
		{}
		
		/// Begin drawing primitives.
		///  @param prim [in] primitive type to draw, one of rcDebugDrawPrimitives.
		///  @param size [in] size of a primitive, applies to point size and line width only.
		virtual void begin(duDebugDrawPrimitives prim, float size = 1.0f)
		{
			
		}
		
		/// Submit a vertex
		///  @param pos [in] position of the verts.
		///  @param color [in] color of the verts.
		virtual void vertex(const float* pos, unsigned int color)
		{
			RN::Debug::AddLinePoint(RN::Vector3(pos[0], pos[1], pos[2]), RN::Color(color, color, color, 1.0f));
		}
		
		/// Submit a vertex
		///  @param x,y,z [in] position of the verts.
		///  @param color [in] color of the verts.
		virtual void vertex(const float x, const float y, const float z, unsigned int color)
		{
			RN::Debug::AddLinePoint(RN::Vector3(x, y, z), RN::Color(color, color, color, 1.0f));
		}
		
		/// Submit a vertex
		///  @param pos [in] position of the verts.
		///  @param color [in] color of the verts.
		virtual void vertex(const float* pos, unsigned int color, const float* uv)
		{
			RN::Debug::AddLinePoint(RN::Vector3(x, y, z), RN::Color(color, color, color, 1.0f));
		}
		
		/// Submit a vertex
		///  @param x,y,z [in] position of the verts.
		///  @param color [in] color of the verts.
		virtual void vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v)
		{
			RN::Debug::AddLinePoint(RN::Vector3(x, y, z), RN::Color(color, color, color, 1.0f));
		}
		
		/// End drawing primitives.
		virtual void end() = 0;
	};*/
	
	struct FileIO : public duFileIO
	{
		FileIO()
		{
			_file = fopen("/Users/Sidney/Desktop/Mesh.obj", "wb");
		}
		
		virtual ~FileIO()
		{
			fclose(_file);
		}
		
			virtual bool isWriting() const { return true; }
		virtual bool isReading() const { return false; }
		virtual bool write(const void* ptr, const size_t size) { fwrite(ptr, size, 1, _file); return true; }
		virtual bool read(void* ptr, const size_t size) { return false; }
			
		FILE *_file;
	};
	
	RNDefineSingleton(NavigationManager)
	
	NavigationManager::NavigationManager() :
		_navMesh(nullptr),
		_query(nullptr),
		_crowd(nullptr),
		cellSize(0.3f),
		cellHeight(0.2f),
		agentHeight(1.0f),
		agentRadius(0.3f),
		agentMaxClimb(0.4f),
		agentMaxSlope(38.0f),
		regionMinSize(50.0f),
		regionMergeSize(20.0f),
		edgeMaxLength(12.0f),
		edgeMaxError(1.3f),
		detailSampleDist(6.0f),
		detailSampleMaxError(1.0)
	{}
	
	NavigationManager::~NavigationManager()
	{
		if(_navMesh)
			dtFreeNavMesh(_navMesh);
		
		if(_crowd)
			dtFreeCrowd(_crowd);
		
		if(_query)
			dtFreeNavMeshQuery(_query);
	}
	
	
	void NavigationManager::Lock()
	{
		_lock.Lock();
	}
	
	void NavigationManager::Unlock()
	{
		_lock.Unlock();
	}
	
	
	void NavigationManager::SetNavMesh(RN::Mesh *mesh)
	{
		if(_navMesh)
		{
			dtFreeNavMesh(_navMesh);
			_navMesh = nullptr;
		}
		
		if(_crowd)
		{
			dtFreeCrowd(_crowd);
			_crowd = nullptr;
		}
		
		if(_query)
		{
			dtFreeNavMeshQuery(_query);
			_query = nullptr;
		}
		
		if(!mesh)
			return;
		
		int vertexCount = static_cast<int>(mesh->GetVerticesCount());
		RN::Vector3 *vertices = new RN::Vector3[vertexCount];
		float *fVertices = reinterpret_cast<float *>(vertices);
		
		int triangleCount = static_cast<int>(mesh->GetIndicesCount());
		int *triangles = new int[triangleCount];
		
		
		// Copy the vertices over
		{
			RN::Mesh::Chunk chunk = mesh->GetChunk();
			RN::Mesh::ElementIterator<RN::Vector3> iterator = chunk.GetIterator<RN::Vector3>(RN::MeshFeature::Vertices);
			
			for(size_t i = 0; i < vertexCount; i ++)
			{
				vertices[i] = *iterator ++;
			}
		}
		
		
		// Copy the indices
		size_t indicesType = mesh->GetDescriptorForFeature(RN::MeshFeature::Indices)->elementSize;
		switch(indicesType)
		{
			case 2:
			{
				const uint16 *tTriangles = mesh->GetIndicesData<uint16>();
				for(size_t i = 0; i < triangleCount; i ++)
				{
					triangles[i] = static_cast<int>(tTriangles[i]);
				}
				
				break;
			}
				
			case 4:
			{
				const uint32 *tTriangles = mesh->GetIndicesData<uint32>();
				for(size_t i = 0; i < triangleCount; i ++)
				{
					triangles[i] = static_cast<int>(tTriangles[i]);
				}
				
				break;
			}
				
			default:
				throw RN::Exception(RN::Exception::Type::InconsistencyException, "");
				break;
		}
		
		
		triangleCount /= 3;
		
		
		rcContext *context = new rcContext(false);
		
		rcConfig config;
		memset(&config, 0, sizeof(rcConfig));
		
		config.cs = cellSize;
		config.ch = cellHeight;
		
		config.walkableHeight         = static_cast<int>(ceilf(agentHeight / config.ch));
		config.walkableRadius         = static_cast<int>(ceilf(agentRadius / config.cs));
		config.walkableClimb          = static_cast<int>(floorf(agentMaxClimb / config.ch));
		config.walkableSlopeAngle     = agentMaxSlope;
		
		config.minRegionArea          = static_cast<int>(rcSqr(regionMinSize));
		config.mergeRegionArea        = static_cast<int>(rcSqr(regionMergeSize));
		config.maxEdgeLen             = static_cast<int>(edgeMaxLength / config.cs);
		config.maxSimplificationError = edgeMaxError;
		
		config.maxVertsPerPoly        = static_cast<int>(DT_VERTS_PER_POLYGON);
		config.detailSampleDist       = config.cs * detailSampleDist;
		config.detailSampleMaxError   = config.ch * detailSampleMaxError;
		
		
		
		rcCalcBounds(fVertices, vertexCount, config.bmin, config.bmax);
		rcCalcGridSize(config.bmin, config.bmax, config.cs, &config.width, &config.height);
		
		rcHeightfield *heightField = rcAllocHeightfield();
		rcCreateHeightfield(context, *heightField, config.width, config.height, config.bmin, config.bmax, config.cs, config.ch);
		
		
		unsigned char *triangleFlags = new unsigned char[triangleCount];
		memset(triangleFlags, 0, triangleCount * sizeof(unsigned char));
		
		rcMarkWalkableTriangles(context, config.walkableSlopeAngle, fVertices, vertexCount, triangles, triangleCount, triangleFlags);
		rcRasterizeTriangles(context, fVertices, vertexCount, triangles, triangleFlags, triangleCount, *heightField);
		
		delete [] triangleFlags;
		
		
		rcFilterLowHangingWalkableObstacles(context, config.walkableClimb, *heightField);
		rcFilterLedgeSpans(context, config.walkableHeight, config.walkableClimb, *heightField);
		rcFilterWalkableLowHeightSpans(context, config.walkableHeight, *heightField);
		
		rcCompactHeightfield *compactHeightField = rcAllocCompactHeightfield();
		rcBuildCompactHeightfield(context, config.walkableHeight, RC_WALKABLE_AREA, *heightField, *compactHeightField);
		rcFreeHeightField(heightField);
		
		rcErodeWalkableArea(context, config.walkableRadius, *compactHeightField);
		rcBuildDistanceField(context, *compactHeightField);
		rcBuildRegions(context, *compactHeightField, config.borderSize, config.minRegionArea, config.mergeRegionArea);
		
		rcContourSet *contours = rcAllocContourSet();
		rcBuildContours(context, *compactHeightField, config.maxSimplificationError, config.maxEdgeLen, *contours);
		
		rcPolyMesh *polyMesh = rcAllocPolyMesh();
		rcBuildPolyMesh(context, *contours, config.maxVertsPerPoly, *polyMesh);
		
		for(int i = 0; i <polyMesh->npolys; i ++)
		{
			if(polyMesh->areas[i] == RC_WALKABLE_AREA)
			{
				polyMesh->areas[i] = 0;
				polyMesh->flags[i] = 1;
			}
		}
		
		rcPolyMeshDetail *detailPolyMesh = rcAllocPolyMeshDetail();
		rcBuildPolyMeshDetail(context, *polyMesh, *compactHeightField, config.detailSampleDist, config.detailSampleMaxError, *detailPolyMesh);
		
		
		rcFreeCompactHeightfield(compactHeightField);
		rcFreeContourSet(contours);
		
		
		if(config.maxVertsPerPoly <= DT_VERTS_PER_POLYGON)
		{
			dtNavMeshCreateParams params;
			memset(&params, 0, sizeof(dtNavMeshCreateParams));
			
			params.verts     = polyMesh->verts;
			params.vertCount = polyMesh->nverts;
			params.polys     = polyMesh->polys;
			params.polyAreas = polyMesh->areas;
			params.polyFlags = polyMesh->flags;
			params.polyCount = polyMesh->npolys;
			params.nvp       = polyMesh->nvp;
			
			params.detailMeshes     = detailPolyMesh->meshes;
			params.detailVerts      = detailPolyMesh->verts;
			params.detailVertsCount = detailPolyMesh->nverts;
			params.detailTris       = detailPolyMesh->tris;
			params.detailTriCount   = detailPolyMesh->ntris;
			
			params.walkableHeight   = agentHeight;
			params.walkableRadius   = agentRadius;
			params.walkableClimb    = agentMaxClimb;
			
			rcVcopy(params.bmin, polyMesh->bmin);
			rcVcopy(params.bmax, polyMesh->bmax);
			
			params.cs = config.cs;
			params.ch = config.ch;
			
			
			unsigned char *navData;
			int navDataSize;
			
			if(dtCreateNavMeshData(&params, &navData, &navDataSize))
			{
				FileIO io;
				duDumpPolyMeshDetailToObj(*detailPolyMesh, &io);
				
				_navMesh = dtAllocNavMesh();
				_navMesh->init(navData, navDataSize, DT_TILE_FREE_DATA);
				
				_crowd = dtAllocCrowd();
				_crowd->init(kWHCrowdMaxAgents, agentRadius, _navMesh);
				
				_query = dtAllocNavMeshQuery();
				_query->init(_navMesh, 64);
			}
		}
		
		rcFreePolyMesh(polyMesh);
		rcFreePolyMeshDetail(detailPolyMesh);
		
		delete [] triangles;
		delete [] vertices;
		
		delete context;
	}
	
	void NavigationManager::Update(float delta)
	{
		if(!_crowd)
			return;
		
		Lock();
		_crowd->update(delta, nullptr);
		Unlock();
	}
	
	void NavigationManager::Clear()
	{
		RN::LockGuard<NavigationManager *> lock(this);
		
		if(_navMesh)
		{
			dtFreeNavMesh(_navMesh);
			_navMesh = nullptr;
		}
		
		if(_crowd)
		{
			dtFreeCrowd(_crowd);
			_crowd = nullptr;
		}
		
		if(_query)
		{
			dtFreeNavMeshQuery(_query);
			_query = nullptr;
		}
		
		_agents.clear();
	}
	
	void NavigationManager::SetTarget(const RN::Vector3 &target)
	{
		if(!_crowd)
			return;
		
		RN::LockGuard<NavigationManager *> lock(this);
		
		dtPolyRef polyRef;
		_query->findNearestPoly(&target.x, _crowd->getQueryExtents(), _crowd->getFilter(), &polyRef, nullptr);
		
		RN::Vector3 velocity(0.0f);
		
		for(NavigationAgent *agent : _agents)
		{
			if(agent->_index != -1)
			{
				if(agent->_standStill)
				{
					_crowd->requestMoveVelocity(agent->_index, &velocity.x);
					continue;
				}
				
				_crowd->requestMoveTarget(agent->_index, polyRef, &target.x);
			}
		}
	}
	
	void NavigationManager::AddAgent(NavigationAgent *agent)
	{
		if(!_crowd)
			return;
		
		RN::Vector3 position = agent->GetParent()->GetWorldPosition();
		dtCrowdAgentParams params;
		
		params.radius = agentRadius;
		params.height = agentHeight;
		params.maxAcceleration = 10.0f;
		params.maxSpeed        = 6.0f;
		params.collisionQueryRange   = params.radius * 4;
		params.pathOptimizationRange = params.radius * 11;
		params.separationWeight      = 3;
		params.obstacleAvoidanceType = 0;
		params.updateFlags = DT_CROWD_ANTICIPATE_TURNS | DT_CROWD_OBSTACLE_AVOIDANCE | DT_CROWD_SEPARATION | DT_CROWD_OPTIMIZE_VIS;
		
		Lock();
		
		int index = _crowd->addAgent(&position.x, &params);
		agent->_index = index;
		
		_agents.push_back(agent);
		
		Unlock();
	}
	
	void NavigationManager::RemoveAgent(NavigationAgent *agent)
	{
		Lock();
		
		if(agent->_index != -1)
		{
			_crowd->removeAgent(agent->_index);
			agent->_index = -1;
		}
		
		_agents.erase(std::remove(_agents.begin(), _agents.end(), agent), _agents.end());
		
		Unlock();
	}
	
	
	// Navigation agent
	
	NavigationAgent::NavigationAgent() :
		_index(-1),
		_standStill(false)
	{}
	
	NavigationAgent::~NavigationAgent()
	{
		if(_index != -1)
			Disable();
	}
	
	
	void NavigationAgent::Enable()
	{
		if(_index == -1)
			NavigationManager::GetSharedInstance()->AddAgent(this);
	}
	
	void NavigationAgent::Disable()
	{
		if(_index != -1)
			NavigationManager::GetSharedInstance()->RemoveAgent(this);
	}
	
	void NavigationAgent::GetData(RN::Vector3 &position, RN::Vector3 &target)
	{
		if(_index == -1)
			return;
		
		NavigationManager *manager = NavigationManager::GetSharedInstance();
		manager->Lock();
		
		const dtCrowdAgent *agent = manager->_crowd->getAgent(_index);
		
		position.x = agent->npos[0];
		position.y = agent->npos[1];
		position.z = agent->npos[2];
		
		target.x = agent->targetPos[0];
		target.y = agent->targetPos[1];
		target.z = agent->targetPos[2];
		
		manager->Unlock();
	}
	
	void NavigationAgent::Update(float delta)
	{
		RN::Vector3 position = GetPosition();
		RN::Vector3 target;
		
		GetData(position, target);
		SetPosition(position);
	}
}
