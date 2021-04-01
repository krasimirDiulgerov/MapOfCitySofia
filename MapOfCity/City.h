#ifndef _CITYMAP_H
#define _CITYMAP_H
#include<vector>
#include<string>
#include"Intersection.h"

using Path = std::pair<std::vector<Intersection>, int>;
//this class is used for the bonus topic
class Location {
private:
	std::vector<Intersection*>* allIntersections;
	std::vector<std::string>closed;
	Intersection* current;
	//finds Intersection with that name
	Intersection* findIntersection(std::string name);
public:
	//constructor
	Location(std::string, std::vector<Intersection*>*);
	//constructor
	Location(Intersection*, std::vector<Intersection*>*);
	//in a vector of intersections checks whether it contains an Intersection with a specific name
	bool hasIntersection(std::vector<Intersection>intersections, std::string name);
	//this is used in getShortest path which is then used for the move option
	std::pair<std::vector<Intersection>, int> findShortestPath(std::vector<std::pair<std::vector<Intersection>,int>>);
	//izvinqvai za sledvashtoto
	std::vector<std::pair<std::vector<Intersection>, int>> allPathsBetween(std::vector<Intersection>currPath, int currLength, Intersection inter1, Intersection inter2, std::vector<std::pair<std::vector<Intersection>, int>>& result, bool considerClosed);
	//used for move 
	std::pair<std::vector<Intersection>, int> getShortestPath(std::string target, std::ostream& os, bool consideredClosed = true);
	//moves the Location according to the Paths
	Location move(std::string, std::ostream& os);
	//changes the Location we are at
	Location change(std::string, std::ostream& os);
	//prints the name
	void show(std::ostream& os);
	//prints all neighbours
	void showNeighbours(std::ostream& os);
	//closes the intersection with that name
	void close(std::string);
	//opens it
	void open(std::string);
	//prints all closed intersections
	void showClosed(std::ostream& os);
};
//main class
class City
{
	std::vector<Intersection*> map;
	//checks for intersection with that name
	bool hasIntersection(std::vector<Intersection>intersections, std::string name);
	//helper for the main function that searches for path between two intersections
	bool hasPathBetweenHelper(std::vector<Intersection>& visited, Intersection inter1, Intersection inter2);
	//searches for the specific big tour but fails
	std::vector<Intersection> findBigCircle(Intersection start, Intersection end, std::vector<Intersection> visited, std::vector<Intersection>result);
	//prints the big Tour
	void printBigCircle(Intersection start, Intersection end, std::ostream& os);
	//function that finds all paths between two intersections that are later sorted 
	std::vector<Path> allPathsBetween(std::vector<Intersection>currPath, int currLength, Intersection inter1, Intersection inter2, std::vector<Path>& result, bool considerClosed);
	//prints paths
	void printPaths(std::vector<Path>paths, std::ostream& out);
	// helping function that is used in expandPaths
	Path findCircle(Intersection start, Intersection current, std::vector<Intersection>path, std::vector<Intersection>& visited, int length, bool considerClosed);
	//function that is used in sorting the paths
	std::vector<Path> expandPaths(std::vector<Path>& paths, bool considerClosed);
	//sorting the paths we got
	std::vector<Path> reduceAndSortPaths(std::vector<Path>paths);

public:
	//constructor
	City() {};
	//adds Intersection
	bool addIntersection(Intersection* inter);
	using iterator = std::vector<Intersection*>::iterator;
	//getter
	iterator getIterator();
	//iterator for the start
	iterator begin() {
		return map.begin();
	}
	//iterator for the end
	iterator end() {
		return map.end();
	}
	//iterator for the next
	iterator next(iterator it) {
		return it++;
	}
	//imports intersections from a file
	void importIntersections(std::string path);
	//exports intersections to a file
	void exportIntersections(std::string path);
	//used to see if the intersection is unique
	bool isUnique(std::string name);
	//getter that finds a Location with a name
	Location getLocation(std::string name);
	//builds an Intersection using a name
	Intersection* buildIntersection(std::string expression);
	//getter
	Intersection* getIntersection(std::string name);
	//prints
	void print();
	//shows if there is a path between two intersections
	bool hasPathBetween(std::string name1, std::string name2);
	//sorts and returns the shortest 3 paths between 2 intersections
	std::vector<std::pair<std::vector<Intersection>, int>>getShortestPaths(std::string name1, std::string name2, bool considerClosed, std::ostream& os);
	//prints the 3 paths
	void showTheShortestPaths(std::string name1, std::string name2, bool considerClosed, std::ostream& os);
	//shows whether we can go on a  small Tour around the map
	bool hasSmallTour(std::string name1);
	//shows whether we can go on a tour around all the Locations on a map
	bool hasTourToAllIntersections(std::ostream& os);
	//shows whether from a specific Location we can reach all the other Locations
	bool canGoToAllIntersections(std::string name);
	//prints all Dead Ends
	void showDeadEnds(std::ostream& os);


};

#endif 