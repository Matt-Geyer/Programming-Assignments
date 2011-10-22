#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
//#include "huff.cpp"

using namespace std;

int main( int argc, char** argv ){
    string fileName = "chars.txt";
    string fileOutName = "";
    char c;
    int charTotal = 0;
    map<string,int> histo;

    if( argc > 1 ) fileName = argv[1];
    ifstream fin(fileName.c_str());
  
    // Read the entire contents of the input buffer
    stringstream file;
    file << fin.rdbuf();
    // Convert the entire file to a string
    string fileContents = file.str();
    
    string key;
    charTotal = fileContents.length();
    for( int i = 0; i < fileContents.length(); i++ ){
        // I ended up using a map of strings so that
        // I could print the newline char as \n 
        key = fileContents.substr(i,1);
        if(fileContents[i] == '\n'){
            histo["\\n"] += 1;
        }
        else{
            histo[key] += 1;
        }
    }
    fin.close();

    // construct the output file name
    fileOutName = fileName.erase(fileName.length() - 4);
    fileOutName.append("-freq.txt");

    // Ouput the frequency table
    ofstream fout(fileOutName.c_str());
    
    // write the total number of chars
    fout << charTotal << endl;

    // Loop through the histogram and write
    map<string,int>::iterator it;
    for( it = histo.begin(); it != histo.end(); it++){
        fout << (*it).first << " " << (*it).second << endl;
    }
   
    fout.close();    

    return 0;
}


