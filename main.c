#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// PROTÓTIPOS DAS FUNÇÕES DE TESTE
int **importa_matriz(char *nome_arquivo, int **matriz, int *tamanho);
void mostra_matriz(int **matriz, int tamanho);
int verifica_simetria(int **matriz, int tamanho);
int verifica_loop(int **matriz, int tamanho);
int verifica_multiAresta(int **matriz, int tamanho);

// PROTÓTIPOS DAS FUNÇÕES DAS QUESTÕES
void maior_grau(int **matriz, int tamanho);
void salva_graus(int **matriz, int tamanho, char *nome_arquivo);
void vertices_isolados(int **matriz, int tamanho);
void grafo_gerador(int **matriz, int tamanho, char *nome_arquivo);
int *conexao_vertices(int **matriz, int *vetor_caminho, int tamanho, int inicio, int fim);


int main(void) {
    int linha, tamanho = 0, **matriz = NULL;
    
    /* ============================================================ */
    /* IMPORTAÇÃO DA MATRIZ ADJACENTE DO GRAFO */
    matriz = importa_matriz("dados_matriz.txt", matriz, &tamanho);
    if (tamanho > 0) {
        /* ============================================================ */
        /* ANÁLISES SOBRE O GRAFO */
        printf("O GRAFO APRESENTA:\n");
        printf(" - %d Vertices\n", tamanho);
        printf("%s", verifica_simetria(matriz, tamanho) ? " - Simetria\n" : "");
        printf("%s", verifica_loop(matriz, tamanho) ? " - Vertices com loop\n" : "");
        printf("%s", verifica_multiAresta(matriz, tamanho) ? " - Multi-arestas\n" : "");
        
        /* ============================================================ */
        /* QUESTÕES APLICÁVEIS */

        // 01: ENCONTRAR O MAIOR GRAU
        maior_grau(matriz, tamanho);

        // 02: CALCULAR O GRAU DE TODOS OS VÉRTICES E SALVAR EM ARQUIVO
        salva_graus(matriz, tamanho, "dados_grafos_graus.txt");   

        // 03: ENCONTRAR VERTICES ISOLADOS
        vertices_isolados(matriz, tamanho);
        
        // 10: GERAR UM SUBGRAFO INDUZIDO, COM VÉRTICES MÚLTIPLOS DE 5
        grafo_gerador(matriz, tamanho, "dados_grafo_gerador.txt");

        // 11: ENCONTRAR O MAIOR CLIQUE EXISTENTE NA MATRIZ
        

        // 12: ENCONTRAR UM CAMINHO ENTRE O PRIMEIRO E O ÚLTIMO VÉRTICE
        int *caminho_entre_vetores = NULL;
        caminho_entre_vetores = conexao_vertices(matriz, caminho_entre_vetores, tamanho, 0, tamanho-1);
        // Exibe as informações no Terminal
        printf("\n----------------------------------------");
        printf("\nConexao do vertices %d e %d:\n", 0, tamanho-1);

        if (caminho_entre_vetores != NULL) {
            printf("%d", caminho_entre_vetores[0]);
            for (linha = 1; caminho_entre_vetores[linha] != -1; linha++) {
                printf(" -> %d", caminho_entre_vetores[linha]);
            }
            printf("\n");
        } else {
            printf("Nao existe caminho entre os vertices.\n");
        }
        free(caminho_entre_vetores);    
    }


    /* ============================================================ */
    /* QUESTÕES NÃO APLICÁVEIS */

    // 04: ENCONTRAR VÉRTICE SUMIDOURO
    // 05: ENCONTRAR VÉRTICE FONTE
    // 06: CALCULAR GRAU DE RECEPÇÃO E EMISSÃO E SALVAR EM ARQUIVO
    // 07: GERAR GRAFO COMPLEMENTAR
    // 08: GERAR GRAFO INVERTIDO E SALVAR EM ARQUIVO
    // 09: SALVAR GRAFO COMPLEMENTAR EM ARQUIVO


    // Libera a memória da matriz, antes de terminar a execução:
    for (linha = 0; linha < tamanho; linha++) {
        free(matriz[linha]);
    }
    free(matriz);

    return 0;
}

