#include <am.h>
#include <treecore.h>

void __am_timer_init()
{
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime)
{
  // uint64_t tmpTime = 0;
  // system ticker impl
  // asm volatile("csrr %0, mcycle;" : "=r"(tmpTime): );
  // mmio impl
  uptime->us = inl(RTC_ADDR); // BUG: bit width!
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc)
{
  static bool isFirst = true;
  if (isFirst)
  {
    isFirst = false;
    rtc->second = 0;
    rtc->minute = 0;
    rtc->hour = 0;
    rtc->day = 0;
    rtc->month = 0;
    rtc->year = 1900;
  }
  else
  {
    if (rtc->second <= 59)
    {
      ++rtc->second;
    }
    else
    {
      rtc->second = 0;
      ++rtc->minute;
    }

    if (rtc->minute == 60)
    {
      rtc->minute = 0;
      ++rtc->hour;
    }

    if (rtc->hour == 24)
    {
      rtc->hour = 0;
      ++rtc->day;
    }

    if (rtc->day == 30)
    {
      rtc->day = 0;
      ++rtc->month;
    }

    if (rtc->month == 12)
    {
      rtc->month = 0;
      ++rtc->year;
    }

    rtc->year = 1900;
  }
}
