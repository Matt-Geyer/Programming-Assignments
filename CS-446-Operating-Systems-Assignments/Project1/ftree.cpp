// Family Tree Program
// CS 446 Assignment #1
// Matt Geyer
// 2/22/11 
//
// This program takes a specificly formatted text file
// representing a family tree and then prints that family
// tree using processes 
//

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

char** parseTree( char* );

int main( int argc , char** argv ){

    char temp[10];
    int gen, pid, i, j, nc;
    pid = gen = i = j = nc = 0;
    char p1, p2, cn;
    bool parent = false;

    // Pass the filename to parseTree to get a 2D array representation of the file
    char** cArr = parseTree( argv[1] );
    
    // initialize p1 to the first parent just to get the ball rollin'
    p1 = cArr[0][0];
    
    // the while 1 just ensures that after a fork the loop will be repeated
    // for the new process... it will break out of the while when the time is right.. 
    while( 1 ){
        
        // Print a tab for every generation
        if( p1 != '\0' )
             for(int j = 0; j < gen; j++ ) cout << '\t';
      
        // Determine if p1 is married ( ie a parent )
        int k = 0;
        parent = false;

        // Check for p1 in the first or second column
        // if p1 appears in column 0 or 1 then it is a parent
        while( cArr[k][0] != '\0' ){
           
            // if p1 is in column 0 then p2 is in column 1
            if( cArr[k][0] == p1 ){
                p2 = cArr[k][1];
                parent = true;
                break;
            }
            // else if p1 is in column 1 then p2 is in column 0
            else if( cArr[k][1] == p1 ){
                p2 = cArr[k][0];
                parent = true;
                break;
            }
                
            k++;
        }

        // If p1 is a parent then we need to create a process
        // for all of its children
        if( parent ){

            // convert the number of children from a char to an int
            // and store it in variable nc (num children)
            temp[0] = cArr[k][2];
            sscanf( temp , "%d", &nc );

            // print p1-p2 (ie A-B ).. the generation tabs are already in place
            cout<< p1 <<  "-" << p2 << endl;
          
            // loop for every child.. the +1 is to make sure it hits the terminating char
            for( int l = 0; l < nc + 1; l++ ){
                      
                // as this loops p1 will be set to every child
                // ie 1 = 0 so p1 = cArr[k][l + 3] => cArr[k][0 + 3] which is the first child of p2 and the old p1 etc..
                p1 = cArr[k][l + 3];
                   
                // break for the terminating char.. not sure if this is necessary
                if( p1 == '\0' ) break;
                   
                // since we have a new value for p1 we fork.. we do this for every child
                pid = fork();
         
                // if pid == 0 then we can be sure it is a child node
                // we break out of the for loop so that we don't fork more than we need too
                if( pid == 0 ) break;
                    
                // if we're a parent node then we want to wait for our child to finish their process
                else{
                    // once one child finishes pid is reset to 0 so that we can fork again for the next child
                    waitpid( pid , NULL , 0 );
                    pid = 0;
                }
            }               
        } 
  
        // The letter is not a parent and has no children
        else{
             // So we print it by its lonely self
             if( p1 != '\0'){
                 cout << p1 << endl;
                 // reset generations to 0
                 gen = 0;
             }
             // break out of the while loop... i think.. it also just works..
             break;  
        } 
        // increase generations and our row in the array
        i++;
        gen++;
       
    }

    return 0;
}

char** parseTree( char* file ){
    // Parse the text file and return
    // a 2D array representing the values

    // open the input file
    ifstream fin( file );
    
    // Allocate some memory..probably more than necessary
    char** cArr = new char*[50];
    for( int i = 0; i < 50; i++ )
        cArr[i] = new char[50];

    char temp[10];

    // loop through the file
    int i = 0,
        num_child = 0;
    while( !fin.eof()){

        // read in two parents
        fin >> cArr[i][0];
        fin >> cArr[i][1];
        
        // read in number of children
        fin >> num_child;
  
        // convert and store number of children into array
        sprintf( temp , "%d" , num_child );
        cArr[i][2] = temp[0];

        // loop for number of children and store 
        for( int j = 0; j < num_child; j++ ){
            // j + 3 to account for offset of things already added
            fin >> cArr[i][j + 3];
        }
        // terminate line
        cArr[i][num_child + 3] = '\0';

        // move to next line
        i++;
    }
    
    fin.close(); 
    // terminate tree
    cArr[i - 1][0] = '\0';
  
    return cArr;
}
