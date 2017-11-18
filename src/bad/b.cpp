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
constexpr int LOG_SIZE = 1 << 12;
double log_d[LOG_SIZE];
uint8_t log_[LOG_SIZE];
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
    uint16_t test[MAX_V];
    {
      for (int i = 0; i < MAX_V; ++i) X[i] = V;
      for (int i = 0; i < V; ++i) X[i] = i;
      int T = V;
      if (V % 4) T += 4 - V % 4;
      for (int i = 0; i < LOG_SIZE; ++i) {
        log_d[i] = -6 * log((i + 0.5) / LOG_SIZE) / TIME_LIMIT;
      }
      while (true) {
        double time = TIME_LIMIT - timer.getElapsed();
        if (time < 0) break;
        for (int i = 0; i < LOG_SIZE; ++i) log_[i] = log_d[i] * time;
        for (int t = 0; t < 0x10000; ++t) {
          int p1 = get_random() % T;
          int p2 = get_random() % T;
          auto C = [](int x) { return x & (~(0b11)); };
          if (C(p1) == C(p2)) continue;
          auto value = [&](int p) {
            int t = C(p);
            int v = 0;
            v += W[X[p]][X[t + 0]];
            v += W[X[p]][X[t + 1]];
            v += W[X[p]][X[t + 2]];
            v += W[X[p]][X[t + 3]];
            return v;
          };
          int pv = value(p1) + value(p2);
          swap(X[p1], X[p2]);
          int nv = value(p1) + value(p2);
          if ((pv - nv) > log_[get_random() & (LOG_SIZE - 1)])
            swap(X[p1], X[p2]);
        }
      }
      memcpy(test, X, sizeof(X));
    }
    int r = min(KR, (int)(sqrt(V * 1.2) + 1.9));
    {
      int t = 0;
      uint16_t T[MAX_V];
      for (int i = 0; i < MAX_V; ++i) T[i] = V;
      for (int i = 1; i < r; i += 2) {
        for (int j = 1; j < r; j += 2) {
          if (t < V) {
            T[((i + 0) << 5) + (j + 0)] = X[t + 0];
            T[((i + 1) << 5) + (j + 0)] = X[t + 1];
            T[((i + 0) << 5) + (j + 1)] = X[t + 2];
            T[((i + 1) << 5) + (j + 1)] = X[t + 3];
            t += 4;
          }
        }
      }
      memcpy(X, T, sizeof(T));
    }
    {
      auto value = [](int p) {
        int v = 0;
        for (int d : dir) v += W[X[p]][X[p + d]];
        return v;
      };
      for (int i = 0; i < LOG_SIZE; ++i) {
        log_d[i] = -6 * log((i + 0.5) / LOG_SIZE) / TIME_LIMIT;
      }
      auto value_ = [&](int p) {
        return value(p) + value(p + 1) + value(p + ROW) + value(p + ROW + 1);
      };
      auto swap_ = [&](int i, int j) {
        swap(X[i], X[j]);
        swap(X[i + 1], X[j + 1]);
        swap(X[i + ROW], X[j + ROW]);
        swap(X[i + ROW + 1], X[j + ROW + 1]);
      };
      auto shuffle = [&](int p, int s) {
        constexpr static int dir[] = {0, 1, ROW, ROW + 1};
        swap(X[p + dir[3]], X[p + dir[s % 4]]);
        s /= 4;
        swap(X[p + dir[2]], X[p + dir[s % 3]]);
        s /= 3;
        swap(X[p + dir[1]], X[p + dir[s % 2]]);
      };
      constexpr int perm = 1 * 2 * 3 * 4;
      constexpr static int rev[perm] = {6,  4,  5,  7,  1,  2,  0,  3,
                                        9,  8,  10, 11, 14, 13, 12, 15,
                                        16, 18, 17, 19, 20, 21, 22, 23};
      timer.restart();
      while (true) {
        double time = TIME_LIMIT - timer.getElapsed();
        if (time < 0) break;
        for (int i = 0; i < LOG_SIZE; ++i) log_[i] = log_d[i] * time;
        for (int t = 0; t < 0x1000; ++t) {
          int p1 = ((((get_random() % (r >> 1)) << 1) + 1) << 5) |
                   (((get_random() % (r >> 1)) << 1) + 1);
          int p2 = ((((get_random() % (r >> 1)) << 1) + 1) << 5) |
                   (((get_random() % (r >> 1)) << 1) + 1);
          if (p1 == p2) continue;
          int pv = value_(p1) + value_(p2);
          int s1 = get_random() % perm;
          int s2 = get_random() % perm;
          shuffle(p1, s1);
          shuffle(p2, s2);
          swap_(p1, p2);
          int nv = value_(p1) + value_(p2);
          if ((pv - nv) > log_[get_random() & (LOG_SIZE - 1)]) {
            swap_(p1, p2);
            shuffle(p1, rev[s1]);
            shuffle(p2, rev[s2]);
          }
        }
      }
    }
    {
      for (int i = 0; i < V; i += 4) {
        auto check = [&](int p, int v) {
          for (int d : dir)
            if (v == X[p + d]) return true;
          return false;
        };
        int p = 0;
        for (int j = 0; j < MAX_V; ++j)
          if (test[i] == X[j]) {
            p = j;
            break;
          }
        for (int j = 1; j < 4; ++j) {
          if (test[i] != V && !check(p, test[i + j]))
            cerr << "error " << V << " " << p << " " << test[i] << " "
                 << test[i + j] << "\n";
        }
      }
    }
    if (false) {
      for (int i = 1; i <= r; ++i) {
        for (int j = 1; j <= r; ++j) {
          int p = i * ROW + j;
          if (X[p] < V) {
            fprintf(stderr, "%4d", X[p]);
          } else {
            fprintf(stderr, "    ");
          }
        }
        fprintf(stderr, "\n\n");
      }
    }
  }
  {  // output
    for (int i = 0; i < MAX_V; ++i) {
      if (X[i] < V) printf("%d %d\n", X[i] + 1, (i / ROW - 1) * KR + i % ROW);
    }
  }
}