/*  console.c -- Ncurses console for GRUB.  */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2003  Free Software Foundation, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
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
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <curses.h>
#include <grub/machine/console.h>
#include <grub/term.h>
#include <grub/types.h>

static int grub_console_attr = A_NORMAL;

static void
grub_ncurses_putchar (grub_uint32_t c)
{
  addch (c | grub_console_attr);
}

static void
grub_ncurses_setcolorstate (grub_term_color_state state)
{
  switch (state) 
    {
    case GRUB_TERM_COLOR_STANDARD:
      grub_console_attr = A_NORMAL;
      break;
    case GRUB_TERM_COLOR_NORMAL:
      grub_console_attr = A_NORMAL;
      break;
    case GRUB_TERM_COLOR_HIGHLIGHT:
      grub_console_attr = A_STANDOUT;
      break;
    default:
      break;
    }
}

/* XXX: This function is never called.  */
static void
grub_ncurses_setcolor (grub_uint8_t normal_color, grub_uint8_t highlight_color)
{
  color_set (normal_color << 8 | highlight_color, 0);
}

static int
grub_ncurses_checkkey (void)
{
  return 1;
}

static int
grub_ncurses_getkey (void)
{
  int c = getch ();

  switch (c)
    {
    case KEY_LEFT:
      c = GRUB_CONSOLE_KEY_LEFT;
      break;

    case KEY_RIGHT:
      c = GRUB_CONSOLE_KEY_RIGHT;
      break;
      
    case KEY_UP:
      c = GRUB_CONSOLE_KEY_UP;
      break;

    case KEY_DOWN:
      c = GRUB_CONSOLE_KEY_DOWN;
      break;

    case KEY_IC:
      c = GRUB_CONSOLE_KEY_IC;
      break;

    case KEY_DC:
      c = GRUB_CONSOLE_KEY_DC;
      break;

    case KEY_BACKSPACE:
      /* XXX: For some reason ncurses on xterm does not return
	 KEY_BACKSPACE.  */
    case 127: 
      c = GRUB_CONSOLE_KEY_BACKSPACE;
      break;

    case KEY_HOME:
      c = GRUB_CONSOLE_KEY_HOME;
      break;

    case KEY_END:
      c = GRUB_CONSOLE_KEY_END;
      break;

    case KEY_NPAGE:
      c = GRUB_CONSOLE_KEY_NPAGE;
      break;

    case KEY_PPAGE:
      c = GRUB_CONSOLE_KEY_PPAGE;
      break;
    }

  return c;
}

static grub_uint16_t
grub_ncurses_getxy (void)
{
  int x;
  int y;

  getyx (stdscr, y, x);

  return (x << 8) | y;
}

static void
grub_ncurses_gotoxy (grub_uint8_t x, grub_uint8_t y)
{
  move (y, x);
}

static void
grub_ncurses_cls (void)
{
  clear ();
  refresh ();
}

static void
grub_ncurses_setcursor (int on)
{
  curs_set (on ? 1 : 0);
}

static void
grub_ncurses_refresh (void)
{
  refresh ();
}

static grub_err_t
grub_ncurses_init (void)
{
  initscr ();
  cbreak (); 
  noecho ();
  scrollok (stdscr, TRUE);

  nonl ();
  intrflush (stdscr, FALSE);
  keypad (stdscr, TRUE);
  start_color ();

  return 0;
}

static grub_err_t
grub_ncurses_fini (void)
{
  endwin ();
  return 0;
}


static struct grub_term grub_ncurses_term =
  {
    .name = "console",
    .init = grub_ncurses_init,
    .fini = grub_ncurses_fini,
    .putchar = grub_ncurses_putchar,
    .checkkey = grub_ncurses_checkkey,
    .getkey = grub_ncurses_getkey,
    .getxy = grub_ncurses_getxy,
    .gotoxy = grub_ncurses_gotoxy,
    .cls = grub_ncurses_cls,
    .setcolorstate = grub_ncurses_setcolorstate,
    .setcolor = grub_ncurses_setcolor,
    .setcursor = grub_ncurses_setcursor,
    .refresh = grub_ncurses_refresh,
    .flags = 0,
    .next = 0
  };

void
grub_console_init (void)
{
  grub_term_register (&grub_ncurses_term);
  grub_term_set_current (&grub_ncurses_term);
}