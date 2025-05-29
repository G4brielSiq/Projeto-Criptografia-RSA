#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32
#include <direct.h>
#define mkdir_p(dir) _mkdir(dir)
#else
#include <sys/stat.h>
#define mkdir_p(dir) mkdir(dir, 0755)
#endif

unsigned long long converter_caractere(char c)
{
    switch (c)
    {
    case 'A':
        return 2;
    case 'B':
        return 3;
    case 'C':
        return 4;
    case 'D':
        return 5;
    case 'E':
        return 6;
    case 'F':
        return 7;
    case 'G':
        return 8;
    case 'H':
        return 9;
    case 'I':
        return 10;
    case 'J':
        return 11;
    case 'K':
        return 12;
    case 'L':
        return 13;
    case 'M':
        return 14;
    case 'N':
        return 15;
    case 'O':
        return 16;
    case 'P':
        return 17;
    case 'Q':
        return 18;
    case 'R':
        return 19;
    case 'S':
        return 20;
    case 'T':
        return 21;
    case 'U':
        return 22;
    case 'V':
        return 23;
    case 'W':
        return 24;
    case 'X':
        return 25;
    case 'Y':
        return 26;
    case 'Z':
        return 27;
    case ' ':
        return 28;
    default:
        return 0;
    }
}

char converter_numero(unsigned long long num)
{
    switch (num)
    {
    case 2:
        return 'A';
    case 3:
        return 'B';
    case 4:
        return 'C';
    case 5:
        return 'D';
    case 6:
        return 'E';
    case 7:
        return 'F';
    case 8:
        return 'G';
    case 9:
        return 'H';
    case 10:
        return 'I';
    case 11:
        return 'J';
    case 12:
        return 'K';
    case 13:
        return 'L';
    case 14:
        return 'M';
    case 15:
        return 'N';
    case 16:
        return 'O';
    case 17:
        return 'P';
    case 18:
        return 'Q';
    case 19:
        return 'R';
    case 20:
        return 'S';
    case 21:
        return 'T';
    case 22:
        return 'U';
    case 23:
        return 'V';
    case 24:
        return 'W';
    case 25:
        return 'X';
    case 26:
        return 'Y';
    case 27:
        return 'Z';
    case 28:
        return ' ';
    default:
        return '?';
    }
}

int eh_primo(unsigned long long n)
{
    if (n <= 1)
        return 0;
    if (n <= 3)
        return 1;
    if (n % 2 == 0 || n % 3 == 0)
        return 0;

    for (unsigned long long i = 5; i * i <= n; i += 6)
    {
        if (n % i == 0 || n % (i + 2) == 0)
            return 0;
    }

    return 1;
}

unsigned long long calcular_mdc(unsigned long long e, unsigned long long a)
{
    while (a != 0)
    {
        unsigned long long temp = a;

        a = e % a;
        e = temp;
    }

    return e;
}

// int mdc(int a, int b) {

//     if (b == 0) {
//         return a;
//     }

//     else {
//         return mdc(b, a % b);
//     }
// }

unsigned long long calcular_inverso_modular(unsigned long long e, unsigned long long a)
{
    long long d = 0, aux_d = 1;
    long long r = (long long)a, aux_r = (long long)e;

    while (aux_r != 0)
    {
        // monta o algoritmo de euclides
        long long q = r / aux_r;
        long long tmp = d - q * aux_d;
        d = aux_d;
        aux_d = tmp;

        //"desfaz o algoritmo de euclides" = monta a congruencia linear
        long long tmp2 = r - q * aux_r;
        r = aux_r;
        aux_r = tmp2;
    }

    if (r > 1)
        return 0;
    if (d < 0)
        d += a;

    return (unsigned long long)d;
}

unsigned long long calcular_potencia_modular(unsigned long long base, unsigned long long exp, unsigned long long mod)
{
    unsigned long long resultado = 1;

    while (exp > 0)
    {
        if (exp % 2 == 1)
        {
            resultado = (resultado * base) % mod;
        }

        base = (base * base) % mod;
        exp /= 2;
    }

    return resultado;
}

