/* 
https://adventofcode.com/2015/day/1

Santa was hoping for a white Christmas, but his weather machine's "snow" function is powered by stars, and he's fresh out! To save Christmas, he needs you to collect fifty stars by December 25th.

Collect stars by helping Santa solve puzzles. Two puzzles will be made available on each day in the Advent calendar; the second puzzle is unlocked when you complete the first. Each puzzle grants one star. Good luck!

Here's an easy puzzle to warm you up.

Santa is trying to deliver presents in a large apartment building, but he can't find the right floor - the directions he got are a little confusing. He starts on the ground floor (floor 0) and then follows the instructions one character at a time.

An opening parenthesis, (, means he should go up one floor, and a closing parenthesis, ), means he should go down one floor.

The apartment building is very tall, and the basement is very deep; he will never find the top or bottom floors.

For example:

(()) and ()() both result in floor 0.
((( and (()(()( both result in floor 3.
))((((( also results in floor 3.
()) and ))( both result in floor -1 (the first basement level).
))) and )())()) both result in floor -3.
To what floor do the instructions take Santa?


*/


#include <iostream>
#include <fstream>
using namespace std;


int main(){
    ifstream myfile("input.txt");  // ifstream for reading 

    int floor = 0; 

    int position = 0; 


    if (myfile.is_open()){

        char c; 

        while (myfile.good()) {
            myfile.get(c); 
            position++; 

            if (myfile.eof()) {
                cout << "End of file reached" << endl;                
            } else if (c == '('){
                floor++;
            }else if (c == ')') {
                floor--;
            } else {
                cout << "Unexpected char: " << c << endl;
            }

            if (floor == -1) {
                cout << "Basement pos: " << position << endl;
            }

        }
    
        myfile.close(); 
        cout << "Floor: " << floor << endl;

    } else {
        cout << "Could not open file" << endl;
    }

    return 0; 

}