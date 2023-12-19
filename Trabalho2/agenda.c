/* Alunos:
    Gabriel da Costa Merlin - 12544420
    Mateus Bernal Leffeck - 13673318
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N_PREFERENCIAS 3
#define TAMANHO_PALAVRA 100
#define TAMANHO_UF 4
#define LIMIAR 0.5

typedef struct data_t
{
    int dia;
    int mes;
    int ano;
} data;

typedef struct pessoa_t
{
    char nome[TAMANHO_PALAVRA];
    data nascimento;
    char cidade[TAMANHO_PALAVRA];
    char uf[TAMANHO_UF];
    float preferencias[N_PREFERENCIAS];
    int grupo;
    // inativo_ativo = 0: registro inativo
    // inativo.ativo = 1: registro ativo
    // Usamos isso para facilitar a deleção dos registros. Caso ele esteja inativo, uma série de ações não o levam em conta.
    int inativo_ativo;
} pessoa;

char *ler_palavra(char *palavra);
pessoa *trocar_registros(int i, int j, pessoa *registros);
pessoa *importar_registros(int *total_registros, pessoa *registros);
pessoa *cadastrar_registro(int total_registros, pessoa *registros);
pessoa *editar_registro(int total_registros, pessoa *registros);
pessoa *excluir_registro(int total_registros, pessoa *registros);
void buscar_por_nome(int total_registro, pessoa *registros);
void buscar_por_data(int total_registro, pessoa *registros);
void buscar_por_grupo(int total_registro, pessoa *registros);
pessoa *ordenar_por_nomes(int total_registros, pessoa *registros);
pessoa *ordenar_por_data(int total_registros, pessoa *registros);
pessoa *ordenar_por_grupo(int total_registros, pessoa *registros);
double calcular_distancia(int i, int j, pessoa *registros, double **grupos);
pessoa *agrupar(int total_registros, pessoa *registros);
void exportar_registros(int total_registros, pessoa *registros);

int main(void)
{
    int opcao, total_registros = 0;
    int opcao_secundaria, contador = 0;
    pessoa *registros = NULL;

    // Nesse laço, imprimimos o menu e de acordo com a escolha do usuário, ele cai em um caso do switch case. Este chamará uma função que realizará o que foi solicitado.
    do
    {
        printf("\nMenu:\n");
        printf(" [1] Importar registros de um arquivo\n");
        printf(" [2] Cadastrar novo registro\n");
        printf(" [3] Editar registro\n");
        printf(" [4] Excluir registro\n");
        printf(" [5] Buscar registro\n");
        printf(" [6] Ordenar registros\n");
        printf(" [7] Agrupar registros\n");
        printf(" [8] Exportar registros para um arquivo\n");
        printf(" [0] Sair\n");

        printf("Digite uma opção: ");
        scanf(" %d", &opcao);

        switch (opcao)
        {
        case 1:
            registros = importar_registros(&total_registros, registros);

            break;
        case 2:
            total_registros++;

            registros = cadastrar_registro(total_registros, registros);

            break;
        case 3:
            registros = editar_registro(total_registros, registros);

            break;
        case 4:
            registros = excluir_registro(total_registros, registros);

            break;
        case 5:
            // Como foi solicitado diferentes formas de realizar a mesma operação (buscar registros), incluimos um outro switch case para que tenhamos essa possibilidade de escolha.
            do
            {
                printf("    Submenu:\n");
                printf("     [1] Buscar pelo nome ou parte dele\n");
                printf("     [2] Buscar por data de nascimento\n");
                printf("     [3] Buscar pelo grupo\n");
                printf("     [0] Sair\n");

                printf("    Digite uma opção: ");
                scanf("%d", &opcao_secundaria);
                switch (opcao_secundaria)
                {
                case 1:
                    buscar_por_nome(total_registros, registros);

                    contador++;

                    break;
                case 2:
                    buscar_por_data(total_registros, registros);

                    contador++;

                    break;
                case 3:
                    buscar_por_grupo(total_registros, registros);

                    contador++;

                    break;
                case 0:
                    contador++;

                    printf("\nBusca não realizada!");

                    break;
                default:
                    printf("\nOpcão inválida!\n");

                    break;
                }
            } while (contador == 0);

            break;
        case 6:;
            // Devido ao mesmo motivo do case 5, colocamos um switch case dentro do case 6.
            do
            {
                printf("    Submenu:\n");
                printf("     [1] Ordenar pelo nome\n");
                printf("     [2] Ordenar pela data de nascimento\n");
                printf("     [3] Ordenar pelo grupo\n");
                printf("     [0] Sair\n");

                printf("    Digite uma opção: ");
                scanf(" %d", &opcao_secundaria);

                switch (opcao_secundaria)
                {
                case 1:
                    contador++;

                    registros = ordenar_por_nomes(total_registros, registros);

                    break;
                case 2:
                    contador++;

                    registros = ordenar_por_data(total_registros, registros);

                    break;
                case 3:
                    contador++;

                    registros = ordenar_por_grupo(total_registros, registros);

                    break;
                case 0:
                    contador++;

                    printf("\nOrdenação não realizada!");

                    break;
                default:
                    printf("\nOpção inválida!\n");

                    break;
                }
            } while (contador == 0);

            break;
        case 7:
            registros = agrupar(total_registros, registros);

            break;
        case 8:
            exportar_registros(total_registros, registros);

            break;
        case 0:
            free(registros);

            printf("\nTchau!\n\n");

            break;
        default:
            printf("\nOpção inválida!\n");

            break;
        }

    } while (opcao != 0);

    return 0;
}

char *ler_palavra(char *palavra)
{
    // Função criada para ler palavras digitadas no terminal e transformá-las em strings (nome e cidade) que serão armazenadas nos registros
    char caractere_atual;
    int i;

    scanf(" %c", &caractere_atual);

    for (i = 0; caractere_atual != '\n'; i++)
    {
        palavra = (char *)realloc(palavra, (i + 1) * sizeof(char));
        if (palavra == NULL)
        {
            printf("Erro ao alocar memória\n");

            exit(1);
        }

        palavra[i] = caractere_atual;

        scanf("%c", &caractere_atual);
    }

    // Colocando um '\0' no final da palavra para ser mais fácil manipulá-lo com uma string e também para retirar "impurezas", como um '\r', da palavra
    palavra = (char *)realloc(palavra, (i + 1) * sizeof(char));
    if (palavra == NULL)
    {
        printf("Erro ao alocar memória\n");

        exit(1);
    }
    palavra[i] = '\0';

    return palavra;
}

pessoa *trocar_registros(int i, int j, pessoa *registros)
{
    // Função criada para facilitar e organizar a ordenação dos registros durante as funções de ordenação
    pessoa auxiliar;

    auxiliar = registros[i];
    registros[i] = registros[j];
    registros[j] = auxiliar;

    return registros;
}

pessoa *importar_registros(int *total_registros, pessoa *registros)
{
    // Função criada para realizar a impotação dos registros armazenados em memória secundária (arquivos)
    int numero_registros = 0;

    FILE *arquivo = fopen("arq_backup.bin", "rb");
    if (arquivo == NULL)
    {
        printf("\nErro ao abrir arquivo\n");

        free(registros);

        exit(1);
    }

    pessoa *p_auxiliar = (pessoa *)malloc(1 * sizeof(pessoa));

    // Método para contar quantos registros estão armazenados no arquivo
    while (fread(p_auxiliar, sizeof(pessoa), 1, arquivo))
    {
        numero_registros++;
    }

    fseek(arquivo, 0, SEEK_SET);

    free(p_auxiliar);

    registros = (pessoa *)realloc(registros, numero_registros * sizeof(pessoa));

    // Lendo todos os registros do arquivo e os armazenando num vetor
    fread(registros, sizeof(pessoa), numero_registros, arquivo);

    *total_registros = numero_registros;

    printf("\nImportação realizada com sucesso!\n");

    fclose(arquivo);

    return registros;
}

pessoa *cadastrar_registro(int total_registros, pessoa *registros)
{
    // Função criada para gerar novos registros e cadastrar neles as informações necessárias
    int indice_atual = total_registros - 1;

    registros = (pessoa *)realloc(registros, total_registros * sizeof(pessoa));
    if (registros == NULL)
    {
        printf("\nErro ao alocar memória\n");
    }

    // Inicializando o nome do novo registro com '0'
    memset(registros[indice_atual].nome, '0', TAMANHO_PALAVRA);
    printf("Informe o nome: ");
    char *nome = NULL;
    nome = ler_palavra(nome);
    strcpy(registros[indice_atual].nome, nome);
    free(nome);

    printf("Informe a data de nascimento na forma dd/mm/aaaa (Ex: 10/10/1984): ");
    scanf("%d / %d / %d", &registros[indice_atual].nascimento.dia, &registros[indice_atual].nascimento.mes, &registros[indice_atual].nascimento.ano);

    // Inicializando a cidade do novo registro com '0'
    memset(registros[indice_atual].cidade, '0', TAMANHO_PALAVRA);
    printf("Informe a cidade: ");
    char *cidade = NULL;
    cidade = ler_palavra(cidade);
    strcpy(registros[indice_atual].cidade, cidade);
    free(cidade);

    // Inicializando a UF do novo registro com '0'
    memset(registros[indice_atual].uf, '0', TAMANHO_UF);
    printf("Informe a Unidade Federativa (Ex: GO): ");
    char *uf = NULL;
    uf = ler_palavra(uf);
    strcpy(registros[indice_atual].uf, uf);
    free(uf);

    printf("Informe, entre 0 e 1, a preferência por Samba: ");
    scanf("%f", &registros[indice_atual].preferencias[0]);

    printf("Informe, entre 0 e 1, a preferência por Rock: ");
    scanf("%f", &registros[indice_atual].preferencias[1]);

    printf("Informe, entre 0 e 1, a preferência por MPB: ");
    scanf("%f", &registros[indice_atual].preferencias[2]);

    // grupo = -1: não possui grupo
    registros[indice_atual].grupo = -1;

    // inativo_ativo = 0: registro inativo
    // inativo.ativo = 1: registro ativo
    registros[indice_atual].inativo_ativo = 1;

    printf("\nCadastro realizado com sucesso!\n");

    return registros;
}

pessoa *editar_registro(int total_registros, pessoa *registros)
{
    // Função criada para que possamos realizar mudanças nos registros já existentes, sem precisar apagá-los e registrá-los novamente.
    int registro_selecionado;

    printf("Digite o número do registro a ser editado: ");
    scanf("%d", &registro_selecionado);

    /* Estruturas condicionais para verificar se o registros chamado atende às seguintes condições:
        1 - Registro não excede a quantidade de registros cadastrados (Ex: Se tivermos 5 registros, os registros de número 5 para cima não existirão).
        2 - Registro ainda está ativo. */
    if (registro_selecionado >= total_registros)
    {
        printf("\nRegistro inexistente\n");
    }
    else if (registros[registro_selecionado].inativo_ativo == 0)
    {
        printf("\nRegistro inexistente\n");
    }
    else
    {
        // Inicializando o nome do registro a ser editado com '0'
        memset(registros[registro_selecionado].nome, '0', TAMANHO_PALAVRA);
        printf("Informe o nome: ");
        char *nome = NULL;
        nome = ler_palavra(nome);
        strcpy(registros[registro_selecionado].nome, nome);
        free(nome);

        printf("Informe a data de nascimento na forma dd/mm/aaaa (Ex: 10/10/1984): ");
        scanf("%d / %d / %d", &registros[registro_selecionado].nascimento.dia, &registros[registro_selecionado].nascimento.mes, &registros[registro_selecionado].nascimento.ano);

        // Inicializando a cidade do registro a ser editado com '0'
        memset(registros[registro_selecionado].cidade, '0', TAMANHO_PALAVRA);
        printf("Informe a cidade: ");
        char *cidade = NULL;
        cidade = ler_palavra(cidade);
        strcpy(registros[registro_selecionado].cidade, cidade);
        free(cidade);

        // Inicializando a UF do registro a ser editado com '0'
        memset(registros[registro_selecionado].uf, '0', TAMANHO_UF);
        printf("Informe a Unidade Federativa (Ex: GO): ");
        char *uf = NULL;
        uf = ler_palavra(uf);
        strcpy(registros[registro_selecionado].uf, uf);
        free(uf);

        printf("Informe, entre 0 e 1, a preferência por Samba: ");
        scanf("%f", &registros[registro_selecionado].preferencias[0]);

        printf("Informe, entre 0 e 1, a preferência por Rock: ");
        scanf("%f", &registros[registro_selecionado].preferencias[1]);

        printf("Informe, entre 0 e 1, a preferência por MPB: ");
        scanf("%f", &registros[registro_selecionado].preferencias[2]);

        // Na edição de registros, não poderemos mudar o grupo à que o registro pertence e nem sua situação de atividade (se ele está ativo ou inativo).

        printf("\nEdição realizada com sucesso\n");
    }

    return registros;
}

