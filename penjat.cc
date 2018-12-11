#include <stdlib.h>
#include <time.h>
#include <iostream>

using namespace std;

 
int random(int min, int max) {   
    srand(time(NULL));
    return rand() % (max - min) + min;
}

int main() { }
