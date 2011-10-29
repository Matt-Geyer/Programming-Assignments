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
    void print();
};

point::point( float cx , float cy ){ 
    x = cx;
    y = cy;
}
void point::print(){ 
    cout << "( " << x << " , " << y << " )" << endl; 
    return;
}

void bestFit( vector<point*>& , float& , float& );
float error( vector<point*>& , float , float );
void subVec( vector<point*>& , vector<point*>& , int , int );
float segSquares( vector<point*>& , int );
void findSegs( int );

float* M;
float** eij;
vector<point*> points;

int main( int argc , char** argv ){

    string fileName = "one-line.dat";
    if( argc > 1 ){
        fileName = argv[1];
    }
  
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
    cout << points.size() << endl;
    segSquares( points , points.size() );
    findSegs( points.size() );

    for( int i = 0; i < points.size(); i++ ){
        //cout << M[i] << endl;
    }
    return 0;
}

void findSegs( int j ){
    float C = 1.0, sum = 0.0, min = 100000.0, min_i = 0.0;
    if( j == 0 ) return;
  
    else{
        for( int i = 1; i < j; i++ ){
            sum = eij[i][j] + C + M[i-1];
            //cout << sum << " " << min << endl;
            if( sum < min ){
                min = sum;
                min_i = i;
            }
        }
        cout << "Calling findSegs with " << min_i << endl;
        findSegs( min_i - 1 );
        for( int i = min_i; i < j; i++ ){
            points[i]->print();
        }
    }
}
            
        

float segSquares( vector<point*>& points , int n ){
    // do segmented least squares
    M = new float[n];
    M[0] = 0;

    float a = 0,
          b = 0,
          err = 0;    

    vector<point*> sub;

    // Allocate memory for the eij array and initlaize values to 0
    eij = new float*[points.size()];
    for( int i = 0; i < points.size(); i++ ){
        eij[i] = new float[points.size()];
    }
    for( int i = 0; i < points.size(); i++ ){
        for( int j = 0; j < points.size(); j++ ){
            eij[i][j] = 0;
        }
    }

    // compute eij for all pairs i < j
    for( int j = 0; j < points.size(); j++ ){
        for( int i = 0; i < j; i++ ){
            sub.clear();
            subVec( points , sub , i , j );
            bestFit( sub , a , b );
            eij[i][j] = error( sub , a , b );
            //cout << i << " " << j << " " << eij[i][j] << endl;
        }
    }
    
    float C = 1.0, minErr = 99999, min_i = 0, temperr = 0;
    for( int j = 1; j < n; j++ ){
        for( int i = 1; i <= j; i++ ){
            temperr = eij[i][j] + C + M[i-1];
            if( temperr < minErr ){
                minErr = temperr;
                min_i = i;
            }
        }
        //cout << minErr << endl;
        M[j] = minErr;
        minErr = 99999;
    }
    
    return M[n - 1];
}

void subVec( vector<point*>& vec, vector<point*>& subVec, int i , int j ){
    // quick google didn't turn up a quick way to do this.
    
    for( int s = i; s <= j; s++ ){
        subVec.push_back( vec[s] );
    }
    return;
}

float error( vector<point*>& pairs , float a , float b ){
    // Find the error of a line L on a set of pairs P
 
    float err = 0, term = 0;
    for( int i = 0; i < pairs.size(); i++ ){
        term = pairs[i]->y  - ( a * pairs[i]->x + b );
        err += term * term;
    }
    return err;
}

void bestFit( vector<point*>& pairs , float& a , float& b ){
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
