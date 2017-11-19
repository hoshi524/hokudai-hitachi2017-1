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
uint8_t P[MAX_V];
uint16_t X[MAX_V];
constexpr int LOG_SIZE = 1 << 10;
double log_d[LOG_SIZE];
uint8_t log_[LOG_SIZE];

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
    int r = min(KR, (int)(sqrt(V * 1.2) + 0.9));
    int n = 0;
    for (int i = 0; i < MAX_V; ++i) X[i] = V;
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
        int p1 = (((m >> 10) % r + 1) << 5) | ((m >> 15) % r + 1);
        int p2 = (((m >> 20) % r + 1) << 5) | ((m >> 25) % r + 1);
        if (X[p1] == X[p2]) continue;
        int pv = P[p1] + P[p2];
        swap(X[p1], X[p2]);
        int v1 = value(p1), v2 = value(p2);
        if ((pv - v1 - v2) > log_[m & (LOG_SIZE - 1)]) {
          swap(X[p1], X[p2]);
        } else {
          diff(p2, p1);
          diff(p1, p2);
          P[p1] = v1;
          P[p2] = v2;
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