/*
 * Source: https://codehosting.net/blog/BlogEngine/post/PDP-11-Fractals-in-ASCII-Art
 *
 * Adapted to write to rv64-emu UART instead of using putchar().
 *
 * Since we do not implement floating-point instructions, compile
 * this code using the soft-float ABI. IMPORTANT: a compiler that is
 * capable of this is *not* installed in the ca2018 environment, so
 * use the .bin file as provided in the examples tarball.
 */

static volatile char *debugout = (char*)0x200;

int main(void)
{
  float b,e,r,n,d=0.0;
  int h=0,c = 0;
  for(e=1.1; e>-1.2; e-=.1)
  {
    for(b=-2; b<1; b+=.04)
    {
      if ((++c % 76)==0)
      {
        *debugout = '\r';
        *debugout = '\n';
      }
      else *debugout = h;

      n=0;
      h=127;
      for(r=0; r*r+n*n<4 && --h>32; d=r)
      {
        r = r*r-n*n+b;
        n = 2*d*n+e;
      }
    }
  }
}
