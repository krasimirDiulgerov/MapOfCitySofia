#include "Intersection.h"
#include <iostream>



Intersection::Intersection() {
	this->name = " ";
	this->_isClosed = false;
	this->neighbours = std::vector<std::pair<Intersection*, int>>();
	this->incomingNeighbours = std::vector <Intersection*>();
}

Intersection::Intersection(std::string _name) {
	this->name = _name;
	this->_isClosed = false;
	this->neighbours = std::vector<std::pair<Intersection*, int>>();
	this->incomingNeighbours = std::vector <Intersection*>();

}

Intersection::Intersection(Intersection const& inter) {
	this->name = inter.name;
	this->_isClosed = inter._isClosed;
	this->neighbours = inter.neighbours;
	this->incomingNeighbours = inter.incomingNeighbours;

}

Intersection& Intersection::operator= (Intersection const& inter) {
	this->name = inter.name;
	this->_isClosed = inter._isClosed;
	this->neighbours = inter.neighbours;
	this->incomingNeighbours = inter.incomingNeighbours;
	return *this;
}


void Intersection::close() {
	_isClosed = true;
}

void Intersection::open() {
	_isClosed = false;

}

bool Intersection::isClosed() {
	return _isClosed;

}

void Intersection::addNeighbour(Intersection* neigh, int distance) {
	neighbours.push_back(std::make_pair(neigh, distance));
}

void Intersection::addIncomingNeighbour(Intersection* neigh) {
	incomingNeighbours.push_back(neigh);
}

bool Intersection::hasNeighbour(std::string _name) {
	for (int i = 0; i < neighbours.size(); i++) {
		if (neighbours[i].first->getName() == _name)return true;
	}
	return false;
}

int Intersection::neighbourCount() {
	return neighbours.size();
}

int Intersection::incomingNeighbourCount() {
	return incomingNeighbours.size();

}

std::pair<Intersection*, int>* Intersection::getNeighbour(std::string _name) {
	for (std::pair<Intersection*, int>neigh : neighbours) {
		if (_name == neigh.first->getName())
			return &neigh;
	}
	return nullptr;

}

std::vector<std::pair<Intersection*, int>> Intersection::getNeighbours() {
	return neighbours;

}

std::vector<Intersection*>Intersection::getIncomingNeighbours() {
	return incomingNeighbours;
}

void Intersection::print(std::ostream& os) {
	os << name;
	for (std::pair<Intersection*, int>neigh : neighbours) {
		os << " " << neigh.first->getName() << " " << neigh.second;
	}
	os << std::endl;
}

void Intersection::printNeighbours(std::ostream& os) {
	for (std::pair<Intersection*, int>neigh : neighbours) {
		os << " " << neigh.first->getName() << " " << neigh.second << " ";
	}
}