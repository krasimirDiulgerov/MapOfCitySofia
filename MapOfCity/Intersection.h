#ifndef _1_INTERSECTION_H
#define _1_INTERSECTION_H
#include<string>
#include<vector>
#include<utility>


class Intersection
{
private:
	std::string name;
	bool _isClosed;
	std::vector <std::pair<Intersection*, int>>neighbours;
	std::vector <Intersection*>incomingNeighbours;

public:
	//constructor
	Intersection();
	//constructor
	Intersection(std::string name_);
	//constructor
	Intersection(Intersection const& inter);
	//operator=
	Intersection& operator= (Intersection const& inter);
	//getter
	std::string getName() {
		return this->name;
	}
	//used to close an Intersection when wanted from the user
	void close();
	//used to open an Intersection when wanted from the user
	void open();
	//getter
	bool isClosed();
	//used to add neighbours 
	void addNeighbour(Intersection* neigh, int distance);
	//used to add neighbours that have streets coming to the Intersection
	void addIncomingNeighbour(Intersection* neigh);
	//checks whether this Intersection has a neighbour with that name
	bool hasNeighbour(std::string name_);
	//getter
	int neighbourCount();
	//getter
	int incomingNeighbourCount();
	//getter that finds a neighbour with that name
	std::pair<Intersection*, int>* getNeighbour(std::string name_);
	//getter
	std::vector<std::pair<Intersection*, int>> getNeighbours();
	//getter
	std::vector<Intersection*>getIncomingNeighbours();
	//prints
	void print(std::ostream& os);
	//getter
	void printNeighbours(std::ostream& os);



};

#endif