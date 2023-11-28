/*

https://adventofcode.com/2015/day/3

--- Day 3: Perfectly Spherical Houses in a Vacuum ---
Santa is delivering presents to an infinite two-dimensional grid of houses.

He begins by delivering a present to the house at his starting location, and then an elf at the North Pole calls him via radio and tells him where to move next. Moves are always exactly one house to the north (^), south (v), east (>), or west (<). After each move, he delivers another present to the house at his new location.

However, the elf back at the north pole has had a little too much eggnog, and so his directions are a little off, and Santa ends up visiting some houses more than once. How many houses receive at least one present?

For example:

> delivers presents to 2 houses: one at the starting location, and one to the east.
^>v< delivers presents to 4 houses in a square, including twice to the house at his starting/ending location.
^v^v^v^v^v delivers a bunch of presents to some very lucky children at only 2 houses.


How many houses recieve at LEAST one present 


--- Part Two ---
The next year, to speed up the process, Santa creates a robot version of himself, Robo-Santa, to deliver presents with him.

Santa and Robo-Santa start at the same location (delivering two presents to the same starting house), then take turns moving based on instructions from the elf, who is eggnoggedly reading from the same script as the previous year.

This year, how many houses receive at least one present?

For example:

^v delivers presents to 3 houses, because Santa goes north, and then Robo-Santa goes south.
^>v< now delivers presents to 3 houses, and Santa and Robo-Santa end up back where they started.
^v^v^v^v^v now delivers presents to 11 houses, with Santa going one direction and Robo-Santa going the other.


*/ 


#include <iostream>
#include <fstream>
#include <string>


#include <unordered_map>


#include <stdio.h>   // fprintf
using namespace std;

// #include <format>  //C++ 20 and later 

/*

Approach:
    Keep track of where presents have been delivered 
    Can denote house by tuple (x, y) with x and y having INTEGER values
    We dont know how big, so occupancy grid could be rough 
    We also want O(N) lookup

    Hashmap with (x,y) as key and true/false OR number of presents delivered as value 

    See std::unordered_map: https://en.cppreference.com/w/cpp/container/unordered_map
    See also, tuples in C++: https://en.cppreference.com/w/cpp/utility/tuple

    And here: https://stackoverflow.com/questions/20834838/using-tuple-in-unordered-map, note, tuples dont have 
    a standard hash in cpp so would need to implement yourself 

    Simpler implementation with same runtime is to use STRING of cordinates seperated by comma


*/ 



// Quick "inline" function 

inline string house_key(int x, int y) {
    string s = to_string(x);
    s += ",";
    s += to_string(y);
    cout << s << endl;
    return s; 
}



void part1() {

    ifstream myfile("input.txt");  // ifstream for reading 

    unordered_map<string, int> house_map = {{"0,0", 1}}; // put in first house, 1 present 

    int x = 0, y = 0;  // Santa cords 
    string cord_key; 


    if (myfile.is_open()){

        char c; 

        while (myfile.good()) {
            myfile.get(c); 

            if (myfile.eof()) {
                cout << "End of file reached" << endl;                
            } else if (c == '<'){
                x--;
            }else if (c == '>') {
                x++;
            } else if (c == '^'){
                y++;
            } else if (c == 'v') {
                y--; 
            }

            cord_key = house_key(x,y); 

            //if (house_map.contains(cord_key)) { // C++20 Solution
            if (house_map.count(cord_key) > 0) { // Old C++ Solution

                house_map[cord_key] = house_map[cord_key] + 1; 
            } else {
                house_map[cord_key] = 1; 
            }



        }
    
        myfile.close(); 
        cout << "# Of Houses: " << house_map.size() << endl;

    } else {
        cout << "Could not open file" << endl;
    }
}


void part2() {
    ifstream myfile("input.txt");  // ifstream for reading 

    // 2 presents at first house now!
    // NOTE: for some reason, changing the number of presents at first house is changing 
    // where santas are ending up.
.
    // NEED THESE OTHERWISE CHANGES ANSWER! WHY? C++ does NOT do it unless STATIC
    // making these zero fixes bug
    int xa = 0,ya = 0, xb = 0,yb = 0;  // Santa cords, robosanta cords  
 
    string cord_key;

    int *x_ptr, *y_ptr;    // Note, NOT int* x_ptr, y_ptr

    // Lets practice using pointers 

    bool robosanta = false; 
    if (myfile.is_open()){

        char c; 

        // NOTE: increment/decrement operators take precedence over dereferencing

        while (myfile.good()) {
            myfile.get(c); 

            if (robosanta) {
                x_ptr = &xb;
                y_ptr = &yb;
            } else {
                x_ptr = &xa; 
                y_ptr = &ya; 
            }

            if (myfile.eof()) {
                cout << "End of file reached" << endl;    
            } else{             
                if (c == '<'){
                    (*x_ptr)--;
                }else if (c == '>') {
                    (*x_ptr)++;
                } else if (c == '^'){
                    (*y_ptr)++;
                } else if (c == 'v') {
                    (*y_ptr)--; 
                }

                cord_key = house_key(*x_ptr,*y_ptr); 

                //if (house_map.contains(cord_key)) { // C++20 Solution
                if (house_map.count(cord_key) > 0) { // Old C++ Solution

                    house_map[cord_key] = house_map[cord_key] + 1; 
                } else {
                    house_map[cord_key] = 1; 
                }


                if (robosanta) {
                    cout << "robosanta: " << c << endl;
                } else {
                    cout << "santa: " << c <<  endl;
                }

                cout << "xa: " << xa << " ya : " << ya << endl;
                cout << "xb: " << xb << " yb : " << yb << endl;


                // Toggle santas
                robosanta = !robosanta;


            }

             
        }
    
        myfile.close(); 
        cout << "# Of Houses: " << house_map.size() << endl;

    } else {
        cout << "Could not open file" << endl;
    }
}



int main() {


    

    // As exercise, set these so we can run both from command line with args 
    //part1();
    part2();

}