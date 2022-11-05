// Практика по ДМ.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <string>
#include <fstream>
using namespace std;
#define M 500

struct struct_edge {
    int v;
    struct_edge* n;
};

typedef struct_edge* edge;
struct_edge pool[M * M * 2];
edge top = pool, adj[M];
int V, E, match[M], qh, qt, q[M], father[M], base[M];
bool path[M], flower[M];


void add_edge(int u, int v) {

    top->v = v, top->n = adj[u], adj[u] = top++;
    top->v = u, top->n = adj[v], adj[v] = top++;
}

//проверяет посещена ли V или E
int LCA(int root, int u, int v) {
    static bool used[M];
    memset(used, 0, sizeof(used));
    while (true) {
        used[u = base[u]] = true;
        if (u == root) {
            break;
        }
        u = father[match[u]];
    }

    while (true) {
        if (used[v = base[v]]) {
            return v;
        }
        else {
            v = father[match[v]];
        }
    }
}

//находит вершины и смежные им ребра
void mark_blossom(int lca, int u) {
    while (base[u] != lca) {
        int v = match[u];
        flower[base[u]] = flower[base[v]] = true;
        u = father[v];
        if (base[u] != lca) {
            father[u] = v;
        }
    }
}

//если V не насыщена и не посещена то добавляем в увелич путь
void blossom_contraction(int s, int u, int v) {
    int lca = LCA(s, u, v);
    memset(flower, 0, sizeof(flower));
    mark_blossom(lca, u);
    mark_blossom(lca, v);
    if (base[u] != lca) {
        father[u] = v;
    }
    if (base[v] != lca) {
        father[v] = u;
    }
    for (int u = 0; u < V; u++) {
        if (flower[base[u]]) {
            base[u] = lca;
            if (!path[u]) {
                path[q[++qt] = u] = true;
            }
        }
    }
}

//Перебирает все E из V если обнаружен цикл четной длины, то сжимает.Если пришли в свободную вершину, 
// то возвращаем вершину.Если пришли в несвободную вершину, то добавляем в очередь смежную ей в паросочетании.
int find_augmenting_path(int s) {
    memset(path, 0, sizeof(path));
    memset(father, -1, sizeof(father));
    for (int i = 0; i < V; i++) {
        base[i] = i;
    }
    path[q[qh = qt = 0] = s] = true;
    while (qh <= qt) {
        int u = q[qh++];
        for (edge e = adj[u]; e; e = e->n) {
            int v = e->v;
            if (base[u] != base[v] && match[u] != v) {
                if ((v == s) || (match[v] != -1 && father[match[v]] != -1)) {
                    blossom_contraction(s, u, v);
                }

                else {
                    if (father[v] == -1) {
                        father[v] = u;
                        if (match[v] == -1) {
                            return v;
                        }
                        else {
                            if (!path[match[v]]) {
                                path[q[++qt] = match[v]] = true;
                            }
                        }
                    }
                }
            }
        }
    }
    return -1;
}

//Функция ищет смежную вершину в паросочетании, если такой нет, то возвращает значение -1.
int augment_path(int s, int t) {
    int u = t, v, w;
    while (u != -1) {
        v = father[u];
        w = match[v];
        match[v] = u;
        match[u] = v;
        u = w;
    }
    return t != -1;
}

//Функция находит количество ребер в наибольшем паросочетании.
int edmonds() {
    int kolvo = 0;
    memset(match, -1, sizeof(match));
    for (int u = 0; u < V; u++) {
        if (match[u] == -1) {
            kolvo += augment_path(u, find_augmenting_path(u));
        }
    }
    return kolvo;
}


int main()
{
    ifstream input("input.txt");
    ofstream output("output.txt");
    setlocale(LC_ALL, "Rus");
    int u, v;
    input >> V >> E;

    while (E--) {
        input >> u >> v;
        add_edge(u - 1, v - 1);
    }
    output << edmonds() << endl;

   
    for (int i = 0; i < V; i++) {
        if (i < match[i])
        {
            output << i + 1 << " " << match[i] + 1 << endl;
        }
    }
    input.close();
    output.close();
    return 0;
}