// FUNÇÕES DE TESTE
int **importa_matriz(char *nome_arquivo, int **matriz, int *tamanho)
{
    FILE *arquivo;
    int tamanho_matriz, tamanho_linha;
    int linha, coluna;

    arquivo = fopen(nome_arquivo, "rt");

    fscanf(arquivo, "%d", &tamanho_matriz); /* lê o tamanho da matriz */
    while (fgetc(arquivo) != '\n'); /* pula para a próxima linha */
    
    if (tamanho_matriz > 0) {
        matriz = (int**)malloc(tamanho_matriz*sizeof(int*));
        if (matriz == NULL) exit(1);
        for (linha = 0; linha < tamanho_matriz; linha++) {
            matriz[linha] = (int*)malloc(tamanho_matriz*sizeof(int));
            if (matriz[linha] == NULL) exit(1);
        }
    }

    while (!feof(arquivo)) {
        for (linha = 0; linha < tamanho_matriz; linha++) {
            for (coluna = 0; coluna < tamanho_matriz; coluna++) {
                fscanf(arquivo, "%d", &matriz[linha][coluna]);
                // printf("%d ", matriz[linha][coluna]);
            }
            while (fgetc(arquivo) != '\n');
            // printf("\n");
        }
        if (linha == tamanho_matriz) break;
    }

    fclose(arquivo);
    (*tamanho) = tamanho_matriz;

    return matriz;
}

void mostra_matriz(int **matriz, int tamanho)
{
    int linha, coluna;
    for (linha = 0; linha < tamanho; linha++) {
        for (coluna = 0; coluna < tamanho; coluna++) {
            printf(" %d ", matriz[linha][coluna]);
        }
        printf("\n");
    }
}

int verifica_simetria(int **matriz, int tamanho)
{
    int linha, coluna;
    for (linha = 0; linha < tamanho; linha++) {
        for (coluna = 0; coluna < tamanho; coluna++) {
            if (matriz[linha][coluna] != matriz[coluna][linha])
                return 0;
        }
    }
    return 1;
}

int verifica_loop(int **matriz, int tamanho)
{
    int linha;
    for (linha = 0; linha < tamanho; linha++) {
        if (matriz[linha][linha] != 0)
            return 1;   /* possui loop */
    }
    return 0;   /* não possui loop */
}

int verifica_multiAresta(int **matriz, int tamanho)
{
    int linha, coluna;
    for (linha = 0; linha < tamanho; linha++) {
        for (coluna = (linha+1); coluna < tamanho; coluna++)
        {
            if (matriz[linha][coluna] > 1)
                return 1;   /* possui multi-arestas */
        }
    }
    return 0;   /* não possui multi-arestas */
}


// QUESTÕES:
// 01: ENCONTRAR O MAIOR GRAU
void maior_grau(int **matriz, int tamanho)
{
    int grau_maior = 0, grau_vertice;

    // Busca o maior grau
    int linha, coluna;
    for (linha = 0; linha < tamanho; linha++) {
        grau_vertice = 0;
        for (coluna = 0; coluna < tamanho; coluna++) {
            grau_vertice = grau_vertice + matriz[linha][coluna];
        }

        // Atualiza o maior grau
        if (grau_vertice > grau_maior) {
            grau_maior = grau_vertice;
        }
    }

    // Exibe as informações no Terminal
    printf("\n----------------------------------------");
    printf("\nMaior grau: %d\n", grau_maior);
    printf("\nVertice(s) com maior grau:\n");
    
    // Busca os vértices com o maior grau:
    for (linha = 0; linha < tamanho; linha++) {
        grau_vertice = 0;
        for (coluna = 0; coluna < tamanho; coluna++) {
            grau_vertice = grau_vertice + matriz[linha][coluna];
        }
        
        // Imprime vértices:
        if (grau_vertice == grau_maior) {
            printf("%d ", linha);
        }
    }
    printf("\n");
}    

// 02: CALCULAR O GRAU DE TODOS OS VÉRTICES E SALVAR EM ARQUIVO
void salva_graus(int **matriz, int tamanho, char *nome_arquivo)
{
    // Abre o arquivo
    FILE *arquivo = fopen(nome_arquivo, "wt");

    // Escreve o cabeçalho
    fprintf(arquivo, "%7s\t%4s\n", "VERTICE", "GRAU");

    // Calcula o grau de cada vértice e salva no arquivo
    int linha, coluna, grau_vertice;
    for (linha = 0; linha < tamanho; linha++) {
        grau_vertice = 0;
        for (coluna = 0; coluna < tamanho; coluna++) {
            grau_vertice = grau_vertice + matriz[linha][coluna];
        }

        fprintf(arquivo, "%7d\t%4d\n", linha, grau_vertice);
    }

    fclose(arquivo);
}

