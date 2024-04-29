#include <iostream>
#include <conio.h>  // For _kbhit() and _getch()
#include <Windows.h>
#include <vector>

using namespace std;

struct Position {
    int x, y;
};

enum Direction { UP, DOWN, LEFT, RIGHT };

class SnakeGame {
private:
    const int width = 20;
    const int height = 10;
    vector<Position> snake;
    Position food;
    Direction dir;
    bool gameOver;
    const char snakeChar = 'O';
    const char foodChar = '*';
    const char emptySpace = ' ';
    
    void updateDifficulty() {
        int score = snake.size() - 1;
        if (score < 10) {
            Sleep(200); // Начальный уровень
        }
        else if (score >= 10 && score < 20) {
            Sleep(150); // Средний уровень
        }
        else {
            Sleep(100); // Продвинутый уровень
        }
    }
public:
    SnakeGame() {
        gameOver = false;
        dir = UP;
        Position initPos = { width / 2, height / 2 };
        snake.push_back(initPos);
        generateFood();
    }

    void generateFood() {
        bool onSnake;
        do {
            onSnake = false;
            food = { 1 + rand() % (width - 2), 1 + rand() % (height - 2) };
            for (auto& segment : snake) {
                if (segment.x == food.x && segment.y == food.y) {
                    onSnake = true;
                    break;
                }
            }
        } while (onSnake);
    }

    void updateDirection() {
        if (_kbhit()) {
            switch (_getch()) {
            case 'w': dir = UP;    break;
            case 's': dir = DOWN;  break;
            case 'a': dir = LEFT;  break;
            case 'd': dir = RIGHT; break;
            }
        }
    }

    void moveSnake() {
        Position newHead = snake.front();
        switch (dir) {
        case UP:    newHead.y--; break;
        case DOWN:  newHead.y++; break;
        case LEFT:  newHead.x--; break;
        case RIGHT: newHead.x++; break;
        }

        // Check if new head position collides with wall or itself
        if (newHead.x <= 0 || newHead.y <= 0 || newHead.x >= width - 1 || newHead.y >= height - 1) {
            gameOver = true;
            return;
        }
        for (auto& segment : snake) {
            if (segment.x == newHead.x && segment.y == newHead.y) {
                gameOver = true;
                return;
            }
        }

        // Move the snake
        snake.insert(snake.begin(), newHead);
        if (newHead.x == food.x && newHead.y == food.y) {
            generateFood();
        }
        else {
            snake.pop_back();
        }
    }

    void display() {
        system("cls");  // Clear the console
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                if (x == 0 || y == 0 || x == width - 1 || y == height - 1) {
                    cout << "#";
                }
                else if (x == food.x && y == food.y) {
                    cout << foodChar;
                }
                else {
                    bool isBodyPart = false;
                    for (auto& segment : snake) {
                        if (segment.x == x && segment.y == y) {
                            cout << snakeChar;
                            isBodyPart = true;
                            break;
                        }
                    }
                    if (!isBodyPart) {
                        cout << emptySpace;
                    }
                }
            }
            cout << endl;
        }
        cout << "Score: " << snake.size() - 1 << endl;
    }

    void run() {
        while (!gameOver) {
            updateDirection();
            moveSnake();
            display();
            updateDifficulty(); // Обновление сложности в зависимости от счета
            Sleep(200);  
        }
        cout << "Game Over! Final Score: " << snake.size() - 1 << endl;
    }
};

int main() {
    srand(time(0));  // Seed random number generator
    SnakeGame game;
    game.run();
    return 0;
}
