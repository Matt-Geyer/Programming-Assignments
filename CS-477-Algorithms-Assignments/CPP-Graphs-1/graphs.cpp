#include "Graph.h"
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <list>
#include <map>
#include <queue>
using namespace std;

void buildStateGraph( Graph& , bool );
void buildWordGraph( Graph& , bool );
bool hamming_time( string , string );

int top_kw = 5, top_ks = 5;

int main( int argc, char **argv ){
   
    // print the k top results in order 
    if( argc == 3 ){
        sscanf( argv[1] , "%d" , &top_kw );      
        sscanf( argv[2] , "%d" , &top_ks );
    }
    if( top_kw < 0 || top_kw > 5758 ) top_kw = 5;
    if( top_ks < 0 || top_ks > 49 ) top_ks = 5;

    string st_fname = "usa-matrix.txt";
    string wd_fname = "word-matrix.txt";

    // Loading by file, though possible, is commented 
    // out so that the program runs completely
 
    //Graph states( "statetest.txt" );

    Graph states( 49 );
    
    //Graph words( wd_fname );

    Graph words( 5758 );
    
    // Perform the full build step 
    buildWordGraph( words , true );
    
    // Print the adjaceny matrix to file
    words.printAdjMatrix( wd_fname );

    // Build the states graph
    buildStateGraph( states , true );
   
    // Print state Adjaceny matrix to file
    states.printAdjMatrix( st_fname );
    
    return 0;
}

bool hamming_time( string a , string b ){
    // determine if two 5 letter words have a hamming distance of 1 
    
    int ham_count = 0;
    for( int i = 0; i < 5; i++ ){
        if( a[i] != b[i] ){
             if( ham_count > 0 ){
                return false;
             }
             ham_count++;
        }
    }

    if( ham_count == 1 ){
        return true;
    }
    
    return false;
}
               

void buildWordGraph( Graph& g , bool buildGraph ){
    // build the graph for words list

    string fileName = "sgb-words.txt";
    ifstream fin( fileName.c_str() );

    list<string> words, twords;
    list<string>::iterator iter_i, iter_j;
    map<string, int> wmap;
    map<int, string> parallel_wmap;
    string w1, w2;
    queue<string> wsorted;

    // loop through every word and add it to the map with an id to use in the graph
    // as well as to a list for comparison 
    int k = 0;
    while( !fin.eof() ){
        fin >> w1;
        words.push_back( w1 );
        wmap[w1] = k;
        parallel_wmap[k] = w1;
        k++;
    }
 
    if( buildGraph ){
        // huge loop to check hamming distance for every word... for every word
        for( iter_i = words.begin(); iter_i != words.end(); iter_i++ ){
           for( iter_j = words.begin(); iter_j != words.end(); iter_j++ ){
                if( hamming_time( *iter_i , *iter_j ) ){
                    g.setEdge( wmap[*iter_i] , wmap[*iter_j] );
                }
            }
        }
        g.setMap( parallel_wmap );
    }

    int max, ec;
    string wmax, wprev;
    twords = words;
    int sentinal = 0;

    // This loop builds a list of the top k vertexes by edge count
    while( sentinal < top_kw ){ 
        max = -1, ec = 0;
        for( iter_i = twords.begin(); iter_i != twords.end(); iter_i++ ){
            ec = g.edgeCount( wmap[*iter_i] );
            if( ec > max ){
                max = ec;
                wmax = *iter_i;
            }
        }

        // add to the sorted list and remove from tmap
        wsorted.push( wmax );
        twords.remove( wmax );
        sentinal++;
    }
 
    // Print results to console
    cout << "The top " << top_kw << " vertexes sorted by vertex degree.." << endl << endl;
    string w;
    for( int i = 0; i < top_kw; i++ ){
        w = wsorted.front();
        cout << "#" << i + 1 << " - " << w << " with " << g.edgeCount( wmap[w] ) << " edges" << endl;
        wsorted.pop();
    }
    cout << endl << endl;
}
    


void buildStateGraph( Graph& g , bool buildGraph ){
    // build the graph of the contiguous states

    string fileName = "contiguous-usa.txt";
    ifstream fin( fileName.c_str() );

    list<string> states, tstates;
    list<string>::iterator iter;
    map<string, int> smap;
    map<int, string> parallel_smap;
    string st1, st2;   
    queue<string> ssorted;

    // loop through every state in the file and add it to the list   
    while( !fin.eof() ){
        fin >> st1;
        states.push_back( st1 );
    }
    fin.close();

    // sort and get rid of duplicates
    states.sort();
    states.unique();

    // build a map with a unique integer assigned as the value for each state
    int k = 0;    
    for( iter = states.begin(); iter != states.end(); iter++ ){
        smap[*iter] = k;
        parallel_smap[k] = *iter;
        k++;
    }    

    // This step can be skipped if the graph has already been built
    // and its adjaceny matrix saved to file..
    if( buildGraph ){
        // re-open and use the map ids to build the graph
        fin.open( fileName.c_str() );
        while( !fin.eof() ){
            fin >> st1;
            fin >> st2;
        
            // add an edge between the two states to the graph
            g.setEdge( smap[st1], smap[st2] );
        }
        g.setMap( parallel_smap );
        fin.close();
    }

    int max, ec;
    string smax, sprev;
    tstates = states;
    int sentinal = 0;
    // Build a list of the top k vertexes by edge degree
    while( sentinal < top_ks ){ 
        max = -1, ec = 0;
        for( iter = tstates.begin(); iter != tstates.end(); iter++ ){
            ec = g.edgeCount( smap[*iter] );
            if( ec > max ){
                max = ec;
                smax = *iter;
            }
        }

        // add to the sorted list and remove from tmap
        ssorted.push( smax );
        tstates.remove( smax );
        sentinal++;
    }
    // Print results to console
    cout << "The top " << top_ks << " vertexes sorted by vertex degree.." << endl << endl;
    string w;
    for( int i = 0; i < top_ks; i++ ){
        w = ssorted.front();
        cout << "#" << i + 1 << " - " << w << " with " << g.edgeCount( smap[w] ) << " edges" << endl;
        ssorted.pop();
    }
    cout << endl << endl;

    return;
}        

        
