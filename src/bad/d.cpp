#include <bits/stdc++.h>
using namespace std;

inline unsigned get_random() {
  static unsigned y = 2463534242;
  return y ^= (y ^= (y ^= y << 13) >> 17) << 5;
}

constexpr int ROW = 1 << 5;
constexpr int MAX_V = ROW * ROW;

int V, E, KV, KE, KR;
int w[MAX_V][MAX_V];
int x[MAX_V];
int best[MAX_V];
bool ok[MAX_V];

struct Node {
  int pos;
  int vertex;
  int value;
};

Node state[MAX_V][512];
Node* bestVertex[MAX_V];

int main() {
  {  // input
    memset(w, 0, sizeof(w));
    memset(ok, false, sizeof(ok));
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
    int R = min(KR, ROW - 2);
    int bestScore = 0;
    random_device seed_gen;
    mt19937 engine(seed_gen());
    for (int time = 0; time < 400; ++time) {
      for (int i = 1; i <= R; ++i) {
        for (int j = 1; j <= R; ++j) {
          int p = i * ROW + j;
          ok[p] = true;
          for (int k = 0; k < V; ++k) {
            state[p][k].pos = p;
            state[p][k].vertex = k;
            state[p][k].value = 0;
          }
          bestVertex[p] = &state[p][get_random() % V];
        }
      }
      vector<int> vertexes;
      vector<int> positions;
      for (int i = 0; i < MAX_V; ++i) x[i] = V;
      for (int i = 0; i < V; ++i) vertexes.push_back(i);
      shuffle(vertexes.begin(), vertexes.end(), engine);
      int score = 0;
      auto add = [&](int p) {
        if (ok[p]) {
          ok[p] = false;
          positions.push_back(p);
        }
      };
      add((R / 2 + 1) * ROW + (R / 2 + 1));
      for (int i = 0; i < V; ++i) {
        Node* n = bestVertex[positions[0]];
        for (int p : positions) {
          if (bestVertex[p]->value < 0) {
            int value = -1;
            for (int v : vertexes) {
              if (value < state[p][v].value) {
                value = state[p][v].value;
                bestVertex[p] = &state[p][v];
              }
            }
          }
          if (n->value < bestVertex[p]->value) n = bestVertex[p];
        }
        x[n->pos] = n->vertex;
        for (int i = 0; i < vertexes.size(); ++i) {
          if (n->vertex == vertexes[i]) {
            vertexes.erase(vertexes.begin() + i);
            break;
          }
        }
        for (int i = 0; i < positions.size(); ++i) {
          if (n->pos == positions[i]) {
            positions.erase(positions.begin() + i);
            break;
          }
        }
        score += n->value;
        for (int p : positions) {
          state[p][n->vertex].value = -1;
        }
        auto update = [&](int n, int p) {
          if (x[p] != V) return;
          int value = -1;
          if (ok[p]) {
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
        update(n->pos, n->pos - ROW - 1);
        update(n->pos, n->pos - ROW);
        update(n->pos, n->pos - ROW + 1);
        update(n->pos, n->pos - 1);
        update(n->pos, n->pos + 1);
        update(n->pos, n->pos + ROW - 1);
        update(n->pos, n->pos + ROW);
        update(n->pos, n->pos + ROW + 1);
        add(n->pos + 1);
        add(n->pos - 1);
        add(n->pos + ROW);
        add(n->pos - ROW);
      }
      if (bestScore < score) {
        bestScore = score;
        memcpy(best, x, sizeof(x));
      }
    }
    // cerr << "bestScore : " << bestScore << endl;
  }
  {  // output
    for (int i = 0; i < MAX_V; ++i) {
      if (best[i] < V)
        printf("%d %d\n", best[i] + 1, (i / ROW - 1) * KR + i % ROW);
    }
  }
}