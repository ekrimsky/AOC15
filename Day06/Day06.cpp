/*
--- Day 6: Probably a Fire Hazard ---
Because your neighbors keep defeating you in the holiday house decorating contest year after year, you've decided to deploy one million lights in a 1000x1000 grid.

Furthermore, because you've been especially nice this year, Santa has mailed you instructions on how to display the ideal lighting configuration.

Lights in your grid are numbered from 0 to 999 in each direction; the lights at each corner are at 0,0, 0,999, 999,999, and 999,0. The instructions include whether to turn on, turn off, or toggle various inclusive ranges given as coordinate pairs. Each coordinate pair represents opposite corners of a rectangle, inclusive; a coordinate pair like 0,0 through 2,2 therefore refers to 9 lights in a 3x3 square. The lights all start turned off.

To defeat your neighbors this year, all you have to do is set up your lights by doing the instructions Santa sent you in order.

For example:

turn on 0,0 through 999,999 would turn on (or leave on) every light.
toggle 0,0 through 999,0 would toggle the first line of 1000 lights, turning off the ones that were on, and turning on the ones that were off.
turn off 499,499 through 500,500 would turn off (or leave off) the middle four lights.
After following the instructions, how many lights are lit?
*/ 



/*
--- Part Two ---
You just finish implementing your winning light pattern when you realize you mistranslated Santa's message from Ancient Nordic Elvish.

The light grid you bought actually has individual brightness controls; each light can have a brightness of zero or more. The lights all start at zero.

The phrase turn on actually means that you should increase the brightness of those lights by 1.

The phrase turn off actually means that you should decrease the brightness of those lights by 1, to a minimum of zero.

The phrase toggle actually means that you should increase the brightness of those lights by 2.

What is the total brightness of all lights combined after following Santa's instructions?

For example:

turn on 0,0 through 0,0 would increase the total brightness by 1.
toggle 0,0 through 999,999 would increase the total brightness by 2000000.
*/ 


#include <iostream>
#include <string> // Note: need iostream first 
#include <fstream>

#include <sstream>

using namespace std;

/*

Parsing input 
    - Instruction, coordinate A, coordinate B 

    - Coordinate B is after "through"
    - Coordinate A is before "through"
    - Instruction before coordiante A 


*/ 
enum instruction {on, off, toggle}; 


// Some chat gpt help here - typedfef for a pointer to change lights functions
// What we if we wanted it to be non-fixed size? 
typedef void (*ChangeLightsFunction)(int (&arr)[1000][1000], instruction, int[2], int[2]);
//template <size_t rows, size_t cols> typedef  void (*ChangeLightsFunction)(int (&arr)[rows][cols], instruction, int[2], int[2]);


// Could do this and then change syntax below to
// ChangeLightsFunction<rows, cols> ptr = &change_lights1;
//template <size_t rows, size_t cols> using ChangeLightsFunction = void (*)(int (&arr)[rows][cols], instruction, int[2], int[2]);



// https://stackoverflow.com/questions/8767166/passing-a-2d-array-to-a-c-function




void parse_line(string line, instruction &inst, int cordA[2], int cordB[2]) {
    // See strtok

    string str_cordA, str_cordB; 


    stringstream ss(line); 

    string cur_word;
    ss >> cur_word; 
    if (cur_word == "toggle") { // can use == for strings but not c-strings 
        inst = toggle;
    } else {
        ss >> cur_word; 
        if (cur_word == "on") {
            inst = on;
        } else {
            inst = off; 
        }
    }

    ss >> str_cordA; 
    ss >> cur_word;
    ss >> str_cordB; 


    // Now split
    cordA[0] = stoi(str_cordA.substr(0, str_cordA.find(",")));
    cordA[1] = stoi(str_cordA.substr(str_cordA.find(",") + 1));

    cordB[0] = stoi(str_cordB.substr(0, str_cordB.find(",")));
    cordB[1] = stoi(str_cordB.substr(str_cordB.find(",") + 1));

}


// See non-type template arrays, there are a handful of ways to do this 


// https://stackoverflow.com/questions/8767166/passing-a-2d-array-to-a-c-function
// template <size_t rows, size_t cols> void process_2d_array_template(int (&array)[rows][cols])

// Could write more efficient code to keep track of number of lights that are on and when they are toggled


// These are non-type template parameters - definitely makes it more confusing to have a pointer to the function this way 

template <size_t rows, size_t cols> void change_lights1(int (&arr)[rows][cols], instruction inst, int cordA[2], int cordB[2]) {
    int count = 0;


    for (int row_idx = cordA[0]; row_idx <= cordB[0]; row_idx++) {
        for (int col_idx = cordA[1]; col_idx <= cordB[1]; col_idx++) {
            if (inst == toggle) {
                arr[row_idx][col_idx] = (!arr[row_idx][col_idx]);
            } else if (inst == on) {
                arr[row_idx][col_idx] = 1;
            } else {
                arr[row_idx][col_idx] = 0; 
            }
        }

    }
}



template <size_t rows, size_t cols> void change_lights2(int (&arr)[rows][cols], instruction inst, int cordA[2], int cordB[2]) {
    int count = 0;


    for (int row_idx = cordA[0]; row_idx <= cordB[0]; row_idx++) {
        for (int col_idx = cordA[1]; col_idx <= cordB[1]; col_idx++) {
            if (inst == toggle) {
                arr[row_idx][col_idx] = arr[row_idx][col_idx] + 2;
            } else if (inst == on) {
                arr[row_idx][col_idx] += 1;
            } else {
                arr[row_idx][col_idx] += -1; 
                if (arr[row_idx][col_idx] < 0) {arr[row_idx][col_idx] = 0;} // flooring
            }
        }

    }
}




template <size_t rows, size_t cols> int count_lights(int (&arr)[rows][cols]) {
    int count = 0;


    for (int row_idx = 0; row_idx < rows; row_idx++) {
        for (int col_idx = 0; col_idx < cols; col_idx++) {

            if (arr[row_idx][col_idx]) {
                count += arr[row_idx][col_idx]; 
            }
        }

    }

    return count;   
}


int main(int argc, char* argv[]) {
    ifstream myfile("input.txt");  // ifstream for reading 


    instruction inst; 

    int cordA[2], cordB[2]; 


    int which_check;
    if (argc > 1) {
        which_check = stoi(argv[1]); // string to int 
    } else {
        which_check = 1; 
    }


    int arr[1000][1000] = {{0}}; // all lights start off 
 
    //    

    //template <size_t rows, size_t cols> void (*change_fun)(&int[rows][cols], instruction, int[2], int[2]);  // Pointer to a FUNCTION 
    //void (*change_fun)(&int[1000][1000], instruction, int[2], int[2]);  // Pointer to a FUNCTION 
    

    ChangeLightsFunction change_fun = &change_lights1;

    
    if (which_check == 2) {
        change_fun = &change_lights2;
    } 
    cout << which_check << endl; 
 

    string line; 
    if (myfile.is_open()){

        while (myfile.good()) {
    
            getline(myfile, line); 

            parse_line(line, inst, cordA, cordB);

            change_fun(arr, inst, cordA, cordB);


            cout << cordA[0] << "," << cordA[1] << " " << cordB[0] << "," << cordB[1] << endl; 




        }

        int count = count_lights(arr);
        cout << "Lights ON: " << count << endl;


        myfile.close(); 
    } else {
        cout << "Could not open file" << endl;
    }




    return 0; 
}