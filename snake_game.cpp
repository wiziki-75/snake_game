#include <iostream>
#include <vector>
#include <utility>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <thread>
#include <chrono>

#define WIDTH 50
#define HEIGHT 25

using namespace std;

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

struct Snake {
    pair<int, int> head;
    vector<pair<int, int>> body;
    Direction dir;
};

struct Game {
    Snake snake;
    vector<pair<int, int>> apples;
    bool gameOver;
    int score;
    int speed;
};

void Setup(Game& game) {
    srand(static_cast<unsigned>(time(nullptr))); // Seed for random number generator

    game.gameOver = false;
    game.snake.dir = STOP;
    game.snake.head = make_pair(WIDTH / 2, HEIGHT / 2);
    game.snake.body.push_back(game.snake.head);
    game.score = 0;
    game.speed = 100; // initial speed

    // Initialize apples at random positions
    for (int i = 0; i < 3; ++i) {
        game.apples.push_back(make_pair(rand() % WIDTH, rand() % HEIGHT));
    }
}

void ClearScreen() {
    // Portable way to clear the screen instead of system-specific calls
    cout << "\033[2J\033[1;1H";
}

void Draw(const Game& game) {
    ClearScreen(); // Clear the console

    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0 || j == WIDTH - 1)
                cout << "#"; // Walls
            else if (make_pair(j, i) == game.snake.head)
                cout << "O"; // Snake head
            else if (find(game.snake.body.begin(), game.snake.body.end(), make_pair(j, i)) != game.snake.body.end())
                cout << "x"; // Snake body
            else if (find(game.apples.begin(), game.apples.end(), make_pair(j, i)) != game.apples.end())
                cout << "A"; // Apples
            else
                cout << " "; // Empty space
        }
        cout << "#"; // Right wall
        cout << endl;
    }

    for (int i = 0; i < WIDTH + 2; i++)
        cout << "#";
    cout << endl;
    cout << "Appuyez sur une touche (Haut Z, Bas S, Gauche Q, Droite D) puis sur entrée pour bouger " << endl;
    cout << "Score: " << game.score << endl;
}

void Input(Game& game) {
    char input;
    cin >> input;

    switch (input) {
        case 'q':
            if (game.snake.dir != RIGHT) game.snake.dir = LEFT;
            break;
        case 'd':
            if (game.snake.dir != LEFT) game.snake.dir = RIGHT;
            break;
        case 'z':
            if (game.snake.dir != DOWN) game.snake.dir = UP;
            break;
        case 's':
            if (game.snake.dir != UP) game.snake.dir = DOWN;
            break;
        case 'x':
            game.gameOver = true;
            break;
    }
}


void Logic(Game& game) {
    pair<int, int> prev = game.snake.head;
    pair<int, int> prev2;
    int n = game.snake.body.size();

    // Moving the snake head
    switch (game.snake.dir) {
        case LEFT:
            game.snake.head.first--;
            break;
        case RIGHT:
            game.snake.head.first++;
            break;
        case UP:
            game.snake.head.second--;
            break;
        case DOWN:
            game.snake.head.second++;
            break;
        default:
            break;
    }

    // Check if it hit the wall or its body
    if (game.snake.head.first <= 0 || game.snake.head.first >= WIDTH - 1 || 
        game.snake.head.second < 0 || game.snake.head.second >= HEIGHT ||
        find(game.snake.body.begin() + 1, game.snake.body.end(), game.snake.head) != game.snake.body.end()) {
        game.gameOver = true;
        return;
    }

    // Move the body
    for (int i = 0; i < n; i++) {
        prev2 = game.snake.body[i];
        game.snake.body[i] = prev;
        prev = prev2;
    }

    // Check if eating apple
    auto appleIt = find(game.apples.begin(), game.apples.end(), game.snake.head);
    if (appleIt != game.apples.end()) {
        game.score += 10;
        game.apples.erase(appleIt);
        game.apples.push_back(make_pair(rand() % WIDTH, rand() % HEIGHT)); // add new apple
        game.snake.body.push_back(prev); // increase the size of the snake
    }
}

int main() {
    Game game;
    Setup(game);

    while (!game.gameOver) {
        Draw(game);
        Input(game);
        Logic(game);

        std::this_thread::sleep_for(std::chrono::milliseconds(game.speed));
    }

    cout << "Game Over! Final Score: " << game.score << endl;

    return 0;
}

