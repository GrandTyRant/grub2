/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2006  Free Software Foundation, Inc.
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

#ifndef GRUB_VBEFILL_MACHINE_HEADER
#define GRUB_VBEFILL_MACHINE_HEADER	1

#include <grub/video.h>

void
grub_video_i386_vbefill_R8G8B8A8 (struct grub_video_render_target *dst,
                                  grub_video_color_t color,  int x, int y,
                                  int width, int height);

void
grub_video_i386_vbefill_R8G8B8 (struct grub_video_render_target *dst,
                                grub_video_color_t color, int x, int y,
                                int width, int height);

void
grub_video_i386_vbefill_index (struct grub_video_render_target *dst,
                               grub_video_color_t color, int x, int y,
                               int width, int height);

#endif /* ! GRUB_VBEFILL_MACHINE_HEADER */