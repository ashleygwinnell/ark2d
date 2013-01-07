/*
 * AStar.cpp
 *
 *  Created on: 30 Dec 2012
 *      Author: Ashley
 */
 
#include "AStar.h"   

namespace ARK {
	namespace Pathfinding {


		bool DefaultAStarDataMap::isBlocked(AStar* map, unsigned int x, unsigned int y) {
			return map->getNode(x, y)->isBlocked();
		}

		AStar::AStar(unsigned int width, unsigned int height):
			m_heuristic( NULL ),  
			m_dataMap( NULL ),
			m_closedSet(), 
			m_openSet(),  
			m_allowDiagonal(true),
			m_nodes(),
			m_width(width),   
			m_height(height),      
			m_current(NULL),
			m_sourceX(0),       
			m_sourceY(0), 
			m_distance(0),
			m_x(0.0f),
			m_y(0.0f),
			m_eachSize(1)
		{
			init();  
		} 

		AStar::AStar(unsigned int width, unsigned int height, float x, float y, float eachSize):
			m_heuristic( NULL ), 
			m_dataMap( NULL ), 
			m_closedSet(),
			m_openSet(),
			m_allowDiagonal(true),  
			m_nodes(),
			m_width(width),  
			m_height(height),     
			m_current(NULL),
			m_sourceX(0),       
			m_sourceY(0),
			m_distance(0),
			m_x(x),
			m_y(y),
			m_eachSize(eachSize)
		{
			init();
		}

		void AStar::init()  
		{
			m_heuristic = new ManhattanHeuristic(10);
			m_dataMap = new DefaultAStarDataMap();

			// use same format as TiledMapTileset
			for (unsigned int y = 0; y < m_height; y++)
			{
				for (unsigned int x = 0; x < m_width; x++)
				{
					m_nodes.add( new AStarNode(x, y) );
				}
			}
		}
		void AStar::setHeuristic(AStarHeuristic* h)  {
			if (m_heuristic != NULL) {
				delete m_heuristic;
				m_heuristic = NULL;
			}
			m_heuristic = h;
		}
		void AStar::setDataMap(AStarDataMap* map) {
			if (m_dataMap != NULL) {
				delete m_dataMap;
				m_dataMap = NULL;
			}
			m_dataMap = map;
		}

		void AStar::reset() {
			// reset the A* pathfinding state.
			m_current = NULL;
			m_sourceX = 0;
			m_sourceY = 0;
			m_distance = 0;

			m_latestPath.clear();
			for(unsigned int i = 0; i < m_nodes.size(); ++i) {
				m_nodes.get(i)->reset();
			}
		} 

		void AStar::setAllowDiagonal(bool b) {
			m_allowDiagonal = b;
		}

