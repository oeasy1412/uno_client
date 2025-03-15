#ifndef _DEF_H_
#define _DEF_H_

#define DEBUG 0

#if DEBUG == 1
#define LOG(x) std::cout << x << '\n'
#else
#define LOG(x)
#endif

#endif