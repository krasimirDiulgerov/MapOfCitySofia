#include<iostream>
#include<sstream>
#include<queue>
#include<cmath>
#include<fstream>
#include<string>

#include"Intersection.h"
#include"City.h"




Location::Location(std::string name, std::vector<Intersection*>* intersections) {
	closed = std::vector<std::string>();
	allIntersections = intersections;
	current = findIntersection(name);
}


Location::Location(Intersection* inter, std::vector<Intersection*>* intersections) {

	current = inter;
	closed = std::vector<std::string>();
	allIntersections = intersections;


}

bool Location::hasIntersection(std::vector<Intersection>intersections, std::string name) {
	using Iterator = std::vector<Intersection>::iterator;
	for (int i = 0; i < intersections.size(); i++) {
		if (intersections[i].getName() == name)return true;
	}
	return false;
}

std::pair<std::vector<Intersection>, int>Location::findShortestPath(std::vector<std::pair<std::vector<Intersection>, int>>paths) {
	int minValue = 999999;
	std::pair<std::vector<Intersection>, int>min;
	for (std::pair<std::vector<Intersection>, int>path : paths) {
		if (minValue > path.second) {
			min = path;
			minValue = path.second;
		}
	}
	return min;
}
//izvinqvai za sledvashtoto
std::vector<std::pair<std::vector<Intersection>, int>> Location::allPathsBetween(std::vector<Intersection>currPath, int currLength, Intersection inter1, Intersection inter2, std::vector<std::pair<std::vector<Intersection>, int>>& result, bool considerClosed) {
	if (hasIntersection(currPath, inter1.getName()))return result;
	if (considerClosed && inter1.isClosed())return result;
	currPath.push_back(inter1);
	for (std::pair<Intersection*, int>neigh : inter1.getNeighbours()) {
		Intersection next = *(neigh.first);
		allPathsBetween(currPath, currLength + neigh.second, next, inter2, result, considerClosed);
	}
	if (inter1.hasNeighbour(inter2.getName())) {
		currPath.push_back(inter2);
		result.push_back(std::make_pair(currPath, currLength + inter1.getNeighbour(inter2.getName())->second));
		return result;
	}
	return result;
}

std::pair<std::vector<Intersection>, int> Location::getShortestPath(std::string target, std::ostream& os, bool consideredClosed ) {
	using Paths = std::vector<std::pair<std::vector<Intersection>, int>>;

	Intersection inter2 = *(findIntersection(target));
	Paths temp;
	Paths paths = allPathsBetween(std::vector<Intersection>(), 0, *current, inter2, temp, consideredClosed);
	if (paths.size() == 0) {
		os << "There is no path between these intersections,it might be because of closed intersection." << std::endl;
		return std::make_pair(std::vector<Intersection>(), 0);
	}
	return findShortestPath(paths);
}

Location Location::move(std::string name, std::ostream& os) {
	std::pair<std::vector<Intersection>, int>shortestPath = getShortestPath(name, os,true);
	if (shortestPath.first.size() > 0) {
		for (Intersection inter : shortestPath.first) {
			os << inter.getName() << " ";
		}
		os << std::endl;
		os << "Moved to " << name << std::endl;
		return change(name, os);
	}
	return *this;
}

Location Location::change(std::string name, std::ostream& os) {
	Intersection* temp = findIntersection(name);
	if (!temp)
		os << "There is no intersection with this name, so we didnt change it" << std::endl;
	else {
		os << "Changed to " << name << std::endl;
		current = temp;
	}
	return *this;
}

void Location::close(std::string name) {
	Intersection* temp = findIntersection(name);
	temp->close();
	closed.push_back(temp->getName());

}

void Location::show(std::ostream& os) {
	os << current->getName() << std::endl;
}

void Location::showNeighbours(std::ostream& os) {
	current->printNeighbours(os);
	
}


void Location::open(std::string name) {
	Intersection* temp = findIntersection(name);
	temp->open();
	if (closed.size() > 0) {
		if (closed[closed.size() - 1] == name)closed.pop_back();
		else
			for (std::vector<std::string>::iterator it = closed.begin(); it < closed.end(); it++) {
				if ((*it) == name)it = closed.erase(it);
			}
	}
	else {
		std::cout << "No closed intersections" << std::endl;
	}
}

void Location::showClosed(std::ostream& os) {
	if (closed.size() == 0){
		os << "There are no closed intersections" << std::endl;
		return;
	}
	for (std::string name : closed) {
		os << name << " ";
	}
	os << std::endl;
}

Intersection* Location::findIntersection(std::string name) {
	for (Intersection* inter : *allIntersections) {
		if (inter->getName() == name)return inter;
	}
	return nullptr;
}


