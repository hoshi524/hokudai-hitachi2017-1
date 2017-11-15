#include <bits/stdc++.h>
using namespace std;

constexpr int ROW = 1 << 6;
constexpr int MAX_V = ROW * ROW;

unsigned get_random() {
  static unsigned y = 2463534242;
  return y ^= (y ^= (y ^= y << 13) >> 17) << 5;
}

double get_random_double() { return (double)get_random() / UINT_MAX; }

int V, E, KV, KE, KR;
int s[MAX_V];
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
        w[u][v] = t;
        w[v][u] = t;
      }
    }
    scanf("%d%d\n", &KV, &KE);
    KR = sqrt(KV);
  }
  {  // solve
    int r = sqrt(V);
    if (r * r < V) ++r;
    int n = 0;
    for (int i = 0; i < MAX_V; ++i) x[i] = V;
    for (int i = 1; i <= r && n < V; ++i) {
      for (int j = 1; j <= r && n < V; ++j) {
        x[i * ROW + j] = n++;
      }
    }
    auto value = [](int p) {
      int v = 0;
      v += w[x[p]][x[p - ROW - 1]];
      v += w[x[p]][x[p - ROW]];
      v += w[x[p]][x[p - ROW + 1]];
      v += w[x[p]][x[p - 1]];
      v += w[x[p]][x[p + 1]];
      v += w[x[p]][x[p + ROW - 1]];
      v += w[x[p]][x[p + ROW]];
      v += w[x[p]][x[p + ROW + 1]];
      return v;
    };
    constexpr int max_time = 0x4000000;
    for (int time = 0; time < max_time; ++time) {
      int p1 = (get_random() % r + 1) * ROW + (get_random() % r + 1);
      int p2 = (get_random() % r + 1) * ROW + (get_random() % r + 1);
      if (p1 == p2) continue;
      int pv = value(p1) + value(p2);
      int t = x[p1];
      x[p1] = x[p2];
      x[p2] = t;
      int nv = value(p1) + value(p2);
      if (nv >= pv || (pv - nv) < -5 * log(get_random_double()) *
                                      (max_time - time) / max_time) {
      } else {
        t = x[p1];
        x[p1] = x[p2];
        x[p2] = t;
      }
    }

    int minRow = ROW;
    int minCol = ROW;
    for (int i = 0; i < MAX_V; ++i) {
      if (x[i] < V) {
        int r = i & (ROW - 1);
        int c = i >> 6;
        if (minRow > r) minRow = r;
        if (minCol > c) minCol = c;
      }
    }
    for (int i = 0; i < MAX_V; ++i) {
      if (x[i] < V) {
        x[i - minCol * ROW - minRow] = x[i];
        x[i] = V;
      }
    }
  }
  {  // output
    for (int i = 0; i < MAX_V; ++i) {
      if (x[i] < V) printf("%d %d\n", x[i] + 1, i / ROW * KR + i % ROW + 1);
    }
  }
}