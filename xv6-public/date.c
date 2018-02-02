#include "types.h"
#include "user.h"
#include "date.h"

static char* days[] = {
  0, 
  "Mon",
  "Tue",
  "Wed",
  "Thu",
  "Fri",
  "Sat",
  "Sun",
};

static char* months[] = {
  0,
  "Jan",
  "Feb",
  "Mar",
  "Apr",
  "May",
  "Jun",
  "Jul",
  "Aug",
  "Sep",
  "Oct",
  "Nov",
  "Dec",
};

int
main(int argc, char *argv[])
{
  struct rtcdate r;

  if (date(&r)) {
    printf(2, "date failed\n");
    exit();
  }

  // your code to print the time in any format you like...
  printf(1, "%s %s  %d ", days[r.day], months[r.month], r.day);
  printf(1, "%d%d:%d%d:%d%d ", 
      r.hour / 10, 
      r.hour % 10,
      r.minute / 10,
      r.minute % 10,
      r.second / 10,
      r.second % 10);
  printf(1, "%d\n", r.year);
  exit();
}
