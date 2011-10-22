#include<iostream>
#include<string>
#include<queue>
#include "hNode.h"
#include<map>
#include<fstream>
#include<vector>
#include <sstream>

using namespace std;

void readFreqs(string , int&, map<string,float>& );
void encode( hNode* , string );
void writeCompressed( string , string , map<string,string>&);
class compare{
    public:
    bool operator() (hNode* a , hNode* b ){
        return( a->w > b->w );
    }
};

map<string,string> encoding;

int main(int argc, char** argv){

    string freqTable = "chars-freq.txt";
    string fileName = "chars.txt";
    if( argc > 1 ){
        freqTable = argv[1];
    }
    if( argc > 2 ){
        fileName = argv[2];
        cout << fileName << endl;
    }
    // A map with the weight of a char
    map<string,float> wMap;
    
    // Priority queue for building the tree
    priority_queue< hNode*, vector<hNode*>, compare> huffmanpq;
    int charTotal = 0;

    // Read in the frequency information from the specified file
    readFreqs( freqTable, charTotal, wMap );

    // Build the huffman tree
     
    // For every symbol create a leaf node
    // and add it to the priority queue
    hNode *node;
    int i = 0;
    map<string,float>::iterator it;
    for( it = wMap.begin(); it != wMap.end(); it++ ){
        // Create a new node with weight freq / total
        node = new hNode((*it).second/charTotal);
        i++;
        // convert the string to a char for the node's symbol
        if( (*it).first == "\\n" ){
            node->symbol = '\n';
        }
        else{
            node->symbol = (*it).first[0];
        }
        node->l = node->r = NULL;
        huffmanpq.push(node);
    }

    hNode *a,*b;
    float total = 0;
    // While there is more than one node
    // in the priority queue
    while( huffmanpq.size() > 1 ){

        // Remove the two nodes of highest priority ( lowest probability )
        a = huffmanpq.top();
        huffmanpq.pop();
        b = huffmanpq.top();
        huffmanpq.pop();
        // create a new internal node with the summed probability
        node = new hNode(a->w + b->w);
        node->l = a;
        node->r = b;
        a->p = b->p = node;
   
        // Add that the node to the queue        
        huffmanpq.push(node);
    }

    // Here is the root of the binary tree.. hopefully! 
    hNode *root = huffmanpq.top();
    encode(root,"");

    string key = "",
           value = "";
    int bits = 0;
    float avgbit = 0;
    map<string,string>::iterator iter;
    for( iter = encoding.begin(); iter != encoding.end(); iter++ ){
        key = (*iter).first;
        value = (*iter).second; 
        if(key[0] == '\n'){
            key = "\\n";
        }
        avgbit += encoding[key].length();
        bits += wMap[key] * encoding[key].length();
        
        cout << key << " " << value << endl;
    }
    writeCompressed(fileName,"compressed.txt", encoding);   

    return 0;
}

void writeCompressed( string fileIn, string fileOut, map<string,string>& wmap){
    // Write the compressed binary data to the output file
    string compressed = "";
    char temp;
    string key = "";
    ifstream fin(fileIn.c_str());
    //ofstream fout(fileOut.c_str(),ios::binary);
    map<string,int> histo;
    // Read the entire contents of the input buffer
    stringstream file;
    file << fin.rdbuf();
    // Convert the entire file to a string
    string fileContents = file.str();
    
    int charTotal = fileContents.length();
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

    key = "";
    string value = "";
    int bits = 0;
    float avgbit = 0;
    map<string,string>::iterator iter;
    for( iter = wmap.begin(); iter != wmap.end(); iter++ ){
        key = (*iter).first;
        value = (*iter).second; 
        if(key[0] == '\n'){
            key = "\\n";
        }
        avgbit += wmap[key].length();
        bits += histo[key] * wmap[key].length();
        
        //cout << key << " " << value << endl;
   }

        
    cout << "# of bits unencoded: " << charTotal * 8 << endl; 
    cout << "# of bits when encoded:" << bits << endl;
    cout << "Avg bit per symbol encoded: " << avgbit/wmap.size() << endl; 

    return;
}

void encode( hNode* root , string enc ){
    if( root != NULL ){
        string key = "";
        if( root->l == NULL && root->r == NULL){
            if(root->symbol == '\n'){
                key = "\\n";
            }
            else{
                key += root->symbol;
            }
            encoding[key] = enc;
        }
        encode(root->l , enc + "0");
        encode(root->r , enc + "1");
    }
}

void readFreqs( string ft, int& count, map<string,float>& tmap){
    // Read in the frequency table file
    ifstream fin(ft.c_str());
    string key;
    float value, space_count;
    fin >> count;
   
    // Slight trickery! 
    fin >> space_count;
    tmap[" "] = space_count;
    
    while( !fin.eof() ){
        fin >> key;
        fin >> value;
        
        tmap[key] = value;
    }
    
    fin.close();
    return;
}

