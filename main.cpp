#include <iostream>
#include <algorithm>
#include <random>
#include <vector>
#include <windows.h>
#include <conio.h>

#define BEGIN_MENU "BEGIN_SCREEN\n press 1 to close \n press 2 to introduction\n press any key to begin\n"
#define END_MENU "END_SCREEN\n press any key to end\n"
#define DESCRIPTION_MENU "DESCRIPTION_SCREEN\n\n GREAT GAME WITH THREE GREAT \"NO\"\n - no description\n - no introduction\n - no annotation\n\n also no check for incorrect input\n NEVER INPUT ANYTHING WEIRD bro\n\n press any key to return\n"

#define ROW 10
#define COL 5

#define MIN 0
#define MAX 4

std::random_device seed;
std::ranlux48 engine(seed());
std::uniform_int_distribution<> distrib(MIN, MAX);
int random = distrib(engine);

enum TYPE { P, Z, N };

class Spirit {
public:
    TYPE type;
    int num;
    Spirit(TYPE type = N, int num = 0);
    void draw() const;
    void operator=(const Spirit &spirit);
    void destroy();
};

class Game {
public:
    unsigned int time;
    Spirit game_map[ROW+1][COL];
    Spirit storage[COL];
    Game() { init(); }
    void init();
    void run();
    void update();
    bool check() const;
    void draw() const;

    bool draw_begin_menu() const;
    void draw_end_menu() const;
    void draw_description_menu() const;

    bool func();
    void random_Z();
    void random_P();
    void attack();
    void move();

    void remove_from_map(int from_row, int from_col);
    void remove_from_storage(int from_col);

    bool judge_storage_empty() const;

    bool check_index(int& from_row, int& from_col, int& to_row, int& to_col) const;
};


int main() {
    Game game;
    game.run();
    return 0;
}



void Game::run() {
    if(draw_begin_menu() == false) return;
    init();
    draw();
    while(check() == true) {
        update();
    }
    draw_end_menu();
}

void Game::update() {
    time++;
    while(!func()) {
        system("cls");
    }
    draw();
    Sleep(500);
    attack();
    draw();
    Sleep(500);
    move();
    random_Z();
    draw();
}

void Game::init() {
    for (int i = 0; i < ROW + 1; i++) {
        for (int j = 0; j < COL; j++) {
            game_map[i][j].destroy();
        }
    }
    for (int i = 0; i < COL; i++) {
        storage[i].destroy();
    }
    storage[0] = Spirit(P, 4);
    time = 0;
}

bool Game::draw_begin_menu() const {
FLAG:
    system("cls");
    printf(BEGIN_MENU);
    switch(getch()) {
        case '1': return false;
        case '2': draw_description_menu(); goto FLAG;
    }
    return true;
}

void Game::draw_end_menu() const {
    system("cls");
    printf(END_MENU);
    getch();
}

void Game::draw_description_menu() const {
    system("cls");
    printf(DESCRIPTION_MENU);
    getch();
}

bool Game::check() const {
    for(int i = 0; i < COL; i++) {
        if(game_map[ROW][i].type == Z) return false;
    }
    return true;
}

void Game::draw() const {
    system("cls");
    printf("time : %d\n", time);
    printf("=================================================================================\n");
    for (int i = 0; i < ROW; i++) {
        for (int j = 0; j < COL; j++) {
            printf("|\t");
            game_map[i][j].draw();
            printf("\t");
        }
        if(i == 7 || i == 9) 
            printf("|\n=================================================================================\n");
        else 
            printf("|\n|---------------|---------------|---------------|---------------|---------------|\n");
    }
    printf("\nstorage\n");
    printf("=================================================================================\n");
    for (int i = 0; i < 5; i++) {
        printf("|\t");
        storage[i].draw();
        printf("\t");
    }
    printf("|\n=================================================================================\n");
}

