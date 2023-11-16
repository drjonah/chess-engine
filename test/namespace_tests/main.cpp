#include <iostream>
#include "file1.h"
using namespace std;

int main() {

    file::display();
    cout << file::data << endl;

    return 0;
}