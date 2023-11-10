#include "timer.h"

using namespace std;

timer::timer() {
    start_time = chrono::system_clock::now();
}

timer::~timer() {
    if(!completed) end();
}

double timer::end() {
    auto end_time = chrono::system_clock::now();
    chrono::duration<double> elapsed_time = end_time - start_time;
    cout<<"Elapsed time: " << elapsed_time.count() << "s\n";
    completed = true;
    return elapsed_time.count();
}
