/*
--- Day 9: All in a Single Night ---
Every year, Santa manages to deliver all of his presents in a single night.

This year, however, he has some new locations to visit; his elves have provided him the distances between every pair of locations. He can start and end at any two (different) locations he wants, but he must visit each location exactly once. What is the shortest distance he can travel to achieve this?

For example, given the following distances:

London to Dublin = 464
London to Belfast = 518
Dublin to Belfast = 141
The possible routes are therefore:

Dublin -> London -> Belfast = 982
London -> Dublin -> Belfast = 605
London -> Belfast -> Dublin = 659
Dublin -> Belfast -> London = 659
Belfast -> Dublin -> London = 605
Belfast -> London -> Dublin = 982
The shortest of these is London -> Dublin -> Belfast = 605, and so the answer is 605 in this example.

What is the distance of the shortest route?
*/


/*

Approach: Solve the TSP. The TSP goes to every location and ends where it started.
Because the TSP goes to EVERY location (and ends where it started), the start location for TSP
doesn't matter. From a TSP solution, we can simply shift the solution so the longest path is at the end
(return to home) then delete the longest path. 

^ Is this necceseraly true? Maybe not actually 

-- From our approach, we can compute from some starting city, the minimum distance to any other city 
-- If DONT need to return home, maybe starting city matters? 


There are many TSP approaches, we will try the Held-Karp Dynamic Programming approach 

To make processing input simpler, assign each city a number (1 to N)
Define a distance lookup 
By default, we will will define our distance lookup with lower numbered city first 

Data structure for 2 way unique map (city name <--> nunber)
Can do with 2 hashmaps - one to map number to city and vice versa 


For distance map, can use std pair, for worse space complxity but better time complexity
We will just put in BOTH distances into the map 

Need a way to lookup the distance function g(S_i, si) which is shortest path from city 0 
to city si through all the cities in the set S_i 
    - Can use a map 
    - The set S_i generically is unordered but we could also use it sorted so that it defines a unique key, should be able to use vector
        of ints for the key 
    - 



TODO For part 2: 
    - held-karp return min
    - wrapper to iterate over starting locations 
    - simple option to change sign in shortest path algorithm






*/

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <unordered_map>
#include <map>

#include <algorithm>
#include <numeric>





#include <bits/stdc++.h>

using namespace std; 



