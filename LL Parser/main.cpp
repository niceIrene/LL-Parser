#include "base.h"
#include<iomanip>
using namespace std;

int main()
{
    string fileName="in.txt";
    base test(fileName);
    cout<<endl;
    test.makeTable();
        return 0;
}
