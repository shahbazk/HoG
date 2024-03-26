#include "HOG-BASE.h"

void HOG_BASE::print_details(bool verbose){
    int hsz = 0;
    for(bool a:marked)hsz+=a;
    if(verbose){
        std::cout << "HOG Size: " << hsz << "\n";
    }
    else{
        std::cout << "," << hsz;
    }
}