bool City::hasIntersection(std::vector<Intersection>intersections, std::string name) {
	
	for (int i = 0; i < intersections.size(); i++) {
		if (intersections[i].getName() == name)return true;
	}
	return false;
}

bool City::hasPathBetweenHelper(std::vector<Intersection>& visited, Intersection inter1, Intersection inter2) {
	if (hasIntersection(visited, inter1.getName()))return false;
	if (inter1.hasNeighbour(inter2.getName()))return true;
	visited.push_back(inter1);
	for (std::pair<Intersection*, int> neigh : inter1.getNeighbours()) {
		Intersection next = *(neigh.first);
		if (hasPathBetweenHelper(visited, next, inter2))return true;
	}
	return false;
}

bool isInVisited(std::vector<Intersection>visited, Intersection inter) {
	for (int i = 0; i < visited.size(); i++) {
		if (visited[i].getName() == inter.getName())return true;
	}
	return false;

}

std::vector<Intersection> City::findBigCircle(Intersection start, Intersection end, std::vector<Intersection> visited, std::vector<Intersection>result) {
	if (hasIntersection(result, start.getName()))return result;

	visited.push_back(start);

	if (start.hasNeighbour(end.getName())) {
		if (result.size() == map.size() - 1) {
			result.push_back(end);
			return result;
		}
	}
	for (int i = 0; i < start.neighbourCount(); i++) {
		Intersection next = *(start.getNeighbours()[i].first);
		if (!isInVisited(visited, next)) {
			std::vector<Intersection> temp = findBigCircle(next, end, visited, result);
			if (temp.size() == map.size()) {
				result = temp;
				break;
			}
		}
	}

	return result;
}

void City::printBigCircle(Intersection start, Intersection end, std::ostream& os) {
	std::vector<Intersection> path = findBigCircle(start, end, std::vector<Intersection>(), std::vector<Intersection>());
	for (Intersection inter : path) {
		os << inter.getName();
	}
	os << std::endl;
}

std::vector<Path> City::allPathsBetween(std::vector<Intersection>currPath, int currLength, Intersection inter1, Intersection inter2, std::vector<Path>& result, bool considerClosed) {
	if (hasIntersection(currPath, inter1.getName()))return result;
	if (considerClosed && inter1.isClosed())return result;
	currPath.push_back(inter1);
	for (std::pair<Intersection*, int>neigh : inter1.getNeighbours()) {
		Intersection next = *(neigh.first);
		allPathsBetween(currPath, currLength + neigh.second, next, inter2, result, considerClosed);
	}
	if (inter1.hasNeighbour(inter2.getName())) {
		currPath.push_back(inter2);
		result.push_back(std::make_pair(currPath, currLength + inter1.getNeighbour(inter2.getName())->second));
		return result;
	}
	return result;
}

void City::printPaths(std::vector<Path>paths, std::ostream& out) {
	int i = 0;
	for (std::pair<std::vector<Intersection>, int> path : paths) {
		out << "Path " << ++i << ". ";
		for (Intersection inter : path.first) {
			out << inter.getName() << " ";
		}
		out << "has length " << path.second << std::endl;
	}
}

Path City::findCircle(Intersection start, Intersection current, std::vector<Intersection>path, std::vector<Intersection>& visited, int length, bool considerClosed) {
	std::pair<std::vector<Intersection>, int>holder = std::make_pair(path, length);

	if (hasIntersection(visited, current.getName()))return holder;

	visited.push_back(current);
	path.push_back(current);

	if (current.hasNeighbour(start.getName())) {
		path.push_back(start);
		return std::make_pair(path, (length + current.getNeighbour(start.getName())->second));
	}

	bool hasPathInANeighbour = false;
	std::pair<std::vector<Intersection>, int>* result = nullptr;
	for (std::pair<Intersection*, int> neigh : current.getNeighbours()) {
		Intersection next = *(neigh.first);
		if (!considerClosed || !next.isClosed()) {
			std::pair<std::vector<Intersection>, int>circle = findCircle(start, next, path, visited, length + neigh.second, considerClosed);
			if (circle.second != 0 && (start.getName() == circle.first[(circle.first.size() - 1)].getName()))
				return circle;
		}
	}
	return holder;

}

std::vector<Intersection>expandAPath(std::vector<Intersection> path, std::vector<Intersection> circle) {
	std::vector<Intersection>::iterator it1 = path.begin();
	for (it1; it1 < path.end(); it1++) {
		if ((*it1).getName() == circle[0].getName()) {
			break;
		}
	}
	path.insert(it1 + 1, circle.begin() + 1, circle.end());
	return path;
}

