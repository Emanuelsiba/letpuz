// #include <iostream>

// int main() {
//     std::cout << "\033[33m emanuel\033[0m" << std::endl;
//     return 0;
// }

// #include <iostream>
// #include <cstdlib>  // Para usar system()

// void text_to_speech(const std::string &text) {
//     std::string command = "espeak \"" + text + "\"";
//     system(command.c_str());
// }

// int main() {
//     std::string text = "you win";
//     text_to_speech(text);
//     return 0;
// }
// #include <iostream>
// #include <cstdlib>
// #include <termios.h>
// #include <unistd.h>
// #include <vector>

// // Dimensões do tabuleiro
// const int WIDTH = 20;
// const int HEIGHT = 10;

// // Posição inicial do ponto
// int x = WIDTH / 2;
// int y = HEIGHT / 2;

// // Função para capturar teclas
// std::string get_key() {
//     struct termios oldt, newt;
//     char ch;
//     std::string key = "";

//     tcgetattr(STDIN_FILENO, &oldt);
//     newt = oldt;
//     newt.c_lflag &= ~(ICANON | ECHO);
//     tcsetattr(STDIN_FILENO, TCSANOW, &newt);

//     ch = getchar();  // Captura o primeiro caractere
//     if (ch == '\033') {  // Verifica se é uma sequência de escape
//         key += ch;
//         key += getchar();  // Captura o segundo caractere ('[')
//         key += getchar();  // Captura o terceiro caractere (A, B, C, ou D)
//     } else {
//         key += ch;
//     }

//     tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
//     return key;
// }

// // Função para exibir o tabuleiro
// void draw_board() {
//     system("clear"); // Limpa a tela
//     for (int row = 0; row < HEIGHT; row++) {
//         for (int col = 0; col < WIDTH; col++) {
//             if (row == y && col == x)
//                 std::cout << "*"; // Desenha o ponto
//             else
//                 std::cout << " "; // Espaço vazio
//         }
//         std::cout << std::endl;
//     }
// }

// int main() {
//     std::cout << "Use as setas para mover o ponto. Pressione 'q' para sair.\n";
//     while (true) {
//         draw_board();
//         std::string key = get_key();

//         // Movimenta o ponto com base na tecla pressionada
//         if (key == "\033[A" && y > 0) y--;         // Seta para cima
//         else if (key == "\033[B" && y < HEIGHT - 1) y++; // Seta para baixo
//         else if (key == "\033[C" && x < WIDTH - 1) x++;  // Seta para a direita
//         else if (key == "\033[D" && x > 0) x--;         // Seta para a esquerda
//         else if (key == "q") break;               // Sai do programa
//     }

//     std::cout << "Programa encerrado." << std::endl;
//     return 0;
// }
#include <iostream>
#include <cstdlib>
#include <termios.h>
#include <unistd.h>
#include <thread>
#include <chrono>

// Dimensões do tabuleiro
const int WIDTH = 20;
const int HEIGHT = 10;

// Posição inicial do ponto
int x = WIDTH / 2;
int y = HEIGHT / 2;

// Direção inicial do movimento
int dx = 0; // Movimento horizontal
int dy = 0; // Movimento vertical

// Função para capturar teclas
std::string get_key() {
    struct termios oldt, newt;
    char ch;
    std::string key = "";

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();  // Captura o primeiro caractere
    if (ch == '\033') {  // Verifica se é uma sequência de escape
        key += ch;
        key += getchar();  // Captura o segundo caractere ('[')
        key += getchar();  // Captura o terceiro caractere (A, B, C, ou D)
    } else {
        key += ch;
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return key;
}

// Função para exibir o tabuleiro
void draw_board() {
    system("clear"); // Limpa a tela
    for (int row = 0; row < HEIGHT; row++) {
        for (int col = 0; col < WIDTH; col++) {
            if (row == y && col == x)
                std::cout << "*"; // Desenha o ponto
            else
                std::cout << " "; // Espaço vazio
        }
        std::cout << std::endl;
    }
    std::cout << "Use as setas para mover. Pressione 'q' para sair.\n";
}

int main() {
    std::cout << "Pressione as setas para mover. 'q' para sair.\n";

    // Executa a lógica do jogo em uma thread separada
    std::thread input_thread([]() {
        while (true) {
            std::string key = get_key();
            if (key == "\033[A") { dx = 0; dy = -1; } // Seta para cima
            else if (key == "\033[B") { dx = 0; dy = 1; } // Seta para baixo
            else if (key == "\033[C") { dx = 1; dy = 0; } // Seta para a direita
            else if (key == "\033[D") { dx = -1; dy = 0; } // Seta para a esquerda
            else if (key == "q") { exit(0); } // Encerra o programa
        }
    });

    // Loop principal do jogo
    while (true) {
        draw_board();

        // Atualiza a posição do ponto
        x += dx;
        y += dy;

        // Impede o ponto de sair do tabuleiro
        if (x < 0) x = 0;
        if (x >= WIDTH) x = WIDTH - 1;
        if (y < 0) y = 0;
        if (y >= HEIGHT) y = HEIGHT - 1;

        // Pequeno atraso para controlar a velocidade do movimento
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }

    input_thread.join(); // Aguarda a thread de entrada (nunca será alcançada)
    return 0;
}
