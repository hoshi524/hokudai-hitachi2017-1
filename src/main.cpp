#include <bits/stdc++.h>
using namespace std;

constexpr int MAX_ROW = 1 << 5;
constexpr int MAX_V = 1 << 10;

int V, E, R, KV, KE, KR;
int s[MAX_V];
int e[MAX_V][MAX_V];
int w[MAX_V][MAX_V];
int X[MAX_V];
int* x = X + MAX_ROW;
int BEST[MAX_V];
int* best = BEST + MAX_ROW;
int vertexes[MAX_V];

inline int _abs(int v) {
  int mask = v >> 31;
  return (v + mask) ^ mask;
}

inline bool connect(int i, int j) { return _abs(i % R - j % R) < 2; }

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
    KR = sqrt(KV);
    R = sqrt(V);
    if (R * R < V) ++R;
  }
  {  // solve
    int score = 0;
    random_device seed_gen;
    mt19937 engine(seed_gen());
    for (int time = 0; time < 100; ++time) {
      for (int i = 0; i < MAX_V; ++i) X[i] = V;
      for (int i = 0; i < V; ++i) vertexes[i] = i;
      shuffle(vertexes, vertexes + V, engine);
      int s = 0;
      for (int i = 0; i < V; ++i) {
        int n = 0, v = -1;
        for (int j = 0; j < V - i; ++j) {
          int t = 0;
          auto add = [&](int i, int j) {
            if (connect(i, j)) t += w[x[i]][x[j]];
          };
          x[i] = vertexes[j];
          add(i, i - 1);
          add(i, i - R);
          add(i, i - R - 1);
          add(i, i - R + 1);
          if (v < t) {
            v = t;
            n = j;
          }
        }
        x[i] = vertexes[n];
        vertexes[n] = vertexes[V - i - 1];
        s += v;
      }
      if (score < s) {
        score = s;
        memcpy(BEST, X, sizeof(X));
      }
    }
    fprintf(stderr, "score = %d\n", score);
  }
  {  // output
    for (int i = 0; i < MAX_V; ++i) {
      if (best[i] < V) printf("%d %d\n", best[i] + 1, i / R * KR + i % R + 1);
    }
  }
}