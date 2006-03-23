/*
 * Copyright 2004 Andrew De Ponte
 * 
 * This file is part of zsrep.
 * 
 * zsrep is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 * 
 * zsrep is distributed in the hopes that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with zsrep; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

/*
 * Copyright 2004 Andrew De Ponte.
 *
 * This file is part of oam_player.
 *
 * oam_player is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * oam_player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with oam_player; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/**
 * \file DebugControl.h
 * \brief A header which provides controlable debug support.
 * \author Andrew De Ponte
 *
 * This header is a little macro work that makes debug and warning statements
 * within the rest of the projects code a lot cleaner. It allows enabling and
 * disabling of any number of sections for warnings or debugs using a simple
 * define scheme.
 */

#ifndef DEBUG_CONTROL
#define DEBUG_CONTROL

// Here I define DC_YES and DC_NO for the debug control so that below I can
// define each of the sections for debug or warning messages and enable them
// or disable them at will.
#define DC_YES printf
#define DC_NO if (0) printf

#define D_CONFMANAGER DC_NO

#endif
