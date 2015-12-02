//#include <iostream>
//#include <vector>
//using namespace std;

double cfrac_tan(double x, int n) {
    double f = 0.0;
    double ak = -x * x;
    double bk = 1;
    int i;
    for (i = n; i > 0; i--) {
        if (i == 1) {
            ak = x;
        }
        else {}
        bk = (i * 2) -1;
        f = ak/(bk + f);
    }
    return f;
}

int main(){
        double r = cfrac_tan(25.2, 3);
        cout << r;
        cout << "\n";
        return 0;
}