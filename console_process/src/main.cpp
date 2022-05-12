#include <iostream> 
#include <vector>


using namespace std;
int main(void) { 
    std::cout << "RChat ";
    while(true){
        string x;
        getline(cin, x);
        cerr << "(Received from main process)" << x << endl;
        cout << x << endl;
    }
    return 1;
}