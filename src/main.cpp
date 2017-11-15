#include <bits/stdc++.h>
using namespace std;

constexpr int ROW = 1 << 5;
constexpr int CENTOR = ROW / 2 * ROW + ROW / 2;
constexpr int MAX_V = 1 << 10;

int V, E, KV, KE, KR;
int s[MAX_V];
int w[MAX_V][MAX_V];
int X[MAX_V];
int* x = X + ROW;
int BEST[MAX_V];
int* best = BEST + ROW;
int vertexes[MAX_V];
int positions[MAX_V];

inline int _abs(int v) {
  int mask = v >> 31;
  return (v + mask) ^ mask;
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
        w[u][v] = t;
        w[v][u] = t;
      }
    }
    scanf("%d%d\n", &KV, &KE);
    KR = sqrt(KV);
  }
  {  // solve
    int bestScore = 0;
    random_device seed_gen;
    mt19937 engine(seed_gen());
    for (int time = 0; time < 100; ++time) {
      for (int i = 0; i < MAX_V; ++i) X[i] = V;
      for (int i = 0; i < V; ++i) vertexes[i] = i;
      shuffle(vertexes, vertexes + V, engine);
      int score = 0, cd = 0, ps = 0;
      for (int i = 0; i < V; ++i) {
        if (ps == 0) {
          if (cd == 0) {
            positions[ps++] = CENTOR;
          } else {
            for (int j = 0; j < cd * 2; ++j) {
              positions[ps++] = CENTOR - cd * ROW - cd + j;
              positions[ps++] = CENTOR - cd * ROW + cd + j * ROW;
              positions[ps++] = CENTOR + cd * ROW + cd - j;
              positions[ps++] = CENTOR + cd * ROW - cd - j * ROW;
            }
          }
          ++cd;
        }
        int vi = 0, pi = 0, value = -1;
        for (int j = 0; j < V - i; ++j) {
          int v = vertexes[j];
          for (int k = 0; k < ps; ++k) {
            int p = positions[k];
            int t = 0;
            t += w[v][x[p - ROW - 1]];
            t += w[v][x[p - ROW]];
            t += w[v][x[p - ROW + 1]];
            t += w[v][x[p - 1]];
            t += w[v][x[p + 1]];
            t += w[v][x[p + ROW - 1]];
            t += w[v][x[p + ROW]];
            t += w[v][x[p + ROW + 1]];
            if (value < t) {
              value = t;
              vi = j;
              pi = k;
            }
          }
        }
        x[positions[pi]] = vertexes[vi];
        vertexes[vi] = vertexes[V - i - 1];
        positions[pi] = positions[--ps];
        score += value;
      }
      if (bestScore < score) {
        bestScore = score;
        memcpy(BEST, X, sizeof(X));
      }
    }
    int minRow = ROW;
    int minCol = ROW;
    for (int i = 0; i < MAX_V; ++i) {
      if (best[i] < V) {
        int r = i & (ROW - 1);
        int c = i >> 5;
        if (minRow > r) minRow = r;
        if (minCol > c) minCol = c;
      }
    }
    for (int i = 0; i < MAX_V; ++i) {
      if (best[i] < V) {
        best[i - minCol * ROW - minRow] = best[i];
        best[i] = V;
      }
    }
    fprintf(stderr, "score = %d\n", bestScore);
  }
  {  // output
    for (int i = 0; i < MAX_V; ++i) {
      if (best[i] < V)
        printf("%d %d\n", best[i] + 1, i / ROW * KR + i % ROW + 1);
    }
  }
}