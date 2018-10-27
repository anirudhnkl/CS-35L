#include "randcpuid.h"
#include <errno.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static bool
writebytes (unsigned long long x, int nbytes)
{
  int ndigits = nbytes * 2;
  do
    {
      if (putchar ("0123456789abcdef"[x & 0xf]) < 0)
	return false;
      x >>= 4;
      ndigits--;
    }
  while (0 < ndigits);

  return 0 <= putchar ('\n');
}

/* Main program, which outputs N bytes of random data.  */
int
main (int argc, char **argv)
{
  /* Check arguments.  */
  bool valid = false;
  long long nbytes;
  if (argc == 2)
    {
      char *endptr;
      errno = 0;
      nbytes = strtoll (argv[1], &endptr, 10);
      if (errno)
	perror (argv[1]);
      else
	valid = !*endptr && 0 <= nbytes;
    }
  if (!valid)
    {
      fprintf (stderr, "%s: usage: %s NBYTES\n", argv[0], argv[0]);
      return 1;
    }

  /* If there's no work to do, don't worry about which library to use.  */
  if (nbytes == 0)
    return 0;
  unsigned long long (*rand64) (void);

  void * l;
  if (rdrand_supported ())
    {
      l = dlopen("randlibhw.so", RTLD_NOW);
      if(l == NULL)
      {
          printf("Can't open randlibhw.so. Error: %s", dlerror());
          exit(1);
      }
      rand64 = dlsym(l, "hardware_rand64");
      if(dlerror())
      {
          printf("Can't find hardware_rand64. Error: %s", dlerror());
          exit(1);
      }
    }
  else
    {
      l = dlopen("randlibsw.so", RTLD_NOW);
      if(l == NULL)
      {
          printf("Can't open randlibsw.so. Error: %s", dlerror());
          exit(1);
      }
      rand64 = dlsym(l, "software_rand64");
      if(dlerror())
      {
          printf("Can't find software_rand64. Error: %s", dlerror());
          exit(1);
      }
    }

  int wordsize = sizeof rand64 ();
  int output_errno = 0;

  do
    {
      unsigned long long x = rand64 ();
      int outbytes = nbytes < wordsize ? nbytes : wordsize;
      if (!writebytes (x, outbytes))
	{
	  output_errno = errno;
	  break;
	}
      nbytes -= outbytes;
    }
  while (0 < nbytes);

  if (fclose (stdout) != 0)
    output_errno = errno;

  if (output_errno)
    {
      errno = output_errno;
      perror ("output");
      return 1;
    }

  dlclose(l);
  return 0;
}