		bool AStar::findPath(unsigned int sourceX, unsigned int sourceY, unsigned int targetX, unsigned int targetY) 
		{
			// reset the A* pathfinding state.
			m_current = NULL;
			m_sourceX = sourceX;
			m_sourceY = sourceY;
			m_distance = 0; 

			m_latestPath.clear();

			if (getNode(targetX, targetY)->isBlocked()) {
				ARK2D::getLog()->w("Target is blocked");
				return false; // m_latestPath;
			}

			// reset nodes
			for(unsigned int i = 0; i < m_nodes.size(); ++i)
			{
				bool thisBlocked = m_nodes.get(i)->isBlocked();
				m_nodes.get(i)->reset();
				m_nodes.get(i)->setBlocked(thisBlocked);
			}

			// set initial state 
			getNode(sourceX, sourceY)->m_cost = 0;
			getNode(sourceX, sourceY)->m_depth = 0;
			m_closedSet.clear();
			m_openSet.clear();
			addNodeToOpen(getNode(sourceX, sourceY));

			getNode(targetX, targetY)->m_parent = NULL;

			unsigned int maxDepth = 0;
			while ((maxDepth < s_maxSearchDistance) && (m_openSet.size() != 0)) 
			{
				unsigned int lx = sourceX;
				unsigned int ly = sourceY;
				if (m_current != NULL) {
					lx = m_current->m_x;
					ly = m_current->m_y;
				}

				m_current = getFirstNodeInOpen();
				m_distance = m_current->m_depth;

				if (m_current == getNode(targetX, targetY)) {
					if (isValidLocation(lx, ly, targetX, targetY)) {
						break;
					}
				}

				removeNodeFromOpen(m_current);
				addNodeToClosed(m_current);

				for (signed int x = -1; x < 2; x++) {
					for (signed int y = -1; y < 2; y++) {

						// current tile. 
						if ((x == 0) && (y == 0)) {
							continue;
						} 

						// if we're not allowing diaganol movement then only one of x or y can be set
						if (!m_allowDiagonal) {
							if ((x != 0) && (y != 0)) {
								continue;
							}
						} 

						// determine the location of the neighbour and evaluate it
						int xp = x + m_current->m_x;
						int yp = y + m_current->m_y;

						if (isValidLocation(m_current->m_x, m_current->m_y, xp, yp)) {

							// the cost to get to this node is cost the current plus the movement
							// cost to reach this node. Note that the heursitic value is only used
							// in the sorted open list
							float nextStepCost = m_current->m_cost + getMovementCost(m_current->m_x, m_current->m_y, xp, yp);
							AStarNode* neighbour = getNode(xp, yp);
							//map.pathFinderVisited(xp, yp);

							// if the new cost we've determined for this node is lower than 
							// it has been previously makes sure the node hasn't been discarded. We've
							// determined that there might have been a better path to get to
							// this node so it needs to be re-evaluated
							if (nextStepCost < neighbour->m_cost) {
								if (neighbour->m_open) {
							    	removeNodeFromOpen(neighbour);
								}
							    if (neighbour->m_closed) {
							    	removeNodeFromClosed(neighbour);
								}
							}

							// if the node hasn't already been processed and discarded then
							// reset it's cost to our current cost and add it as a next possible
							// step (i.e. to the open list)
							if (!neighbour->m_open && !neighbour->m_closed) {
								neighbour->m_cost = nextStepCost;
								neighbour->m_heuristic = getHeuristicCost(xp, yp, targetX, targetY);
								maxDepth = max(maxDepth, neighbour->setParent(m_current));
								addNodeToOpen(neighbour);
							} 

						}
					}
				}
			}
		 
			// since we've got an empty open list or we've run out of search 
			// there was no path. Just return null
			if (getNode(targetX, targetY)->m_parent == NULL) {
				ARK2D::getLog()->w("no valid path to this location.");
				m_latestPath.clear(); 
				return false; //m_latestPath;
				//return null; 
			} 

			// At this point we've definitely found a path so we can uses the parent
			// references of the nodes to find out way from the target location back
			// to the start recording the nodes on the way.
			AStarNode* target = getNode(targetX, targetY);
			while (target != getNode(sourceX, sourceY)) {
				m_latestPath.add(Vector2<int>(target->m_x, target->m_y) ); 
				target = target->m_parent;
			} 
			m_latestPath.add( Vector2<int>(sourceX,sourceY) ); 

			m_latestPath.reverse();

			// thats it, we have our path 
			return true; //m_latestPath;
		}
		Vector<Vector2<int> > AStar::getLatestPath() {
			return m_latestPath;  
		}

