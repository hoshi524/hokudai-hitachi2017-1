#include <bits/stdc++.h>
using namespace std;

constexpr double TIME_LIMIT = 1800;
constexpr int MAX_KVV = 6400;
constexpr int MAX_KV = 60;
constexpr int MAX_V = 505;

unsigned get_random() {
  static unsigned y = 2463534242;
  return y ^= (y ^= (y ^= y << 13) >> 17) << 5;
}

int V, E, KV, KE;
int s[MAX_V];
int e[MAX_V][MAX_V];
int w[MAX_V][MAX_V];
int X[MAX_KVV];
int* x = X + MAX_KV;
int BEST[MAX_KVV];
int* best = BEST + MAX_KV;
bool used[MAX_V];

inline int _abs(int v) {
  int mask = v >> 31;
  return (v + mask) ^ mask;
}

inline bool connect(int i, int j) {
  return _abs(i % KV - j % KV) < 2 && _abs(i / KV - j / KV) < 2;
}

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
  {  // solve
    int score = 0;
    memset(used, false, sizeof(used));
    for (int i = 0; i < MAX_KVV; ++i) {
      X[i] = V;
    }
    int s = 0;
    for (int i = 0; i < V; ++i) {
      int n = 0, v = -1;
      for (int j = 0; j < V; ++j) {
        if (used[j]) continue;
        int t = 0;
        auto add = [&](int i, int j) {
          if (connect(i, j)) t += w[x[i]][x[j]];
        };
        x[i] = j;
        add(i, i - 1);
        add(i, i - KV);
        add(i, i - KV - 1);
        add(i, i - KV + 1);
        if (v < t) {
          v = t;
          n = j;
        }
      }
      used[n] = true;
      x[i] = n;
      s += v;
    }
    if (score < s) {
      score = s;
      memcpy(BEST, X, sizeof(X));
    }
    fprintf(stderr, "score = %d\n", score);
  }
  {  // output
    KV = KV * KV;
    for (int i = 0; i < KV; ++i) {
      if (best[i] < V) printf("%d %d\n", best[i] + 1, i + 1);
    }
  }
}