pessoa *excluir_registro(int total_registros, pessoa *registros)
{
    int registro_selecionado;

    printf("Digite o número do registro a ser excluído: ");
    scanf("%d", &registro_selecionado);

    /* Estruturas condicionais para verificar se o registros chamado atende às seguintes condições:
        1 - Registro não excede a quantidade de registros cadastrados (Ex: Se tivermos 5 registros, os registros de número 5 para cima não existirão).
        2 - Registro ainda está ativo.*/
    if (registro_selecionado >= total_registros)
    {
        printf("\nRegistro inexistente\n");
    }
    else if (registros[registro_selecionado].inativo_ativo == 0)
    {
        printf("\nRegistro inexistente\n");
    }
    else
    {
        // Caso o registro selecionado esteja elegível para deleção, sua atividade irá de 1 (ativo) para 0 (inativo)

        registros[registro_selecionado].inativo_ativo = 0;

        printf("\nExclusão realizada com sucesso\n");
    }

    return registros;
}

void buscar_por_nome(int total_registro, pessoa *registros)
{
    char *aux_nom = NULL;

    printf("Digite o nome a ser buscado: ");
    // Chamar a função de ler palavra para alocá-la dinamicamente.
    aux_nom = ler_palavra(aux_nom);

    int flag = 0;

    for (int i = 0; i < total_registro; i++)
    {
        // Estrutura condicional dentro do laço para, registro a registro, verificarmos se os nomes são iguais.
        if (!strcmp(aux_nom, registros[i].nome) && registros[i].inativo_ativo != 0)
        {
            printf("\nAchado:\n");
            printf("Registro[%d]\n", i);
            printf("%s\n", registros[i].nome);
            printf("\t%02d/%02d/%d\n", registros[i].nascimento.dia, registros[i].nascimento.mes, registros[i].nascimento.ano);
            printf("\t%s - %s\n", registros[i].cidade, registros[i].uf);
            printf("\tSamba: %.2f\n", registros[i].preferencias[0]);
            printf("\tRock:  %.2f\n", registros[i].preferencias[1]);
            printf("\tMPB:   %.2f\n", registros[i].preferencias[2]);
            printf("\tGrupo: %d\n", registros[i].grupo);
            flag++;
        }
        else
        {
            // Se não, usamos strstr para ver se é uma Substring do nome.
            char *sub = strstr(registros[i].nome, aux_nom);

            if (sub != NULL && registros[i].inativo_ativo != 0)
            {
                flag++;
                printf("\nAchado:\n");
                printf("Registro[%d]\n", i);
                printf("%s\n", registros[i].nome);
                printf("\t%02d/%02d/%d\n", registros[i].nascimento.dia, registros[i].nascimento.mes, registros[i].nascimento.ano);
                printf("\t%s - %s\n", registros[i].cidade, registros[i].uf);
                printf("\tSamba: %.2f\n", registros[i].preferencias[0]);
                printf("\tRock:  %.2f\n", registros[i].preferencias[1]);
                printf("\tMPB:   %.2f\n", registros[i].preferencias[2]);
                printf("\tGrupo: %d\n", registros[i].grupo);
            }
        }
    }

    // Usamos a Flag para saber se foi achado ou não um nome nesse laço.
    if (flag == 0)
    {
        printf("\nNão foi achado nenhum nome na lista\n");
    }

    free(aux_nom);
}

