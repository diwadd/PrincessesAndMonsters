#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cassert>

using namespace std;


// --------------------------------------------
// -----------------  Knight  -----------------
// --------------------------------------------


class Knight {
    // Experimental class, all public
    public:
        int f_id;
        int f_x;
        int f_y;
        int f_n_p; // number of princesses escorted by knight
        int f_g; // the knight belong to group f_g

        Knight(int idp = -1, int xp = -1, int yp = -1, int np = 0, int gp = -1): f_id(idp), f_x(xp), f_y(yp), f_n_p(np), f_g(gp) {}
};


ostream & operator<<(ostream & os, const Knight &k){
    os << "KnightObject - Description: id: " << k.f_id << " at (" << k.f_x << "," << k.f_y << ")  in state: " << k.f_n_p;
    return os;
}


// --------------------------------------------
// -------------  Knight Group ----------------
// --------------------------------------------


class KnightGroup {
    public:
        vector<Knight*> f_knights_group;
        int f_number_of_escorted_princesses;

    KnightGroup() {}
    KnightGroup(int &n);

    void update_number_of_escorted_princesses();

};


KnightGroup::KnightGroup(int &n) {
    f_knights_group.resize(n, nullptr);
    f_number_of_escorted_princesses = 0;
}


void KnightGroup::update_number_of_escorted_princesses() {

    f_number_of_escorted_princesses = 0;
    int n = f_knights_group.size();
    for(int i = 0; i < n; i++)
        f_number_of_escorted_princesses += f_knights_group[i]->f_n_p;
}


// --------------------------------------------
// -------  UncontrolledProtagonist  ----------
// --------------------------------------------


class UncontrolledProtagonist {
    public:
        // The princes was lass seen on (last_x, last_y)
        int f_last_x;
        int f_last_y;

        UncontrolledProtagonist() {}
        UncontrolledProtagonist(int xp, int yp): f_last_x(xp), f_last_y(yp) {}

};


// --------------------------------------------
// ------------  Princess  --------------------
// --------------------------------------------


class Princess : public UncontrolledProtagonist {
    public:
        Princess(): UncontrolledProtagonist() {}
        Princess(int xp, int yp): UncontrolledProtagonist(xp, yp) {}
};


ostream & operator<<(ostream & os, const Princess &p){
    os << "PrincessObject - Description: last seen at (" << p.f_last_x << "," << p.f_last_y << ")";
    return os;
}


// --------------------------------------------
// -------------  Monster  --------------------
// --------------------------------------------


class Monster : public UncontrolledProtagonist {
    public:
        Monster(): UncontrolledProtagonist() {}
        Monster(int xp, int yp): UncontrolledProtagonist(xp, yp) {}
};


ostream & operator<<(ostream & os, const Monster &p){
    os << "MonsterObject - Description: last seen at (" << p.f_last_x << "," << p.f_last_y << ")";
    return os;
}


// --------------------------------------------
// ------------  GameState  -------------------
// --------------------------------------------


class GameState {
    public:
        int f_S; // board size
        int f_turn; // current turn
        int f_n_knights; // number of knights
        int f_n_princesses; // number of princesses
        int f_n_monsters; // number of monsters
        vector<Knight> f_knights;
        vector<Princess> f_princesses;
        vector<Monster> f_monsters;

        vector<KnightGroup> f_knight_group_collection;


        GameState() {}

        void set_S(int &S) {f_S = S;}

        void set_princesses(vector<int> &pr);
        void print_princesses();

        void set_monsters(vector<int> &mo);
        void print_monsters();

        void set_knights(int &k);
        void print_knights();


        pair<int, int> _determine_the_number_of_groups();
        void make_groups();
        void print_groups();

};



void GameState::set_princesses(vector<int> &pr) {

    int n = pr.size();
    f_n_princesses = n/2;
    f_princesses.resize( n/2 );
    for(int i = 0; i < n/2; i++) {
        f_princesses[i].f_last_x = pr[2*i];
        f_princesses[i].f_last_y = pr[2*i + 1];
    }
}


void GameState::set_monsters(vector<int> &mo) {

    int n = mo.size();
    f_n_monsters = n/2;
    f_monsters.resize( n/2 );
    for(int i = 0; i < n/2; i++) {
        f_monsters[i].f_last_x = mo[2*i];
        f_monsters[i].f_last_y = mo[2*i + 1];
    }
}


