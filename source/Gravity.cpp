#include <iostream>
#include <typeinfo>

#include "../classes/linalg/linalg.h"

using namespace std;


class Object {
protected:

    vec<double, 3> pos;
    vec<double, 3> vel;

    double mass;

    double time_delta;

public:

    Object() {
        pos = { 0,0,0 };
    }

};


int main() {
    
    

}
