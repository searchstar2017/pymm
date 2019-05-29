#include <iostream>

#include "MyPyMinusMinus.h"

using namespace std;

int main()
{
    PYMM pymm;
    string information;
    string in;

    #if DEBUG
    in = "1+2";
    pymm.Input(in.c_str(), information);
    cout << information << endl;
    #else
    while (cin >> in) {
        pymm.Input(in.c_str(), information);
        cout << information << endl;
        information.clear();
    }
    #endif // DEBUG

    return 0;
}