std::vector<Path> City::expandPaths(std::vector<Path>& paths, bool considerClosed) {
	std::pair<std::vector<Intersection>, int> smallest = paths[0];
	for (Intersection inter : smallest.first) {
		std::vector<Intersection> temp;

		std::pair<std::vector<Intersection>, int> circle = findCircle(inter, inter, std::vector<Intersection>(), temp, 0, considerClosed);
		std::vector<Intersection> expanded = expandAPath(paths[0].first, circle.first);
		std::vector<std::pair<std::vector<Intersection>, int>>result;
		if (circle.second != 0) {
			result = paths;

			if (paths.size() == 1) {
				result.push_back(std::make_pair(expanded, circle.second + smallest.second));
				result.push_back(std::make_pair(expandAPath(expanded, circle.first), 2 * circle.second + smallest.second));
			}
			else if (paths.size() > 1 && (circle.second + smallest.second) < paths[1].second) {
				std::pair<std::vector<Intersection>, int> expanded1 = std::make_pair(expanded, (circle.second + smallest.second));
				if (paths.size() == 2 && (2 * circle.second + smallest.second) >= paths[1].second)result.push_back(paths[1]);
				else if (paths.size() == 2)result.push_back(std::make_pair(expandAPath(expanded1.first, circle.first), 2 * circle.second + smallest.second));
				else if ((2 * circle.second + smallest.second) >= paths[1].second)result[2] = paths[1];
				else result[2] = std::make_pair(expandAPath(expanded1.first, circle.first), 2 * circle.second + smallest.second);
				result[1] = expanded1;
			}
			else if (paths.size() == 2)result.push_back(std::make_pair(expandAPath(paths[0].first, circle.first), circle.second + smallest.second));
			else if (paths.size() > 2 && (circle.second + smallest.second) < paths[2].second)result[2] = std::make_pair(expandAPath(paths[0].first, circle.first), circle.second + smallest.second);
			return result;
		}
		std::cout << "End" << std::endl;
	}
	return paths;
}

std::vector<Path> City::reduceAndSortPaths(std::vector<Path>paths) {
	int resultSize;
	if (paths.size() < 3)resultSize = paths.size();
	else resultSize = 3;
	std::vector<std::pair<std::vector<Intersection>, int>>result;
	int min;
	for (int i = 0; i < resultSize; i++) {
		min = paths[i].second;
		for (int j = i + 1; j < paths.size(); j++) {
			if (paths[j].second < min) {
				std::pair<std::vector<Intersection>, int>temp = paths[i];
				paths[i] = paths[j];
				paths[j] = temp;
				min = paths[i].second;
			}
		}
	}
	for (int k = 0; k < resultSize; k++) {
		result.push_back(paths[k]);
	}
	return result;
}

bool City::addIntersection(Intersection* inter) {
	if (inter != nullptr && isUnique(inter->getName())) {
		map.push_back(inter);
		return true;
	}
	return false;
}

std::vector<Intersection*>::iterator City::getIterator() {
	return std::vector<Intersection*>::iterator();
}


void City::importIntersections(std::string path) {
	std::ifstream is(path, std::ios::in);
	std::string line;
	getline(is, line);
	while (line != "\0") {
		buildIntersection(line);
		getline(is, line);
	}


}

void City::exportIntersections(std::string path) {
	std::ofstream os(path, std::ios::out);
		iterator it = map.begin();
		for (it; it < map.end(); it++) {
			(*it)->print(os);
		}
	
}

bool City::isUnique(std::string name){
	Intersection* inter = getIntersection(name);
	if (inter == nullptr)return true;
	return false;
}

Location City::getLocation(std::string name) {
	return Location(name, &map);
}

bool isInVector(std::vector<std::string>list, std::string str, int index) {
	for (int i = 0; i < list.size(); i++) {
		if (str.compare(list[i]) == 0 && i != index)return true;

	}
	return false;

}

Intersection* City::buildIntersection(std::string expression) {
	std::vector<std::string> strings;
	int index = 0;
	int nextIndex = expression.find_first_of(" ", index + 1);
	//npos e kraqt na stringa
	while (nextIndex != std::string::npos) {
		strings.push_back(expression.substr(index, nextIndex - index));
		index = nextIndex + 1;
		nextIndex = expression.find_first_of(" ", index + 1);
	}
	strings.push_back(expression.substr(index, expression.length()));
	Intersection* intersection = getIntersection(strings[0]);
	if (intersection == nullptr) {
		intersection = new Intersection(strings[0]);
		addIntersection(intersection);
	}
	if (intersection->neighbourCount() > 0 && strings.size() > 1)return nullptr;

	for (int i = 1; i < strings.size(); i += 2) {
		if (strings[i].compare(strings[0]) == 0)return nullptr;
		if (isInVector(strings, strings[i], i))return nullptr;

		Intersection* inters1 = getIntersection(strings[i]);
		std::stringstream stream(strings[i + 1]);
		int distance;
		stream >> distance;
		if (inters1 == nullptr) {
			inters1 = new Intersection(strings[i]);
			map.push_back(inters1);
			
		}
		intersection->addNeighbour(inters1, distance);
		inters1->addIncomingNeighbour(intersection);
	}
	return intersection;
}

