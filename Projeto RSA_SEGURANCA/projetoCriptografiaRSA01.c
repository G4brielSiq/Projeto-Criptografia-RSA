#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

unsigned long long calcular_mdc(unsigned long long a, unsigned long long b)
{
    while (b != 0)
    {
        unsigned long long temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

unsigned long long calcular_inverso_modular(unsigned long long e, unsigned long long phi)
{
    unsigned long long t = 0, new_t = 1;
    unsigned long long r = phi, new_r = e;

    while (new_r != 0)
    {
        unsigned long long quociente = r / new_r;
        unsigned long long temp_t = t - quociente * new_t;
        t = new_t;
        new_t = temp_t;

        unsigned long long temp_r = r - quociente * new_r;
        r = new_r;
        new_r = temp_r;
    }

    if (r > 1)
        return 0; // Inverso não existe

    if (t < 0)
        t += phi;
    return t;
}

unsigned long long converter_caractere(char c)
{
    if (c >= 'a' && c <= 'z')
        return c - 'a' + 2;
    if (c == ' ')
        return 28;
    return 0; // Caractere inválido
}

char converter_numero(unsigned long long num)
{
    if (num >= 2 && num <= 27)
        return (char)(num + 'a' - 2);
    if (num == 28)
        return ' ';
    return '?'; // Caso de erro
}

void gerar_chave_publica()
{
    unsigned long long p, q, n, phi, e, d;

    printf("Digite o valor de P (primo): ");
    scanf("%llu", &p);
    printf("Digite o valor de Q (primo): ");
    scanf("%llu", &q);
    printf("Digite o valor de E (coprimo com (P-1)*(Q-1)): ");
    scanf("%llu", &e);

    if (!eh_primo(p) || !eh_primo(q))
    {
        printf("Erro: P e Q devem ser primos!\n");
        return;
    }

    n = p * q;
    phi = (p - 1) * (q - 1);

    if (calcular_mdc(e, phi) != 1)
    {
        printf("Erro: E deve ser coprimo com (P-1)*(Q-1)!\n");
        return;
    }

    d = calcular_inverso_modular(e, phi);
    if (d == 0)
    {
        printf("Erro: não foi possível calcular o inverso modular!\n");
        return;
    }

    FILE *arquivo = fopen("chave_publica.txt", "w");
    if (arquivo)
    {
        fprintf(arquivo, "%llu %llu", n, e);
        fclose(arquivo);
        printf("Chave pública gerada e salva em chave_publica.txt\n");
    }
    else
    {
        printf("Erro ao criar arquivo chave_publica.txt\n");
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

    FILE *arquivo = fopen("mensagem_encriptada.txt", "w");
    if (arquivo)
    {
        for (int i = 0; mensagem[i] != '\0'; i++)
        {
            unsigned long long caractere = converter_caractere(mensagem[i]);
            unsigned long long caractere_encriptado = calcular_potencia_modular(caractere, e, n);
            fprintf(arquivo, "%llu ", caractere_encriptado);
        }
        fclose(arquivo);
        printf("Mensagem encriptada salva em mensagem_encriptada.txt\n");
    }
    else
    {
        printf("Erro ao criar arquivo mensagem_encriptada.txt\n");
    }
}

void desencriptar()
{
    unsigned long long p, q, e, n, phi, d;
    char mensagem_encriptada[10000];

    printf("Digite o valor de P: ");
    scanf("%llu", &p);
    printf("Digite o valor de Q: ");
    scanf("%llu", &q);
    printf("Digite o valor de E: ");
    scanf("%llu", &e);

    n = p * q;
    phi = (p - 1) * (q - 1);
    d = calcular_inverso_modular(e, phi);
    if (d == 0)
    {
        printf("Erro ao calcular D!\n");
        return;
    }

    printf("Digite a mensagem encriptada (números separados por espaço): ");
    scanf(" %[^\n]", mensagem_encriptada);

    FILE *arquivo = fopen("mensagem_desencriptada.txt", "w");
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
        printf("\nMensagem desencriptada salva em mensagem_desencriptada.txt\n");
    }
    else
    {
        printf("Erro ao criar arquivo mensagem_desencriptada.txt\n");
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
        printf("Escolha uma opção: ");
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
            printf("Saindo...\n");
            break;
        default:
            printf("Opção inválida!\n");
        }
    } while (opcao != 4);

    return 0;
}