void buscar_por_data(int total_registro, pessoa *registros)
{
    int dia, mes, ano, flag = 0;

    printf("Informe a data de nascimento na forma dd/mm/aaaa (Ex: 10/10/1984): ");
    scanf("%d / %d / %d", &dia, &mes, &ano);

    // Laço que procura dentre todos os registros aquele que possui data de nascimento igual à informada.
    for (int i = 0; i < total_registro; i++)
    {
        // Caso haja um registro com a data de nascimento informada, imprimimos ele na tela.
        if ((registros[i].nascimento.dia == dia) && (registros[i].nascimento.mes == mes) && (registros[i].nascimento.ano == ano) && (registros[i].inativo_ativo != 0))
        {
            printf("\nAchado:\n");
            printf("Registro[%d]\n", i);
            printf("%s\n", registros[i].nome);
            printf("\t%02d/%02d/%d\n", registros[i].nascimento.dia, registros[i].nascimento.mes, registros[i].nascimento.ano);
            printf("\t%s - %s\n", registros[i].cidade, registros[i].uf);
            printf("\tSamba: %.2f\n", registros[i].preferencias[0]);
            printf("\tRock:  %.2f\n", registros[i].preferencias[1]);
            printf("\tMPB:   %.2f\n", registros[i].preferencias[2]);
            printf("\tGrupo: %d\n", registros[i].grupo);
            flag++;
        }
    }

    // Caso não exista, mostraremos tal informação na tela.
    if (flag == 0)
    {
        printf("\nNão há ninguém que tenha essa data de nascimento");
    }
}