Intersection* City::getIntersection(std::string name) {
	using vectorIterator = std::vector<Intersection*>::iterator;
	for (iterator it = map.begin(); it != map.end(); ++it) {
		if (name.compare((*it)->getName()) == 0)return *it;
	}
	return nullptr;
}

void City::print() {
	std::cout << std::endl << "All the intersections added in city are: " << std::endl;
	int i = 0;
	for (Intersection* inter : map) {
		std::cout << ++i << ". intersection is" << std::endl;
		inter->print(std::cout);
		std::cout << "Incoming neighbour count = " << inter->incomingNeighbourCount() << std::endl;
	}
	std::cout << std::endl;
}

bool City::hasPathBetween(std::string name1, std::string name2) {
	Intersection inter1 = *(getIntersection(name1));
	Intersection inter2 = *(getIntersection(name2));
	std::vector<Intersection> temp;
	return hasPathBetweenHelper(temp, inter1, inter2);
}

std::vector<std::pair<std::vector<Intersection>, int>> City::getShortestPaths(std::string name1, std::string name2, bool considerClosed, std::ostream& os) {
	using Paths = std::vector<std::pair<std::vector<Intersection>, int>>;
	Paths result;
	if (!hasPathBetween(name1, name2))return result;
	Intersection inter1 = *(getIntersection(name1));
	Intersection inter2 = *(getIntersection(name2));
	Paths temp;
	Paths paths = allPathsBetween(std::vector<Intersection>(), 0, inter1, inter2, temp, considerClosed);
	if (paths.size() == 0) {
		os << "There is no path between these intersections,it might be becaouse of closed intersection." << std::endl;
		return result;
	}
	Paths reducedPaths = reduceAndSortPaths(paths);
	result = expandPaths(reducedPaths, considerClosed);
	return result;
}

void City::showTheShortestPaths(std::string name1, std::string name2, bool considerClosed, std::ostream& os) {
	using Paths = std::vector<std::pair<std::vector<Intersection>, int>>;
	Paths result = getShortestPaths(name1, name2, considerClosed, os);
	if (result.size() > 0) {
		printPaths(result, os);
	}
	else {
		os << "There is no path between these intersections" << std::endl;
	}
}

bool City::hasSmallTour(std::string name1) {
	Intersection inter1 = *getIntersection(name1);
	std::vector<Intersection> temp;
	return hasPathBetweenHelper(temp, inter1, inter1);
}

bool City::hasTourToAllIntersections(std::ostream& os) {
	int outcomingCount = 0;
	int incomingCount = 0;
	Intersection* start = nullptr;
	Intersection* end = nullptr;
	for (iterator it = map.begin(); it != map.end(); ++it) {
		if (outcomingCount > 1 || incomingCount > 1)return false;
		if ((*it)->neighbourCount() == (*it)->incomingNeighbourCount()) {
			if (start == nullptr) {
				start = (*it);
				end = (*it);
			}
		}
		else if (((*it)->neighbourCount() - (*it)->incomingNeighbourCount()) == -1) {
			end = (*it);
			incomingCount++;
		}
		else if (((*it)->neighbourCount() - (*it)->incomingNeighbourCount()) == 1) {
			start = (*it);
			outcomingCount++;
		}
		else return false;
	}
	if ((incomingCount - outcomingCount) != 0)return false;
	if (!canGoToAllIntersections(start->getName()))return false;
	printBigCircle(*start, *end, os);
	return true;


}
//bfs with keeping the visited and checking in the end if they are all
bool City::canGoToAllIntersections(std::string name) {
	Intersection inter = *getIntersection(name);
	std::vector<Intersection>visited;
	std::queue<Intersection> next;
	next.push(inter);
	while (!next.empty()) {
		visited.push_back(inter);
		if (visited.size() == map.size())return true;

		for (std::pair<Intersection*, int>neigh : inter.getNeighbours()) {
			Intersection temp = *(neigh.first);
			if (!hasIntersection(visited, temp.getName()))next.push(temp);
		}
		next.pop();
		if (next.empty())inter = next.front();
	}
	return false;

}

void City::showDeadEnds(std::ostream& os) {
	std::vector<Intersection*>::iterator it = map.begin();
	for (it; it < map.end(); it++) {
		if ((*it)->neighbourCount() == 0) {
			std::vector<Intersection*> neighbours = (*it)->getIncomingNeighbours();
			for (unsigned j = 0; j < (*it)->incomingNeighbourCount(); j++) {
				os << neighbours[j]->getName() << "->";
				os << (*it)->getName() << std::endl;
			}
		}
	}

}