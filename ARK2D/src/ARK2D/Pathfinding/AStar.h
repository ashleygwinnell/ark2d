/*
 * AStar.h
 * 30 Dec 2012
 */

#ifndef ARK_PATHFINDING_ASTAR_H_
#define ARK_PATHFINDING_ASTAR_H_

#include "../Includes.h"
#include "../Namespaces.h"
#include "../Util/Containers/Vector.h"

/**
 * A* path finder implementation.
 *
 * @author Kevin Glass - original implementation in Java
 * 		https://bitbucket.org/kevglass/slick/src/9d7443ec33af/trunk/Slick/src/org/newdawn/slick/util/pathfinding/AStarPathFinder.java?at=default
 * @author Ashley Gwinnell - ported to ARK2D C++ framework
 */

namespace ARK { 
	namespace Pathfinding {

		class AStar;

		class ARK2D_API AStarHeuristic {
			public:
				virtual float getCost(AStar* map, unsigned int x, unsigned int y, unsigned int tx, unsigned int ty) = 0;
				virtual ~AStarHeuristic() {}
		};

		// A heuristic that uses the tile that is closest to the target as the next best tile.
		class ARK2D_API ClosestHeuristic : public AStarHeuristic {
			public:
				virtual float getCost(AStar* map, unsigned int x, unsigned int y, unsigned int tx, unsigned int ty) 
				{
					float dx = tx - x;
					float dy = ty - y;
		                
					return float( sqrt((dx*dx)+(dy*dy)) );
				}
		};

		// A heuristic that uses the tile that is closest to the target as the next best tile. 
		// In this case the sqrt is removed and the distance squared is used instead.
		class ARK2D_API ClosestSquaredHeuristic : public AStarHeuristic {
			public:
				virtual float getCost(AStar* map, unsigned int x, unsigned int y, unsigned int tx, unsigned int ty) 
				{
					float dx = tx - x;
					float dy = ty - y;
		                
					return ((dx*dx) + (dy*dy));
				}
		};

		// A heuristic that drives the search based on the Manhattan distance between the current location and the target.
		class ARK2D_API ManhattanHeuristic : public AStarHeuristic {
			public:
				int m_minimumCost;
				ManhattanHeuristic(int minimumCost): 
					m_minimumCost(minimumCost) { 

				}
				virtual float getCost(AStar* map, unsigned int x, unsigned int y, unsigned int tx, unsigned int ty) 
				{
					return m_minimumCost * (abs(float(x)-float(tx)) + abs(float(y)-float(ty)));
				}
		};

		class ARK2D_API DiagonalShortcutHeuristic : public AStarHeuristic {
			public:
				virtual float getCost(AStar* map, unsigned int x, unsigned int y, unsigned int tx, unsigned int ty) 
				{
					float xDistance = abs(float(x)-float(tx));
					float yDistance = abs(float(y)-float(ty));
					if (xDistance > yDistance) { 
						return 14*yDistance + 10*(xDistance-yDistance);
					} 
					return 14*xDistance + 10*(yDistance-xDistance);
				}
		}; 

		class ARK2D_API AStarDataMap {
			public:
				virtual bool isBlocked(AStar* map, unsigned int x, unsigned int y) = 0;
				virtual ~AStarDataMap() {}
		};

		class ARK2D_API DefaultAStarDataMap : public AStarDataMap {
			public:
				virtual bool isBlocked(AStar* map, unsigned int x, unsigned int y);
		}; 

		/* Coefficients for Matrix M */
		#define M11	 0.0
		#define M12	 1.0
		#define M13	 0.0
		#define M14	 0.0
		#define M21	-0.5
		#define M22	 0.0
		#define M23	 0.5 
		#define M24	 0.0
		#define M31	 1.0
		#define M32	-2.5
		#define M33	 2.0
		#define M34	-0.5
		#define M41	-0.5
		#define M42	 1.5
		#define M43	-1.5
		#define M44	 0.5

		static float catmullRomSpline(float t, float v0, float v1, float v2, float v3) 
		{
			double c1,c2,c3,c4;
			c1 =  	      M12*v1;
			c2 = M21*v0          + M23*v2;
			c3 = M31*v0 + M32*v1 + M33*v2 + M34*v3;
			c4 = M41*v0 + M42*v1 + M43*v2 + M44*v3; 

			return(((c4*t + c3)*t +c2)*t + c1);
		}


		class ARK2D_API AStarNode {
			public:
				unsigned int m_x;
				unsigned int m_y;
				unsigned int m_cost;
				unsigned int m_depth;
				AStarNode* m_parent; 
				float m_heuristic;
				bool m_open;
				bool m_closed;
				bool m_blocked; // can this block be moved in to? 

				AStarNode(unsigned int x, unsigned int y):
					m_x(x),
					m_y(y),
					m_cost(0),
					m_depth(0),
					m_parent(NULL),
					m_heuristic(0.0f),
					m_open(false),
					m_closed(false),
					m_blocked(false) { 

				}
				unsigned int setParent(AStarNode* parent) {
					m_depth = parent->m_depth + 1;
					m_parent = parent;

					return m_depth;
				}
				signed int compareTo(AStarNode* other) {
					float f = m_heuristic + m_cost;
					float of = other->m_heuristic + other->m_cost;

					if (f < of) {
						return -1;
					} else if (f > of) {
						return 1;
					}
					return 0;
				}
				void reset() {
					m_closed = false;
					m_open = false;
					m_cost = 0;
					m_depth = 0;
					m_blocked = false;
				}
				inline bool isBlocked() { return m_blocked; }
				inline void setBlocked(bool b) { m_blocked = b; }
				virtual ~AStarNode() {

				}
		};


