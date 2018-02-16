// C++11
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class PrincessesAndMonsters {
public:
    string dir = "NEWS";
    int t;
    // ----------------------------------------------
    string initialize(int S, vector<int> princesses, vector<int> monsters, int K) {
        srand(1234);
        t = -1;
        // all knights start in top left corner
        return string(K, '0');
    }
    // ----------------------------------------------
    string move(vector<int> status, int P, int M, int timeLeft) {
        t++;
        return string(status.size(), t < 3 ? 'E' : t < 8 ? 'S' : dir[rand() % 4]);
    }
};
// -------8<------- end of solution submitted to the website -------8<-------

template<class T> void getVector(vector<T>& v) {
    for (int i = 0; i < v.size(); ++i)
        cin >> v[i];
}

int main() {
    PrincessesAndMonsters pam;

    int S, P, M, K;
    cin >> S >> P;
    vector<int> princesses(P);
    getVector(princesses);
    cin >> M;
    vector<int> monsters(M);
    getVector(monsters);
    cin >> K;
    
    string retInit = pam.initialize(S, princesses, monsters, K);
    cout << retInit << endl;
    cout.flush();

    for (int st = 0; ; ++st) {
        int nK;
        cin >> nK;
        vector<int> status(K);
        getVector(status);
        int nP, nM;
        cin >> nP >> nM;
        int timeLeft;
        cin >> timeLeft;

        string ret = pam.move(status, nP, nM, timeLeft);
        cout << ret << endl;
        cout.flush();
    }
}
