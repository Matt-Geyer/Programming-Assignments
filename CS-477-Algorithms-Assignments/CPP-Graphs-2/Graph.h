// the graph class
// Same as for assignment 1 with some modifications
// like addition of BFS and DFS for finding connected components

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <stack>

using namespace std;

class Graph{

public:
    Graph();
    Graph( int );
    Graph( string );
    ~Graph();
    void clear();
    int getEdge( int, int );
    bool setEdge( int, int );
    int edgeCount( int ); 
    void printAdjMatrix( string );
    void setMap( map<int,string> );
    map<int,string> vMap;
    int connectedComp();
    int nWordDFS();
    void edgeVector( int , vector<int>& );
    void BFS( int , map<int,vector<int> >& );
    void DFS( int , map<int,vector<int> >& );
private:
    int **graph;
    int vertCount;
    map<int,string> vSeen;

};

Graph::~Graph(){
    // Destructor
   
    for( int i = 0; i < vertCount; i++ ){
        delete[] graph[i];
    }
    delete[] graph;
}

int Graph::nWordDFS(){
    // return the number of edges
    // in the largest DFS tree obtained 

    const string SEEN = "SEEN!";
    vSeen = vMap;
    vector<int> children;
    stack<int> dfss;
    int v, edgeCount, maxEdge = -1;  
    map<int , vector<int> > dfst; 
    int edges = 0;

    // for every vertex
    for( int vi = 0; vi < vertCount; vi++ ){
        // if it isn't part of a DFS already computed..
        if( vSeen[vi] != SEEN ){
            dfst.clear();
            // run DFS with that node as a starting point
            DFS( vi , dfst );
            edges = 0;
            // count edges
            for( int i = 0; i < dfst.size(); i++ ){
                edges += dfst[i].size();
            }
            edgeCount = edges - 1;
        }
        if( edgeCount > maxEdge ){
            maxEdge = edgeCount;
        }
    }
 
    return maxEdge;
}

int Graph::connectedComp(){
    // compute the number of connected components in 
    // the graph using BFS
  
    int concomp = 0;
    const string SEEN = "SEEN!";
    vSeen = vMap;
    vector<int> children;
    queue<int> bfsq;
    int v;   
    map< int , vector<int> > bfsTree;

    // for every vertex
    for( int vi = 0; vi < vertCount; vi++ ){
        // if it isn't part of a connected component already computed..
        if( vSeen[vi] != SEEN ){
            
            // this is a new connected component so increment the counter
            concomp++;

            // run BFS with that node as a starting point
            BFS( vi , bfsTree );           
        }
    }
 
    return concomp;
}


void Graph::DFS( int node ,  map<int, vector<int> >& dfstree ){
    // perform BFS on a node and create the BFS tree
    const string SEEN = "SEEN!";
    vector<int> children;
    stack<int> dfss;
    int v;   
    int layer = 0;

    dfss.push( node );
    vSeen[node] = SEEN;
    
    dfstree[layer].push_back( node );
       
    while( !dfss.empty() ){
        v = dfss.top();
        dfss.pop();      
        vSeen[v] = SEEN;

        children.clear();

        // get a list of all the nodes connected to v
        edgeVector( v , children );
        layer++;
        for( int i = 0; i < children.size(); i++ ){
            if( vSeen[children[i]] != SEEN ){
                dfss.push( children[i] );
                dfstree[layer].push_back( children[i] );
                vSeen[children[i]] = SEEN;
            }
        }
    }
    return;
}

void Graph::BFS( int node ,  map<int, vector<int> >& bfstree ){
    // perform BFS on a node and create the BFS tree
    const string SEEN = "SEEN!";
    vector<int> children;
    queue<int> bfsq;
    int v;   
    int layer = 0;

    bfsq.push( node );
    vSeen[node] = SEEN;
    
    bfstree[layer].push_back( node );
       
    while( !bfsq.empty() ){
        v = bfsq.front();
        bfsq.pop();      
        vSeen[v] = SEEN;

        children.clear();
        // get a list of all the nodes connected to v
        edgeVector( v , children );
        layer++;
        for( int i = 0; i < children.size(); i++ ){
            if( vSeen[children[i]] != SEEN ){
                bfsq.push( children[i] );
                bfstree[layer].push_back( children[i] );
                vSeen[children[i]] = SEEN;
            }
        }
    }
    return;
}

void Graph::edgeVector( int vert , vector<int>& v ){
    // return a vector of vertexes reachable by vert
    
    for( int i = 0; i < vertCount; i++ ){
        if( graph[vert][i] == 1 ){
            v.push_back( i );
        }
    }
    return;
}

Graph::Graph(){
    vertCount = 0;
    graph = NULL;
}

Graph::Graph( int vc ){
    vertCount = vc;
    
    graph = new int*[vertCount];
    for( int i = 0; i < vertCount; i++ ){
        graph[i] = new int[vertCount];
    }
    
    clear();
}

Graph::Graph( string fname ){
    // load a graph from file ( specific format )
    ifstream fin( fname.c_str() );
    
    int x = 0;
    string temp;
    
    fin >> vertCount;

    graph = new int*[vertCount];
    for( int i = 0; i < vertCount; i++ ){
        graph[i] = new int[vertCount];
    }

    for( int i = 0; i < vertCount; i++ ){
        for( int j = 0; j < vertCount; j++ ){
            fin >> graph[i][j];
        }
    }

    vMap.clear();
    
    for( int i = 0; i < vertCount; i++ ){
        fin >> x;
        fin >> temp;
        vMap[x] = temp;
    }
    fin.close(); 
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
