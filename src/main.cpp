#include <bits/stdc++.h>
using namespace std;

constexpr double TIME_LIMIT = 1800;
constexpr int MAX_V = 500;
constexpr int MAX_KV = 3600;

unsigned get_random() {
  static unsigned y = 2463534242;
  return y ^= (y ^= (y ^= y << 13) >> 17) << 5;
}

int V, E, KV, KE;
int s[MAX_V];
int e[MAX_V][MAX_V];
int w[MAX_V][MAX_V];
int x[MAX_V];

int main() {
  {  // input
    memset(s, 0, sizeof(s));
    memset(w, 0, sizeof(w));
    scanf("%d%d\n", &V, &E);
    int u, v, t;
    for (int i = 0; i < E; ++i) {
      scanf("%d%d%d\n", &u, &v, &t);
      if (t) {
        --u;
        --v;
        e[u][s[u]++] = v;
        e[v][s[v]++] = u;
        w[u][v] = t;
        w[v][u] = t;
      }
    }
    scanf("%d%d\n", &KV, &KE);
    KV = sqrt(KV);
  }
  {
    // solve
    for (int i = 0; i < V; ++i) {
      x[i] = i;
    }
  }
  {  // output
    for (int i = 0; i < V; ++i) {
      printf("%d %d\n", i + 1, x[i] + 1);
    }
  }
}