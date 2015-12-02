//#include <iostream>
//#include <vector>
//using namespace std;

int foo(int i, int max){
        if (i < max){
                return  foo(i+1, max) * i;
        }
        else {
                cout << i << "||" << max << "\n";
                return  i;
        cout << "NEVER!!!";
        }
        cout << "NO CHANCE!!";
}

int main(){
        cout << foo(1, 5) << "\n";
        return 0;
}