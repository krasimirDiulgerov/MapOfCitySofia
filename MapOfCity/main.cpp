#include<iostream>
#include"city.h"

std::vector<std::string>proccessInput(std::string input) {
    int pos = 0;
    std::vector<std::string> result;

    pos = input.find_first_of(' ', pos);
    result.push_back(input.substr(0, pos));
    result.push_back(input.substr(pos + 1, input.length() - pos - 1));
    return result;

}

int main() {

	City city;
	city.importIntersections("test12345.txt");
	city.print();

	Location location = city.getLocation("NDK");

    if (city.hasPathBetween("Alabin", "Vitoshka")) {
        std::cout << "There is a path" << std::endl;
    }
    else std::cout << "There is no path" << std::endl;
    
    city.showTheShortestPaths("NDK", "Popa", false, std::cout);
    city.getIntersection("Alabin")->close();
    city.showTheShortestPaths("NDK", "Popa", true, std::cout);
    if (city.hasSmallTour("NDK")) {
        std::cout << "There is a small Tour" << std::endl;
    }
    else std::cout << "There isnt a small Tour"<<std::endl;
    City city1;
    city1.importIntersections("test2314.txt");
    if (city1.hasTourToAllIntersections(std::cout)) {
        std::cout << "There is a Big Tour around the city"<< std::endl;
    }
    else {
        std::cout << "There isnt a big tour around the city"<< std::endl;
    }
    if (city.canGoToAllIntersections("NDK")) {
        std::cout << "This intersection can go to all intersections"<< std::endl;
    }
    else {
        std::cout << "This intersection cant go to all of the intersections"<< std::endl;
    }
    City city2;
    city2.importIntersections("test12345.txt");
    city2.showDeadEnds(std::cout);
   
   do {
        std::string input;
        std::getline(std::cin, input);
        std::vector<std::string> data = proccessInput(input);
        //location, change, neighbours, move, close, open, closed
        if (data[0] == "location") {
            location.show(std::cout);
        }
        else if (data[0] == "change") {
            if (data.size() > 1)
                location = location.change(data[1], std::cout);
            else
                std::cout << "Not enough arguments!" << std::endl;
        }
        else if (data[0] == "neighbours") {
            location.showNeighbours(std::cout);
        }
        else if (data[0] == "move") {
            if (data.size() > 1)
                location = location.move(data[1], std::cout);
            else
                std::cout << "Not enough arguments!" << std::endl;
        }
        else if (data[0] == "close") {
            if (data.size() > 1)
                location.close(data[1]);
            else
                std::cout << "Not enough arguments!" << std::endl;
        }
        else if (data[0] == "open") {
            if (data.size() > 1)
                location.open(data[1]);
            else
                std::cout << "Not enough arguments!" << std::endl;
        }
        else if (data[0] == "closed") {
            location.showClosed(std::cout);
        }
        else if (data[0] == "exit") {
            break;
        }
        else {
            std::cout << "Invalid command!" << std::endl;
        }
    } while (true);


	return 0;
}