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

constexpr double TIME_LIMIT = 2.8;
constexpr int ROW = 1 << 5;
constexpr int MAX_V = ROW * ROW;

inline unsigned get_random() {
  static unsigned y = 2463534242;
  return y ^= (y ^= (y ^= y << 13) >> 17) << 5;
}

inline double get_random_double() { return (double)get_random() / UINT_MAX; }

int V, E, KV, KE, KR;
uint8_t w[500][500];
int x[MAX_V];

int main() {
  {  // input
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
    constexpr int LOG_SIZE = 1 << 12;
    static double log_d[LOG_SIZE];
    static uint8_t log_[LOG_SIZE];
    for (int i = 0; i < LOG_SIZE; ++i) {
      log_d[i] = -5 * log((i + 0.5) / LOG_SIZE) / TIME_LIMIT;
    }
    while (true) {
      double time = TIME_LIMIT - timer.getElapsed();
      if (time < 0) break;
      for (int i = 0; i < LOG_SIZE; ++i) {
        log_[i] = log_d[i] * time;
      }
      for (int t = 0; t < 0x10000; ++t) {
        int p1 = ((get_random() % r + 1) << 5) | (get_random() % r + 1);
        int p2 = ((get_random() % r + 1) << 5) | (get_random() % r + 1);
        if (x[p1] == x[p2]) continue;
        int pv = value(p1) + value(p2);
        swap(x[p1], x[p2]);
        int nv = value(p1) + value(p2);
        if (nv < pv && (pv - nv) > log_[get_random() & (LOG_SIZE - 1)])
          swap(x[p1], x[p2]);
      }
    }
  }
  {  // output
    for (int i = 0; i < MAX_V; ++i) {
      if (x[i] < V) printf("%d %d\n", x[i] + 1, (i / ROW - 1) * KR + i % ROW);
    }
  }
}