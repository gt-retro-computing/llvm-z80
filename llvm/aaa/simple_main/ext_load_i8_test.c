/*char sext_return() {
  // unsigned char lol = *((volatile unsigned char*) 6969);
  return *((char *) 0x1234);
}

unsigned char zext_return() {
  return *((unsigned char*) 0x4321);
}*/

unsigned char simple_arith() {
  unsigned char a = *(volatile int *)1;
  unsigned char b = *(volatile int *)1;
  unsigned char c = a + b;
  return c;
}
