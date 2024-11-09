#include <iostream>
#include <Windows.h>
#include <iomanip>//використовується для функції setw, щоб забезпечити вирівнювання карток на ігровій дошці.
#include <vector>
#include <chrono>//викорситовується для роботи з часом 
#include <fstream>
#include <random>

using namespace std;
using namespace chrono;

// Структура для карток гри
struct Card {
    int value;     // Значення картки
    bool isOpen;   // Стан картки (відкрита/закрита)
};

// Ініціалізація дошки з випадковим розміщенням парних карт
void initializeBoard(vector<vector<Card>>& board, int rows, int cols) {
    vector<int> values;
    for (int i = 0; i < rows * cols / 2; ++i) {
        values.push_back(i); // Додаємо кожну картку двічі
        values.push_back(i);
    }

    random_device rd;
    mt19937 rng(rd());
    shuffle(values.begin(), values.end(), rng);
    //ініціалізує дошку з картками, призначаючи кожній клітині дошки значення з вектора values
    int idx = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            board[i][j].value = values[idx++];
            board[i][j].isOpen = false;
        }
    }
}

// Виведення стану дошки в консоль
void printBoard(const vector<vector<Card>>& board, int openX = -1, int openY = -1) {
    for (int i = 0; i < board.size(); ++i) {
        for (int j = 0; j < board[i].size(); ++j) {
            if (board[i][j].isOpen || (i == openX && j == openY)) {
                cout << "\033[1;32m" << setw(2) << board[i][j].value << "\033[0m ";// setw-встановлення ширини поля бібліотека(iomanip)
            }
            else {
                cout << "\033[1;34m" << setw(2) << '*' << "\033[0m ";
            }
        }
        cout << endl;
    }
}

// Збереження статистики (кількість ходів та час гри) у файл
void saveStatistics(int moves, double duration) {
    ofstream file("memory_game_stats.txt", ios::app); // Відкриваємо файл для додавання
    if (!file) { // Перевіряємо, чи файл відкрився правильно
        cout << "Помилка відкриття файлу для запису!" << endl;
        return;
    }

    // Якщо файл відкрився, записуємо в нього статистику
    file << "Game Statistics\n";
    file << "---------------\n";
    file << "Time: " << duration << " seconds\n"; // Записуємо час гри
    file << "Moves: " << moves << "\n";           // Записуємо кількість ходів
    file << "---------------\n\n";        // Роздільник для кожної гри
    file.close(); // Закриваємо файл
}



int main() {
    system("chcp 1251>null");
    int rows, cols;
    cout << "\033[1;37mВиберіть розмір дошки:\n1. 4x4\n2. 5x6\n\033[0m";
    int choice;
    cin >> choice;

    if (choice == 1) {
        rows = 4;
        cols = 4;
    }
    else if (choice == 2) {
        rows = 5;
        cols = 6;
    }
    else {
        cout << "Неправильний вибір!";
        return 1;
    }

    vector<vector<Card>> board(rows, vector<Card>(cols));
    initializeBoard(board, rows, cols);

    int moves = 0;
    auto startTime = steady_clock::now();

    while (true) {
        int x1, y1, x2, y2;
        printBoard(board);

        cout << "\033[1;37mВведіть координати першої картки (стовпець і рядок): \033[0m";
        cin >> y1 >> x1;
        x1 -= 1;
        y1 -= 1;

        cout << "\033[1;37mВведіть координати другої картки (стовпець і рядок): \033[0m";
        cin >> y2 >> x2;
        x2 -= 1;
        y2 -= 1;

        if ((x1 == x2 && y1 == y2) || board[x1][y1].isOpen || board[x2][y2].isOpen) {
            cout << "Невірний вибір. Спробуйте знову.\n";
            continue;
        }

        moves++;
        board[x1][y1].isOpen = true;
        board[x2][y2].isOpen = true;

        printBoard(board);

        if (board[x1][y1].value == board[x2][y2].value) {
            cout << "\033[1;32mЗбіг знайдено!\033[0m\n";
        }
        else {
            cout << "\033[1;31mЗбіг не знайдено!\033[0m\n";
            board[x1][y1].isOpen = false;
            board[x2][y2].isOpen = false;
        }

        bool gameWon = true;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (!board[i][j].isOpen) {
                    gameWon = false;
                    break;
                }
            }
        }

        if (gameWon) {
            auto endTime = steady_clock::now();
            double duration = duration_cast<seconds>(endTime - startTime).count();
            cout << "Щиро вітаю! Ви завершили гру.\n";
            cout << "Загальна кількість ходів: " << moves << "\n";
            cout << "Загальний час: " << duration << " секунд\n";
            saveStatistics(moves, duration);
            break;
        }
    }

    return 0;
}