bool Game::func() {
    int from_row = 1, from_col = 1, to_row = 1, to_col = 1;
    printf("press 1 : put a new plant or move a plant\npress 2 : upgrade a curent plant\npress any other key : no func\n");
    switch(getch()) {
        case '1':
            printf("press 1 to plant\npress 2 to move\npress any other key to return\n");
            switch(getch()) {
                case '1':
                    if(judge_storage_empty()) {
                        return false;
                    }
                    printf("press 3 num (from_col, to_row, to_col) to plant\n");
                    std::cin >> from_col >> to_row >> to_col;
                    check_index(from_row, from_col, to_row, to_col);
                    game_map[to_row][to_col] = storage[from_col];
                    remove_from_storage(from_col);
                    return true;
                case '2':
                    printf("press 4 num (from_row, from_col, to_row, to_col) to move\n");
                    std::cin >> from_row >> from_col >> to_row >> to_col;
                    check_index(from_row, from_col, to_row, to_col);
                    game_map[to_row][to_col] = game_map[from_row][from_col];
                    remove_from_map(from_row, from_col);
                    return true;
                default:
                    return false;
            }
        case '2':
            printf("press 1 to upgrade from storage\npress 2 to upgrade from map\n");
            switch(getch()) {
                case '1':
                    if(judge_storage_empty()) {
                        return false;
                    }
                    printf("press 3 num (from_col, to_row, to_col) to plant\n");
                    std::cin >> from_col >> to_row >> to_col;
                    check_index(from_row, from_col, to_row, to_col);
                    game_map[to_row][to_col].num *= 2;
                    remove_from_storage(from_col);
                    return true;
                case '2':
                    printf("press 4 num (from_row, from_col, to_row, to_col) to upgrade\n");
                    std::cin >> from_row >> from_col >> to_row >> to_col;
                    check_index(from_row, from_col, to_row, to_col);
                    game_map[to_row][to_col].num *= 2;
                    remove_from_map(from_row, from_col);
                    return true;
                default:
                    return false;
            }
    }
    return true;
}

void Game::remove_from_map(int from_row, int from_col) {
    game_map[from_row][from_col].destroy();
}

void Game::remove_from_storage(int from_col) {
    for(int i = from_col; i < COL; i++) {
        storage[i].destroy();
        if(i + 1 < COL) {
            if(storage[i + 1].type == N) return;
            storage[i] = storage[i + 1];
        }
    }
}

bool Game::judge_storage_empty() const {
    if(storage[0].type == N) {
        printf("storage is empty\n");
        return true;
    }
    return false;
}

void Game::random_Z() {
    int random = distrib(engine);
    if(random < 2) {
        return;
    }
    int random1 = distrib(engine);
    int random2 = distrib(engine);
    int random3 = distrib(engine);
    int tmp = std::min(random2, random3);
    game_map[0][random1] = Spirit(Z, pow(2, tmp));

}

void Game::random_P() {
    int index = 0;
    while(index < COL && storage[index].type == P) {
        index++;
    }
    if(index == COL) return;
    int random = distrib(engine);
    storage[index] = Spirit(P, pow(2, random));
}

void Game::attack() {
    for(int j = 0; j < COL; j++) {
        int cnt = 0;
        if(game_map[ROW-1][j].type == P) cnt += game_map[ROW-1][j].num;
        if(game_map[ROW-2][j].type == P) cnt += game_map[ROW-2][j].num;
        if(cnt == 0) continue;
        bool flag = false;
        for(int i = ROW-1; i >= 0 && cnt > 0; i--) {
            if(game_map[i][j].type == Z) {
                int tmp = game_map[i][j].num;
                game_map[i][j].num -= cnt;
                if(game_map[i][j].num <= 0) {
                    game_map[i][j].destroy();
                    flag ^= 1;
                }
                cnt -= tmp;
            }
        }
        if(flag == true) {
            random_P();
        }
    }

}

void Game::move() {
    for (int i = 15; i >= 0; i--) {
        for (int j = 0; j < 5; j++) {
            if(game_map[i][j].type == Z) {
                game_map[i+1][j] = game_map[i][j];
                game_map[i][j].destroy();
            }
        }
    }
}

bool Game::check_index(int& from_row, int& from_col, int& to_row, int& to_col) const {
    if(from_row < 1 || from_row > 2 || from_col < 1 || from_col > 5 || to_row < 1 || to_row > 2 || to_col < 1 || to_col > 5) {
        printf("invalid index\n");
        return false;
    }
    from_row += 7;
    from_col -= 1;
    to_row += 7;
    to_col -= 1;
    return true;
}

Spirit::Spirit(TYPE type, int num) {
    this->type = type;
    this->num = num;
}

void Spirit::draw() const {
    switch(type) {
        case P:
            printf("\033[1;33mP_%d\033[0m", num);
            break;
        case Z:
            printf("\033[1;31mZ_%d\033[0m", num);
            break;
        default:
            break;
    }
}

void Spirit::operator=(const Spirit& other) {
    this->type = other.type;
    this->num = other.num;
}

void Spirit::destroy() {
    type = N;
}

