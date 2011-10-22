// the dirGraph class..
// This is just a barely modified version of 
// My regular graph class.. the only difference
// Is how things get put into the adjaceny matrix

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <list>
#include <map>
#include <queue>
#include <stack>

using namespace std;

class dirGraph{

public:
    dirGraph();
    dirGraph( int );
    dirGraph( string );
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
    void topoDFS( vector<int>& );
    int inDeg( int );
    void visit( int );
private:
    int **graph;
    int vertCount;
    vector<int> topoSort;

};

void dirGraph::visit( int v ){
    // for DFS topological sort

    vector<int> edgeNodes;
    if( vMap[v] != "v" ){
        vMap[v] = "v";
        edgeNodes.clear();
        edgeVector( v , edgeNodes );
        for( int i = 0; i < edgeNodes.size(); i++ ){
            visit( edgeNodes[i] );
        }
        topoSort.push_back( v );
   }
   return;
}
        

void dirGraph::topoDFS( vector<int>& vec ){
    // do topological sorting via DFS
    
    topoSort.clear();
    vector<int> S;

    for( int i = 0; i < vertCount; i++ ){
        if( inDeg( i ) == 0 ){
            S.push_back( i );
        }
    }
 
    for( int i = 0; i < S.size(); i++ ){
        visit( S[i] );
    }
       
    vec = topoSort;
    return;
}    

int dirGraph::inDeg( int v ){
    // determine the number of incoming edges for vertex v
 
    int deg = 0;
  
    for( int i = 0; i < vertCount; i++ ){
        if( i != v && graph[i][v] == 1 ){
            deg++;
        }
    }

    return deg;
}

int dirGraph::nWordDFS(){
    // return the number of edges
    // in the largest DFS tree obtained 

    const string SEEN = "SEEN!";
    map<int,string> vSeen = vMap;
    vector<int> children;
    stack<int> dfss;
    int v, edgeCount, maxEdge = -1;   

    // for every vertex
    for( int vi = 0; vi < vertCount; vi++ ){
        // if it isn't part of a DFS already computed..
        if( vSeen[vi] != SEEN ){

            // run DFS with that node as a starting point
            dfss.push( vi );
            vSeen[vi] = SEEN;
            edgeCount = 0;
           
            while( !dfss.empty() ){
                v = dfss.top();
                dfss.pop();      
                vSeen[v] = SEEN;

                children.clear();
                // get a list of all the nodes connected to v
                edgeVector( v , children );
                for( int i = 0; i < children.size(); i++ ){
                    if( vSeen[children[i]] != SEEN ){
                        dfss.push( children[i] );
                        vSeen[children[i]] = SEEN;
                        edgeCount++;
                    }
                }
            }
            
        }
        if( edgeCount > maxEdge ){
            maxEdge = edgeCount;
        }
    }
 
    return maxEdge;
}

int dirGraph::connectedComp(){
    // compute the number of connected components in 
    // the graph using BFS

    int concomp = 0;
    const string SEEN = "SEEN!";
    map<int,string> vSeen = vMap;
    vector<int> children;
    queue<int> bfsq;
    int v;   

    // for every vertex
    for( int vi = 0; vi < vertCount; vi++ ){
        // if it isn't part of a connected component already computed..
        if( vSeen[vi] != SEEN ){
            
            // this is a new connected component so increment the counter
            concomp++;

            // run BFS with that node as a starting point
            bfsq.push( vi );
            vSeen[vi] = SEEN;
           
            while( !bfsq.empty() ){
                v = bfsq.front();
                bfsq.pop();      
                vSeen[v] = SEEN;

                children.clear();
                // get a list of all the nodes connected to v
                edgeVector( v , children );
                for( int i = 0; i < children.size(); i++ ){
                    if( vSeen[children[i]] != SEEN ){
                        bfsq.push( children[i] );
                        vSeen[children[i]] = SEEN;
                    }
                }
            }
            
        }
    }
 
    return concomp;
}

void dirGraph::edgeVector( int vert , vector<int>& v ){
    // return a vector of vertexes reachable by vert
    
    for( int i = 0; i < vertCount; i++ ){
        if( graph[vert][i] == 1 ){
            v.push_back( i );
        }
    }
    return;
}

dirGraph::dirGraph(){
    vertCount = 0;
    graph = NULL;
}

dirGraph::dirGraph( int vc ){
    vertCount = vc;
    
    graph = new int*[vertCount];
    for( int i = 0; i < vertCount; i++ ){
        graph[i] = new int[vertCount];
    }
    
    clear();
}

dirGraph::dirGraph( string fname ){
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

void dirGraph::setMap( map<int,string> m ){
    // set the map that associates a row/col in the adjacency
    // matrix with a string name or representation
    vMap = m;
    return;
}
    

void dirGraph::clear(){
    // set the adjaceny matrix to all 0's
    
    for( int i = 0; i < vertCount; i++ ){
        for( int j = 0; j < vertCount; j++ ){
            graph[i][j] = 0;
        }
    }
    return;
}

bool dirGraph::setEdge( int v1, int v2 ){
    // set an edge in the adjaceny matrix for two vertices to 1 
    
    // check bounds 0 <= vertice <= # of vertices
    if( v1 >= vertCount || v1 < 0 || v2 >= vertCount || v2 < 0 ){
        return false;
    }
    // if everything checks out, indicate an edge between the two nodes
    else{
        graph[v1][v2] = 1;
    }
    return true;
}

int dirGraph::getEdge( int v1 , int v2 ){
    // determine if there is an edge between two vertices
   
    if( v1 >= vertCount || v1 < 0 || v2 >= vertCount || v2 < 0 ){
        return 0;
    }
    else{
        return graph[v1][v2];
    }
}

int dirGraph::edgeCount( int v ){
    // count the number of edges a vertice has
    int count = 0;

    for( int i = 0; i < vertCount; i++ ){
        count += graph[v][i];
    }
    return count;
}

void dirGraph::printAdjMatrix( string fname ){
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
