//#include <iostream>
//#include <vector>
//using namespace std;


int main(){

    for(int i=0; i < 100; i++){
        if(++i/10 >=6){
            cout << "++i " << i << " \n";
        }
        else{ }

        if(i++/10 <4){
            cout << "i++ " << i << " \n";
        }
        else { }

    }

    
    return 0;
}
