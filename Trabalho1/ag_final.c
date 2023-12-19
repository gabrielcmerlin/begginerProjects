/* Alunos:
    Gabriel da Costa Merlin - 12544420
    Mateus Bernal Leffeck - 13673318
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

const int COEFICIENTE = 6;

int main(void)
{
    // Iniciando as variaveis
    int quant_populacao, quant_geracoes, auxiliar = COEFICIENTE - 1, coeficientes_equacao[COEFICIENTE], melhor_resposta, taxa_mutacao, resposta_equacao, geracaoatual, brk = 0;
    char Vetor_Valores[6] = {'a', 'b', 'c', 'd', 'e', 'f'};
    srand(time(NULL));

    // Deixando as variaves em laços para nunca sair com valor errado
    do
    {
        printf("Informe a quantidade de população inicial:\n");
        scanf("%d", &quant_populacao);
    } while (quant_populacao <= 0);

    do
    {
        printf("Informe quantas gerações teremos:\n");
        scanf("%d", &quant_geracoes);
    } while (quant_geracoes <= 0);

    do
    {
        printf("Agora informe a porcetagem (1 a 100) da taxa de mutação:\n");
        scanf("%d", &taxa_mutacao);
    } while (taxa_mutacao > 100 || taxa_mutacao < 1);

    // Aparecer o valor das constantes na equação;
    printf("Sabendo que a equacao eh:\n ax^5+bx^4+cx^3+dx^2+ex+f = 0\n");
    for (int i = 0; i < 6; i++)
    {
        printf("Informe o valor de %c na equação\n", Vetor_Valores[i]);
        scanf("%d", &coeficientes_equacao[i]);
    }

    // Gerando de forma pseudoaleatória a populacao inicial
    int populacao[quant_populacao], VTemp_Pais[quant_populacao / 2], filhos[quant_populacao / 2];
    long long int pontuacao_positiva[quant_populacao], pontuacao_verdadeira[quant_populacao];

    for (int i = 0; i < quant_populacao; i++)
    {
        populacao[i] = rand() % 1023;
        populacao[i] = populacao[i] - 511;
    }

    // Laço das geraçoes
    for (int i = 0; i < quant_geracoes; i++)
    {
        // Aqui teremos um break para caso ja tenha encontrado a raiz da equacao
        if (brk == 1)
        {
            break;
        }

        geracaoatual = (i + 1);

        // Calculando as pontuacoes para deixar mais facil a organização dos valores
        for (int j = 0; j < quant_populacao; j++)
        {
            pontuacao_verdadeira[j] = 0;

            for (int k = 0; k < COEFICIENTE; k++, auxiliar--)

            {
                pontuacao_verdadeira[j] += pow(populacao[j], auxiliar) * coeficientes_equacao[k];
            }

            // Teremos a pontuacao como modulo e a pontuacao verdadeira, o uso da verdadeira é guardar valores negativos para
            // mostrar o valor negativo correto ao final do AG
            auxiliar = COEFICIENTE - 1;
            pontuacao_positiva[j] = llabs(pontuacao_verdadeira[j]);
        }

        // Manipulacao da populacao
        for (int k = 0; k < quant_populacao; k++)
        {
            // Ordenando por pontuação positiva, pontuacao verdadeira e seu criador (o X) simultaneamente
            for (int p = 0; p < quant_populacao; p++)
            {
                for (long long int l = p, aux1, aux2, aux3; l < quant_populacao; l++)
                {
                    if (pontuacao_positiva[p] > pontuacao_positiva[l])
                    {
                        aux1 = pontuacao_positiva[p];
                        pontuacao_positiva[p] = pontuacao_positiva[l];
                        pontuacao_positiva[l] = aux1;

                        aux2 = populacao[p];
                        populacao[p] = populacao[l];
                        populacao[l] = aux2;

                        aux3 = pontuacao_verdadeira[p];
                        pontuacao_verdadeira[p] = pontuacao_verdadeira[l];
                        pontuacao_verdadeira[l] = aux3;
                    }
                }
            }

            // Verificando o melhor individuo e se a resposta da equacao ja foi encontrada
            if ((i == 0) && (k == 0)){
                melhor_resposta = populacao[0];
                resposta_equacao = pontuacao_verdadeira[0];
            }
            else
            {
                if (llabs(pontuacao_verdadeira[0]) < llabs(resposta_equacao))
                {
                    melhor_resposta = populacao[0];
                    resposta_equacao = pontuacao_verdadeira[0];
                }
            }

            // Caso já tenha sido encontrada, iremos dar o break
            if (resposta_equacao == 0)
            {
                brk++;
                break;
            }

            // Selecionando os valores em porcentagem para pegar os individuos
            int porcentagem[quant_populacao];

            for (int i = 0; i < quant_populacao; i++)
            {
                porcentagem[i] = 100 - ((i * 100) / quant_populacao);
            }
            // Gerando e jogando a moeda para selecionar os individuos
            int moeda_1;

            moeda_1 = rand() % 100;

            // Iremos ver se a porcetagem de cada individuo é maior q a porcetagem da moeda
            for (int i = 0, j = i; i < quant_populacao; i++, j++)
            {
                if (porcentagem[i] > moeda_1)
                {
                    VTemp_Pais[j] = populacao[i];
                }
                else
                {
                    j--;
                }
            }

            // Usar mascaras de bits para assim tirar os menos significativos e mais significativos
            int opbit = 15, opbit2 = 240;

            for (int m = 0; (m + 1) <= quant_populacao / 2; m++)
            {
                int auxiliar2, auxiliar3;
                auxiliar2 = VTemp_Pais[m] & opbit2;
                auxiliar3 = VTemp_Pais[m + 1] & opbit;
                filhos[m] = auxiliar2 | auxiliar3;
            }

            // Proximos dois FOR usados para juntas os pais e os filhos
            int count = 0, count2 = 0;
            for (; count < quant_populacao / 2; count++)
            {
                populacao[count] = filhos[count];
            }

            for (; count2 < quant_populacao / 2; count2++, count++)
            {
                populacao[count] = VTemp_Pais[count2];
            }

            // Conferir se o vetor criado tem tamanho igual à da população, se não criar mais um filho para deixar igual
            if ((count + count2) != quant_populacao)
            {
                int auxiliar2, auxiliar3;
                auxiliar2 = VTemp_Pais[0] & opbit2;
                auxiliar3 = VTemp_Pais[quant_populacao / 2] & opbit;
                populacao[quant_populacao - 1] = auxiliar2 | auxiliar3;
            }

            // Mutação
            int mascara, moeda_2;

            mascara = rand() % 16;

            for (int i = 0; i < quant_populacao; i++)
            {
                moeda_2 = rand() % 101;

                if (moeda_2 < taxa_mutacao)
                {
                    populacao[i] = populacao[i] ^ mascara;
                }
            }
        }
    }

    // A melhor solução com o valor achado
    if (resposta_equacao == 0)
    {
        printf("O valor de uma das raizes (%d) foi encontrado na geracao %d\nE sua resposta na equacao eh: %d\n", melhor_resposta, geracaoatual, resposta_equacao);
    }
    else
    {
        printf("O valor mais proximo da raiz encontrado foi: %d\nE seu resultado na equacao eh: %d\n", melhor_resposta, resposta_equacao);
    }

    return 0;
}