void GameState::print_princesses() {
    for(int i = 0; i < f_princesses.size(); i++)
        cerr << "Princess id: " << i << ": " << f_princesses[i] << endl;
}


void GameState::print_monsters() {
    for(int i = 0; i < f_monsters.size(); i++)
        cerr << "Monster id: " << i << ": " << f_monsters[i] << endl;
}


void GameState::set_knights(int &k) {
    f_n_knights = k;
    f_knights.resize(k);
    for(int i = 0; i < k; i++)
        f_knights[i].f_id = i;

}

void GameState::print_knights() {
    for(int i = 0; i < f_knights.size(); i++)
        cerr << "Knight id: " << i << ": " << f_knights[i] << endl;
}

pair<int, int> GameState::_determine_the_number_of_groups() {

    // monster_p - probability of meeting a monster
    //double monster_p = f_n_monsters / (f_S*f_S);

    int min_number_of_knights_in_group = 3;
    int number_of_groups = int(f_n_knights/min_number_of_knights_in_group);

    fprintf(stderr, "min_number_of_knights_in_group: %d\n", min_number_of_knights_in_group);
    fprintf(stderr, "number_of_groups: %d\n", number_of_groups);

    return make_pair(min_number_of_knights_in_group, number_of_groups);
}

void GameState::make_groups() {

    pair<int, int> group_parameters = _determine_the_number_of_groups();

    int min_number_of_knights_in_group = group_parameters.first;
    int number_of_groups = group_parameters.second;


    int q = min_number_of_knights_in_group*number_of_groups;
    fprintf(stderr, "q: %d\n", q);
    fprintf(stderr, "f_n_knights: %d\n", f_n_knights);
    if (q == f_n_knights) {
        f_knight_group_collection.resize(number_of_groups);
    } else if (q < f_n_knights) {
        number_of_groups = number_of_groups + 1;
        f_knight_group_collection.resize(number_of_groups);
    } else {
        fprintf(stderr, "Cannot have more groups than knights!\n");
        assert(false);
    }


    int knight_index = 0;
    for(int i = 0; i < number_of_groups; i++) {

        for(int j = 0; j < min_number_of_knights_in_group; j++) {

            Knight *kp = &f_knights[knight_index];
            f_knight_group_collection[i].f_knights_group.push_back(kp);

            knight_index++;
            if (knight_index == f_n_knights)
                break;
        }
    }
}

void GameState::print_groups() {

    int n = f_knight_group_collection.size();
    for(int i = 0; i < n; i++) {

        int n_kinghts = f_knight_group_collection[i].f_knights_group.size();
        fprintf(stderr, "Group id: %d, f_knight_group_collection[i].f_knights_group.size(): %d\n", i, n_kinghts);

        for(int j = 0; j < n_kinghts; j++) {
            cerr << " --> " << *(f_knight_group_collection[i].f_knights_group[j]) << endl;
        }

    }

}

// --------------------------------------------
// --------  PrincessesAndMonsters  -----------
// --------------------------------------------


class PrincessesAndMonsters {
public:
    string f_dir = "NEWS";
    int f_t;
    int f_turn;
    GameState f_gs;


    PrincessesAndMonsters();


    string initialize(int S, vector<int> princesses, vector<int> monsters, int K);
    string move(vector<int> status, int P, int M, int timeLeft);

};



PrincessesAndMonsters::PrincessesAndMonsters() {
        this->f_turn = 0;
        this->f_gs = GameState();
};



string PrincessesAndMonsters::initialize(int S, vector<int> princesses, vector<int> monsters, int K) {

    fprintf(stderr, "Total number of turns: %d\n", S*S*S);

    //for (auto p : princesses)
    //    fprintf(stderr, "princesses content: %d\n", p);

    f_gs.set_S(S);

    f_gs.set_princesses(princesses);
    f_gs.print_princesses();

    f_gs.set_monsters(monsters);
    f_gs.print_monsters();

    f_gs.set_knights(K);
    f_gs.print_knights();

    f_gs.make_groups();
    f_gs.print_groups();

    srand(1234);
    f_t = -1;
    // all knights start in top left corner
    return string(K, '0');
}



string PrincessesAndMonsters::move(vector<int> status, int P, int M, int timeLeft) {
    f_t++;

    f_turn++;
    fprintf(stderr, "Turn: %d\n", f_turn);

    return string(status.size(), f_t < 3 ? 'E' : f_t < 20 ? 'S' : f_dir[rand() % 4]);
}





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
