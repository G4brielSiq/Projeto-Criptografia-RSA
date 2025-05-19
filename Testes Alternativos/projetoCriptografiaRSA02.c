#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_MSG_LEN 1024

// Função para calcular o MDC
int mdc(int a, int b) {
    if (b == 0) return a;
    return mdc(b, a % b);
}

// Exponenciação modular
long long mod_exp(long long base, long long exp, long long mod) {
    long long res = 1;
    base %= mod;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % mod;
        exp /= 2;
        base = (base * base) % mod;
    }
    return res;
}

// Inverso modular
int inverso_modular(int e, int phi) {
    int t = 0, novo_t = 1;
    int r = phi, novo_r = e;

    while (novo_r != 0) {
        int quoc = r / novo_r;
        int temp = novo_t;
        novo_t = t - quoc * novo_t;
        t = temp;

        temp = novo_r;
        novo_r = r - quoc * novo_r;
        r = temp;
    }

    if (r > 1) return -1;
    if (t < 0) t += phi;
    return t;
}

int char_para_int(char c) {
    if (c == ' ') return 28;
    if (c >= 'A' && c <= 'Z') return c - 'A' + 2;
    return 0;
}

char int_para_char(int i) {
    if (i == 28) return ' ';
    if (i >= 2 && i <= 27) return 'A' + (i - 2);
    return '?';
}

void gerar_chave() {
    int p, q, e;
    printf("Digite dois primos p e q: ");
    scanf("%d %d", &p, &q);
    printf("Digite o expoente e: ");
    scanf("%d", &e);

    int n = p * q;
    int phi = (p - 1) * (q - 1);

    if (mdc(e, phi) != 1) {
        printf("Erro: e nao e relativamente primo de phi(n).\n");
        return;
    }

    FILE *f = fopen("public_key.txt", "w");
    fprintf(f, "%d %d", n, e);
    fclose(f);
    printf("Chave publica salva em 'public_key.txt'\n");
}

void encriptar() {
    char mensagem[MAX_MSG_LEN];
    int n, e;

    printf("Digite a mensagem (apenas A-Z e espaco): ");
    getchar();
    fgets(mensagem, MAX_MSG_LEN, stdin);
    mensagem[strcspn(mensagem, "\n")] = 0;

    printf("Digite n e e da chave publica: ");
    scanf("%d %d", &n, &e);

    FILE *f = fopen("mensagem_encriptada.txt", "w");

    for (int i = 0; mensagem[i] != '\0'; i++) {
        int m = char_para_int(mensagem[i]);
        long long c = mod_exp(m, e, n);
        fprintf(f, "%lld ", c);
    }

    fclose(f);
    printf("Mensagem encriptada salva em 'mensagem_encriptada.txt'\n");
}

void desencriptar() {
    int p, q, e;
    printf("Digite p, q e e: ");
    scanf("%d %d %d", &p, &q, &e);

    int n = p * q;
    int phi = (p - 1) * (q - 1);
    int d = inverso_modular(e, phi);

    if (d == -1) {
        printf("Erro: nao foi possivel calcular o inverso de e.\n");
        return;
    }

    FILE *f = fopen("mensagem_encriptada.txt", "r");
    FILE *saida = fopen("mensagem_desencriptada.txt", "w");

    long long c;
    while (fscanf(f, "%lld", &c) != EOF) {
        long long m = mod_exp(c, d, n);
        char ch = int_para_char((int)m);
        fprintf(saida, "%c", ch);
    }

    fclose(f);
    fclose(saida);
    printf("Mensagem desencriptada salva em 'mensagem_desencriptada.txt'\n");
}

int main() {
    int opcao;
    printf("1 - Gerar chave publica\n2 - Encriptar\n3 - Desencriptar\nEscolha uma opcao: ");
    scanf("%d", &opcao);

    switch (opcao) {
        case 1:
            gerar_chave();
            break;
        case 2:
            encriptar();
            break;
        case 3:
            desencriptar();
            break;
        default:
            printf("Opcao invalida.\n");
    }

    return 0;
}