		class ARK2D_API PriorityAStarNodeList {
			public:
				Vector<AStarNode*> m_data;
		 
			public:
				PriorityAStarNodeList(): m_data() {
					m_data.setUsingList(true);
				}

				AStarNode* first() { 
					return m_data.get(0);
				}

				void clear() {
					m_data.clear();
				}

				void add(AStarNode* node) {
					for (unsigned int i = 0; i < m_data.size();i++) {
						if (((AStarNode*) m_data.get(i))->compareTo(node) > 0) {
							m_data.add(i, node);
							break;
						}
					}
					if (!m_data.contains(node)) {
						m_data.add(node);
					}
				}
				void remove(AStarNode* node) {
					m_data.remove(node);
				}
				unsigned int size() {
					return m_data.size();
				}
				bool contains(AStarNode* node) {
					return m_data.contains(node);
				}
		};

		class ARK2D_API AStar {
			public:
				AStarHeuristic* m_heuristic;
				AStarDataMap* m_dataMap;

				Vector<AStarNode*> m_closedSet;
				PriorityAStarNodeList m_openSet;

				bool m_allowDiagonal;

				Vector<AStarNode*> m_nodes;
				unsigned int m_width;
				unsigned int m_height;
		 
				AStarNode* m_current;
				unsigned int m_sourceX; 
				unsigned int m_sourceY;

				unsigned int m_distance;
				static const unsigned int s_maxSearchDistance = 60;

				Vector<Vector2<int> > m_latestPath;

				float m_x;
				float m_y;
				float m_eachSize;

			public: 

				AStar(unsigned int width, unsigned int height);
				AStar(unsigned int width, unsigned int height, float x, float y, float eachSize);

				void init();
				void setHeuristic(AStarHeuristic* h);
				void setDataMap(AStarDataMap* map);

				void setAllowDiagonal(bool b);

				bool findPath(unsigned int sourceX, unsigned int sourceY, unsigned int targetX, unsigned int targetY);
				Vector<Vector2<int> > getLatestPath();
				//PathGroup* getLatestPathLine();
				//Vector<Vector2<int> >& getLatestPathReference();

				void randomiseAdjacent();
				void removeAdjacent();

				void reset();

				inline float getX() { return m_x; }
				inline float getY() { return m_y; }
				inline unsigned int getWidth() { return m_width; }
				inline unsigned int getHeight() { return m_height; }
				inline float getEachSize() { return m_eachSize; }

				AStarNode* getNode(unsigned int x, unsigned int y);
				AStarNode* getFirstNodeInOpen();
				void addNodeToOpen(AStarNode* node);
		        void addNodeToClosed(AStarNode* node);
		        void removeNodeFromOpen(AStarNode* node);
		        void removeNodeFromClosed(AStarNode* node);

		        bool isValidLocation(unsigned int sourceX, unsigned int sourceY, unsigned int x, unsigned int y);
		        float getMovementCost(unsigned int sourceX, unsigned int sourceY, unsigned int x, unsigned int y);
				float getHeuristicCost(unsigned int x, unsigned int y, unsigned int tx, int unsigned ty);
		 
				void render();
				void render(float x, float y);

				static Vector2<float> catmullrom(Vector<Vector2<int> >& catmullPoints, float t) {
					float totalDistance = 0.0f;
					for(unsigned int i = 0; i < catmullPoints.size()-1; i++) {
						totalDistance += MathUtil::distance(catmullPoints.get(i).getX(), catmullPoints.get(i).getY(), catmullPoints.get(i+1).getX(), catmullPoints.get(i+1).getY());
					}

					unsigned int thisIndex = 1;
					float thisTime = t;   

					// find which two points we're between
					float eachTime = 1.0f / float(catmullPoints.size()-3);
					for(float i = 0; i < catmullPoints.size()-3; i += 1.0f) 
					{
						if (thisTime >= i*eachTime && thisTime <= (i+1)*eachTime) {
							//thisTime = (i*eachTime) / ((i+1)*eachTime);

							thisTime = (thisTime - (i*eachTime)) / eachTime;
							//float pc = (thisTime - runningDistance) / thisDistance;
							//thisTime = runningDistance + (thisDistance * pc);

							//thisTime = (thisTime - runningDistance) / thisDistance;

							//thisTime = (thisTime - runningDistance) / thisDistance;
							//thisTime = (thisTime - runningDistance) * (runningDistance / (runningDistance + thisDistance));
							//thisTime -= runningDistance;
							thisIndex = int(i) + 1;
							break;
						} 
						//runningDistance += thisDistance;
						//runningDistanceWhole = thisDistanceWhole;
					}

					float x = catmullRomSpline(
						thisTime, 
						catmullPoints.get(thisIndex-1).getX(),
						catmullPoints.get(thisIndex).getX(), 
						catmullPoints.get(thisIndex+1).getX(),
						catmullPoints.get(thisIndex+2).getX()
					);  
					float y = catmullRomSpline(
						thisTime, 
						catmullPoints.get(thisIndex-1).getY(),
						catmullPoints.get(thisIndex).getY(), 
						catmullPoints.get(thisIndex+1).getY(),
						catmullPoints.get(thisIndex+2).getY()
					);  

					return Vector2<float>(x, y);

				}
				
				virtual ~AStar();
		};

		class ARK2D_API AStarUtil {
			public:
				static void randomiseAdjacent(AStar* a, Vector<Vector2<int> >& items);
				static void removeAdjacent(AStar* a, Vector<Vector2<int> >& items);
		};

	} 
}

#endif /* ARK_PATHFINDING_ASTAR_H_ */
