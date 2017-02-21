//
// Copyright 2001-2002 by Francesco Bradascio.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
// USA.
//
// Please report all bugs and problems to "fbradasc@katamail.com".
//
//         FlScan is based in part on the work of
//         the FLTK project (http://www.fltk.org).
//

#ifndef FLSCAN_THREAD
#define FLSCAN_THREAD

#ifdef THREADED_SAVING
    #define FL_LOCK_BEGIN()    Fl::lock();
    #define FL_LOCK_END(a)     Fl::unlock(); Fl::awake(a);
    #define FL_TOGGLE_THREAD() running.toggle();
    #define FL_THREAD_MSG(m)   Fl::awake(m);
#else
    #define FL_LOCK_BEGIN()
    #define FL_LOCK_END(a)
    #define FL_TOGGLE_THREAD() 
    #define FL_THREAD_MSG(m)
#endif

#endif /* FLSCAN_THREAD */
