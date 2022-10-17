#include "utils.h"

#include <time.h>
#include <errno.h>
#include <math.h>
#include <thread>
#include <chrono>



double get_time_in_seconds() {
  struct timespec tms;

  /* The C11 way */
  /* if (! timespec_get(&tms, TIME_UTC)) { */

  // /* POSIX.1-2008 way */
  // if (clock_gettime(CLOCK_REALTIME, &tms)) {
  //   return -1;
  // }
  auto now = std::chrono::system_clock::now().time_since_epoch();
  return (double)std::chrono::duration_cast<std::chrono::seconds>(now).count();


  // /* seconds, multiplied with 1 million */
  // int64_t micros = tms.tv_sec * 1000000;
  // /* Add full microseconds */
  // micros += tms.tv_nsec / 1000;
  // /* round up if necessary */
  // if (tms.tv_nsec % 1000 >= 500) {
  //   ++micros;
  // }
  // double seconds = ((double) micros) / 1000000.0;
  // return seconds;
}

int milisleep(long msec) {
  struct timespec ts;
  int res;

  if (msec < 0)
  {
    errno = EINVAL;
    return -1;
  }

  // ts.tv_sec = msec / 1000;
  // ts.tv_nsec = (msec % 1000) * 1000000;

  long total_msec = msec * 1000000;

  std::chrono::milliseconds ms(msec);
  std::this_thread::sleep_for(ms);
  // do {
  //   res = nanosleep(&ts, &ts);
  // } while (res && errno == EINTR);

  return res;
}

uint8_t get_next_id(uint8_t* id) {
  uint8_t new_id = *id;
  if (*id == UINT8_MAX) {
    *id = 0;
  } else {
    *id += 1;
  }
  return new_id;
}