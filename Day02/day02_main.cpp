/*
https://adventofcode.com/2015/day/2

--- Day 2: I Was Told There Would Be No Math ---
The elves are running low on wrapping paper, and so they need to submit an order for more. They have a list of the dimensions (length l, width w, and height h) of each present, and only want to order exactly as much as they need.

Fortunately, every present is a box (a perfect right rectangular prism), which makes calculating the required wrapping paper for each gift a little easier: find the surface area of the box, which is 2*l*w + 2*w*h + 2*h*l. The elves also need a little extra paper for each present: the area of the smallest side.

For example:

A present with dimensions 2x3x4 requires 2*6 + 2*12 + 2*8 = 52 square feet of wrapping paper plus 6 square feet of slack, for a total of 58 square feet.
A present with dimensions 1x1x10 requires 2*1 + 2*10 + 2*10 = 42 square feet of wrapping paper plus 1 square foot of slack, for a total of 43 square feet.
All numbers in the elves' list are in feet. How many total square feet of wrapping paper should they order?



--- Part Two ---
The elves are also running low on ribbon. Ribbon is all the same width, so they only have to worry about the length they need to order, which they would again like to be exact.

The ribbon required to wrap a present is the shortest distance around its sides, or the smallest perimeter of any one face. Each present also requires a bow made out of ribbon as well; the feet of ribbon required for the perfect bow is equal to the cubic feet of volume of the present. Don't ask how they tie the bow, though; they'll never tell.

For example:

A present with dimensions 2x3x4 requires 2+2+3+3 = 10 feet of ribbon to wrap the present plus 2*3*4 = 24 feet of ribbon for the bow, for a total of 34 feet.
A present with dimensions 1x1x10 requires 1+1+1+1 = 4 feet of ribbon to wrap the present plus 1*1*10 = 10 feet of ribbon for the bow, for a total of 14 feet.


*/ 


/*
Approach 
    - Read line by line 
    - Parse
    - Compute 

Pointer Practice:
    - Function that takes in line and fills in width, height, length by REFERENCE



*/ 





#include <iostream>
#include <fstream>
#include <string>
using namespace std;



// https://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c


void parse_line(string my_string, int& w, int& h, int& l) { // pass ints by ref 

    // Assume formatted WxHxL
    string delim = "x"; 


    // string substr (size_t pos = 0, size_t len = npos) const;
    // pos = position of first character 
    // len = number of characters to copy 
    w = stoi(my_string.substr(0, my_string.find(delim)));  //stoi = "string to int"
    my_string.erase(0, my_string.find(delim) + delim.length()); // cut begining 
    h = stoi(my_string.substr(0, my_string.find(delim)));  //stoi = "string to int"
    my_string.erase(0, my_string.find(delim) + delim.length()); // cut begining 
    l = stoi(my_string); 

}


void update_area_length(int w, int h, int l, int& area, int& bow) {

    // Total side areas PLUS area of smallest side 

    int a, b, c, d;
    a = w*h;
    b = w*l; 
    c = h*l;

    // Practice with ternary operators 
    d = (a < b) ? a : b; 
    d = (c < d) ? c : d;  
    area += 2*a+2*b+2*c+d; 


    //
    int bow_knot, bow_loop;
    int prm_a, prm_b, prm_c;

    prm_a = 2*(w + h);
    prm_b = 2*(l + h);
    prm_c = 2*(w + l);


    bow_knot = w*h*l; 
    bow_loop = (prm_a < prm_b) ? prm_a : prm_b;
    bow_loop = (bow_loop < prm_c) ? bow_loop : prm_c;

    bow += bow_knot + bow_loop;

}



int main(){
    ifstream myfile("input.txt");  // ifstream for reading 

    string line;   // NOTE: in namespace std with <string>

    int area = 0; 
    int bow = 0; 

    int l, w, h; 
    if (myfile.is_open()){

        while (myfile.good()) {
    
            getline(myfile, line); 
            parse_line(line, w, h, l); // function is pass by reference but dont use & here - ONly in declaration

            update_area_length(w, h, l, area, bow); 

            cout << w << " x " << h << " x " << l << endl; 

        }

        cout << "Area: " << area << endl; 
        cout << "Bow: " << bow << endl; 

        myfile.close(); 
    } else {
        cout << "Could not open file" << endl;
    }

    return 0; 

}