		/*PathGroup* AStar::getLatestPathLine() { 
			//GameContainer* container = ARK2D::getContainer();
			//float oneWidth = float(container->getWidth()) / float(m_width);
			//float oneHeight = float(container->getHeight()) / float(m_height);
			float oneSize = m_eachSize; //(oneWidth < oneHeight) ? oneWidth : oneHeight;
			
			PathGroup* group = new PathGroup();
			if (m_latestPath.size() == 2) 
			{
				ARK::Path::Path* path = new ARK::Path::Path();
				path->setDuration(1.0f);
				path->setEasing(Easing::LINEAR);
				path->addPoint(oneSize * m_latestPath.get(0).getX(), oneSize * m_latestPath.get(0).getY());
				path->addPoint(oneSize * m_latestPath.get(1).getX(), oneSize * m_latestPath.get(1).getY());
				group->addPath(path);
			} 
			else if (m_latestPath.size() == 3) 
			{
				float duration = 1.0f;
				float points = 3;
				float eachDuration = duration / points;

				const Vector2<int>& point = m_latestPath.get(1);
				float normal = ( 
					(MathUtil::anglef(point.getX(), point.getY(), m_latestPath.get(0).getX(), m_latestPath.get(0).getY())) 
					+ (MathUtil::anglef(point.getX(), point.getY(), m_latestPath.get(2).getX(), m_latestPath.get(2).getY()))
					/ 2.0f 
				);

				float handleOneDistance = MathUtil::distance(point.getX(), point.getY(), m_latestPath.get(0).getX(), m_latestPath.get(0).getY()) * 0.5f;
				float handleOneX = point.getX();
				float handleOneY = point.getY();
				MathUtil::moveAngle(handleOneX, handleOneY, normal + 90, handleOneDistance);

				float handleTwoDistance = MathUtil::distance(point.getX(), point.getY(), m_latestPath.get(2).getX(), m_latestPath.get(2).getY()) * 0.5f;
				float handleTwoX = point.getX();
				float handleTwoY = point.getY();
				MathUtil::moveAngle(handleTwoX, handleTwoY, normal - 90, handleTwoDistance);

				ARK::Path::Path* path = new ARK::Path::Path();
				path->setDuration(eachDuration);
				path->setEasing(Easing::LINEAR);
				path->addPoint(oneSize * m_latestPath.get(0).getX(), oneSize * m_latestPath.get(0).getY());
				path->addPoint(oneSize * handleOneX, oneSize * handleOneY);
				path->addPoint(oneSize * m_latestPath.get(1).getX(), oneSize * m_latestPath.get(1).getY());
				group->addPath(path);

				path = new ARK::Path::Path();
				path->setDuration(eachDuration);
				path->setEasing(Easing::LINEAR);
				path->addPoint(oneSize * m_latestPath.get(1).getX(), oneSize * m_latestPath.get(1).getY());
				path->addPoint(oneSize * handleTwoX, oneSize * handleTwoY);
				path->addPoint(oneSize * m_latestPath.get(2).getX(), oneSize * m_latestPath.get(2).getY());
				group->addPath(path);
			} 
			else
			{
				float duration = 1.0f;
				float points = m_latestPath.size();
				float eachDuration = duration / points;

				const Vector2<int>& point = m_latestPath.get(1);
				float normal = ( 
					(MathUtil::anglef(point.getX(), point.getY(), m_latestPath.get(0).getX(), m_latestPath.get(0).getY())) 
					+ (MathUtil::anglef(point.getX(), point.getY(), m_latestPath.get(2).getX(), m_latestPath.get(2).getY()))
					/ 2.0f 
				);

				float handleOneDistance = MathUtil::distance(point.getX(), point.getY(), m_latestPath.get(0).getX(), m_latestPath.get(0).getY()) * 0.5f;
				float handleOneX = point.getX();
				float handleOneY = point.getY();
				MathUtil::moveAngle(handleOneX, handleOneY, normal + 90, handleOneDistance);

				ARK::Path::Path* path = new ARK::Path::Path();
				path->setDuration(eachDuration);
				path->setEasing(Easing::LINEAR);
				path->addPoint(oneSize * m_latestPath.get(0).getX(), oneSize * m_latestPath.get(0).getY());
				path->addPoint(oneSize * handleOneX, oneSize * handleOneY);
				path->addPoint(oneSize * m_latestPath.get(1).getX(), oneSize * m_latestPath.get(1).getY());
				group->addPath(path);

				// do all those inbetween bits.
				for (unsigned int i = 0; i < 1; i++)
				{
				
				}

				const Vector2<int>& lastpoint = m_latestPath.get(points-2);
				normal = ( 
					(MathUtil::anglef(lastpoint.getX(), lastpoint.getY(), m_latestPath.get(points-1).getX(), m_latestPath.get(points-1).getY())) 
					+ (MathUtil::anglef(lastpoint.getX(), lastpoint.getY(), m_latestPath.get(points-3).getX(), m_latestPath.get(points-3).getY()))
					/ 2.0f 
				);

				float handleTwoDistance = MathUtil::distance(lastpoint.getX(), lastpoint.getY(), m_latestPath.get(points-1).getX(), m_latestPath.get(points-1).getY()) * 0.5f;
				float handleTwoX = lastpoint.getX();
				float handleTwoY = lastpoint.getY();
				MathUtil::moveAngle(handleTwoX, handleTwoY, normal - 90, handleTwoDistance);

				path = new ARK::Path::Path();
				path->setDuration(eachDuration);
				path->setEasing(Easing::LINEAR);
				path->addPoint(oneSize * m_latestPath.get(points-2).getX(), oneSize * m_latestPath.get(points-2).getY());
				path->addPoint(oneSize * handleTwoX, oneSize * handleTwoY);
				path->addPoint(oneSize * m_latestPath.get(points-1).getX(), oneSize * m_latestPath.get(points-1).getY());
				group->addPath(path);
			}

			return group;
		}*/

