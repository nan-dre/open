#include <fstream>
#include <map>
#include <iostream>

using namespace std;
ifstream in("rom.txt");
ofstream out("rom.out");
map<string,int> mp;
int main()
{
    string s;
    int a,b,c,d;
    mp["mov"]=0;
    mp["draw"]=1;
    mp["clear"] = 2;
    mp["goto"] = 3;
    mp["exit"] = 4;
    mp["+"] = 5;
    mp["-"] = 6;
    mp["*"] = 7;
    mp["/"] = 8;
    mp["%"] = 9;
    mp["<="] = 10;
    mp["<"] = 11;
    mp["=="] = 12;
    mp[">="] = 13;
    mp[">"] = 14;
    mp["!"] = 15;
    while(in>>s)
    {
        in>>a>>b>>c>>d;
        cout<<((mp[s] << 2) + a)<<" "<<((b << 6)  + (c >> 1))<<" "<<(((c & 1) << 7) + d)<<"\n";
        out<<char((mp[s] << 2) + a)<<char((b << 6)  + (c >> 1))<<char(((c & 1) << 7) + d);
    }
    return 0;
}