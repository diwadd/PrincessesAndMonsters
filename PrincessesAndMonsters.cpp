#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include <cassert>
#include <algorithm>

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
        string f_order;

        Knight(int idp = -1, 
               int xp = -1,
               int yp = -1,
               int np = 0,
               int gp = -1,
               string sp = "NONE"): f_id(idp), f_x(xp), f_y(yp), f_n_p(np), f_g(gp), f_order(sp) {}
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
        string f_moves = "NEWS";
        int f_S; // board size
        int f_turn; // current turn
        int f_n_knights; // number of knights
        int f_n_princesses; // number of princesses
        int f_n_monsters; // number of monsters
        vector<Knight> f_knights;
        vector<Princess> f_princesses;
        vector<Monster> f_monsters;
        vector<KnightGroup> f_knight_group_collection;

        string f_current_global_order_name;
        string f_current_order_instructions;

        pair<int, int> f_global_assembly_point;


        GameState() {}

        void set_S(int &S) {f_S = S;}

        void set_princesses(vector<int> &pr);
        void print_princesses();

        void set_monsters(vector<int> &mo);
        void print_monsters();

        void set_knights(int &k);
        void update_initial_knight_positions(pair<int, int> &pos);
        void update_knights_number_of_princesses(vector<int> &status);
        void print_knights();

        int get_number_of_escorted_princesses_at_cm(pair<int, int> &cm_point);

        pair<int, int> _determine_the_number_of_groups();
        void make_groups();
        void print_groups();


        pair<int, int> princess_center_of_mass();
        char princess_cm_closest_entrence(pair<int, int> &cm);


        string move_towards_point(pair<int, int> &point);
        void move_towards_point(pair<int, int> &point, string &move_order);
        void move_knight_towards_point(pair<int, int> &point, string &move_order, int &id);
        bool princess_cm_reached(pair<int, int> &cm_point);
        bool check_if_knight_reached_princess_cm(pair<int, int> &cm_point, int &id);
        bool check_if_all_knights_reached_princess_cm(pair<int, int> &cm_point);

        void send_global_order(string &order);
        void send_order_to_all_knights(string &order);
        void send_order_to_a_fraction_of_knights(string &order, double &fraction);

        void random_disperse(string &move_order);
        void check_and_set_princess_escort_during_random_disperse(string &move_order);

};



void GameState::set_princesses(vector<int> &pr) {

    int n = pr.size();
    f_n_princesses = n/2;
    f_princesses.resize( n/2 );
    for(int i = 0; i < n/2; i++) {
        f_princesses[i].f_last_x = pr[2*i + 1];
        f_princesses[i].f_last_y = pr[2*i];
    }
}


