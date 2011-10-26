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
float error( vector<point*> , float , float );

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
    points.pop_back();
    fin.close();
    
    float a = 0,
          b = 0,
          err = 0;    
    bestFit( points , a , b );
    err = error( points , a , b );
    cout << a << " " << b << " error: " << err << endl;

    return 0;
}

float error( vector<point*> pairs , float a , float b ){
    // Find the error of a line L on a set of pairs P
 
    float err = 0, term = 0;
    for( int i = 0; i < pairs.size(); i++ ){
        term = pairs[i]->y  - ( a * pairs[i]->x + b );
        err += term * term;
    }
    return err;
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