		void AStar::randomiseAdjacent() {
			AStarUtil::randomiseAdjacent(this, m_latestPath);
		}
		void AStar::removeAdjacent() {
			AStarUtil::removeAdjacent(this, m_latestPath);
		}


		AStarNode* AStar::getNode(unsigned int x, unsigned int y) {
			// we don't have a 2d array, so figure this shit out.
			unsigned int id = (y * m_width) + x;
			return m_nodes.get(id);
		}

		AStarNode* AStar::getFirstNodeInOpen() {
			return m_openSet.first();
		}

		void AStar::addNodeToOpen(AStarNode* node) {
		    node->m_open = true;
		    m_openSet.add(node);
		}
		void AStar::addNodeToClosed(AStarNode* node) {
			node->m_closed = true;
			m_closedSet.add(node);
		}

		void AStar::removeNodeFromOpen(AStarNode* node) {
			node->m_open = false;
			m_openSet.remove(node);
		}
		void AStar::removeNodeFromClosed(AStarNode* node) {
			node->m_closed = false;
			m_closedSet.remove(node);
		}


		bool AStar::isValidLocation(unsigned int sourceX, unsigned int sourceY, unsigned int x, unsigned int y) {
			// TODO: rewrite so it's not backwards. 

			bool invalid = (x < 0) || (y < 0) || (x >= m_width) || (y >= m_height);

			if ((!invalid) && ((sourceX != x) || (sourceY != y))) {
				//this.mover = mover;
				m_sourceX = sourceX;
				m_sourceY = sourceY;
				invalid = getNode(x, y)->isBlocked(); // map.blocked(this, x, y);
			}
			return !invalid;
		}
		float AStar::getMovementCost(unsigned int sourceX, unsigned int sourceY, unsigned int x, unsigned int y) {
			m_sourceX = sourceX;
			m_sourceY = sourceY;

			// introduce concept of a map and movement costs for example 2d hills / terrain.

			//return map.getCost(this, tx, ty);
			return 1.0f;
		}
		float AStar::getHeuristicCost(unsigned int x, unsigned int y, unsigned int tx, int unsigned ty) {
			// A heuristic that uses the tile that is closest to the target as the next best tile.
			return m_heuristic->getCost(this, x, y, tx, ty);
		}


