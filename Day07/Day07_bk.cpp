/*
--- Day 7: Some Assembly Required ---
This year, Santa brought little Bobby Tables a set of wires and bitwise logic gates! Unfortunately, little Bobby is a little under the recommended age range, and he needs help assembling the circuit.

Each wire has an identifier (some lowercase letters) and can carry a 16-bit signal (a number from 0 to 65535). A signal is provided to each wire by a gate, another wire, or some specific value. Each wire can only get a signal from one source, but can provide its signal to multiple destinations. A gate provides no signal until all of its inputs have a signal.

The included instructions booklet describes how to connect the parts together: x AND y -> z means to connect wires x and y to an AND gate, and then connect its output to wire z.

For example:

123 -> x means that the signal 123 is provided to wire x.
x AND y -> z means that the bitwise AND of wire x and wire y is provided to wire z.
p LSHIFT 2 -> q means that the value from wire p is left-shifted by 2 and then provided to wire q.
NOT e -> f means that the bitwise complement of the value from wire e is provided to wire f.
Other possible gates include OR (bitwise OR) and RSHIFT (right-shift). If, for some reason, you'd like to emulate the circuit instead, almost all programming languages (for example, C, JavaScript, or Python) provide operators for these gates.

*/ 



#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>



#include <unordered_map>
#include <algorithm>

using namespace std; 

enum gate_t {RSHIFT, LSHIFT, AND, OR, NOT, NONE};

// Define a struct for parsing inputs 
struct instruction_t {
    gate_t gate;
    string input_a;    // what if integer? Could get fancy and use a UNION - internet says dont unless you need 
    string input_b;
    string output;
    uint16_t output_value; // numerical value of output once defined 
    bool defined = false; // until it is defined 
};

vector<string> parse_line(string line) {
    vector<string> word_vector; 
    stringstream ss(line); 
    string next_word;
    while (ss >> next_word) { // will be false when get through stream 
        word_vector.push_back(next_word);
    }
    return word_vector;
}

instruction_t get_instruction(string line) {
    vector<string> word_vector = parse_line(line); 
    instruction_t inst;

    if (word_vector.size() == 3) {   // simple pass through
        inst.input_a = word_vector[0]; // NOTE: this may be numeric, but still string 
        inst.gate = NONE;  
        inst.output = word_vector[2];
    } else if (word_vector.size() == 4) { // negation 
        inst.input_a = word_vector[1];
        inst.gate = NOT; 
        inst.output = word_vector[3];
    } else if (word_vector.size() == 5) {
        inst.input_a = word_vector[0];
        // need to find matching enum 
        if (word_vector[1].compare("LSHIFT") == 0) {
            inst.gate = LSHIFT; 
        } else if (word_vector[1].compare("RSHIFT") == 0) {
            inst.gate = RSHIFT;
        } else if (word_vector[1].compare("AND") == 0 ) {
            inst.gate = NOT;
        } else if (word_vector[1].compare("OR") == 0) {
            inst.gate = OR;
        } else {
            cout << "Unexpected gate: " << word_vector[1] << endl;
        }
        inst.input_b = word_vector[2];
        inst.output = word_vector[4];
    } else {
        cout << "Unexpected word vector size!" << endl;
    }
    return inst;
}

// Can make const since not modifying and 
// https://stackoverflow.com/questions/4654636/how-to-determine-if-a-string-is-a-number-with-c
bool is_number(const string& s)
{
    return !s.empty() && find_if(s.begin(), s.end(), [](unsigned char c) { return !isdigit(c); }) == s.end();
}



int main(int argc, char* argv[]) {
    ifstream myfile("input.txt");  // ifstream for reading 
    //ifstream myfile("simple_input.txt");  


    vector<instruction_t> inst; 

    string line; 
    if (myfile.is_open()){
        while (myfile.good()) {    
            getline(myfile, line); 
            inst.push_back(get_instruction(line)); 
        }
        myfile.close(); 
    } else {
        cout << "Could not open file" << endl;
    }

    // Now loop through until applied 

    // maps string to wire signal value 
    unordered_map<string, uint16_t> wire_map = {}; // put in first house, 1 present 

    // Loop until all values defined 

    bool all_defined = false;

    cout << "Updating definitions..." << endl; 

    int loops = 0; 
    while (!all_defined) {
        all_defined = true; // can get set to false below 
        for (int idx = 0; idx < inst.size(); idx++) {
            // Check if instruction fully defined

            if (inst[idx].gate == NONE) {
                if (is_number(inst[idx].input_a)) {
                    // here, input a isnt really a number 
                    inst[idx].output_value = stoi(inst[idx].input_a);
                    inst[idx].defined = true; 
                    wire_map[inst[idx].output] = inst[idx].output_value;
                } else if (wire_map.count(inst[idx].input_a) > 0){
                    // input may be char but may be defined in hash map already
                    // if input not yet defined, move on -- unless do we have loops?  
                    inst[idx].output_value = wire_map[inst[idx].input_a];
                    inst[idx].defined = true; 
                    wire_map[inst[idx].output] = wire_map[inst[idx].input_a];
                } else {
                    all_defined = false; 
                }
            } else if (inst[idx].gate == NOT) {
                if (wire_map.count(inst[idx].input_a) > 0) {
                    inst[idx].output_value = ~wire_map[inst[idx].input_a]; 
                    inst[idx].defined = true;
                    wire_map[inst[idx].output] = inst[idx].output_value;
                } else {
                    all_defined = false; 
                }

            } else if ((inst[idx].gate == LSHIFT) || (inst[idx].gate == RSHIFT)) {
                // Only apply if input defined 
                if (wire_map.count(inst[idx].input_a) > 0) {
                    uint16_t shift = (uint16_t) stoi(inst[idx].input_b);
                    if (inst[idx].gate == LSHIFT) {
                        inst[idx].output_value = wire_map[inst[idx].input_a] << shift;
                    } else { // rshift 
                        inst[idx].output_value = wire_map[inst[idx].input_a] >> shift;
                    }
                    inst[idx].defined = true;
                    wire_map[inst[idx].output] = inst[idx].output_value;
                } else {
                    all_defined = false; 
                }
            } else if (inst[idx].gate == OR) {

                if ((wire_map.count(inst[idx].input_a) > 0) && 
                    ((wire_map.count(inst[idx].input_b) > 0))) {
                        inst[idx].output_value = wire_map[inst[idx].input_a] | wire_map[inst[idx].input_b]; 
                        wire_map[inst[idx].output] = inst[idx].output_value;
                        inst[idx].defined = true;
                } else {
                    all_defined = false; 
                }
            } else if (inst[idx].gate == AND) {
                if ((wire_map.count(inst[idx].input_a) > 0) && 
                    ((wire_map.count(inst[idx].input_b) > 0))) {
                        inst[idx].output_value = wire_map[inst[idx].input_a] & wire_map[inst[idx].input_b]; 
                        wire_map[inst[idx].output] = inst[idx].output_value;
                        inst[idx].defined = true;
                } else {
                    all_defined = false; 
                }
            } else {
                cout << "Invalid instruction" << endl;
            }



        }

        cout << "a: " << wire_map["a"] << endl;

        loops++;
        cout << "Loop iters: " << loops << endl;
    }

    // Iterate over map to print 

    //for (auto const& item : wire_map) {
    //    cout << "Channel: " << item.first << ", Value: " << item.second << endl;
    //}
    cout << "a: " << wire_map["a"] << endl;
}


// Better approach - parse tree? 