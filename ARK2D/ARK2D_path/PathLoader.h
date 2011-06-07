/*
 * ARK2DPathLoader.h
 *
 *  Created on: 12 Sep 2010
 *      Author: Ashley
 */

#ifndef PATHLOADER_H_
#define PATHLOADER_H_

#include <vector>
#include <string>
#include "../Cast.h"
using std::vector;
using std::string;


class PathPoint {
	public:
		PathPoint(): x(0), y(0), x_easing(0), y_easing(0), millis(0) { }
		float x;
		float y;
		unsigned int x_easing;
		unsigned int y_easing;
		unsigned int millis;
		string toString() {
			string s;
			s += "{";
				//if (x != 0) {
					s += "x:";
					s += Cast::toString<float>(x);
					s += ",";
				//}
				//if (y != 0) {
					s += "y:";
						s += Cast::toString<float>(y);
					s += ",";
				//}
				//if (x_easing != 0) {
					s += "x_easing:";
						s += Cast::toString<unsigned int>(x_easing);
					s += ",";
				//}
				//if (y_easing != 0) {
					s += "y_easing:";
						s += Cast::toString<unsigned int>(y_easing);
					s += ",";
				//}
				//if (millis != 0) {
					s += "millis:";
						s += Cast::toString<unsigned int>(millis);
				//}
			s += "}";
			return s;
		}
};

class Path {
	private:
		string m_id;
		string m_name;
		string m_type;
		vector<PathPoint> m_points;

	public:
		Path() {

		}
		void setId(string id) {
			m_id = id;
		}
		void setName(string name) {
			m_name = name;
		}
		void setType(string type) {
			m_type = type;
		}
		void addPoint(PathPoint point) {
			m_points.push_back(point);
		}
		vector<PathPoint> getPoints() {
			return m_points;
		}
		string getId() {
			return m_id;
		}
		string getType() {
			return m_type;
		}
		string getName() {
			return m_name;
		}
		string toString() {
			string s;
			s += "{";
				s += "id:" + m_id;
				s += ",";
				s += "name:" + m_name;
				s += ",";
				s += "type:" + m_type;
				s += ",";
				s += "points:[";
					for (unsigned int i = 0; i < m_points.size(); i++) {
						PathPoint p = m_points.at(i);
						s += p.toString();
						if (i != m_points.size() - 1) {
							s += ",";
						}
					}
				s += "]";
			s += "}";
			return s;
		}
};

class PathLoader {
	public:
		const char* m_file;
		const char* m_file_contents;
		PathLoader(const char* file);
		Path* load();
		virtual ~PathLoader();
};

#endif /* PATHLOADER_H_ */
