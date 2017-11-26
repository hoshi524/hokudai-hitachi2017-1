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

constexpr double TIME_LIMIT = 1.9;
constexpr int ROW = 1 << 5;
constexpr int MAX_V = ROW * ROW;

inline unsigned get_random() {
  static unsigned y = 2463534242;
  return y ^= (y ^= (y ^= y << 13) >> 17) << 5;
}

int V, E, KV, KE, KR;
uint8_t W[512][512];
uint16_t X[MAX_V];
constexpr int LOG_SIZE = 1 << 10;
double log_d[LOG_SIZE];
uint8_t log_[LOG_SIZE];

int main() {
  {  // input
    memset(W, 0, sizeof(W));
    scanf("%d%d\n", &V, &E);
    int u, v, t;
    for (int i = 0; i < E; ++i) {
      scanf("%d%d%d\n", &u, &v, &t);
      if (t) {
        --u;
        --v;
        W[u][v] = t;
        W[v][u] = t;
      }
    }
    scanf("%d%d\n", &KV, &KE);
    KR = sqrt(KV);
  }
  {  // solve
    int R = min(KR, (int)(sqrt(V * 1.2) + 0.9));
    int n = 0;
    for (int i = 0; i < MAX_V; ++i) X[i] = V;
    for (int i = 1; i <= R && n < V; ++i) {
      for (int j = 1; j <= R && n < V; ++j) {
        X[i * ROW + j] = n++;
      }
    }
    uint8_t P[MAX_V][MAX_V];
    memset(P, 0, sizeof(P));
    auto add = [&](int p) {
      if (X[p] >= V) return;
      uint8_t* w = W[X[p]];
      for (int i = 0; i < V; ++i) {
        auto add_ = [&](int n) { P[n][i] += w[i]; };
        add_(p - ROW - 1);
        add_(p - ROW);
        add_(p - ROW + 1);
        add_(p - 1);
        add_(p + 1);
        add_(p + ROW - 1);
        add_(p + ROW);
        add_(p + ROW + 1);
      }
    };
    auto sub = [&](int p) {
      if (X[p] >= V) return;
      uint8_t* w = W[X[p]];
      for (int i = 0; i < V; ++i) {
        auto sub_ = [&](int n) { P[n][i] -= w[i]; };
        sub_(p - ROW - 1);
        sub_(p - ROW);
        sub_(p - ROW + 1);
        sub_(p - 1);
        sub_(p + 1);
        sub_(p + ROW - 1);
        sub_(p + ROW);
        sub_(p + ROW + 1);
      }
    };
    for (int i = 1; i <= R; ++i) {
      for (int j = 1; j <= R; ++j) {
        add(i * ROW + j);
      }
    }
    for (int i = 0; i < LOG_SIZE; ++i) {
      log_d[i] = -4 * log((i + 0.5) / LOG_SIZE) / TIME_LIMIT;
    }
    while (true) {
      double time = TIME_LIMIT - timer.getElapsed();
      if (time < 0) break;
      for (int i = 0; i < LOG_SIZE; ++i)
        log_[i] = min(20.0, round(log_d[i] * time));
      for (int t = 0; t < 0x10000; ++t) {
        unsigned m = get_random();
        int p1 = (((m >> 10) % R + 1) << 5) | ((m >> 15) % R + 1);
        int p2 = (((m >> 20) % R + 1) << 5) | ((m >> 25) % R + 1);
        int v1 = X[p1];
        int v2 = X[p2];
        if (v1 == v2) continue;
        int v = P[p1][v1] + P[p2][v2];
        int n = P[p1][v2] + P[p2][v1];
        if (v - n <= log_[m & (LOG_SIZE - 1)]) {
          sub(p1);
          sub(p2);
          swap(X[p1], X[p2]);
          add(p1);
          add(p2);
        }
      }
    }
  }
  {  // output
    for (int i = 0; i < MAX_V; ++i) {
      if (X[i] < V) printf("%d %d\n", X[i] + 1, (i / ROW - 1) * KR + i % ROW);
    }
  }
}