struct path_t {
    int distance; 
    vector<int> sequence; 
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

// NOTE: pair is fine key for regular map but not unordered map
// Would need to define hash OR use ordered map 
void get_map_data(unordered_map<string, int> &to_city_number, 
        unordered_map<int, string> &to_city_name,  
        map<pair<int, int>, int> &city_distances) {

    ifstream myfile("input.txt");  
    //ifstream myfile("simple_input.txt");   // we get the right answer on this one 

    string line; 
    vector<string> wvect; 

    int new_city_idx = 0;

    string city_a, city_b; 
    int cdistance; 

    if (myfile.is_open()){
        while (myfile.good()) {    
            getline(myfile, line); 
            wvect = parse_line(line);

            city_a = wvect[0];
            city_b = wvect[2]; 
            cdistance = stoi(wvect[4]);        

            if (to_city_number.count(city_a) == 0) {
                to_city_number[city_a] = new_city_idx;
                to_city_name[new_city_idx] = city_a; 
                new_city_idx++;
            }
            if (to_city_number.count(city_b) == 0) {
                to_city_number[city_b] = new_city_idx;
                to_city_name[new_city_idx] = city_b; 
                new_city_idx++;
            }

            // Put into the map with both 
            city_distances[make_pair(to_city_number[city_a], to_city_number[city_b])] = cdistance;
            city_distances[make_pair(to_city_number[city_b], to_city_number[city_a])] = cdistance;
        }
        myfile.close(); 
    } else {
        cout << "Could not open file" << endl;
    }
}



int countSetBits(int n)
{
    int count = 0;
    while (n) {
        count += n & 1;
        n >>= 1;
    }
    return count;
}

// https://stackoverflow.com/questions/12991758/creating-all-possible-k-combinations-of-n-items-in-c
// Smart implementation would get all bit tables first

// Well this sorcery seems to have an error in some cases so we are fucked there cool



// TODO
// Switch to hasing and compare runtimes
// Understand the crazy bit-table code 


// https://stackoverflow.com/questions/12991758/creating-all-possible-k-combinations-of-n-items-in-c

void print_vector(const vector<int> &v) {
    for (int i=0; i < v.size(); i++) {
        cout << v[i] << " ";  
    }
}

/*
Notes:
Why can we get away with calling PERMUTATIONS?
Because bits are all the same. So if we treat it as a MASK
we wont get duplicates. 
*/

// https://en.cppreference.com/w/cpp/algorithm/prev_permutation
vector<vector<int>> generate_subsets(int set_size, vector<int>numbers) {
    int n = numbers.size(); 
    string bitmask(set_size, 1); // K leading 1's
    bitmask.resize(n, 0); // N-K trailing 0's

    vector<vector<int>> subsets; 

 
    
    // print integers and permute bitmask
    do {
        vector<int> new_combo; 
        for (int i = 0; i < n; i++) // [0..N-1] integers
        {
            if (bitmask[i]) {
                new_combo.push_back(numbers[i]);
            }
        }
        subsets.push_back(new_combo); 
    } while (std::prev_permutation(bitmask.begin(), bitmask.end()));

    return subsets;
}



// TODO -- templating - how to make work for map or unordered map?
// see here https://stackoverflow.com/questions/25749917/how-can-i-make-a-function-that-takes-either-a-map-or-an-unordered-map
int held_karp_tsp(unordered_map<string, int> &to_city_number, 
        unordered_map<int, string> &to_city_name,  
        map<pair<int, int>, int> &city_distances, int starting_city=0, bool use_min=true) {

    // Vectors can be compared so can use a map which uses comparisons but not an unordered_map which uses hashing 
    // Would need to define a hash function, which you can do. For now, use a regular map
    //unordered_map<vector<int>, path_t> shortest_path_distances; 
    // Key should be subset AND terminal city? 
    // Maps of Maps to make the hashing a little easier 
    // First key is terminal city 
    unordered_map<int, map<vector<int>, path_t>> shortest_path_distances; 
    //map<vector<int>, path_t> shortest_path_distances; 
    const int num_cities = to_city_name.size(); 

    /*
    Phase 1
    Initialization of distances from city 0 direct to every city 
    */
    path_t path;  // memory question - when added to the maps are these by reference or value? VALUE! (unless specifially do pointers)




    for (int city_idx = 0; city_idx < num_cities; city_idx++) {
        if (city_idx != starting_city) {
            path.distance = city_distances[make_pair(starting_city, city_idx)];
            cout << "Base path distance: " << path.distance << " " << to_city_name[starting_city] << " to " << to_city_name[city_idx] << endl;
            path.sequence = {starting_city};  // total sequence 
            //shortest_path_distances[key] = path; 
            map<vector<int>, path_t> terminal_distances;
            vector<int> key = {city_idx}; // key for terminal distance is the SET 
            terminal_distances[key] = path; 
            shortest_path_distances[city_idx] = terminal_distances; // key here is Terminal city 
        }
    }

    // shortest_path_distance -> input a set of cities AND a terminal city,  get a distance 


    vector<int> numbers(num_cities);  // delete start city below 
    iota(numbers.begin(),numbers.end(), 0); //https://en.cppreference.com/w/cpp/algorithm/iota
    numbers.erase(numbers.begin() + starting_city); // pull out 

    /*
    vector<vector<int>> foo = generate_subsets(4, numbers); 
    for (auto & element : foo) {
        print_vector(element); cout << endl;
    }  
    */ 


    for (int set_size = 2; set_size <= num_cities-1; set_size++) {
        // list of all possible subsets (vector of vectors)
        vector<vector<int>> subsets = generate_subsets(set_size, numbers);  // nmbers doesnt include srtart incity 


        for (auto &subset : subsets) {
            // For a GIVEN subset, compute for ALL cities (k) in that subset, the optimal path to that city 
            //int min_idx = 0;
            //int min_distance = 1e6; // TODO -- ma

            // start with min set size of 2 becuae will be excluding "k"
            
            for (int skip_idx = 0; skip_idx < subset.size(); skip_idx++) {    // finding the index to remove is gonna be a pain!
                // For each city in the subset 
                int k = subset[skip_idx];
                vector<int> set_no_k(subset);  // creates a copy 
                set_no_k.erase(set_no_k.begin() + skip_idx); // note, skip idx is in the vector NOT the global city idx 

                // Iterate over all the other cities 
                int best_distance = -1; // flag 
                int check_distance;
                int best_check_idx = -1; // flag  
                for (int check_idx = 0; check_idx < subset.size(); check_idx++) {

                    // subset[check_idx] wil be the the city before the terminal city (k)

                    if (check_idx != skip_idx) { // idxs references to set vector 

                        /*
                        if (shortest_path_distances[subset[check_idx]].count(set_no_k) == 0) {
                            cout << "SET NO THERE??? ====================" << endl;
                            cout << " check city: " << subset[check_idx] << endl;
                            cout << " k: " << k << endl;

                            cout << "subset NO K:  " ;
                            for (int i=0; i < set_no_k.size(); i++)  
                                cout << set_no_k[i] << " ";  
                            cout << endl;
                            cout << "subset:  " ;
                            for (int i=0; i < subset.size(); i++)  
                                cout << subset[i] << " ";  
                            cout << endl;

                        }
                        */
                        // shortest path to terminal index that does NOT include k 
                        // plus distance from terminal index to k 
                        check_distance = shortest_path_distances[subset[check_idx]][set_no_k].distance + 
                                                            city_distances[make_pair(k, subset[check_idx])];

                        if (use_min) {
                            if ((best_distance == -1) || (check_distance < best_distance)) {
                                best_distance = check_distance;  // now min distance not -1 
                                best_check_idx = check_idx; 
                            }
                        } else {
                            if ((best_distance == -1) || (check_distance > best_distance)) {
                                best_distance = check_distance;  // now min distance not -1 
                                best_check_idx = check_idx; 
                        }

                        }
 
                    }
                } 
                // NOW can do the update
                path_t new_path;
                new_path.distance = best_distance;
                vector<int> new_sequence(shortest_path_distances[subset[best_check_idx]][set_no_k].sequence); 
                new_sequence.push_back(subset[best_check_idx]);
                new_path.sequence = new_sequence;

                // to get to k with with this subset (that include k) this is shortest oath  
                shortest_path_distances[k][subset] = new_path;

                                /*
                cout << "best check idx: " << best_check_idx << endl;
                cout << "subset size: " << subset.size() << endl;
                cout << "k: " << k << endl;
                cout << "best m: " << subset[best_check_idx] << endl;
                cout << "Distance: " << min_distance << endl;
                */ 



            }


        }
            
    }


    /*
    
    Phase 2 Find which has the smallest to close the gap 
    
    */
    // For each terminal city 
    // except base city 
    vector<int> all_cities(num_cities);  // -1 for base city
    iota(all_cities.begin(),all_cities.end(),0); //https://en.cppreference.com/w/cpp/algorithm/iota
    all_cities.erase(all_cities.begin() + starting_city);


    cout << "All cities: ";
    for (int i=0; i<all_cities.size(); i++)  {
        cout << all_cities[i] << " ";  
    }
    cout << endl;

   
    vector<int> total_loop_distances(num_cities-1);
    vector<vector<int>> sequence_distance_vectors;  // using push back so DONT allocate a size



    for (int idx=0; idx<all_cities.size(); idx++) {

        vector<int> seq_dst_vec;



        int city_idx = all_cities[idx];
        cout << "Ending City: " << to_city_name[city_idx] << endl;

        path_t path = shortest_path_distances[city_idx][all_cities];


        for (auto & seq_idx : path.sequence) {
            cout << "-> " << to_city_name[seq_idx];
        }
        cout << endl;
        cout << "Total Distance: " << path.distance << endl;

        cout << "Distance sanity check: " << endl;
        int cum = 0;


        

        for (int aa = 1; aa < path.sequence.size(); aa++) {
            int tmp = city_distances[make_pair(path.sequence[aa], path.sequence[aa-1])];
            cum += tmp;
            seq_dst_vec.push_back(tmp); 
            cout << "+" << tmp;
        }
        int to_end = city_distances[make_pair(path.sequence.back(), city_idx)];
        cum += to_end;
        cout << "+" << to_end;
        seq_dst_vec.push_back(to_end);
        cout << endl;
        cout << "Total: " << cum << endl;

        int closing_dist = city_distances[make_pair(starting_city, city_idx)] ;
        cout << "Closing dist: " << closing_dist << endl;
        cout << "Looped: " << cum + closing_dist << endl;
        cout << endl; 



        seq_dst_vec.push_back(closing_dist);
        

        

        cout << "hhhh" << endl;

        sequence_distance_vectors.push_back(seq_dst_vec); 
        
    }  

    // Lessonn to learn, print statements NOT reliable for identifying where segfault occurs
    // here it is in the block after the next cout but it never printed 


    cout << "size: " << sequence_distance_vectors.size() << endl;
    
    vector<int> open_loop_distances;

    for (auto & seq_dist_vec : sequence_distance_vectors) {
    //for (int idx = 0; idx < sequence_distance_vectors.size(); idx++){
    //    vector<int> seq_dist_vec = sequence_distance_vectors[idx]; 
        
        int sum = std::accumulate(seq_dist_vec.begin(), seq_dist_vec.end(), 0);
        
        // max_element reutrns and iterator, * gives of the value pointed to 
        int rmv_dist;
        if (use_min) {
            rmv_dist = *std::max_element(begin(seq_dist_vec), end(seq_dist_vec));
        } else {
            rmv_dist = *std::min_element(begin(seq_dist_vec), end(seq_dist_vec));
        }
        cout << "Closed: " << sum << " max: " << rmv_dist << " Open loop path: " << sum - rmv_dist << endl;

        open_loop_distances.push_back(sum - rmv_dist);
    }

    if (use_min) {
        return *std::min_element(begin(open_loop_distances), end(open_loop_distances)); 
    } else {
        return *std::max_element(begin(open_loop_distances), end(open_loop_distances)); 
    }
    
    

    /*

    /*
    
    Close the loop
    
    */

    /*    
    for (int idx=0; idx<all_cities.size(); idx++) {

        if (shortest_path_distances[all_cities[idx]].count(all_cities) == 0) {
            cout << "NOT FILLED IN ?" << endl;

            throw std::runtime_error("not filled in?");
        }

        int city_idx = all_cities[idx];

        total_loop_distances[idx] = shortest_path_distances[city_idx][all_cities].distance + city_distances[make_pair(0, city_idx)];
        cout << "Dist " << total_loop_distances[idx] << endl;
        
        //path_t path = shortest_path_distances[city_idx][all_cities];
        //path.distance = total_distances[idx] ;
        //path.sequence.push_back(0);   
        //shortest_path_distances[city_idx][all_cities] = path;      
    }
    */ 




    

    //https://stackoverflow.com/questions/55304846/how-to-get-the-index-of-the-smallest-element-in-a-vector

    //auto it = std::min_element(begin(total_loop_distances), end(total_loop_distances));
    //std::cout << "index of smallest element: " << distance(begin(total_distances), it);
    //int min_idx = std::distance(begin(total_loop_distances), it);
    //cout << "Min idx: " << min_idx << endl;
    //cout << "Min distance: " << total_loop_distances[min_idx] << endl;


    /*

        Lets loop through all ENDING Cities
    
    */
   // TODO -- add in final 

   


      

    /*
    
    Phase 3 - Delete the longest se
    
    */

}

int main(int argc, char* argv[]) {


    unordered_map<string, int> to_city_number; 
    unordered_map<int, string> to_city_name;
    // 
    //unordered_map<pair<int, int>, int, PairHasher> city_distances;
    map<pair<int, int>, int> city_distances;

    get_map_data(to_city_number, to_city_name, city_distances);


    // run it from different starting locations 
    // get 207
    vector<int> min_distances;
    for (int starting_city = 0; starting_city < to_city_number.size(); starting_city++) {
        min_distances.push_back( held_karp_tsp(to_city_number, to_city_name, city_distances,starting_city)); 

    }

    cout << "Min Distances:" << endl; 
    print_vector(min_distances);
    cout << endl;
    // Getting 209 but thats not right, dont know if high or low????



    vector<int> max_distances;
    for (int starting_city = 0; starting_city < to_city_number.size(); starting_city++) {
        max_distances.push_back( held_karp_tsp(to_city_number, to_city_name, city_distances,starting_city, false)); 

    }

    cout << "Max Distances:" << endl; 
    print_vector(max_distances);
    cout << endl;
    
    /*
    for (int a = 0; a < foo.size(); a++) {
        for (int i = 0; i < 5; ++i) {
            int combo = combos[a];
            if ((combo >> i) & 1)
                cout << foo[i] << ' ';
        }
        cout << endl;
    }
    */ 
    
    // part 2 what happens if we 
}
