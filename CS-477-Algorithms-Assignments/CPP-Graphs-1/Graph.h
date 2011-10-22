// Graph class
//
// This is a basic graph class I wrote for the HW
// It is represented by an adjaceny matrix and has
// only the necessary functionality to solve the assignment

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>

using namespace std;

class Graph{

public:
    Graph();
    ~Graph();
    Graph( int );
    Graph( string );
    void clear();
    int getEdge( int, int );
    bool setEdge( int, int );
    int edgeCount( int ); 
    void printAdjMatrix( string );
    void setMap( map<int,string> );
    map<int,string> vMap;
private:
    int **graph;
    int vertCount;

};

Graph::Graph(){
    vertCount = 0;
    graph = NULL;
}

Graph::Graph( int vc ){
    // Construct a graph with the passed number of vertexs
    vertCount = vc;
    
    // Allocate new memory for the adjaceny matrix
    graph = new int*[vertCount];
    for( int i = 0; i < vertCount; i++ ){
        graph[i] = new int[vertCount];
    }
    // Start the graph with all 0s
    clear();
}

Graph::Graph( string fname ){
    // load a graph from file ( specific format )
    ifstream fin( fname.c_str() );
    
    int x = 0;
    string temp;
    
    // Read the vertice count.. first thing in the file
    fin >> vertCount;

    // Allocate memory
    graph = new int*[vertCount];
    for( int i = 0; i < vertCount; i++ ){
        graph[i] = new int[vertCount];
    }

    // Read the adjaceny matrix 
    for( int i = 0; i < vertCount; i++ ){
        for( int j = 0; j < vertCount; j++ ){
            fin >> graph[i][j];
        }
    }

    vMap.clear();
    // Read in the map that correlates an id in the adjacency matrix to a string
    for( int i = 0; i < vertCount; i++ ){
        fin >> x;
        fin >> temp;
        vMap[x] = temp;
    }
    fin.close(); 
} 

Graph::~Graph(){
    // Destructor
    
    for( int i = 0; i < vertCount; i++ ){
        delete[] graph[i];
    }
    
    delete[] graph;
}

void Graph::setMap( map<int,string> m ){
    // set the map that associates a row/col in the adjacency
    // matrix with a string name or representation
    vMap = m;
    return;
}
    

void Graph::clear(){
    // set the adjaceny matrix to all 0's
    
    for( int i = 0; i < vertCount; i++ ){
        for( int j = 0; j < vertCount; j++ ){
            graph[i][j] = 0;
        }
    }
    return;
}

bool Graph::setEdge( int v1, int v2 ){
    // set an edge in the adjaceny matrix for two vertices to 1 
    
    // check bounds 0 <= vertice <= # of vertices
    if( v1 >= vertCount || v1 < 0 || v2 >= vertCount || v2 < 0 ){
        return false;
    }
    // if everything checks out, indicate an edge between the two nodes
    else{
        graph[v1][v2] = 1;
        graph[v2][v1] = 1;
    }
    return true;
}

int Graph::getEdge( int v1 , int v2 ){
    // determine if there is an edge between two vertices
   
    if( v1 >= vertCount || v1 < 0 || v2 >= vertCount || v2 < 0 ){
        return 0;
    }
    else{
        return graph[v1][v2];
    }
}

int Graph::edgeCount( int v ){
    // count the number of edges a vertice has
    int count = 0;

    for( int i = 0; i < vertCount; i++ ){
        count += graph[v][i];
    }
    return count;
}

void Graph::printAdjMatrix( string fname ){
    // print the adjacency matrix to a file
    ofstream fout(fname.c_str());

    fout << vertCount << endl;
    
    for( int i = 0; i < vertCount; i++ ){
        for( int j = 0; j < vertCount; j++ ){
            fout << graph[i][j] << " ";
        }
        fout << endl;
    }

    // print the keymap that names the nodes
    for( int i = 0; i < vertCount; i++ ){
        fout << i << " " << vMap[i] << endl;
    }
    
    fout.close();
}
