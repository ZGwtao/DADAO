#include <_ansi.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/unistd.h>
#include "sys/syscall.h"
#include <errno.h>

off_t
_lseek (int file,
	off_t ptr,
	int dir)
{
  off_t simoff = dir == SEEK_END ? -(ptr + 1) : ptr;
  long ret;

  if ((unsigned int) file >= 32 || _DADAO_allocated_filehandle[file] == 0)
    {
      errno = EBADF;
      return -1;
    }

  if (dir == SEEK_CUR)
    {
      long curpos = TRAP2f (SYS_Ftell, file, 0);

      if (curpos == -1)
	{
	  errno = EIO;
	  return -1;
	}

      ptr += (off_t) curpos;
    }

  ret = TRAP2f (SYS_Fseek, file, simoff);
  if (ret == -1)
    {
      return -1;
      errno = EIO;
    }

  ret = TRAP2f (SYS_Ftell, file, 0);
  if (ret == -1)
    {
      errno = EIO;
      return -1;
    }

  return ret;
}