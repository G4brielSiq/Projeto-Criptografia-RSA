#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// --------------------------- Funções auxiliares -----------------------------

// Verifica se um número é primo
int eh_primo(int num) {
    if (num < 2) return 0;
    for (int i = 2; i <= sqrt(num); i++) {
        if (num % i == 0)
            return 0;
    }
    return 1;
}

// Calcula o MDC de dois números (Algoritmo de Euclides)
int mdc(int a, int b) {
    while (b != 0) {
        int temp = a % b;
        a = b;
        b = temp;
    }
    return a;
}

// Calcula a exponenciação modular: (base^exp) % mod
int exp_mod(int base, int exp, int mod) {
    int result = 1;
    base = base % mod;
    while (exp > 0) {
        if (exp % 2 == 1)
            result = (result * base) % mod;
        exp = exp >> 1; // exp = exp / 2
        base = (base * base) % mod;
    }
    return result;
}

// Calcula o inverso modular de e módulo totiente usando o Algoritmo de Euclides Estendido
int inverso_modular(int e, int totiente) {
    int t = 0, novo_t = 1;
    int r = totiente, novo_r = e;

    while (novo_r != 0) {
        int quociente = r / novo_r;

        int temp = t;
        t = novo_t;
        novo_t = temp - quociente * novo_t;

        temp = r;
        r = novo_r;
        novo_r = temp - quociente * novo_r;
    }

    if (r > 1) return -1; // e não tem inverso
    if (t < 0) t += totiente;

    return t;
}

// Converte letra (A-Z e espaço) em número (2-28)
int letra_para_numero(char c) {
    if (c >= 'A' && c <= 'Z')
        return c - 'A' + 2;
    else if (c == ' ')
        return 28;
    else
        return -1; // caractere inválido
}

// Converte número (2-28) em letra (A-Z e espaço)
char numero_para_letra(int n) {
    if (n >= 2 && n <= 27)
        return 'A' + (n - 2);
    else if (n == 28)
        return ' ';
    else
        return '?'; // caractere inválido
}

// --------------------------- Etapas principais -----------------------------

// Etapa 1: Gerar chave pública
void gerar_chave_publica() {
    int p, q, e;

    printf("Digite o valor de p (primo): ");
    scanf("%d", &p);
    printf("Digite o valor de q (primo): ");
    scanf("%d", &q);

    if (!eh_primo(p) || !eh_primo(q)) {
        printf("Erro: p e q devem ser números primos.\n");
        return;
    }

    int n = p * q;
    int totiente = (p - 1) * (q - 1);

    printf("Digite o valor de e (1 < e < %d) tal que mdc(e, %d) = 1: ", totiente, totiente);
    scanf("%d", &e);

    if (mdc(e, totiente) != 1) {
        printf("Erro: e deve ser relativamente primo a (p-1)*(q-1).\n");
        return;
    }

    FILE *arquivo = fopen("chave_publica.txt", "w");
    if (!arquivo) {
        printf("Erro ao criar o arquivo.\n");
        return;
    }

    fprintf(arquivo, "%d %d\n", n, e);
    fclose(arquivo);

    printf("Chave pública gerada com sucesso!\n");
}

// Etapa 2: Encriptar mensagem
void encriptar() {
    char mensagem[256];
    int n, e;

    printf("Digite a mensagem (somente letras A-Z e espaço): ");
    getchar(); // limpar buffer
    fgets(mensagem, sizeof(mensagem), stdin);
    mensagem[strcspn(mensagem, "\n")] = '\0'; // remover \n

    printf("Digite a chave pública (n e e): ");
    scanf("%d %d", &n, &e);

    FILE *arquivo = fopen("mensagem_encriptada.txt", "w");
    if (!arquivo) {
        printf("Erro ao criar o arquivo.\n");
        return;
    }

    for (int i = 0; i < strlen(mensagem); i++) {
        char c = mensagem[i];
        int m = letra_para_numero(c);
        if (m == -1) continue;
        int cifra = exp_mod(m, e, n);
        fprintf(arquivo, "%d ", cifra);
    }

    fclose(arquivo);
    printf("Mensagem encriptada com sucesso!\n");
}

// Etapa 3: Desencriptar mensagem
void desencriptar() {
    int p, q, e;

    printf("Digite o valor de p: ");
    scanf("%d", &p);
    printf("Digite o valor de q: ");
    scanf("%d", &q);
    printf("Digite o valor de e: ");
    scanf("%d", &e);

    int n = p * q;
    int totiente = (p - 1) * (q - 1);
    int d = inverso_modular(e, totiente);

    if (d == -1) {
        printf("Erro: Não foi possível calcular o inverso modular.\n");
        return;
    }

    FILE *arquivo = fopen("mensagem_encriptada.txt", "r");
    if (!arquivo) {
        printf("Erro ao abrir o arquivo de mensagem encriptada.\n");
        return;
    }

    FILE *saida = fopen("mensagem_desencriptada.txt", "w");
    if (!saida) {
        printf("Erro ao criar o arquivo de saída.\n");
        return;
    }

    int cifra;
    while (fscanf(arquivo, "%d", &cifra) != EOF) {
        int m = exp_mod(cifra, d, n);
        char letra = numero_para_letra(m);
        fputc(letra, saida);
    }

    fclose(arquivo);
    fclose(saida);

    printf("Mensagem desencriptada com sucesso!\n");
}

// --------------------------- Função principal -----------------------------

int main() {
    int opcao;

    printf("===== SISTEMA DE CRIPTOGRAFIA RSA =====\n");
    printf("1 - Gerar chave pública\n");
    printf("2 - Encriptar mensagem\n");
    printf("3 - Desencriptar mensagem\n");
    printf("Escolha uma opção: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            gerar_chave_publica();
            break;
        case 2:
            encriptar();
            break;
        case 3:
            desencriptar();
            break;
        default:
            printf("Opção inválida.\n");
    }

    return 0;
}