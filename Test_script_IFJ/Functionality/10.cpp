//#include <iostream>
//#include <vector>
//using namespace std;

int factorial(int n);

int main() {
    int vysl;

    vysl = factorial(5);
    cout << vysl << "\n";

    return 0;
}

int factorial( int n) {
    int temp_result;
    auto decremented_n = n - 1;
   
    if (n < 2) {
        return 1;
    } 
    else {
        temp_result = factorial(decremented_n);
        return n * temp_result;
    }
}