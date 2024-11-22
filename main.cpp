#include <iostream>
#include <cstdlib>
#include <termios.h>
#include <unistd.h>


char matrix[3][3] = {
    {'a', 'b', 'c'},
    {'d', 'e', 'f'},
    {'g', '_', 'h'}
};

char matrix_orderned[3][3] = {
    {'a', 'b', 'c'},
    {'d', 'e', 'f'},
    {'g', 'h', '_'}
};


std::string get_key() {
    struct termios oldt, newt;
    char ch;
    std::string key = "";

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();  
    if (ch == '\033') { 
        key += ch;
        key += getchar();  
        key += getchar();  
    } else {
        key += ch;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return key;
}


bool check_victory(char matrix[3][3], char matrix_orderned[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (matrix[i][j] != matrix_orderned[i][j]) {
                return false;
            }
        }
    }
    return true;
}


void display_matrix(int moves) {
    std::cout << "\033[31m╔═════╦═════╦═════╗\033[0m\n";
    for (int i = 0; i < 3; i++) {
        std::cout << "\033[32m║\033[0m";
        for (int j = 0; j < 3; j++) {
            std::cout << "  " << matrix[i][j] << "  \033[32m║\033[0m";
        }
        std::cout << std::endl;
        if (i < 2)
            std::cout << "\033[34m╠═════╬═════╬═════╣\033[0m\n";
    }
    std::cout << "\033[31m╚═════╩═════╩═════╝\033[0m\n";
    std::cout << "\033[33mMovimentos realizados: " << moves << "\033[0m\n";
}

void text_to_speech(const std::string &text) {
    std::string command = "espeak \"" + text + "\"";
    system(command.c_str());
}


int main() {

    int moves = 0;

    while (true) {
        std::system("clear");
        display_matrix(moves);
        // std::cout << "Use the arrows to move (_).\n";

        if (check_victory(matrix, matrix_orderned)) {
            std::string text = "you win";
            text_to_speech(text);
            break;
        }

      
        std::string key = get_key();

        int row, col;
        for (row = 0; row < 3; row++) {
            for (col = 0; col < 3; col++) {
                if (matrix[row][col] == '_') {
                    goto found;
                }
            }
        }
    found:

        bool moved = false;
        
        if (key == "\033[A" && row > 0) { 
            std::swap(matrix[row][col], matrix[row - 1][col]);
            moved = true;
        } else if (key == "\033[B" && row < 2) { 
            std::swap(matrix[row][col], matrix[row + 1][col]);
            moved = true;
        } else if (key == "\033[C" && col < 2) { 
            std::swap(matrix[row][col], matrix[row][col + 1]);
            moved = true;
        } else if (key == "\033[D" && col > 0) { 
            std::swap(matrix[row][col], matrix[row][col - 1]);
            moved = true;
        }

        if (moved)
            moves++;
        
    }

    return 0;
}