void buscar_por_grupo(int total_registro, pessoa *registros)
{
    int grupo_buscado, flag = 0;

    printf("Digite o grupo a ser pesquisado: ");
    scanf("%d", &grupo_buscado);

    // Laço que procura dentre todos os registros aqueles que se encaixam no grupo informado.
    for (int i = 0; i < total_registro; i++)
    {
        // Caso haja um registro pertencente ao grupo informado, imprimimos ele na tela.
        if ((registros[i].grupo == grupo_buscado) && registros[i].inativo_ativo != 0)
        {
            printf("\nAchado:\n\n");
            printf("Registro[%d]\n", i);
            printf("%s\n", registros[i].nome);
            printf("\t%02d/%02d/%d\n", registros[i].nascimento.dia, registros[i].nascimento.mes, registros[i].nascimento.ano);
            printf("\t%s - %s\n", registros[i].cidade, registros[i].uf);
            printf("\tSamba: %.2f\n", registros[i].preferencias[0]);
            printf("\tRock:  %.2f\n", registros[i].preferencias[1]);
            printf("\tMPB:   %.2f\n", registros[i].preferencias[2]);
            printf("\tGrupo: %d\n", registros[i].grupo);
            flag++;
        }
    }

    // Caso não exista, mostraremos tal informação na tela.
    if (flag == 0)
    {
        printf("\nNão há ninguém nesse grupo!\n");
    }
}

