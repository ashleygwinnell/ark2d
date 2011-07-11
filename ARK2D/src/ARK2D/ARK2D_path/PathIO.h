/*
 * PathIO.h
 *
 *  Created on: 10 Jul 2011
 *      Author: Ashley
 */

#ifndef PATHIO_H_
#define PATHIO_H_

#include "PathGroup.h"
#include "../ErrorDialog.h"
#include "../StringUtil.h"
#include "../Easing.h"
#include "../ARK2D_JSON.h"

#include <string>
using namespace std;

class PathIO {
	public:
		static PathGroup* createFromFile(string file);
		static PathGroup* createFromJSON(string json);
		static string getAsJSON(PathGroup* g);
};

#endif /* PATHIO_H_ */
