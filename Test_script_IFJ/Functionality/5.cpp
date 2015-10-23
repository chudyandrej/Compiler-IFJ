//#include <iostream>
//#include <vector>
//using namespace std;


double division(int x, int y){
	return x/y;
}

string func(){
	return "ahoj\n";
}

double anotherFunc(double x){
	return x*2;
}

string myPrint(string x){
	cout<<x<<'\n';
	return "good";
}

int main(){
	
	double a;
	int x=12;
	int y=5;
	a=division(x,y);	
	cout << func();
	cout << a << "\n" << func();
	a=anotherFunc(a);
	myPrint("bla bla print \n it jfdsa");
	cout << a << "\n";
	string _variable="text textj;sdkfj\n";
	cout << myPrint(_variable) << '\n';

	return 0;
}
