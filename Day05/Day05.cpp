/*
Santa needs help figuring out which strings in his text file are naughty or nice.

A nice string is one with all of the following properties:

It contains at least three vowels (aeiou only), like aei, xazegov, or aeiouaeiouaeiou.
It contains at least one letter that appears twice in a row, like xx, abcdde (dd), or aabbccdd (aa, bb, cc, or dd).
It does not contain the strings ab, cd, pq, or xy, even if they are part of one of the other requirements.
For example:

ugknbfddgicrmopn is nice because it has at least three vowels (u...i...o...), a double letter (...dd...), and none of the disallowed substrings.
aaa is nice because it has at least three vowels and a double letter, even though the letters used by different rules overlap.
jchzalrnumimnmhp is naughty because it has no double letter.
haegwjzuvuyypxyu is naughty because it contains the string xy.
dvszwmarrgswjxmb is naughty because it contains only one vowel.
*/


/*

--- Part Two ---
Realizing the error of his ways, Santa has switched to a better model of determining whether a string is naughty or nice. None of the old rules apply, as they are all clearly ridiculous.

Now, a nice string is one with all of the following properties:

It contains a pair of any two letters that appears at least twice in the string without overlapping, like xyxy (xy) or aabcdefgaa (aa), but not like aaa (aa, but it overlaps).
It contains at least one letter which repeats with exactly one letter between them, like xyx, abcdefeghi (efe), or even aaa.
For example:

qjhvhtzxzqqjkmpb is nice because is has a pair that appears twice (qj) and a letter that repeats with exactly one letter between them (zxz).
xxyxx is nice because it has a pair that appears twice and a letter that repeats with one between, even though the letters used by each rule overlap.
uurcxstgmygtbstg is naughty because it has a pair (tg) but no repeat with a single letter between them.
ieodomkazucvgmuy is naughty because it has a repeating letter with one between (odo), but no pair that appears twice.
How many strings are nice under these new rules?


-------------------------------------------------------------------------------
New criteria:
    = repeating pair (eg. abab) without overlap (e.g. aaa would NOT count)
    = xyx - triplet (repeat with one letter in between)

    For first criteria, can do as O(n^2), look at first 2 chars and check the rest of 
    the string, then move forward and keep checking (taking care to account for no overlap)



    For second criteria can simply loop and compare to two strings ago 
*/


// TODO: command line inputs for part 1 v part 2 

#include <iostream>
#include <string> // Note: need iostream first 
#include <fstream>


using namespace std;


// How to keep track of vowels? Bool array -- as practice, use pointers to functions 


bool nicestring1(string input_string) {
    // 3 distinct vowels 
    // One double letter
    // No forbidden substrings (ab, cd, pq, xy)

    bool vowel_check = false, double_letter_check = false;

    int vowel_count = 0; 
    bool vowels[5] = {false};  // will initialize all to false, note, same syntax may not work for true 
    string vowel_str = "aeiou"; 

    char cur_char, prev_char;


    // NOTE: if we cared, we could do an O(1) lookup using more memory by definined an array 
    // with true/false for each ASCII character OR better yet, -1 flag for not a vowel
    // and a corresponding index for each vowel so we could check the count 
    // https://www.asciitable.com/ 

    int vowel_idx; 
    for (int idx = 0; idx < input_string.length(); idx++) {

        cur_char = input_string[idx]; 

        /*
                    Check Vowels 
        */
        // https://cplusplus.com/reference/string/string/find/
        if (!vowel_check) {
            vowel_idx = vowel_str.find(cur_char);
            if (vowel_idx != -1) {
                //if (!vowels[vowel_idx]) {    // distinct vowels 
                if (true) {

                    vowels[vowel_idx] = true;
                    vowel_count++; 
                } 
            }
            if (vowel_count > 2) {
                vowel_check = true;   // there are enough distinct vowels 
            }
        }



        // For forbidden substring and double letter check, check current and previous letter 
        if (idx > 0) {
            /*
                Check double letters 
            */
            if (!double_letter_check) {
                if (cur_char == prev_char) {
                    double_letter_check = true; 
                }
            }


            /*
                Check forbiden 
            */
            if ((cur_char == 'b' && prev_char == 'a') || (cur_char == 'd' && prev_char == 'c') ||
                (cur_char == 'q' && prev_char == 'p') || (cur_char == 'y' && prev_char == 'x')) {
                return false; 
            }

        }
        prev_char = cur_char;
    }
    return (vowel_check && double_letter_check);
}




bool nicestring2(string input_string) {
    
    char cur_char, prev_char;

    bool triplet_check = false, double_letter_check = false; 

    /*

        Triplet check

    */ 
    for (int idx = 2; idx < input_string.length(); idx++) {
        if (input_string[idx] == input_string[idx-2]) {
            triplet_check = true;
            break; 
        }
    }
    if (!triplet_check) {
        return false; 
    }



    int vowel_idx; 

    string pair; 

    for (int idx = 1; idx < input_string.length() - 2; idx++) {
        // Start at position one, end at position length() - 3 
        pair = input_string.substr(idx - 1, 2); 
        if (input_string.find(pair, idx + 1) != -1) {
            double_letter_check = true;
            break;
        }
    }

    return (triplet_check && double_letter_check);
}

// Arg C is arg count, argv array of character vectors 
// argv 0 is name of program, 
int main(int argc, char* argv[]) {


    int which_check;
    if (argc > 1) {
        which_check = stoi(argv[1]); // string to int 
    } else {
        which_check = 1; 
    }

    bool (*check_fun)(string);  // Pointer to a FUNCTION which takes a string 
    if (which_check == 1) {
        check_fun = nicestring1;
    } else {
        check_fun = nicestring2; 
    }

    cout << which_check << endl; 


    ifstream myfile("input.txt");  // ifstream for reading 

    int nicecount = 0;

    string line; 
    if (myfile.is_open()){

        while (myfile.good()) {
    
            getline(myfile, line); 

//            cout << "String: " << line;

            if (check_fun(line)) {
                nicecount++;
              //  cout << ", Nice!" << endl;
            } else {
                //cout << ", Naughty" << endl; 
            }

        }

        cout << "Nice strings: " << nicecount << endl; 

        myfile.close(); 
    } else {
        cout << "Could not open file" << endl;
    }




    return 0; 
}