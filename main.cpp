#include <iostream>
#include "StableTimedFifo.h"

enum class BranchType{
    TypeA = 0,
    TypeB = 1
};

template<BranchType type>
int TemplatedBranchSelection();

template<>
int TemplatedBranchSelection<BranchType::TypeA>() {

}

template<>
int TemplatedBranchSelection<BranchType::TypeB>() {

}

int main()
{
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
