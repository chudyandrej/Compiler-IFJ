//#include <iostream>
//#include <vector>
//using namespace std;


int main(){

    string str="nejaky text bal";
    cout << length(str) << "\n";
    
    cout << length("escape\n") << "\n";
    
    cout << length("\\\nksdjf") << "\n";
    
    str="%%\n\\\n";
    cout << length(str) << "\n";
    
    str="nejaky text balsdsdsdsdsalkdsalk ldsfj;ads jf saf\n dslkfj \t dfads;fs;sdfdfjsfdssab;dcn[o'a54+9863269866436514dsflkjdsa;fsajkdsa;lksajlkfdsaj;lk64764564";
    cout << length(str) << "\n";


    cout << sort("abcdefghijklmnoprstuvxyz") << "\n";

    str=sort("cbda");
    cout<< str << "\n";

    str="zyxvutsrponmlkjihgfedcba";
    str=sort(str);
    cout << str <<"\n";


    cout << find("aaaabcd", "abcd") << "\n";
    cout << find(str,"www") << "\n";
    str="dac";
    cout << find( "abcdac",str) << "\n";
    string str1;
    str1="ahoj";
    str="aho4dsajahoj";
    cout << find(str, str1) << "\n";


    cout << concat(str1,str) << "\n";

    str=concat("ddbbcc","alalal");
    cout << str << "\n";


    cout << substr("ahojblabla", 4, 3) << "\n";
    cout << substr("ahojblabla", 0, 0) << "\n";
    cout << substr("ahojblabla", 5, 50) << "\n";
    cout << substr("ahojblabla", 0, 1) << "\n";


    return 0;
}
