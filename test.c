#include <stdio.h>

void gpio(unsigned int i)
{
  printf("unsinged int %d\n", i);
  return;
}
void gpioInt(int i)
{
  printf("int %d\n", i);
  return;
}

int main(void)
{
  unsigned int i = 23;
  gpio(i);
  gpioInt(i);
}
