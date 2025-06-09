#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define MAX 100
#define MAX_TENTATIVAS 20
#define NUM_PERGUNTAS_PARA_DICA 3

typedef struct {
    char nome[50];
    char nacionalidade[30];
    char posicao[20];
    int idade;
    char pe[10];                
    const char* clube_antigo;
} Jogador;

// Função auxiliar para converter string para minúsculas
void to_lower_str(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int main() {
    Jogador jogadores[] = {
    {"Rafa", "Portugues", "Avancado", 31, "Direito", "Braga"},
    {"Otamendi", "Argentino", "Defesa", 36, "Direito", "Manchester City"},
    {"Di Maria", "Argentino", "Medio", 36, "Esquerdo", "Juventus"},
    {"Joao Neves", "Portugues", "Medio", 19, "Direito", "Benfica B"},
    {"Trubin", "Ucraniano", "Guarda-redes", 23, "Ambos", "Shakhtar"},
    {"Aursnes", "Noruegues", "Medio", 28, "Esquerdo", "Feyenoord"},
    {"Bah", "Dinamarques", "Defesa", 26, "Direito", "Slavia Praga"},
    {"Tengstedt", "Dinamarques", "Avancado", 25, "Esquerdo", "Rosenborg"},
    {"Antonio Silva", "Portugues", "Defesa", 20, "Direito", "Benfica B"},
    {"Kokçu", "Turco", "Medio", 23, "Direito", "Feyenoord"},
    {"Arthur Cabral", "Brasileiro", "Avancado", 26, "Direito", "Fiorentina"},
    {"Tomas Araujo", "Portugues", "Defesa", 22, "Direito", "Benfica B"},
    {"Jurasek", "Checo", "Defesa", 23, "Esquerdo", "Slavia Praga"},
    {"Joao Mario", "Portugues", "Medio", 31, "Direito", "Inter"},
    {"Samuel Soares", "Portugues", "Guarda-redes", 23, "Direito", "Benfica B"},
    {"Joao Felix", "Portugues", "Avancado", 24, "Direito", "Benfica B"}
};



    srand(time(NULL));
    int total = sizeof(jogadores) / sizeof(jogadores[0]);
    Jogador escolhido = jogadores[rand() % total];

    char buffer[MAX];

    int fd_read = open("fifo1", O_RDONLY);
    int fd_write = open("fifo2", O_WRONLY);

    if (fd_read < 0 || fd_write < 0) {
        perror("Erro ao abrir as FIFOs");
        exit(1);
    }

    printf("[Servidor] Jogador escolhido: %s\n", escolhido.nome);

    int tentativas = 0;
    int perguntas_erradas = 0;

    // Dicas simples baseadas no jogador
    char dica1[100], dica2[100], dica3[100], dica4[100];
    snprintf(dica1, sizeof(dica1), "Nacionalidade: %s.", escolhido.nacionalidade);
    snprintf(dica2, sizeof(dica2), "Posicao: %s.", escolhido.posicao);
    snprintf(dica3, sizeof(dica3), "Pe dominante: %s.", escolhido.pe);
    snprintf(dica4, sizeof(dica4), "Equipa anterior: %s.", escolhido.clube_antigo);

    while (1) {
        ssize_t n = read(fd_read, buffer, MAX);
        if (n <= 0) {
            perror("Erro na leitura ou cliente saiu");
            break;
        }

        buffer[strcspn(buffer, "\n")] = 0;  // Remove newline
        to_lower_str(buffer);  // Converter para minúsculas

        tentativas++;
        if (tentativas > MAX_TENTATIVAS) {
            char msg[150];
            snprintf(msg, sizeof(msg), "Fim do jogo! Excedeste o numero de tentativas. O jogador era: %s\n", escolhido.nome);
            write(fd_write, msg, strlen(msg) + 1);
            break;
        }

        // Criar nome do jogador em minúsculas para comparar
        char nome_escolhido_lower[50];
        strcpy(nome_escolhido_lower, escolhido.nome);
        to_lower_str(nome_escolhido_lower);

        if (strcmp(buffer, "desisto") == 0) {
            char msg[100];
            snprintf(msg, sizeof(msg), "Fim do jogo! Desististe. O jogador era: %s\n", escolhido.nome);
            write(fd_write, msg, strlen(msg) + 1);
            break;
        } else if (strstr(buffer, nome_escolhido_lower)) {
            const char* msg = "Parabens, acertaste o jogador!\n";
            write(fd_write, msg, strlen(msg) + 1);
            break;
        } else if (strstr(buffer, "portugues")) {
            const char* msg = strcmp(escolhido.nacionalidade, "Portugues") == 0 ? "Sim.\n" : "Nao.\n";
            write(fd_write, msg, strlen(msg) + 1);
        }else if (strstr(buffer, "noruegues")) {
            const char* msg = strcmp(escolhido.nacionalidade, "Noruegues") == 0 ? "Sim.\n" : "Nao.\n";
            write(fd_write, msg, strlen(msg) + 1);
        }else if (strstr(buffer, "dinamarques")) {
            const char* msg = strcmp(escolhido.nacionalidade, "Dinamarques") == 0 ? "Sim.\n" : "Nao.\n";
            write(fd_write, msg, strlen(msg) + 1);
        }else if (strstr(buffer, "argentino")) {
            const char* msg = strcmp(escolhido.nacionalidade, "Argentino") == 0 ? "Sim.\n" : "Nao.\n";
            write(fd_write, msg, strlen(msg) + 1);
        }else if (strstr(buffer, "brasileiro")) {
            const char* msg = strcmp(escolhido.nacionalidade, "Brasileiro") == 0 ? "Sim.\n" : "Nao.\n";
            write(fd_write, msg, strlen(msg) + 1);
        }else if (strstr(buffer, "checo")) {
            const char* msg = strcmp(escolhido.nacionalidade, "Checo") == 0 ? "Sim.\n" : "Nao.\n";
            write(fd_write, msg, strlen(msg) + 1);
        }else if (strstr(buffer, "turco")) {
            const char* msg = strcmp(escolhido.nacionalidade, "Turco") == 0 ? "Sim.\n" : "Nao.\n";
            write(fd_write, msg, strlen(msg) + 1);
        }else if (strstr(buffer, "ucraniano")) {
            const char* msg = strcmp(escolhido.nacionalidade, "Ucraniano") == 0 ? "Sim.\n" : "Nao.\n";
            write(fd_write, msg, strlen(msg) + 1);
        } else if (strstr(buffer, "mais de 30")) {
            const char* msg = escolhido.idade > 30 ? "Sim.\n" : "Nao.\n";
            write(fd_write, msg, strlen(msg) + 1);
        } else if (strstr(buffer, "medio")) {
            const char* msg = strcmp(escolhido.posicao, "Medio") == 0 ? "Sim.\n" : "Nao.\n";
            write(fd_write, msg, strlen(msg) + 1);
        } else if (strstr(buffer, "defesa")) {
            const char* msg = strcmp(escolhido.posicao, "Defesa") == 0 ? "Sim.\n" : "Nao.\n";
            write(fd_write, msg, strlen(msg) + 1);
        } else if (strstr(buffer, "avancado")) {
            const char* msg = strcmp(escolhido.posicao, "Avancado") == 0 ? "Sim.\n" : "Nao.\n";
            write(fd_write, msg, strlen(msg) + 1);
        } else if (strstr(buffer, "guarda")) {
            const char* msg = strcmp(escolhido.posicao, "Guarda-redes") == 0 ? "Sim.\n" : "Nao.\n";
            write(fd_write, msg, strlen(msg) + 1);
        } else if (strstr(buffer, "pe esquerdo")) {
            const char* msg = strcmp(escolhido.pe, "Esquerdo") == 0 ? "Sim.\n" : "Nao.\n";
            write(fd_write, msg, strlen(msg) + 1);
        } else if (strstr(buffer, "pe direito")) {
            const char* msg = strcmp(escolhido.pe, "Direito") == 0 ? "Sim.\n" : "Nao.\n";
            write(fd_write, msg, strlen(msg) + 1);
        } else if (strstr(buffer, "ambos")) {
            const char* msg = strcmp(escolhido.pe, "Ambos") == 0 ? "Sim.\n" : "Nao.\n";
            write(fd_write, msg, strlen(msg) + 1);
        } else if (strstr(buffer, "manchester city")) {
           const char* msg = strcmp(escolhido.clube_antigo, "Manchester City") == 0 ? "Sim.\n" : "Nao.\n";
           write(fd_write, msg, strlen(msg) + 1);
        } else if (strstr(buffer, "braga")) {
           const char* msg = strcmp(escolhido.clube_antigo, "Braga") == 0 ? "Sim.\n" : "Nao.\n";
           write(fd_write, msg, strlen(msg) + 1);
        } else if (strstr(buffer, "juventus")) {
           const char* msg = strcmp(escolhido.clube_antigo, "Juventus") == 0 ? "Sim.\n" : "Nao.\n";
           write(fd_write, msg, strlen(msg) + 1);
        } else if (strstr(buffer, "benfica b")) {
          const char* msg = strcmp(escolhido.clube_antigo, "Benfica B") == 0 ? "Sim.\n" : "Nao.\n";
          write(fd_write, msg, strlen(msg) + 1);
        } else if (strstr(buffer, "shakhtar")) {
          const char* msg = strcmp(escolhido.clube_antigo, "Shakhtar") == 0 ? "Sim.\n" : "Nao.\n";
          write(fd_write, msg, strlen(msg) + 1);
        } else if (strstr(buffer, "feyenoord")) {
          const char* msg = strcmp(escolhido.clube_antigo, "Feyenoord") == 0 ? "Sim.\n" : "Nao.\n";
          write(fd_write, msg, strlen(msg) + 1);
        } else if (strstr(buffer, "slavia praga")) {
          const char* msg = strcmp(escolhido.clube_antigo, "Slavia Praga") == 0 ? "Sim.\n" : "Nao.\n";
          write(fd_write, msg, strlen(msg) + 1);
        } else if (strstr(buffer, "rosenborg")) {
	      const char* msg = strcmp(escolhido.clube_antigo, "Rosenborg") == 0 ? "Sim.\n" : "Nao.\n";
          write(fd_write, msg, strlen(msg) + 1);
        } else if (strstr(buffer, "fiorentina")) {
          const char* msg = strcmp(escolhido.clube_antigo, "Fiorentina") == 0 ? "Sim.\n" : "Nao.\n";
          write(fd_write, msg, strlen(msg) + 1);
        } else if (strstr(buffer, "inter")) {
          const char* msg = strcmp(escolhido.clube_antigo, "Inter") == 0 ? "Sim.\n" : "Nao.\n";
          write(fd_write, msg, strlen(msg) + 1);
        }else {
            perguntas_erradas++;

            if (perguntas_erradas > 0 && perguntas_erradas % NUM_PERGUNTAS_PARA_DICA == 0) {
                char msg_dica[256];
                int dica_num = (perguntas_erradas / NUM_PERGUNTAS_PARA_DICA - 1) % 4;
                const char* dica;
                switch (dica_num) {
                    case 0: dica = dica1; break;
                    case 1: dica = dica2; break;
                    case 2: dica = dica3; break;
                    case 3: dica = dica4; break;
                    default: dica = ""; break;
                }
                snprintf(msg_dica, sizeof(msg_dica),
                    "Pergunta nao reconhecida ou palpite errado.\n DICA:%s\n", dica);
                write(fd_write, msg_dica, strlen(msg_dica) + 1);
            } else {
                const char* msg = "Pergunta nao reconhecida ou palpite errado.\n";
                write(fd_write, msg, strlen(msg) + 1);
            }
        }
    }

    close(fd_read);
    close(fd_write);

    return 0;
}

