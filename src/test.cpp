#include <bits/stdc++.h>
#include <sys/time.h>
using namespace std;

class Timer {
 public:
  void restart();
  double getElapsed();

  Timer();

 private:
  static double rdtsc_per_sec_inv;

  double getTimeOfDay();
  unsigned long long int getCycle();

  double start_time;
  unsigned long long int start_clock;
};
double Timer::rdtsc_per_sec_inv = -1;

inline double Timer::getElapsed() {
  if (rdtsc_per_sec_inv != -1)
    return (double)(getCycle() - start_clock) * rdtsc_per_sec_inv;

  const double RDTSC_MEASUREMENT_INTERVAL = 0.1;
  double res = getTimeOfDay() - start_time;
  if (res <= RDTSC_MEASUREMENT_INTERVAL) return res;

  rdtsc_per_sec_inv = 1.0 / (getCycle() - start_clock);
  rdtsc_per_sec_inv *= getTimeOfDay() - start_time;
  return getElapsed();
}

inline void Timer::restart() {
  start_time = getTimeOfDay();
  start_clock = getCycle();
}

Timer::Timer() { restart(); }

inline double Timer::getTimeOfDay() {
  timeval tv;
  gettimeofday(&tv, 0);
  return tv.tv_sec + tv.tv_usec * 0.000001;
}

inline unsigned long long int Timer::getCycle() {
  unsigned int low, high;
  __asm__ volatile("rdtsc" : "=a"(low), "=d"(high));
  return ((unsigned long long int)low) | ((unsigned long long int)high << 32);
}

Timer timer;

inline unsigned get_random() {
  static unsigned y = 2463534242;
  return y ^= (y ^= (y ^= y << 13) >> 17) << 5;
}

constexpr double TIME_LIMIT = 1.9;
constexpr int ROW = 1 << 6;
constexpr int MAX_V = 1 << 9;
constexpr int MAX_KV = ROW * ROW;

int V, E, KV, KE, KR;
uint8_t W[MAX_V][MAX_V];
uint8_t P[MAX_KV];
uint16_t X[MAX_KV];

int bestScore = 0;
uint16_t best[MAX_KV];

struct Node {
  int16_t pos;
  int16_t vertex;
  int16_t value;
};
Node state[MAX_KV][MAX_V];

int value(int p) {
  uint8_t* w = W[X[p]];
  return w[X[p - ROW - 1]] + w[X[p - ROW]] + w[X[p - ROW + 1]] + w[X[p - 1]] +
         w[X[p + 1]] + w[X[p + ROW - 1]] + w[X[p + ROW]] + w[X[p + ROW + 1]];
}

void diff(int p, int n) {
  uint8_t* wp = W[X[p]];
  uint8_t* wn = W[X[n]];
  auto set = [&](int t) {
    P[t] -= wp[X[t]];
    P[t] += wn[X[t]];
  };
  set(n - ROW - 1);
  set(n - ROW);
  set(n - ROW + 1);
  set(n - 1);
  set(n + 1);
  set(n + ROW - 1);
  set(n + ROW);
  set(n + ROW + 1);
}

int main() {
  {  // input
    memset(W, 0, sizeof(W));
    scanf("%d%d\n", &V, &E);
    int u, v, t;
    for (int i = 0; i < E; ++i) {
      scanf("%d%d%d\n", &u, &v, &t);
      --u;
      --v;
      W[u][v] = t;
      W[v][u] = t;
    }
    scanf("%d%d\n", &KV, &KE);
    KR = sqrt(KV);
  }
  {  // Hill Climbing
    Node* bestVertex[MAX_KV];
    bool ok[MAX_KV];
    memset(ok, false, sizeof(ok));
    vector<int16_t> vertexes(MAX_V);
    vector<int16_t> positions(MAX_KV);
    mt19937 engine(get_random());
    for (int time = 0; time < 100; ++time) {
      vertexes.clear();
      positions.clear();
      for (int i = 0; i < MAX_KV; ++i) X[i] = V;
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
        if (X[p] != V) return;
        int value = -1;
        if (ok[p]) {
          ok[p] = false;
          positions.emplace_back(p);
          for (int v : vertexes) {
            int t = 0;
            t += W[v][X[p - ROW - 1]];
            t += W[v][X[p - ROW]];
            t += W[v][X[p - ROW + 1]];
            t += W[v][X[p - 1]];
            t += W[v][X[p + 1]];
            t += W[v][X[p + ROW - 1]];
            t += W[v][X[p + ROW]];
            t += W[v][X[p + ROW + 1]];
            state[p][v].value = t;
            if (value < state[p][v].value) {
              value = state[p][v].value;
              bestVertex[p] = &state[p][v];
            }
          }
        } else {
          for (int v : vertexes) {
            state[p][v].value += W[v][X[n]];
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
              if (X[q] < V && W[bestVertex[p]->vertex][X[q]] == 0) --t;
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
        X[n->pos] = n->vertex;
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
        memcpy(best, X, sizeof(X));
      }
    }
  }
  {  // Simulated Annealing
    constexpr int LOG_SIZE = 1 << 10;
    double log_d[LOG_SIZE];
    uint8_t log_[LOG_SIZE];
    int r = min(KR, (int)(sqrt(V * 1.2) + 0.9));
    int n = 0;
    for (int i = 0; i < MAX_KV; ++i) X[i] = V;
    for (int i = 1; i <= r && n < V; ++i) {
      for (int j = 1; j <= r && n < V; ++j) {
        X[i * ROW + j] = n++;
      }
    }
    memset(P, 0, sizeof(P));
    for (int i = 1; i <= r; ++i) {
      for (int j = 1; j <= r; ++j) {
        int p = i * ROW + j;
        P[p] = value(p);
      }
    }
    for (int i = 0; i < LOG_SIZE; ++i) {
      log_d[i] = -5 * log((i + 0.5) / LOG_SIZE) / TIME_LIMIT;
    }
    while (true) {
      double time = TIME_LIMIT - timer.getElapsed();
      if (time < 0) break;
      for (int i = 0; i < LOG_SIZE; ++i)
        log_[i] = min(20.0, round(log_d[i] * time));
      for (int t = 0; t < 0x10000; ++t) {
        unsigned m = get_random();
        int p1 = (((m >> 0) % r + 1) << 6) | ((m >> 6) % r + 1);
        int p2 = (((m >> 12) % r + 1) << 6) | ((m >> 18) % r + 1);
        if (X[p1] == X[p2]) continue;
        int pv = P[p1] + P[p2];
        swap(X[p1], X[p2]);
        int v1 = value(p1), v2 = value(p2);
        if ((pv - v1 - v2) > log_[get_random() & (LOG_SIZE - 1)]) {
          swap(X[p1], X[p2]);
        } else {
          diff(p2, p1);
          diff(p1, p2);
          P[p1] = v1;
          P[p2] = v2;
        }
      }
    }
    int score = 0;
    for (int i = 1; i <= r; ++i) {
      for (int j = 1; j <= r; ++j) {
        score += value(i * ROW + j);
      }
    }
    score /= 2;
    // cerr << bestScore << " " << score << endl;
    if (bestScore < score) {
      bestScore = score;
      memcpy(best, X, sizeof(X));
    }
  }
  {  // output
    for (int i = 0; i < MAX_KV; ++i) {
      if (best[i] < V)
        printf("%d %d\n", best[i] + 1, (i / ROW - 1) * KR + i % ROW);
    }
  }
}