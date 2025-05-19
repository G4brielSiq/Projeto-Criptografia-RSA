#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir_p(dir) _mkdir(dir)
#else
#include <unistd.h>
#define mkdir_p(dir) mkdir(dir, 0777)
#endif

void criar_pastas() {
    mkdir_p("Chaves publicas");
    mkdir_p("Mensagens");
}

int calcular_primo(mpz_t n) {
    if (mpz_cmp_ui(n, 1) <= 0)
        return 0;
    if (mpz_cmp_ui(n, 2) == 0 || mpz_cmp_ui(n, 3) == 0)
        return 1;
    if (mpz_even_p(n) || mpz_divisible_ui_p(n, 3))
        return 0;

    return mpz_probab_prime_p(n, 25);
}

void gerar_chave_publica() {
    char p_str[1000], q_str[1000], e_str[1000];
    mpz_t n, produtopq, p, q, e, p1, q1, mdc;

    printf("Digite o valor de P (primo): ");
    scanf("%999s", p_str);
    printf("Digite o valor de Q (primo): ");
    scanf("%999s", q_str);
    printf("Digite o valor de E (coprimo com (P-1)*(Q-1)): ");
    scanf("%999s", e_str);

    mpz_inits(n, produtopq, p, q, e, p1, q1, mdc, NULL);
    mpz_set_str(p, p_str, 10);
    mpz_set_str(q, q_str, 10);
    mpz_set_str(e, e_str, 10);

    if (calcular_primo(p) == 0 || calcular_primo(q) == 0) {
        printf("Erro: P e Q devem ser números primos!\n");
        mpz_clears(n, produtopq, p, q, e, p1, q1, mdc, NULL);
        return;
    }

    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q1, q, 1);
    mpz_mul(produtopq, p1, q1);
    mpz_gcd(mdc, e, produtopq);

    if (mpz_cmp_ui(mdc, 1) != 0) {
        printf("Erro: E deve ser coprimo com (P-1)*(Q-1)!\n");
        mpz_clears(n, produtopq, p, q, e, p1, q1, mdc, NULL);
        return;
    }

    mpz_mul(n, p, q);

    criar_pastas();  // Garante que a pasta exista
    FILE *arquivo = fopen("Chaves publicas/chave_publica.txt", "w");
    if (arquivo) {
        gmp_fprintf(arquivo, "%Zd %Zd", n, e);
        fclose(arquivo);
        printf("Chave pública salva em Chaves publicas/chave_publica.txt\n");
    } else {
        printf("Erro ao criar arquivo de chave pública\n");
    }

    mpz_clears(n, produtopq, p, q, e, p1, q1, mdc, NULL);
}

void encriptar() {
    char n_str[1000], e_str[1000], mensagem[1000];
    mpz_t n, e;

    printf("Digite o valor de N: ");
    scanf("%999s", n_str);
    printf("Digite o valor de E: ");
    scanf("%999s", e_str);
    printf("Digite a mensagem para encriptar: ");
    scanf(" %[^\n]", mensagem);

    mpz_inits(n, e, NULL);
    mpz_set_str(n, n_str, 10);
    mpz_set_str(e, e_str, 10);

    criar_pastas();
    FILE *arquivo = fopen("Mensagens/mensagem_encriptada.txt", "w");
    if (arquivo) {
        for (int i = 0; mensagem[i] != '\0'; i++) {
            mpz_t caractere_encriptado;
            mpz_init(caractere_encriptado);
            mpz_set_ui(caractere_encriptado, (unsigned char)mensagem[i]);
            mpz_powm(caractere_encriptado, caractere_encriptado, e, n);
            gmp_fprintf(arquivo, "%Zd ", caractere_encriptado);
            mpz_clear(caractere_encriptado);
        }
        fclose(arquivo);
        printf("Mensagem encriptada salva em Mensagens/mensagem_encriptada.txt\n");
    } else {
        printf("Erro ao criar arquivo de mensagem encriptada\n");
    }

    mpz_clears(n, e, NULL);
}

void desencriptar() {
    char p_str[1000], q_str[1000], e_str[1000];
    mpz_t p, q, e, p1, q1, produtopq, d, n;

    printf("Digite o valor de P: ");
    scanf("%999s", p_str);
    printf("Digite o valor de Q: ");
    scanf("%999s", q_str);
    printf("Digite o valor de E: ");
    scanf("%999s", e_str);

    mpz_inits(p, q, e, p1, q1, produtopq, d, n, NULL);
    mpz_set_str(p, p_str, 10);
    mpz_set_str(q, q_str, 10);
    mpz_set_str(e, e_str, 10);

    mpz_sub_ui(p1, p, 1);
    mpz_sub_ui(q1, q, 1);
    mpz_mul(produtopq, p1, q1);
    mpz_invert(d, e, produtopq);
    if (mpz_cmp_ui(d, 0) < 0) {
        mpz_add(d, d, produtopq);
    }
    mpz_mul(n, p, q);

    printf("Digite a mensagem encriptada (números separados por espaço): ");
    char mensagem[10000];
    scanf(" %[^\n]", mensagem);

    criar_pastas();
    FILE *arquivo = fopen("Mensagens/mensagem_desencriptada.txt", "w");
    if (arquivo) {
        char *token = strtok(mensagem, " ");
        while (token != NULL) {
            mpz_t numero, resultado;
            mpz_inits(numero, resultado, NULL);
            mpz_set_str(numero, token, 10);
            mpz_powm(resultado, numero, d, n);

            unsigned long char_val = mpz_get_ui(resultado);
            fprintf(arquivo, "%c", (char)char_val);
            printf("%c", (char)char_val);

            mpz_clears(numero, resultado, NULL);
            token = strtok(NULL, " ");
        }
        fclose(arquivo);
        printf("\nMensagem desencriptada salva em Mensagens/mensagem_desencriptada.txt\n");
    } else {
        printf("Erro ao criar arquivo de mensagem desencriptada\n");
    }

    mpz_clears(p, q, e, p1, q1, produtopq, d, n, NULL);
}

int main() {
    int opcao;
    do {
        printf("\nMenu:\n");
        printf("1. Gerar chave pública\n");
        printf("2. Encriptar mensagem\n");
        printf("3. Desencriptar mensagem\n");
        printf("4. Sair\n");
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
            case 4:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 4);

    return 0;
}