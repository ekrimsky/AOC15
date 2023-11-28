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
            inst.gate = AND;
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


// Whoops -- looks like direct numbers can be passed to the AND/OR too 

// Are we gonna get a stack overflow?? 

// Seems to be running very slowly - do we NEED to save intermediate values to make it even feasible? 
uint16_t eval_circuit(unordered_map<string, instruction_t> &inst_map, string channel) {

    //cout << "Channel " << channel << endl;

    // Base case for when channel is already number (and not in map)
    if (is_number(channel)) {
        return (uint16_t) stoi(channel);
    }

    if (inst_map[channel].defined) {
        //cout << "ch " << channel << " defined" << endl;
        return inst_map[channel].output_value; 
    }

    uint16_t ret_val; 

    if (inst_map[channel].gate == NONE) {
        if (is_number(inst_map[channel].input_a)) {

            
            uint16_t output_value =  (uint16_t)  stoi(inst_map[channel].input_a);
            //return output_value; 

            ret_val = output_value;

        } else {
            //return eval_circuit(inst_map, inst_map[channel].input_a);
            ret_val = eval_circuit(inst_map, inst_map[channel].input_a);

        }
    } else if (inst_map[channel].gate == NOT) {
        //return ~eval_circuit(inst_map, inst_map[channel].input_a);
        ret_val = ~eval_circuit(inst_map, inst_map[channel].input_a);

    } else if ((inst_map[channel].gate == LSHIFT) || (inst_map[channel].gate == RSHIFT)) {
        //cout << "Channel: " << channel << ", shift Number: " << inst_map[channel].input_b << endl;

        uint16_t shift = (uint16_t) stoi(inst_map[channel].input_b);
        if (inst_map[channel].gate == LSHIFT) {
            //return (eval_circuit(inst_map, inst_map[channel].input_a) << shift);
            ret_val = (eval_circuit(inst_map, inst_map[channel].input_a) << shift);
        
        } else { // rshift 
            //return (eval_circuit(inst_map, inst_map[channel].input_a) >> shift);
            ret_val = (eval_circuit(inst_map, inst_map[channel].input_a) >> shift);
        }
    } else if (inst_map[channel].gate == OR) {
        //return (eval_circuit(inst_map, inst_map[channel].input_a) | eval_circuit(inst_map, inst_map[channel].input_b));
        ret_val = (eval_circuit(inst_map, inst_map[channel].input_a) | eval_circuit(inst_map, inst_map[channel].input_b));

    } else if (inst_map[channel].gate == AND) {

        //return (eval_circuit(inst_map, inst_map[channel].input_a) & eval_circuit(inst_map, inst_map[channel].input_b));
        ret_val =  (eval_circuit(inst_map, inst_map[channel].input_a) & eval_circuit(inst_map, inst_map[channel].input_b));
    
    } else {
        cout << "Invalid instruction" << endl;
    }


    inst_map[channel].output_value = ret_val; 
    inst_map[channel].defined = true;
    return ret_val;
}



//Function to print all the values
void print_channels(unordered_map<string, instruction_t> &inst_map) {
    for (auto const& item : inst_map) { // TODO: review iterators 
        uint16_t value = eval_circuit(inst_map, item.first);
        cout << "Channel: " << item.first << ", Value: " << value << endl;
    }
}   

int main(int argc, char* argv[]) {
    //ifstream myfile("simple_input.txt");  
    //ifstream myfile("input.txt");  // ifstream for reading 
    ifstream myfile("input2.txt");  // For part 2 



    //vector<instruction_t> inst; 
    unordered_map<string, instruction_t> inst_map; 


    string line; 
    instruction_t cur_inst; 
    if (myfile.is_open()){
        while (myfile.good()) {    
            getline(myfile, line); 
            cur_inst = get_instruction(line);
            //inst.push_back(get_instruction(line));
            inst_map[cur_inst.output] = cur_inst;  
        }
        myfile.close(); 
    } else {
        cout << "Could not open file" << endl;
    }

    uint16_t part1_result = eval_circuit(inst_map, "a"); //16076
    cout << "a: " << eval_circuit(inst_map, "a") << endl;
    //print_channels(inst_map);

    // part 2 what happens if we 
}

// Ininpit file traceback from line 79, 199 
// see also 55, 233 (b), 285 
// like where is n ever defined? k and m 
// Better approach - parse tree? 