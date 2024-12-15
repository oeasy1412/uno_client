#define DEBUG 1

#if DEBUG == 1
#define LOG(x) std::cout << x << '\n'
#elif define(LOG_RELEASE)
#define LOG(x)
#endif