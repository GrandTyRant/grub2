/* ls.c - command to list files and devices */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2003  Free Software Foundation, Inc.
 *
 *  GRUB is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <grub/types.h>
#include <grub/misc.h>
#include <grub/mm.h>
#include <grub/err.h>
#include <grub/dl.h>
#include <grub/normal.h>
#include <grub/arg.h>
#include <grub/disk.h>
#include <grub/device.h>
#include <grub/term.h>
#include <grub/partition.h>
#include <grub/file.h>

static const struct grub_arg_option options[] =
  {
    {"long", 'l', 0, "Show a long list with more detailed information", 0, 0},
    {"human-readable", 'h', 0, "Print sizes in a human readable format", 0, 0},
    {"all", 'a', 0, "List all files", 0, 0},
    {0, 0, 0, 0, 0, 0}
  };

static const char grub_human_sizes[] = {' ', 'K', 'M', 'G', 'T'};

static grub_err_t
grub_ls_list_disks (int longlist)
{
  auto int grub_ls_print_disks (const char *name);
  int grub_ls_print_disks (const char *name)
    {
      grub_device_t dev;
      auto int print_partition (const grub_partition_t p);
      
      int print_partition (const grub_partition_t p)
	{
	  char *pname = grub_partition_get_name (p);

	  if (pname)
	    {
	      if (longlist)
		grub_print_partinfo (dev, pname);
	      else
		grub_printf ("(%s,%s) ", name, pname);
	    }

	  return 0;
	}
      
      dev = grub_device_open (name);
      grub_errno = GRUB_ERR_NONE;
      
      if (dev)
	{
	  if (longlist)
	    grub_printf ("Disk: %s\n", name);
	  else
	    grub_printf ("(%s) ", name);

	  if (dev->disk && dev->disk->has_partitions)
	    {
	      grub_partition_iterate (dev->disk, print_partition);
	      grub_errno = GRUB_ERR_NONE;
	    }

	  grub_device_close (dev);
	}
  
      return 0;
    }
  
  grub_disk_dev_iterate (grub_ls_print_disks);
  grub_putchar ('\n');
  grub_refresh ();

 
  return 0;
}

static grub_err_t
grub_ls_list_files (const char *dirname, int longlist, int all, int human)
{
  char *device_name;
  grub_fs_t fs;
  const char *path;
  grub_device_t dev;

  static int print_files (const char *filename, int dir)
    {
      if (all || filename[0] != '.')
	grub_printf ("%s%s ", filename, dir ? "/" : "");
      
      return 0;
    }
     
  static int print_files_long (const char *filename, int dir)
    {
      char pathname[grub_strlen (dirname) + grub_strlen (filename) + 1];

      if ((! all) && (filename[0] == '.'))
	return 0;

      if (! dir)
	{
	  grub_file_t file;
	  
	  if (dirname[grub_strlen (dirname) - 1] == '/')
	    grub_sprintf (pathname, "%s%s", dirname, filename);
	  else
	    grub_sprintf (pathname, "%s/%s", dirname, filename);

	  /* XXX: For ext2fs symlinks are detected as files while they
	     should be reported as directories.  */
	  file = grub_file_open (pathname);
	  if (! file)
	    {
	      grub_errno = 0;
	      return 0;
	    }

	  if (! human)
	    grub_printf ("%-12d", file->size);
	  else
	    {
	      float fsize = file->size;
	      int fsz = file->size;
	      int units = 0;
	      char buf[20];
	      
	      while (fsz / 1024)
		{
		  fsize /= 1024;
		  fsz /= 1024;
		  units++;
		}

	      if (units)
		{
		  grub_sprintf (buf, "%0.2f%c", fsize, grub_human_sizes[units]);
		  grub_printf ("%-12s", buf);
		}
	      else
		grub_printf ("%-12d", file->size);
	      
	    }
	  (fs->close) (file);
      	}
      else
	grub_printf ("%-12s", "DIR");

      grub_printf ("%s%s\n", filename, dir ? "/" : "");

      return 0;
    }

  device_name = grub_file_get_device_name (dirname);
  dev = grub_device_open (device_name);
  if (! dev)
    goto fail;

  fs = grub_fs_probe (dev);
  path = grub_strchr (dirname, ')');
  if (! path)
    path = dirname;
  else
    path++;
  
  if (! path && ! device_name)
    {
      grub_error (GRUB_ERR_BAD_ARGUMENT, "invalid argument");
      goto fail;
    }
      
  if (! path)
    {
      if (grub_errno == GRUB_ERR_UNKNOWN_FS)
	grub_errno = GRUB_ERR_NONE;
	  
      grub_printf ("(%s): Filesystem is %s.\n",
		   device_name, fs ? fs->name : "unknown");
    }
  else if (fs)
    {
      if (longlist)
	(fs->dir) (dev, path, print_files_long);
      else
	(fs->dir) (dev, path, print_files);
      grub_putchar ('\n');
      grub_refresh ();
    }

 fail:
  if (dev)
    grub_device_close (dev);
      
  grub_free (device_name);

  return 0;
}

static grub_err_t
grub_cmd_ls (struct grub_arg_list *state, int argc, char **args)
{
  static int grub_ls_print_files (const char *filename, int dir)
    {
      if (state[2].set/*all*/ || filename[0] != '.')
	grub_printf ("%s%s ", filename, dir ? "/" : "");
      
      return 0;
    }

  if (argc == 0)
  grub_ls_list_disks (state[0].set);
  else
    grub_ls_list_files (args[0], state[0].set, state[2].set,
			state[1].set);

  return 0;
}

#ifdef GRUB_UTIL
void
grub_ls_init (void)
{
  grub_register_command ("ls", grub_cmd_ls, GRUB_COMMAND_FLAG_BOTH,
			 "ls [OPTIONS...] [DIR]",
			 "List devices and files", options);
}

void
grub_ls_fini (void)
{
  grub_unregister_command ("ls");
}
#else /* ! GRUB_UTIL */
GRUB_MOD_INIT
{
  (void)mod;			/* To stop warning. */
  grub_register_command ("ls", grub_cmd_ls, GRUB_COMMAND_FLAG_BOTH,
			 "ls [OPTIONS...] [DIR]",
			 "List devices and files", options);
}

GRUB_MOD_FINI
{
  grub_unregister_command ("ls");
}
#endif /* ! GRUB_UTIL */