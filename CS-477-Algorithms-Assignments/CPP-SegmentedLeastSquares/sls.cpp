// Matt Geyer
// CS 477 Algorithms HW 5
// Segmented Least Squares
//
// Do segmented least squares using dynamic programming

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class point{
  public:
    float x,y;
    point( float , float );
};

point::point( float cx , float cy ){ 
    x = cx;
    y = cy;
}

void bestFit( vector<point*> , float& , float& );

int main( int argc , char** argv ){

    string fileName = "one-line.dat";
    if( argc > 1 ){
        fileName = argv[1];
    }
  
    // Went with pointer to prevent destructor
    // being called on my points if incase I need to remove them!
    vector<point*> points;  
  
    // Read in the initial points
    ifstream fin(fileName.c_str());
    float x,y;
    point* tp;
    while( !fin.eof() ){
        fin >> x >> y;
        tp = new point( x , y );
        points.push_back( tp );
    }
    fin.close();
    
    float aa = 0, bb = 0;           
    bestFit( points , aa , bb );
    cout << aa << " " << bb << endl;

    return 0;
}

void bestFit( vector<point*> pairs , float& a , float& b ){
   // Find the line of best fit on a set of points P

   int n = pairs.size();

   // Breaking the equation into parts
   float s1 = 0;
   for( int i = 0; i < n; i++ ){
       s1 += pairs[i]->x * pairs[i]->y;
   }
   s1 *= n;
    
   float sumx = 0,
         sumy = 0,
         s2 = 0;
   for( int i = 0; i < n; i++ ){
       sumx += pairs[i]->x;
       sumy += pairs[i]->y;
   }
   s2 = sumx * sumy;

   float s3 = 0;
   for( int i = 0; i < n; i++ ){
       s3 += pairs[i]->x * pairs[i]->x;
   }
   s3 *= n;

   // Solve for a
   a = ( s1 - s2 )/( s3 - ( sumx * sumx) );
   // Solve for b
   b = ( sumy - ( a * sumx ) ) / n; 
    
   return;
}