// 03: ENCONTRAR VERTICES ISOLADOS
void vertices_isolados(int **matriz, int tamanho)
{
    int grau_vertice, contador_isolados = 0;

    // Busca e contabiliza os vértices isolados
    int linha, coluna;
    for (linha = 0; linha < tamanho; linha++) {
        grau_vertice = 0;
        for (coluna = 0; coluna < tamanho; coluna++) {
            grau_vertice = grau_vertice + matriz[linha][coluna];
        }

        // Verifica se é isolado
        if (grau_vertice == 0) {
            contador_isolados++;
        }
    }

    // Exibe as informações no terminal
    printf("\n----------------------------------------");
    printf("\nForam encontrados %d vertice(s) isolado(s):\n", contador_isolados);

    // Busca os vértices isolados
    for (linha = 0; linha < tamanho; linha++) {
        grau_vertice = 0;
        for (coluna = 0; coluna < tamanho; coluna++) {
            grau_vertice = grau_vertice + matriz[linha][coluna];
        }
        
        // Imprime vértice
        if (grau_vertice == 0) {
            printf("%d ", linha);
        }
    }
    printf("\n");
}

// 10: GERAR UM SUBGRAFO INDUZIDO, COM VÉRTICES MÚLTIPLOS DE 5
void grafo_gerador(int **matriz, int tamanho, char *nome_arquivo)
{
    FILE *arquivo = fopen(nome_arquivo, "wt");

    // Contabiliza os vértices multiplos de 5
    int linha, coluna, contador_multiplos = 0;
    for (linha = 0; linha < tamanho; linha++) {
        if (linha % 5 == 0) {
            contador_multiplos++;
        }
    }

    // Salva tamanho do subgrafo    
    fprintf(arquivo, "%d %d\n", contador_multiplos, contador_multiplos);

    // Gera a matriz do subgrafo com os vértices múltiplos de 5:
    for (linha = 0; linha < tamanho; linha++) {
        if (linha % 5 == 0) {
            for (coluna = 0; coluna < tamanho; coluna++) {
                if (coluna % 5 == 0) {
                    fprintf(arquivo, "%d ", matriz[linha][coluna]);
                }
            }
            fprintf(arquivo, "\n");
        }
    }

    fclose(arquivo);
}

// 11: ENCONTRAR O MAIOR CLIQUE EXISTENTE NA MATRIZ


// 12: ENCONTRAR UM CAMINHO ENTRE O PRIMEIRO E O ÚLTIMO VÉRTICE
int *conexao_vertices(int **matriz, int *vetor_caminho, int tamanho, int inicio, int fim)
{
    // Tipo estático para não repetir inicialização em caso se entrar na recursão
    static int id_caminho = 0;
    int id_caminho_origem;

    if (matriz == NULL) /* matriz vazia */
        return NULL;

    if (id_caminho == 0) {
        vetor_caminho = (int*)malloc(tamanho+1*sizeof(int));
        if (vetor_caminho == NULL) return NULL;
    }
    
    vetor_caminho[id_caminho] = inicio; /* adiciona vértice no caminho */
    id_caminho++;
    id_caminho_origem = id_caminho;    /* salva o vértice de origem dessa recursão */

    if (inicio == fim)  {   /* matriz 1x1 */
        vetor_caminho[id_caminho] = -1;
        return vetor_caminho;
    }

    if (matriz[inicio][fim] >= 1) { /* há conexão direta entre o primeiro e o último vértice */
        vetor_caminho[id_caminho] = fim;
        id_caminho++;
        vetor_caminho[id_caminho] = -1; /* marcador de parada no fim */
        return vetor_caminho;
    }

    // verifica os vértices vizinhos, recursivamente:
    int i, *caminho_temp;
    for (i = inicio+1; i <= fim; i++) {
        if (matriz[inicio][i] >= 1) {
            caminho_temp = conexao_vertices(matriz, vetor_caminho, tamanho, i, fim);

            if (caminho_temp != NULL) { /* encontrou uma conexão */

                vetor_caminho = caminho_temp;
                id_caminho = 0;
                
                return vetor_caminho;         /* finaliza busca */
            } 

            else { /* entrou numa ramificação que não se conecta ao último vértice */
                id_caminho = id_caminho_origem;    /* índice retorna ao vértice de origem do ramo */
                /* a busca continua */
            }
        }
    }
    
    /* Não há um caminho ligando o primeiro e o último vértice */
    return NULL;
}
