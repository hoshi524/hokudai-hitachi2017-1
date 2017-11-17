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

constexpr double TIME_LIMIT = 1.8;
constexpr int ROW = 1 << 5;
constexpr int MAX_V = ROW * ROW;

inline unsigned get_random() {
  static unsigned y = 2463534242;
  return y ^= (y ^= (y ^= y << 13) >> 17) << 5;
}

inline double get_random_double() { return (double)get_random() / UINT_MAX; }

int V, E, KV, KE, KR;
uint8_t W[500][500];
uint16_t X[MAX_V];
uint16_t T[MAX_V];
constexpr static int dir[] = {-ROW - 1, -ROW,    -ROW + 1, -1,
                              1,        ROW - 1, ROW,      ROW + 1};

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
    int r = sqrt(V);
    if (r * r < V) ++r;
    auto RP = [r]() {
      return ((get_random() % r + 1) << 5) | (get_random() % r + 1);
    };
    int n = 0;
    for (int i = 0; i < MAX_V; ++i) X[i] = V;
    for (int i = 1; i <= r && n < V; ++i) {
      for (int j = 1; j <= r && n < V; ++j) {
        X[i * ROW + j] = n++;
      }
    }
    auto value = [](int p) {
      int v = 0;
      for (int d : dir) v += W[X[p]][X[p + d]];
      return v;
    };
    while (TIME_LIMIT > timer.getElapsed()) {
    start:
      memcpy(T, X, sizeof(X));
      int s = RP(), v = 0;
      for (int i = 0; i < 5; ++i) {
        int np = 0, nv = -0x10000;
        for (int j = 0; j < (r << 1); ++j) {
          int n = RP();
          if (X[s] == X[n]) continue;
          int s1 = value(s);
          int s2 = value(n);
          swap(X[s], X[n]);
          int n1 = value(s);
          int n2 = value(n);
          if (s1 + s2 < n1 + n2 + v) {
            goto start;
          }
          int x = n1 - s1;
          if (nv < x) {
            nv = x;
            np = n;
          }
          swap(X[s], X[n]);
        }
        v -= value(s) + value(np);
        swap(X[s], X[np]);
        v += value(s) + value(np);
        s = np;
      }
      memcpy(X, T, sizeof(T));
    }
  }
  {  // output
    for (int i = 0; i < MAX_V; ++i) {
      if (X[i] < V) printf("%d %d\n", X[i] + 1, (i / ROW - 1) * KR + i % ROW);
    }
  }
}