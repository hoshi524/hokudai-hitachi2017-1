#include <bits/stdc++.h>
using namespace std;

constexpr int ROW = 1 << 6;
constexpr int MAX_V = 1 << 9;
constexpr int MAX_KV = ROW * ROW;

int V, E, KV, KE, KR;
uint8_t w[MAX_V][MAX_V];
uint16_t x[MAX_KV];
uint16_t best[MAX_KV];

struct Node {
  int16_t pos;
  int16_t vertex;
  int16_t value;
};
Node state[MAX_KV][MAX_V];

int main() {
  {  // input
    memset(w, 0, sizeof(w));
    scanf("%d%d\n", &V, &E);
    int u, v, t;
    for (int i = 0; i < E; ++i) {
      scanf("%d%d%d\n", &u, &v, &t);
      --u;
      --v;
      w[u][v] = t;
      w[v][u] = t;
    }
    scanf("%d%d\n", &KV, &KE);
    KR = sqrt(KV);
  }
  {  // hill climbing
    Node* bestVertex[MAX_KV];
    bool ok[MAX_KV];
    memset(ok, false, sizeof(ok));
    vector<int16_t> vertexes(MAX_V);
    vector<int16_t> positions(MAX_KV);

    int bestScore = 0;
    mt19937 engine(2463534242);
    for (int time = 0; time < 100; ++time) {
      vertexes.clear();
      positions.clear();
      for (int i = 0; i < MAX_KV; ++i) x[i] = V;
      for (int i = 0; i < V; ++i) vertexes.emplace_back(i);
      shuffle(vertexes.begin(), vertexes.end(), engine);
      for (int i = 1; i <= KR; ++i) {
        for (int j = 1; j <= KR; ++j) {
          int p = i * ROW + j;
          ok[p] = true;
          bestVertex[p] = &state[p][vertexes[0]];
          for (int k = 0; k < V; ++k) {
            state[p][k].pos = p;
            state[p][k].vertex = k;
            state[p][k].value = 0;
          }
        }
      }
      int score = 0;
      auto update = [&](int n, int p) {
        if (x[p] != V) return;
        int value = -1;
        if (ok[p]) {
          ok[p] = false;
          positions.emplace_back(p);
          for (int v : vertexes) {
            int t = 0;
            t += w[v][x[p - ROW - 1]];
            t += w[v][x[p - ROW]];
            t += w[v][x[p - ROW + 1]];
            t += w[v][x[p - 1]];
            t += w[v][x[p + 1]];
            t += w[v][x[p + ROW - 1]];
            t += w[v][x[p + ROW]];
            t += w[v][x[p + ROW + 1]];
            state[p][v].value = t;
            if (value < state[p][v].value) {
              value = state[p][v].value;
              bestVertex[p] = &state[p][v];
            }
          }
        } else {
          for (int v : vertexes) {
            state[p][v].value += w[v][x[n]];
            if (value < state[p][v].value) {
              value = state[p][v].value;
              bestVertex[p] = &state[p][v];
            }
          }
        }
      };
      update(-1, (KR / 2 + 1) * ROW + (KR / 2 + 1));
      for (int i = 0; i < V; ++i) {
        Node* n = bestVertex[positions[0]];
        {
          int value = INT_MIN;
          for (int p : positions) {
            if (bestVertex[p]->value < 0) {
              int value = INT_MIN;
              for (int v : vertexes) {
                if (value < state[p][v].value) {
                  value = state[p][v].value;
                  bestVertex[p] = &state[p][v];
                }
              }
            }
            int t = bestVertex[p]->value << 4;
            auto sub = [&](int q) {
              if (x[q] < V && w[bestVertex[p]->vertex][x[q]] == 0) --t;
            };
            sub(p - ROW - 1);
            sub(p - ROW);
            sub(p - ROW + 1);
            sub(p - 1);
            sub(p + 1);
            sub(p + ROW - 1);
            sub(p + ROW);
            sub(p + ROW + 1);
            if (value < t) {
              value = t;
              n = bestVertex[p];
            }
          }
        }
        x[n->pos] = n->vertex;
        vertexes.erase(find(vertexes.begin(), vertexes.end(), n->vertex));
        positions.erase(find(positions.begin(), positions.end(), n->pos));
        score += n->value;
        for (int p : positions) state[p][n->vertex].value = -1;
        update(n->pos, n->pos - ROW - 1);
        update(n->pos, n->pos - ROW);
        update(n->pos, n->pos - ROW + 1);
        update(n->pos, n->pos - 1);
        update(n->pos, n->pos + 1);
        update(n->pos, n->pos + ROW - 1);
        update(n->pos, n->pos + ROW);
        update(n->pos, n->pos + ROW + 1);
      }
      if (bestScore < score) {
        bestScore = score;
        memcpy(best, x, sizeof(x));
      }
    }
    // cerr << "bestScore : " << bestScore << endl;
  }
  {  // output
    for (int i = 0; i < MAX_KV; ++i) {
      if (best[i] < V)
        printf("%d %d\n", best[i] + 1, (i / ROW - 1) * KR + i % ROW);
    }
  }
}