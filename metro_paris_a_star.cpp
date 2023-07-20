#include <iostream>
#include <fstream>
#include <vector>
#include <queue>

using namespace std;

int tempo = 0; // tempo total da viagem em minutos
int linha_atual = 0; // linha atual 
int baldeacoes = 0; // numero de baldeacoes

// define a estrutura para representar um nó do grafo
struct Node {
    int id;                // identificador da estação
    float custo_acumulado;   // custo acumulado do caminho até o nó
    float custo_restante;    // estimativa heurística do custo restante até o destino
    float custo_total;       // f_cost = g_cost + h_cost ou seja o custo total
    vector<int> path;      // caminho percorrido até o nó
    bool operator<(const Node &o) const { return custo_total > o.custo_total; }
};

vector<vector<float>> read_adj_matrix(const string& file_name, int n, int m) {
    ifstream file(file_name);
    vector<vector<float>> matrix(n, vector<float>(m));
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) file >> matrix[i][j];
    file.close();
    return matrix;
}

void a_star_metro(int origem, int destino) {

    vector<vector<float>> distancias_diretas = read_adj_matrix("distancias_diretas.txt", 14, 14); 
    vector<vector<float>> distancias_reais = read_adj_matrix("distancias_reais.txt", 14, 14);
    vector<vector<float>> linhas_estacoes = read_adj_matrix("linhas_estacoes.txt", 14, 14);       

    vector<bool> visitados(14, false); // Vetor para verificar nós já visitados
     
    // cria a fronteira
    priority_queue<Node> fronteira; // a fronteira é uma fila de prioridade, que armazena os nós a serem expandidos

    // cria o nó inicial
    Node inicial;
    inicial.id = origem;
    inicial.custo_restante = distancias_diretas[origem-1][destino-1]; 
    inicial.custo_acumulado = 0;
    inicial.custo_total = inicial.custo_acumulado + inicial.custo_restante;
    inicial.path.push_back(origem);
    
    // insere o nó inicial na fronteira
    fronteira.push(inicial);


    // verifica se a fronteira está vazia
    while(!fronteira.empty()){
      Node atual = fronteira.top();
      fronteira.pop();

      // se chegou no destino
      if(atual.id == destino){  

        // contabilizar baldeacoes
        for(int i = 0; i<atual.path.size()-1; i++){
          if(i == 0){ // primeira estacao
            linha_atual = linhas_estacoes[atual.path[i]-1][atual.path[i+1]-1];
          } else { // demais estacoes
            if(linhas_estacoes[atual.path[i]-1][atual.path[i+1]-1] != linha_atual && linha_atual != 0 ){ // se a linha mudou
              linha_atual = linhas_estacoes[atual.path[i]-1][atual.path[i+1]-1];
              tempo+=4;
              baldeacoes++;
            }
          }
        }

       // contabilizar distancias
        for(int i = 0; i<atual.path.size()-1; i++){
          tempo += distancias_reais[atual.path[i]-1][atual.path[i+1]-1] * 2;
        }

        printf("Caminho: ");
        for (int elemento : atual.path) printf(" -> %d", elemento);
        printf("\n");
        printf("Numero de baldeacoes: %d\n", baldeacoes);
        printf("Tempo: %d horas e %d minutos\n\n", tempo/60, tempo%60);
        break;
      }

      // se nao chegou no destino
      else {
          // Ignora os nos ja visitados
          if (visitados[atual.id - 1]) continue; 
          // Marca o no atual como visitado
          else visitados[atual.id - 1] = true; 

        // encontrar os vizinhos
        for(int i = 0; i<14; i++){ // percorre todas as estacoes buscando vizinhos
          // se for vizinho
          if(distancias_reais[i][atual.id-1] != -1.0){ 
            Node Vizinho;
            Vizinho.id = i+1;
            // calcula o custo do vizinho, que é o custo do nó atual mais o custo da aresta que leva ao vizinho
            Vizinho.custo_acumulado = atual.custo_acumulado + distancias_reais[i][atual.id-1];
            Vizinho.custo_restante = distancias_diretas[i][destino-1];
            Vizinho.custo_total = Vizinho.custo_acumulado + Vizinho.custo_restante;
            Vizinho.path = atual.path;
            Vizinho.path.push_back(Vizinho.id);
            fronteira.push(Vizinho);
          }
        } 
      }
    }

}

int main(void){
                                                                                               
    int origem, destino;

    printf("\nInsira a origem e o destino (O D): ");
    scanf("%d %d", &origem, &destino);

    a_star_metro(origem, destino); 

}
