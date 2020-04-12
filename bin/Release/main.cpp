#include <bits/stdc++.h>
#define TILE_WIDTH 40
#define f first
#define s second

using namespace std;

typedef pair<int,int> pi;


vector< pi > A;

int main()
{
    string inpath = "test.txt"; /// type your metadata path here
    string outpath = "test.out";

    freopen(inpath.c_str(), "r", stdin);
    freopen(outpath.c_str(), "w", stdout);

    int pos_x, pos_y;
    while(cin >> pos_x >> pos_y)
    {
        A.push_back({pos_x, pos_y});
    }

    vector< pi > tiles;

    map<pi,int> M;

    for(int i=1;i<A.size();i++)
    {
        int a = A[i-1].f;
        int b = A[i-1].s;

        int delta_a;
        int delta_b;

        if(A[i-1].f < A[i].f) delta_a = 40;
        else if(A[i-1].f > A[i].f) delta_a = -40;
        else delta_a = 0;

        if(A[i-1].s < A[i].s) delta_b = 40;
        else if(A[i-1].s > A[i].s) delta_b = -40;
        else delta_b = 0;

        while(a != A[i].f || b != A[i].s)
        {
            if(M[{a, b}]) {}
            else
            {
                tiles.push_back({a, b});
                M[{a, b}] = 1;
            }
            a += delta_a;
            b += delta_b;
        }
    }

    cout << tiles.size() << endl;
    for(auto v : tiles) cout << v.f << " " << v.s << endl;
}
