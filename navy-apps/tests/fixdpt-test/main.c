#include <stdio.h>
#include <assert.h>
#include <fixedptc.h>

int main()
{
  printf("%s,%d: Hello fixdpt!\n", __func__, __LINE__);
  fixedpt a = fixedpt_rconst(1.2);
  fixedpt b = fixedpt_fromint(10);
  int c = 0;
  if (b > fixedpt_rconst(7.9))
  {
    c = fixedpt_toint(fixedpt_div(fixedpt_mul(a + FIXEDPT_ONE, b), fixedpt_rconst(2.3)));
  }
  assert(c == 9);

  fixedpt d1 = fixedpt_rconst(2.32342342234234);
  fixedpt d2 = fixedpt_rconst(2.00000000000000);
  printf("%s's abs: %d\n", "2.32342342234234", fixedpt_toint(fixedpt_abs(d1)));
  printf("%s's abs: %d\n", "2.00000000000000", fixedpt_toint(fixedpt_abs(d2)));
  printf("%s's floor: %d\n", "2.32342342234234", fixedpt_toint(fixedpt_floor(d1)));
  printf("%s's floor: %d\n", "2.00000000000000", fixedpt_toint(fixedpt_floor(d2)));
  printf("%s's ceil: %d\n", "2.32342342234234", fixedpt_toint(fixedpt_ceil(d1)));
  printf("%s's ceil: %d\n", "2.00000000000000", fixedpt_toint(fixedpt_ceil(d2)));

  fixedpt e1 = fixedpt_rconst(-2.32342342234234);
  fixedpt e2 = fixedpt_rconst(-2.00000000000000);
  
  printf("%s's abs: %d\n", "-2.32342342234234", fixedpt_toint(fixedpt_abs(e1)));
  printf("%s's abs: %d\n", "-2.00000000000000", fixedpt_toint(fixedpt_abs(e2)));
  printf("%s's floor: %d\n", "-2.32342342234234", fixedpt_toint(fixedpt_floor(e1)));
  printf("%s's floor: %d\n", "-2.00000000000000", fixedpt_toint(fixedpt_floor(e2)));
  printf("%s's ceil: %d\n", "-2.32342342234234", fixedpt_toint(fixedpt_ceil(e1)));
  printf("%s's ceil: %d\n", "-2.00000000000000", fixedpt_toint(fixedpt_ceil(e2)));

  return 0;
}
