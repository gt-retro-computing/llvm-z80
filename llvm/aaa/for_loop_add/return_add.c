unsigned char main() {
  unsigned char i = 0;
  for (unsigned char j = 0; j < 10; j++) {
    i += ++j;
  }
  return i;
}
