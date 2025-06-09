#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX 100

int main() {
    char buffer[MAX];

    int fd_write = open("fifo1", O_WRONLY);  // Escreve perguntas
    int fd_read = open("fifo2", O_RDONLY);   // Lê respostas

    if (fd_write < 0 || fd_read < 0) {
        perror("Erro ao abrir as FIFOs");
        exit(1);
    }

    printf("Tenta adivinhar o jogador do Benfica!\n");
    printf("Podes fazer perguntas sobre:\n");
    printf("   - Nacionalidade (Ex.: E portugues?)\n");
    printf("   - Posicao(Ex.: E medio?)\n");
    printf("   - Idade(Ex.: Tem mais de 30 anos?)\n");
    printf("   - Clube antigo (Ex.: O clube antigo foi Benfica B?)\n");
    printf("   - Ou tenta adivinhar o nome (ex: 'Di Maria')\n");
    printf("Escreve 'desisto' para sair.\n\n");

    while (1) {
        printf("Palpite ou pergunta: ");
        if (!fgets(buffer, MAX, stdin)) break;

        buffer[strcspn(buffer, "\n")] = 0;  // Remove newline
        write(fd_write, buffer, strlen(buffer) + 1);

        ssize_t n = read(fd_read, buffer, MAX);
        if (n <= 0) {
            perror("Erro ao ler do servidor");
            break;
        }
        
     char *dica_pos = strstr(buffer, "DICA:");
     if (dica_pos) {
       *dica_pos = '\0';  // Termina a string antes da dica
       printf("Servidor: %s\n", buffer);
       printf("DICA: %s\n", dica_pos + 5);  
     } else {
       printf("Servidor: %s", buffer);
     } if (strstr(buffer, "Fim") || strstr(buffer, "Parabens"))
            break;
    }

    close(fd_read);
    close(fd_write);
    return 0;
}
