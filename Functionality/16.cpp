//#include <iostream>
//#include <vector>
//using namespace std;

/**Funkce @my_abs pro tvorbu absolutnĂ­ hodnoty vracĂ­ absolutnĂ­ hodnotu z parametru double z)**/
double my_abs(double x) {
    if (x < 0) {
        return -x;
    }
    else {
        return x;
    }
}

/** Funkce @cfrac_tan vypoÄĂ­tĂĄvĂĄ hodnotu profunkce tan(x) kde @x je parametrem funkce a vyjadĹuje Ăşhel. NĂĄvratovĂĄ hodnota funkce je
@f typu double. Parametry jsou @x (Ăşhel) a @n (poÄet iteracĂ­).**/

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

/** Funkce @unknown_itnum je funkce kterĂĄ zjiĹĄĹĽuje vhodnĂ˝ poÄet iteracĂ­ pro vĂ˝poÄtu pod pĹepĂ­naÄem -m tak, aby byla pĹesnost pĹesnÄ na
10 desetinnĂ˝ch mĂ­st. Parametrem je @x coĹž je Ăşhel v radiĂĄnech**/
double unknown_itnumb (double x) {
    double cf = cfrac_tan(x,2);
    for (int m = 2; my_abs(cf - cfrac_tan(x,m-1)) > 1e-10 ; m++) {
        m++;
        cf = cfrac_tan(x,m);
    }
    return cf;
}

int main() {//HlavnĂ­ funkce programu main
    double d;
    double v;
    d = 50/unknown_itnumb(10);
    cout << d;
    d = unknown_itnumb(50) * d + 2;
    cout << d;
    cout << "\n";
    return 0;
}
