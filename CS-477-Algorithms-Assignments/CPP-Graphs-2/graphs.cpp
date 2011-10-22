#include <iostream>
#include "Graph.h"
#include "dirGraph.h"
#include <vector>
#include <time.h>

using namespace std;

void buildNWordGraph( Graph&, bool, int );
bool hamming_time( string , string );
int main( int argc, char **argv){

    int n = 1, ec = 0;
    bool done = false;
    Graph *nwords;
    vector<int> sorted;
    dirGraph DAG(10);
    

    // Connected Component Answer
    Graph concomp( "word-matrix.txt" );
    cout << "Sgb-words graph has " << concomp.connectedComp() << " connected components.." << endl;
  
    // timed DFS
    clock_t start_tick, end_tick;
    double elapsed;

    start_tick = clock();
    concomp.nWordDFS();
    end_tick = clock();
    elapsed = ( end_tick - start_tick ) / (double)CLOCKS_PER_SEC;
    cout << "DFS on full graph took " << elapsed << " seconds.." << endl;

    // DAG Answer
    map<int,string> dMap;

    dMap[0] = "0";
    dMap[1] = "1";
    dMap[2] = "2";
    dMap[3] = "3";
    dMap[4] = "4";
    dMap[5] = "6";
    dMap[7] = "7";
    dMap[8] = "8";
    dMap[9] = "9";
   
    DAG.setMap( dMap );

    DAG.setEdge( 0 , 7 );
    DAG.setEdge( 0 , 1 );
    DAG.setEdge( 1 , 2 );
    DAG.setEdge( 1 , 3 );
    DAG.setEdge( 3 , 5 );
    DAG.setEdge( 3 , 4 );
    DAG.setEdge( 5 , 9 );
    DAG.setEdge( 5 , 6 );
    DAG.setEdge( 4 , 8 );

    DAG.topoDFS( sorted );
    cout << "DAG sorted: ";
    for( int i = sorted.size() - 1; i >= 0; i--){
        cout << sorted[i] << " ";
    }
    cout << endl;    
   


    // n-word problem #1
    while( !done ){

        nwords = new Graph( n );
        buildNWordGraph( *nwords , true , n );
        ec = nwords->nWordDFS();
        if( ec >= 10 ){
            cout << "DFS tree with " << ec << " edges found with n =  " << n << endl;
            done = true;
        } 
        delete nwords;
        n++;
    }
    
    
    return 0;
}

void buildNWordGraph( Graph& g , bool buildGraph , int n ){
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
    for( int i = 0; i < n; i++ ){
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