pessoa *ordenar_por_nomes(int total_registros, pessoa *registros)
{
    // Bubble Sort que ordena os registros existentes com base nos nomes (ordem alfabética).
    for (int i = 0; i < total_registros; i++)
    {
        for (int j = i; j < total_registros; j++)
        {
            if ((strcmp(registros[i].nome, registros[j].nome)) > 0)
            {
                registros = trocar_registros(i, j, registros);
            }
        }
    }

    printf("\nOrdenação realizada com sucesso!\n");

    return registros;
}

pessoa *ordenar_por_data(int total_registros, pessoa *registros)
{
    // Bubble Sort que ordena os registros existentes com base nas datas de nascimento (mais novo ao mais velho).
    for (int i = 0; i < total_registros; i++)
    {
        for (int j = i; j < total_registros; j++)
        {
            // Teremos diversas estruturas condicionais encadeadas, porque queremos ordenar nossos registros em grau de dias, meses e anos.
            if (registros[i].nascimento.ano < registros[j].nascimento.ano)
            {
                registros = trocar_registros(i, j, registros);
            }
            else if ((registros[i].nascimento.ano == registros[j].nascimento.ano) && (registros[i].nascimento.mes < registros[j].nascimento.mes))
            {
                registros = trocar_registros(i, j, registros);
            }
            else if ((registros[i].nascimento.ano == registros[j].nascimento.ano) && (registros[i].nascimento.mes == registros[j].nascimento.mes) && (registros[i].nascimento.dia < registros[j].nascimento.dia))
            {
                registros = trocar_registros(i, j, registros);
            }
        }
    }

    printf("\nOrdenação realizada com sucesso!\n");

    return registros;
}

pessoa *ordenar_por_grupo(int total_registros, pessoa *registros)
{
    // Bubble Sort que ordena os registros existentes com base nos grupos que eles pertencem (menor ao maior).
    for (int i = 0; i < total_registros; i++)
    {
        for (int j = i; j < total_registros; j++)
        {
            if (registros[i].grupo > registros[j].grupo)
            {
                registros = trocar_registros(i, j, registros);
            }
        }
    }

    printf("\nOrdenação realizada com sucesso!\n");

    return registros;
}

