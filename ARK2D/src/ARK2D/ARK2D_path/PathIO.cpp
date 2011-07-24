/*
 * PathIO.cpp
 *
 *  Created on: 10 Jul 2011
 *      Author: Ashley
 */

#include "PathIO.h"

PathGroup* PathIO::createFromFile(string file) {
	char* c = StringUtil::file_get_contents(file.c_str());
	if (c == NULL) {
		ErrorDialog::createAndShow(StringUtil::append("Could not load Path file: ", file));
		return NULL;
	}
	return createFromJSON(string(c));
}
PathGroup* PathIO::createFromJSON(string json) {

	try {
		JSONNode* root = libJSON::Parse(json);

		PathGroup* pathGroup = new PathGroup();
		pathGroup->setName(root->GetNode("name")->NodeAsString());
		pathGroup->setDescription(root->GetNode("description")->NodeAsString());

		if (root->GetNode("relative") == NULL) {
			pathGroup->setRelative(false);
		} else {
			pathGroup->setRelative(root->GetNode("relative")->NodeAsBool());
		}

		JSONNode* paths = root->GetNode("paths");
		for(unsigned int i = 0; i < paths->NodeSize(); i++)
		{

			JSONNode* path = paths->Children[i];

			Path* pathObj = new Path();
			pathObj->setDuration(path->GetNode("duration")->NodeAsFloat());
			pathObj->setEasing(Easing::getByString(path->GetNode("easing")->NodeAsString()));

			JSONNode* points = path->GetNode("points");
			for(unsigned int j = 0; j < points->NodeSize(); j++)
			{
				JSONNode* point = points->Children[j];
				pathObj->addPoint(point->GetNode("x")->NodeAsInt(), point->GetNode("y")->NodeAsInt());
			}
			pathGroup->addPath(pathObj);
		}
		return pathGroup;

	} catch(...) {
		ErrorDialog::createAndShow("exception in PathIO::createFromJson");
		return NULL;
	}
}
string PathIO::getAsJSON(PathGroup* g) {

	string nl = "\n";
	string s = "";

	s += "{" + nl;
	s += "	\"name\": \"" + g->getName() + "\"," + nl;
	s += "	\"description\": \"" + g->getDescription() + "\"," + nl;
	s += "	\"relative\": " + string((g->isRelative()==true)?"true":"false") + "," + nl;
	s += "	\"paths\": [ "+ nl;
	for(unsigned int i = 0; i < g->getPaths().size(); i++) {
		s += "		{"+ nl;
		s += "			\"duration\": ";
		s += Cast::toString<float>(g->getPath(i)->getDuration());
		s += "," + nl;
		s += "			\"easing\": \"" + Easing::getByInt(g->getPath(i)->getEasing()) + "\","+ nl;
		s += "			\"points\": ["+ nl;
		for(unsigned int j = 0; j < g->getPath(i)->getPoints().size(); j++) {
			s += "				{"+ nl;
			s += "					\"x\":";
			s += Cast::toString<int>((int)g->getPath(i)->getPoint(j)->getX());
			s += ","+ nl;
			s += "					\"y\":";
			s += Cast::toString<int>((int)g->getPath(i)->getPoint(j)->getY());
			s += nl;
			if (j != g->getPath(i)->getPoints().size() - 1) {
				s += "				},"+ nl;
			} else {
				s += "				}"+ nl;
			}
		}
		s += "			]"+ nl;
		if (i != g->getPaths().size() - 1) {
			s += "		},"+ nl;
		} else {
			s += "		}" + nl;
		}
	}
	s += "	]"+ nl;
	s += "}"+ nl;
	return s;
}