void GameState::set_monsters(vector<int> &mo) {

    int n = mo.size();
    f_n_monsters = n/2;
    f_monsters.resize( n/2 );
    for(int i = 0; i < n/2; i++) {
        f_monsters[i].f_last_x = mo[2*i + 1];
        f_monsters[i].f_last_y = mo[2*i];
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

void GameState::update_initial_knight_positions(pair<int, int> &pos) {

    for(int i = 0; i < f_n_knights; i++) {
        f_knights[i].f_x = pos.first;
        f_knights[i].f_y = pos.second;
    }
}


void GameState::update_knights_number_of_princesses(vector<int> &status) {

    for(int i = 0; i < f_n_knights; i++)
        f_knights[i].f_n_p = status[i];
}


void GameState::print_knights() {
    for(int i = 0; i < f_knights.size(); i++)
        cerr << "Knight id: " << i << ": " << f_knights[i] << endl;
}

int GameState::get_number_of_escorted_princesses_at_cm(pair<int, int> &cm_point) {

    int n_princesses = 0;
    for(int i = 0; i < f_n_knights; i++) {
        if (f_knights[i].f_n_p <= 0)
            continue;
        if (f_knights[i].f_x == cm_point.first && f_knights[i].f_y == cm_point.second)
            n_princesses = n_princesses + f_knights[i].f_n_p;
    }
    return n_princesses;
}

pair<int, int> GameState::_determine_the_number_of_groups() {

    // monster_p - probability of meeting a monster
    //double monster_p = f_n_monsters / (f_S*f_S);

    int min_number_of_knights_in_group = f_n_knights;
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


pair<int, int> GameState::princess_center_of_mass() {

    int x_cm = 0;
    int y_cm = 0;

    for(int i = 0; i < f_n_princesses; i++) {
        x_cm = x_cm + f_princesses[i].f_last_x;
        y_cm = y_cm + f_princesses[i].f_last_y;
    }

    x_cm = x_cm/f_n_princesses;
    y_cm = y_cm/f_n_princesses;

    fprintf(stderr, "Princess center of mass (x_cm, y_cm): (%d, %d)\n", x_cm, y_cm);

    return make_pair(x_cm, y_cm);

}

char GameState::princess_cm_closest_entrence(pair<int, int> &cm) {

    pair<int, int> top_left = make_pair(0, 0); // 0
    pair<int, int> top_right = make_pair(f_S-1, 0); // 1

    pair<int, int> bottom_right = make_pair(f_S-1, f_S-1); // 2
    pair<int, int> bottom_left = make_pair(0, f_S-1); // 3

    
    int dtl = abs(cm.first - top_left.first) + abs(cm.second - top_left.second);
    int dtr = abs(cm.first - top_right.first) + abs(cm.second - top_right.second);
    int dbr = abs(cm.first - bottom_right.first) + abs(cm.second - bottom_right.second);
    int dbl = abs(cm.first - bottom_left.first) + abs(cm.second - bottom_left.second);

    fprintf(stderr, "Distances to the entrences from the princess center of mass:\n");
    fprintf(stderr, "dtl: %d\n", dtl);
    fprintf(stderr, "dtr: %d\n", dtr);
    fprintf(stderr, "dbl: %d\n", dbl);
    fprintf(stderr, "dbr: %d\n", dbr);

    vector<int> v = {dtl, dtr, dbr, dbl};

    int d_min = 2500; // Max S = 50 so 2500 is enough.
    char closest_entrence_index = -1;
    for(int i = 0; i < v.size(); i++) {
        if (v[i] < d_min) {
            d_min = v[i];
            closest_entrence_index = i;
        }
    }

    if (closest_entrence_index == 0)
        closest_entrence_index = '0';
    else if (closest_entrence_index == 1)
        closest_entrence_index = '1';
    else if (closest_entrence_index == 2)
        closest_entrence_index = '2';
    else if (closest_entrence_index == 3)
        closest_entrence_index = '3';
    else {
        assert(false);
        cerr << "Could not get entrence id!" << endl;    
    }

    fprintf(stderr, "Distance to closest entrance d_min: %d\n", d_min);
    fprintf(stderr, "closest_entrence_index: %c\n", closest_entrence_index);

    return closest_entrence_index;
}


void GameState::move_knight_towards_point(pair<int, int> &point,
                                            string &move_order,
                                            int &id) {

    fprintf(stderr, "Moving towards (%d, %d)\n", point.first, point.second);

    if (point.first > f_knights[id].f_x) {
        move_order[id] = 'E'; // move right
        f_knights[id].f_x = f_knights[id].f_x + 1;
    } else if (point.first < f_knights[id].f_x) {
        move_order[id] = 'W'; // move left
        f_knights[id].f_x = f_knights[id].f_x - 1;
    } else if (point.second > f_knights[id].f_y) {
        move_order[id] = 'S'; // move left
        f_knights[id].f_y = f_knights[id].f_y + 1;
    } else if (point.second < f_knights[id].f_y) {
        move_order[id] = 'N'; // move left
        f_knights[id].f_y = f_knights[id].f_y - 1;
    }

}


void GameState::move_towards_point(pair<int, int> &point, string &move_order) {

    //fprintf(stderr, "Moving towards (%d, %d)\n", point.first, point.second);
    for(int i = 0; i < f_n_knights; i++)
        move_knight_towards_point(point, move_order, i);

}


bool GameState::princess_cm_reached(pair<int, int> &cm_point) {

    // When ORDER_MOVE_TO_PRINCESS_CENTER_OF_MASS is issued all
    // knights should move towards it. It is enough to check
    // just one.
    // This function should be executed only durring ORDER_MOVE_TO_PRINCESS_CENTER_OF_MASS
    if (f_current_global_order_name != "ORDER_MOVE_TO_PRINCESS_CENTER_OF_MASS") {
        cerr << "princess_cm_reached executed not durring ORDER_MOVE_TO_PRINCESS_CENTER_OF_MASS" << endl;
        assert(false);
    }


    if (cm_point.first == f_knights[0].f_x && cm_point.second == f_knights[0].f_y) {
        return true;
    } else {
        return false;
    }
}


bool GameState::check_if_knight_reached_princess_cm(pair<int, int> &cm_point, int &id) {

    if (cm_point.first == f_knights[id].f_x && cm_point.second == f_knights[id].f_y) {
        return true;
    } else {
        return false;
    }
}


bool GameState::check_if_all_knights_reached_princess_cm(pair<int, int> &cm_point) {

    for(int i = 0; i < f_n_knights; i++)
        if (cm_point.first != f_knights[i].f_x && cm_point.second != f_knights[i].f_y)
            return false;

    return true;
}



void GameState::send_global_order(string &order) {
    cerr << "Sending order: " << order << endl;
    f_current_global_order_name = order;

}


void GameState::send_order_to_all_knights(string &order) {

    for(int i = 0; i < f_n_knights; i++)
        f_knights[i].f_order = order;


}


void GameState::send_order_to_a_fraction_of_knights(string &order, double &fraction) {

    int n = int(fraction*f_n_knights);
    int index = 0;

    for(int i = f_n_knights - 1; i >= 0; i--) {

        if (index == n)
            break;

        if (f_knights[i].f_n_p < 0)
            continue;

        f_knights[i].f_order = order;
        index++;
    }
}


void GameState::random_disperse(string &move_order) {

    for(int i = 0; i < f_n_knights; i++) {
        if (f_knights[i].f_n_p < 0)
            continue;

        if (f_knights[i].f_order == "ORDER_RANDOM_PRINCESS_SEARCH") {

            int move_id = rand() % 4;

            move_order[i] = f_moves[move_id];
            // NEWS
            if (move_id == 0)
                f_knights[i].f_y = f_knights[i].f_y - 1;
            else if (move_id == 1)
                f_knights[i].f_x = f_knights[i].f_x + 1;
            else if (move_id == 2)
                f_knights[i].f_x = f_knights[i].f_x - 1;
            else if (move_id == 3)
                f_knights[i].f_y = f_knights[i].f_y + 1;

        }
    }
}


void GameState::check_and_set_princess_escort_during_random_disperse(string &move_order) {

    for(int i = 0; i < f_n_knights; i++) {

        if (f_knights[i].f_n_p < 0)
            continue;

        if (f_knights[i].f_order == "ORDER_RETURN_TO_GLOBAL_ASSEMBLY_POINT") {
            move_knight_towards_point(f_global_assembly_point, move_order, i);

            bool reached_cm = check_if_knight_reached_princess_cm(f_global_assembly_point, i)  ;          
            if (reached_cm == true)
                f_knights[i].f_order = "ORDER_RANDOM_PRINCESS_SEARCH";


        } else if (f_knights[i].f_n_p > 0 && f_knights[i].f_order == "ORDER_RANDOM_PRINCESS_SEARCH") {
            f_knights[i].f_order = "ORDER_RETURN_TO_GLOBAL_ASSEMBLY_POINT";
            move_knight_towards_point(f_global_assembly_point, move_order, i);

            bool reached_cm = check_if_knight_reached_princess_cm(f_global_assembly_point, i)  ;          
            if (reached_cm == true)
                f_knights[i].f_order = "ORDER_RANDOM_PRINCESS_SEARCH";

        } else if (f_knights[i].f_n_p == 0 && f_knights[i].f_order == "ORDER_RANDOM_PRINCESS_SEARCH") {

            int move_id = rand() % 4;

            move_order[i] = f_moves[move_id];
            // NEWS
            if (move_id == 0)
                f_knights[i].f_y = f_knights[i].f_y - 1;
            else if (move_id == 1)
                f_knights[i].f_x = f_knights[i].f_x + 1;
            else if (move_id == 2)
                f_knights[i].f_x = f_knights[i].f_x - 1;
            else if (move_id == 3)
                f_knights[i].f_y = f_knights[i].f_y + 1;

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


    f_gs.f_global_assembly_point = f_gs.princess_center_of_mass();
    char closest_entrence_index = f_gs.princess_cm_closest_entrence(f_gs.f_global_assembly_point);


    pair<int, int> ikp = make_pair(0, 0); // initial knights position
    if (closest_entrence_index == '0') {
        ikp.first = 0;
        ikp.second = 0;
    } else if (closest_entrence_index == '1') {
        ikp.first = S - 1;
        ikp.second = 0;
    } else if (closest_entrence_index == '2') {
        ikp.first = S - 1;
        ikp.second = S - 1;
    } else if (closest_entrence_index == '3') {
        ikp.first = 0;
        ikp.second = S - 1;
    } else {
        assert(false);
        cerr << "Unknown entrance!" << endl;
    }

    f_gs.update_initial_knight_positions(ikp);
    // f_gs.current_order_name = "PRINCESS_CENTER_OF_MASS";

    string order = "ORDER_MOVE_TO_PRINCESS_CENTER_OF_MASS";    
    f_gs.send_global_order(order);
    f_gs.send_order_to_all_knights(order);


    srand(1234);
    f_t = -1;
    // all knights start in top left corner

    string s = string(K, closest_entrence_index);
    cerr << "Knight entrences: " << s << endl;
    return s;
}



string PrincessesAndMonsters::move(vector<int> status, int P, int M, int timeLeft) {
    f_t++;

    f_turn++;
    int n_knights = status.size();
    fprintf(stderr, "Turn: %d\n", f_turn);
    f_gs.update_knights_number_of_princesses(status);
    f_gs.print_knights();

    int n_escorted_princesses = f_gs.get_number_of_escorted_princesses_at_cm(f_gs.f_global_assembly_point);
    cerr << "Total number of princesses: " << P << "; Escorted princesses at cm: " << n_escorted_princesses << endl;

    if (P == n_escorted_princesses && f_gs.f_current_global_order_name != "ORDER_FINAL_RETURN_TO_GLOBAL_ASSEMBLY_POINT") {
        string global_order = "ORDER_FINAL_RETURN_TO_GLOBAL_ASSEMBLY_POINT";
        f_gs.send_global_order(global_order);
        f_gs.send_order_to_all_knights(global_order);
    }



    string move_order = string(n_knights, 'X');
    if (f_gs.f_current_global_order_name == "ORDER_MOVE_TO_PRINCESS_CENTER_OF_MASS") {
        f_gs.move_towards_point(f_gs.f_global_assembly_point, move_order); 
        cerr << "ORDER_MOVE_TO_PRINCESS_CENTER_OF_MASS - Current move order: " << move_order << endl;

        bool cm_reached = f_gs.princess_cm_reached(f_gs.f_global_assembly_point);
        if (cm_reached == true) {
            string order = "ORDER_RANDOM_PRINCESS_SEARCH";
            f_gs.send_global_order(order);
            //f_gs.send_order_to_all_knights(order);
            // Fraction of knights that will search for princesses.            
            double fraction = 0.4;
            f_gs.send_order_to_a_fraction_of_knights(order, fraction);        
        }
        return move_order;


    } else if (f_gs.f_current_global_order_name == "ORDER_RANDOM_PRINCESS_SEARCH") {
        f_gs.check_and_set_princess_escort_during_random_disperse(move_order);
        //f_gs.random_disperse(move_order);
        
        cerr << "ORDER_RANDOM_PRINCESS_SEARCH - Current move order: " << move_order << endl;
        return move_order;


    } else if (f_gs.f_current_global_order_name == "ORDER_FINAL_RETURN_TO_GLOBAL_ASSEMBLY_POINT") {
        
        f_gs.move_towards_point(f_gs.f_global_assembly_point, move_order);
        cerr << "ORDER_FINAL_RETURN_TO_GLOBAL_ASSEMBLY_POINT - Current move order: " << move_order << endl;

        bool cm_reached = f_gs.check_if_all_knights_reached_princess_cm(f_gs.f_global_assembly_point);
        if (cm_reached == true) {
            string global_order = "ORDER_DO_NOTHING";
            f_gs.send_global_order(global_order);
            f_gs.send_order_to_all_knights(global_order);
        }
        return move_order;


    } else if (f_gs.f_current_global_order_name == "ORDER_DO_NOTHING") {

        cerr << "ORDER_DO_NOTHING - Current move order: " << move_order << endl;
    }
    



    return move_order;
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