double calcular_distancia(int i, int j, pessoa *registros, double **grupos)
{
    double distancia, soma = 0;

    for (int k = 0; k < N_PREFERENCIAS; k++)
    {
        soma += pow((grupos[j][k] - registros[i].preferencias[k]), 2);
    }

    distancia = sqrt(soma);

    return distancia;
}

pessoa *agrupar(int total_registros, pessoa *registros)
{
    /*
    Temos que calcular a distancia eclidiana dos pontos
    O primeiro registro vai ser o lider, colocado como grupo 1
    A criação de uma matriz grupos vai ser importante para saber
    quantos grupos temos, os seus valores e a localização do registro q foi o primeiro a entrar nesse grupo
    */
    double distancia = 0, menor[2] = {5, -1}, **grupos = NULL;
    int quant_grupo = 1;
    // Primeiro grupo criado
    registros[0].grupo = 1;

    // Alocar memoria para a matriz de grupos
    grupos = (double **)calloc(quant_grupo, sizeof(double));
    if (grupos == NULL)
    {
        printf("\nErro alocacao de memoria \n");
        exit(1);
    }

    for (int i = 0; i < quant_grupo; i++)
    {
        grupos[i] = calloc((N_PREFERENCIAS + 1), sizeof(double));
        if (grupos[i] == NULL)
        {
            for (int j = 0; j <= i; j++)
            {
                free(grupos[j]);
            }
            free(grupos);
            printf("\nErro de alocacao");
            exit(1);
        }
    }

    // Preencher com valores do primeiro grupo
    for (int i = 0; i < N_PREFERENCIAS; i++)
    {
        grupos[0][i] = registros[0].preferencias[i];
    }
    grupos[0][N_PREFERENCIAS] = 0;

    /*
        Um for dentro do outro para percorrer nossos registros
        E outro os "cabeça de grupo" que temos cadastrados,
        Assim será verificado todos os grupos
    */
    for (int i = 0; i < total_registros; i++)
    {
        for (int j = 0; j < quant_grupo; j++)
        {
            distancia = calcular_distancia(i, j, registros, grupos);
            // Preencher nossa auxiliar com o menor valor achado e o id do criador dessa distancia
            if (distancia < menor[0])
            {
                menor[0] = distancia;
                menor[1] = grupos[j][N_PREFERENCIAS];
            }
        }
        // Se menor que 0.5, entra no grupo que fez a conta
        if (menor[0] <= LIMIAR)
        {
            registros[i].grupo = registros[(int)menor[1]].grupo;
        }
        else
        {
            //  Se maior, temos que aumentar a quantidade de grupos visto que
            //  Sera necessario o acrescimo de mais um grupo  (pois nao entrou na limiar)
            quant_grupo++;

            grupos = (double **)realloc(grupos, sizeof(double *) * quant_grupo);

            grupos[quant_grupo - 1] = (double *)calloc((N_PREFERENCIAS + 1), sizeof(double)); // realoca a memoria para elementos da nova linha
            for (int j = 0; j < N_PREFERENCIAS; j++)
            {
                grupos[(quant_grupo - 1)][j] = registros[i].preferencias[j];
            }
            grupos[quant_grupo - 1][N_PREFERENCIAS] = i;
            registros[i].grupo = quant_grupo;
        }
        // Menor valor colocado com valor alto para ser mudado com certeza
        menor[0] = 5;
    }

    // Free na matriz utilizada para os grupos
    for (int i = 0; i < quant_grupo; i++)
    {
        free(grupos[i]);
    }
    free(grupos);
    printf("\nAgrupamento realizado com sucesso!\n");
    return registros;
}

void exportar_registros(int total_registros, pessoa *registros)
{
    FILE *arquivo = NULL;

    arquivo = fopen("arq_backup.bin", "wb");
    if (arquivo == NULL)
    {
        printf("\nErro ao abrir arquivo\n");

        free(registros);

        exit(1);
    }

    // Escrevendo todos os registros da memória principal (vetor) na memória secundarária (arquivo).
    for (int i = 0; i < total_registros; i++)
    {
        if (registros[i].inativo_ativo == 1)
        {
            fwrite(&registros[i], sizeof(pessoa), 1, arquivo);
        }
    }

    fclose(arquivo);

    printf("\nExportação realizada com sucesso!\n");
}