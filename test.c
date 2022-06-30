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

//ビットを送る関数
// static int sendBit(int bitValue)
// {
//   gpio_set_value(23, bitValue);
//   gpio_set_value(24, 1);
//   ndelay(500);
//   gpio_set_value(24, 0);
//   ndelay(500);
//   return 0;
// }

//バイトを送る関数　ビットシフトで１ビットずつ取り出し
static int sendbyte(int byteValue)
{
  //ビットシフトして、値を取り出す、そしてsendbitを呼ぶ
  int i, sendValue;
  for (i = 7; i > -1; i--)
  {
    //byteValue = byteValue << i;
    sendValue = byteValue >> i & 1;
    //sendBit(sendValue);

    printf("%x\n", sendValue);
  }
  return 0;
}

static int startGpio(void)
{
  //32bit文　0を送信　
  int i;
  for (i = 0; i < 32; i++)
  {
    // gpio_set_value(23, 0);
    // gpio_set_value(24, 1);
    // ndelay(500);
    // gpio_set_value(24, 0);
    // ndelay(500);
    printf("%d\n", i + 1);
  }
  return 0;
}

int main(void)
{
  unsigned int i = 23;
  gpio(i);
  gpioInt(i);
  sendbyte(16);
  int x, y, result;
  x = 14;
  y = 0x0;
  result = x | y;
  printf("%x", result);

  startGpio();
}
