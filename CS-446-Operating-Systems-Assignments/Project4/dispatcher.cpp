#include <iostream>
#include <string.h>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <deque>

using namespace std;

struct job{
    int priority,
        arrival,
        runtime,
        ticksActive,
        firstTick,
        lastTick,
        turnAround;
};

void readJobs( string , vector<job>& );
bool allEmpty( deque<job>& , deque<job>& , deque<job>& , deque<job>& );
void printJob( job& );

int main( int argc , char** argv ){

    vector<job> jobs, completed;
    deque<job> RPQ , PQ1 , PQ2 , PQ3;
    job ajob;
    bool activeRPQ = false;

    // read in the dispatcher text file
    if( argv[1] ) readJobs( argv[1] , jobs );

    // calculate latest arrival
    int maxArrival = jobs[0].arrival;
    for( int i = 1; i < jobs.size(); i++ ){
        if( jobs[i].arrival > maxArrival ){
            maxArrival = jobs[i].arrival;
        }
    }

    // Exit condition if all the queues are empty and every 
    // process has arrived 
    int ticks = -1;
    while( !allEmpty( RPQ , PQ1 , PQ2 , PQ3 ) || ticks <= maxArrival ){
        ticks++;
       
        // check job list for jobs that have arrived
        for( int i = 0; i < jobs.size(); i++ ){
            // If a job has arrived, place it into the appropriate queue
            if( jobs[i].arrival == ticks ){
                switch( jobs[i].priority ){
                    case 0:
                        RPQ.push_back( jobs[i] );
                        break;
                    case 1:
                        PQ1.push_back( jobs[i] );
                        break;
                    case 2:
                        PQ2.push_back( jobs[i] );
                        break;
                    case 3:
                        PQ3.push_back( jobs[i] );
                        break;
                    default:
                        break;
                }
            }
        }

        // a RPQ process runs to completion 
        if( activeRPQ ){

            // increase the ticksActive 
            ajob.ticksActive++;

            // check to see if it can terminate
            if( ajob.ticksActive == ajob.runtime ){

                // allow dispatcher to continue
                activeRPQ = false;
                ajob.lastTick = ticks;

                // push to the completed stack so we can do calculations
                completed.push_back( ajob );
            }
        }
        // check to see if the RPQ has any jobs
        else if( !RPQ.empty() ){

            // make this the active process
            ajob = RPQ.front(); 
            RPQ.pop_front();

            // if this is the first time the job is being run
            // then record the time to calculate turnaround
            if( ajob.ticksActive == 0 ){
                ajob.firstTick = ticks;
            }

            // increment the counter for # of ticks as active job
            ajob.ticksActive++;

            if( ajob.ticksActive != ajob.runtime ){
                // allow the real-time priority process 
                // to run to completion
                activeRPQ = true;
            }
            else{
                // this is necessary for processes that have a run-time of 1
                activeRPQ = false;
                ajob.lastTick = ticks;
                completed.push_back( ajob );
            }            
        }
        // else take care of the user-level queues
        // starting with PQ1
        else if( !PQ1.empty() ){

            // grab active process
            ajob = PQ1.front();
            PQ1.pop_front();

            if( ajob.ticksActive == 0 ){
                ajob.firstTick = ticks;
            }

            ajob.ticksActive++;

            // if it isn't completed place it into the next queue down
            if( ajob.ticksActive != ajob.runtime ){
                 PQ2.push_back( ajob );
            }
            else{
                ajob.lastTick = ticks;
                completed.push_back( ajob );
            }
        }
        // PQ2 is next
        else if( !PQ2.empty() ){
            
            ajob = PQ2.front();
            PQ2.pop_front();
            
            if( ajob.ticksActive == 0 ){
                ajob.firstTick = ticks;
            }

            ajob.ticksActive++;

            if( ajob.ticksActive != ajob.runtime ){
                PQ3.push_back( ajob );
            }
            else{
                ajob.lastTick = ticks;
                completed.push_back( ajob );
            }
        }
        // finally PQ3 
        else if( !PQ3.empty() ){
            
            ajob = PQ3.front();
            PQ3.pop_front();
           
            if( ajob.ticksActive == 0 ){
                ajob.firstTick = ticks;
            }

            ajob.ticksActive++;

            // this time we push back onto PQ3 instead of
            // going down a level
            if( ajob.ticksActive != ajob.runtime ){
                PQ3.push_back( ajob );
            }
            else{
                ajob.lastTick = ticks;
                completed.push_back( ajob );
            }
        }

    }
 
    // Compute various stastitics and display
    float avg_ta = 0.0;
    float avg_wait = 0.0;
    float ta = 0.0;
    float n_avg = 0.0;
    for( int i = 0; i < completed.size(); i++ ){
        ta = completed[i].lastTick - completed[i].arrival + 1;
        avg_ta += ta;
        avg_wait += ta - completed[i].ticksActive;
        n_avg += ta / completed[i].runtime;
    }
    avg_ta /= completed.size();
    avg_wait /= completed.size();
    n_avg /= completed.size();
 
    cout << "Average turn-around: " << avg_ta << endl;
    cout << "Average normalized turn-around: " << n_avg << endl;
    cout << "Average wait time: " << avg_wait << endl;


    return 0;
}

void printJob( job& ajob ){
        // print contents of job
        cout << "Job priority: " << ajob.priority << endl;
        cout << "Job Arrival: " << ajob.arrival << endl;
        cout << "Job Runtime: " << ajob.runtime << endl;
        cout << "ticksActive: " << ajob.ticksActive << endl;
        cout << "startTick: " << ajob.firstTick << endl;
        cout << "lastTick: " << ajob.lastTick << endl;
        cout << "turnAround: " << ajob.turnAround << endl;
        cout << "waitTime: " << ajob.turnAround << "-" <<  ajob.ticksActive << " = ";
        cout << ajob.turnAround - ajob.ticksActive << endl;
        return;
}

bool allEmpty( deque<job>& a, deque<job>& b, deque<job>& c , deque<job>& d){
    // check if all the queues are empty
    return( a.empty() && b.empty() && c.empty() && d.empty() );
}

void readJobs( string fileName , vector<job>& jobList ){
    // read and parse the dispatcher.txt file
    // populate a STL vector with the jobs

    struct job *jobptr;
    ifstream fin;
    fin.open( fileName.c_str() );
    char cptr[256],
         *tok;
    string temp;
    size_t found;
    int count;

    // loop through the .txt file
    while( !fin.eof() ){

        fin.getline( cptr , 256 );
        temp = cptr;

        // little hack to prevent extra looping
        if( temp.empty() ) break;
       
        // split the string with , and space as a delimmiter 
        tok = strtok( cptr , ", " );
        
        // instantiate new job to add to the list
        jobptr = new job;
        jobptr->ticksActive = 0;

        // loop through and populate job
        count = 0;
        while( tok != NULL ){
            temp = tok;
            // convert to int and set 
            if( count == 0 ){
                jobptr->arrival = atoi( tok );
            }
            else if( count == 1 ){
                jobptr->priority = atoi( tok );
            }
            else if( count == 2 ){
                jobptr->runtime = atoi( tok );
            }

            count++;
            tok = strtok( NULL , ", " );
        } 
        // add the job to the list
        jobList.push_back( *jobptr );
    }        

    fin.close();
    return;
}  