		void AStar::render() {
			render(m_x, m_y);
		}
		void AStar::render(float drawX, float drawY) {
			//GameContainer* container = ARK2D::getContainer();
			Renderer* r = ARK2D::getRenderer();
			float alpha = 0.5f;

			//float oneWidth = float(container->getWidth()) / float(m_width);
			//float oneHeight = float(container->getHeight()) / float(m_height);
			float oneSize = m_eachSize; //(oneWidth < oneHeight) ? oneWidth : oneHeight;

			for(unsigned int x = 0; x < m_width; ++x) {
				for(unsigned int y = 0; y < m_height; ++y) {
					AStarNode* node = getNode(x, y);

					float renderX = drawX + (x * oneSize); 
					float renderY = drawY + (y * oneSize);

					if (node->isBlocked()) {
						r->setDrawColorf(1.0f, 0.0f, 0.0f, alpha);
					} else {
						r->setDrawColorf(0.1f, 0.1f, 0.1f, alpha);
					}

					r->fillRect(renderX, renderY, oneSize, oneSize);
				}
			}

			r->setDrawColorf(0.0f, 1.0f, 0.0f, alpha);
			for(unsigned int i = 0; i < m_latestPath.size(); ++i) {
				r->fillRect(
					drawX + (m_latestPath.get(i).getX() * oneSize), 
					drawY + (m_latestPath.get(i).getY() * oneSize), 
					oneSize, 
					oneSize
				); 
			}


			/*r->setDrawColorf(1.0f, 0.0f, 1.0f, alpha);
			r->fillRect(
				m_latestPath.first().getX() * oneSize, 
				m_latestPath.first().getY() * oneSize, 
				oneSize, 
				oneSize
			); 
			r->setDrawColorf(0.0f, 1.0f, 1.0f, alpha);
			r->fillRect(
				m_latestPath.last().getX() * oneSize,   
				m_latestPath.last().getY() * oneSize, 
				oneSize, 
				oneSize
			); */

			return;

			// Catmull-rom render of latestPath.
			if (m_latestPath.size() > 0)
			{
				Vector<Vector2<int> > catmullPoints = m_latestPath;
				catmullPoints.add(0, Vector2<int>(catmullPoints.first().getX(), catmullPoints.first().getY()) );
				catmullPoints.add(catmullPoints.size(), Vector2<int>(catmullPoints.last().getX(), catmullPoints.last().getY()) );
				catmullPoints.add(catmullPoints.size(), Vector2<int>(catmullPoints.last().getX(), catmullPoints.last().getY()) );

				/*Vector<Vector2<int> > catmullPoints;
				catmullPoints.add( Vector2<int>(0, 0) );
				catmullPoints.add( Vector2<int>(0, 0) );
				catmullPoints.add( Vector2<int>(5, 5) );
				catmullPoints.add( Vector2<int>(10, 18) );
				catmullPoints.add( Vector2<int>(15, 23) );
				catmullPoints.add( Vector2<int>(15, 23) );*/

				/*r->setDrawColorf(1.0f, 0.0f, 0.5f, 1.0f);
				for (unsigned int i = 0; i < catmullPoints.size(); ++i) {
					r->fillCircle(
						(catmullPoints.get(i).getX() * oneSize) + (oneSize/2.0f), 
						(catmullPoints.get(i).getY() * oneSize) + (oneSize/2.0f), 
						10, 
						20
					);
				}*/
		  

		 
				 
		 	/*
				for (float t = 0.0f; t <= 1.0f; t += 0.001f) 
				{ 
					Vector2<float> point = AStar::catmullrom(catmullPoints, t); 
					r->setDrawColorf(1.0f, 0.0f, 0.5f, 0.5f);
					r->fillCircle(drawX + (point.getX() * oneSize + (oneSize/2.0f)), drawY + (point.getY() * oneSize + (oneSize/2.0f)), 2, 6);
				}
				Vector2<float> point = AStar::catmullrom(catmullPoints, m_timerTemp); 
				r->setDrawColorf(1.0f, 0.0f, 0.5f, 1.0f);
				r->fillCircle(drawX + (point.getX() * oneSize + (oneSize/2.0f)), drawY + (point.getY() * oneSize + (oneSize/2.0f)), 6, 24);
				*/
			}
		}

		AStar::~AStar() { 
			if (m_heuristic != NULL) {
				delete m_heuristic;
				m_heuristic = NULL;
			}
			for(unsigned int i = 0; i < m_nodes.size(); ++i) {
				AStarNode* n = m_nodes.get(i);
				delete n;
			}
			m_nodes.clear();
			m_openSet.clear();
			m_closedSet.clear(); 
			m_latestPath.clear();
		}
		 

		 


