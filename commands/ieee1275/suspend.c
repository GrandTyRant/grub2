/* suspend.c - command to suspend GRUB and return to Open Firmware  */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2005,2007  Free Software Foundation, Inc.
 *
 *  GRUB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GRUB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <grub/normal.h>
#include <grub/dl.h>
#include <grub/misc.h>
#include <grub/term.h>
#include <grub/ieee1275/ieee1275.h>

static grub_err_t
grub_cmd_suspend (struct grub_arg_list *state  __attribute__ ((unused)),
		  int argc __attribute__ ((unused)),
		  char **args __attribute__ ((unused)))
{
  grub_printf ("Run 'go' to resume GRUB.\n");
  grub_ieee1275_enter ();
  grub_cls ();
  return 0;
}


GRUB_MOD_INIT(ieee1275_suspend)
{
  (void)mod;			/* To stop warning. */
  grub_register_command ("suspend", grub_cmd_suspend, GRUB_COMMAND_FLAG_BOTH,
			 "suspend", "Return to Open Firmware prompt", 0);
}

GRUB_MOD_FINI(ieee1275_suspend)
{
  grub_unregister_command ("suspend");
}
