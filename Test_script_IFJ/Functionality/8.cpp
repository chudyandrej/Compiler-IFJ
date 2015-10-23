//#include <iostream>
//#include <vector>
//using namespace std;


int main(){

    for(int i; i < 100; i++){
        if(++i%10){
            cout << "++i " << i << " \n";
        }
        else{ }

        if(i++%10){
            cout << "i++ " << i << " \n";
        }
        else { }

    }

    
    return 0;
}