		void AStarUtil::randomiseAdjacent(AStar* a, Vector<Vector2<int> >& items) 
		{
			if (items.size() < 2) { return; }
			
			//for(unsigned int i = 0; i < items.size(); ++i) 
			for(unsigned int i = items.size() - 1; i > 0; i--) 
			{
				if (i == 0 || i == items.size()-1) { continue; }
		 
				ARK2D::getLog()->i(StringUtil::append("randomiseAdjacent: ", i));

				const Vector2<int>& prev = items.get(i - 1);
				const Vector2<int>& current = items.get(i);
				const Vector2<int>& next = items.get(i + 1);

				if (next.getX() - current.getX() == current.getX() - prev.getX() && 
					next.getY() - current.getY() == current.getY() - prev.getY()
					) {
					 
					// horizontal (three in a row)
					if (next.getX() - current.getX() == current.getX() - prev.getX() &&
						next.getY() - current.getY() == 0 && 
						current.getY() - prev.getY() == 0) {

						AStarNode* above = a->getNode(current.getX(), current.getY() - 1);
						AStarNode* below = a->getNode(current.getX(), current.getY() + 1);

						float r = MathUtil::randBetweenf(0.0f, 1.0f);
						if (r > 0.33f) { 
							// check Y - 1 before Y + 1
							if (above != NULL && !above->isBlocked()) {
								items.getr(i).setY(current.getY() - 1);
							} else if (below != NULL && !below->isBlocked()) {
								items.getr(i).setY(current.getY() + 1);
							}

						} else if (r > 0.66f) {
							// check Y + 1 before Y - 1
							if (below != NULL && !below->isBlocked()) {
								items.getr(i).setY(current.getY() + 1);
							} else if (above != NULL && !above->isBlocked()) {
								items.getr(i).setY(current.getY() - 1);
							}
						}
					}

					// vertical (three in a column)
					if (next.getY() - current.getY() == current.getY() - prev.getY() &&
						next.getX() - current.getX() == 0 && 
						current.getX() - prev.getX() == 0) {

						AStarNode* leftSide = a->getNode(current.getX() - 1, current.getY());
						AStarNode* rightSide = a->getNode(current.getX() + 1, current.getY());

						float r = MathUtil::randBetweenf(0.0f, 1.0f);
						if (r > 0.33f) { 
							// check X - 1 before X + 1
							if (leftSide != NULL && !leftSide->isBlocked()) {
								items.getr(i).setX(current.getX() - 1);
							} else if (rightSide != NULL && !rightSide->isBlocked()) {
								items.getr(i).setX(current.getX() + 1);
							}

						} else if (r > 0.66f) {
							// check X - 1 before X + 1
							if (rightSide != NULL && !rightSide->isBlocked()) {
								items.getr(i).setX(current.getX() + 1);
							} else if (leftSide != NULL && !leftSide->isBlocked()) {
								items.getr(i).setX(current.getX() - 1);
							}
						}
					}

					// *************
					// TODO!
					// *************
					/*// diagonally, boom. 
					if (next.getY() - current.getY() == current.getY() - prev.getY() &&
						next.getX() - current.getX() == 0 && 
						current.getX() - prev.getX() == 0) {

						float r = MathUtil::randBetweenf(0.0f, 1.0f);
						if (r > 0.33f) { 
							// check X - 1 before X + 1
							if (!a->getNode(current.getX() - 1, current.getY())->isBlocked()) {
								items.getr(i).setX(current.getX() - 1);
							} else if (!a->getNode(current.getX() + 1, current.getY())->isBlocked()) {
								items.getr(i).setX(current.getX() + 1);
							}

						} else if (r > 0.66f) {
							// check X - 1 before X + 1
							if (!a->getNode(current.getX() + 1, current.getY())->isBlocked()) {
								items.getr(i).setX(current.getX() + 1);
							} else if (!a->getNode(current.getX() - 1, current.getY())->isBlocked()) {
								items.getr(i).setX(current.getX() - 1);
							}
						}
					}*/
 
				}  
			}
		}

		void AStarUtil::removeAdjacent(AStar* a, Vector<Vector2<int> >& items) 
		{
			//for(unsigned int i = 0; i < items.size(); ++i) 
			for(signed int i = (signed int) (items.size() - 1); i > 0; --i) 
			{
				if (i == 0 || i == (signed int) (items.size()-1)) { continue; }

				const Vector2<int>& prev = items.get(i - 1);
				const Vector2<int>& current = items.get(i);
				const Vector2<int>& next = items.get(i + 1);

				if (next.getX() - current.getX() == current.getX() - prev.getX() && 
					next.getY() - current.getY() == current.getY() - prev.getY()
					) {
					items.remove(i);
					i = (signed int) (items.size() - 1);
				}
			}
		}

	}
}