void gerar_chave_publica()
{
    unsigned long long p, q, n, a, e, d;

    printf("Digite o valor de P (primo): ");
    scanf("%llu", &p);
    printf("Digite o valor de Q (primo): ");
    scanf("%llu", &q);
    printf("Digite o valor de E (coprimo com (P-1)*(Q-1)): ");
    scanf("%llu", &e);

    if (!eh_primo(p) || !eh_primo(q))
    {
        printf("\nErro: P e Q devem ser primos!\n");
        return;
    }

    n = p * q;
    a = (p - 1) * (q - 1);

    if (calcular_mdc(e, a) != 1)
    {
        printf("\nErro: E deve ser coprimo com a = (P-1)*(Q-1)!\n");
        return;
    }

    d = calcular_inverso_modular(e, a);

    if (d == 0)
    {
        printf("\nErro: não foi possível calcular o inverso modular!\n");
        return;
    }

    mkdir_p("Chaves Publicas");

    FILE *arquivo = fopen("Chaves Publicas/chave_publica.txt", "w");

    if (arquivo)
    {
        fprintf(arquivo, "n = %llu e = %llu", n, e);
        fclose(arquivo);
        printf("\nChave pública salva em Chaves Publicas/chave_publica.txt\n");
    }

    else
    {
        printf("\nErro ao criar arquivo de chave pública!\n");
    }
}

void encriptar()
{
    unsigned long long n, e;
    char mensagem[1000];

    printf("Digite o valor de N: ");
    scanf("%llu", &n);
    printf("Digite o valor de E: ");
    scanf("%llu", &e);
    printf("Digite a mensagem para encriptar: ");
    scanf(" %[^\n]", mensagem);

    mkdir_p("Mensagens");

    FILE *arquivo = fopen("Mensagens/mensagem_encriptada.txt", "w");

    if (arquivo)
    {
        for (int i = 0; mensagem[i] != '\0'; i++)
        {
            unsigned long long caractere = converter_caractere(mensagem[i]);
            unsigned long long caractere_encriptado = calcular_potencia_modular(caractere, e, n);
            fprintf(arquivo, "%llu ", caractere_encriptado);
        }

        fclose(arquivo);
        printf("\nMensagem encriptada salva em Mensagens/mensagem_encriptada.txt\n");
    }

    else
    {
        printf("\nErro ao criar arquivo mensagem_encriptada.txt\n");
    }
}

void desencriptar()
{
    unsigned long long p, q, e, n, a, d;
    char mensagem_encriptada[10000];

    printf("Digite o valor de P: ");
    scanf("%llu", &p);
    printf("Digite o valor de Q: ");
    scanf("%llu", &q);
    printf("Digite o valor de E: ");
    scanf("%llu", &e);

    n = p * q;
    a = (p - 1) * (q - 1);
    d = calcular_inverso_modular(e, a);

    if (d == 0)
    {
        printf("\nErro ao calcular calcular o inverso modular (d)!\n");
        return;
    }

    printf("Digite a mensagem encriptada (números separados por espaço): ");
    scanf(" %[^\n]", mensagem_encriptada);

    mkdir_p("Mensagens");

    FILE *arquivo = fopen("Mensagens/mensagem_desencriptada.txt", "w");

    if (arquivo)
    {
        char *token = strtok(mensagem_encriptada, " ");

        while (token != NULL)
        {
            unsigned long long numero = strtoull(token, NULL, 10);
            unsigned long long resultado = calcular_potencia_modular(numero, d, n);
            char caractere = converter_numero(resultado);
            fprintf(arquivo, "%c", caractere);
            printf("%c", caractere);
            token = strtok(NULL, " ");
        }
        fclose(arquivo);
        printf("\nMensagem desencriptada salva em Mensagens/mensagem_desencriptada.txt\n");
    }

    else
    {
        printf("\nErro ao criar arquivo mensagem_desencriptada.txt\n");
    }
}

int main()
{
    int opcao;

    do
    {
        printf("\nMenu:\n");
        printf("1. Gerar chave pública\n");
        printf("2. Encriptar mensagem\n");
        printf("3. Desencriptar mensagem\n");
        printf("4. Sair\n");
        printf("\nEscolha uma opção: \n");
        scanf("%d", &opcao);

        switch (opcao)
        {
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
            printf("\nSaindo...\n");
            break;
        default:
            printf("\nOpção inválida!\n");
        }

    } while (opcao != 4);

    return 0;
}