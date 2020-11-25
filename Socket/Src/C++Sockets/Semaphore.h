﻿/**
 **	\file Semaphore.h
 **	\date  2007-04-13
 **	\author grymse@alhem.net
**/
/*
Copyright (C) 2007-2011  Anders Hedstrom

This library is made available under the terms of the GNU GPL, with
the additional exemption that compiling, linking, and/or using OpenSSL 
is allowed.

If you would like to use this library in a closed-source application,
a separate license agreement is available. For information about 
the closed-source license agreement for the C++ sockets library,
please visit http://www.alhem.net/Sockets/license.html and/or
email license@alhem.net.

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/
#ifndef _SOCKETS_Semaphore_H
#define _SOCKETS_Semaphore_H

#include "sockets-config.h"
#ifdef _WIN32
#include "socket_include.h"
#include <WinSock2.h>
#include <windows.h>
#else
#include <pthread.h>
#ifdef MACOSX
#include <sys/semaphore.h>
#else
#include <semaphore.h>
#endif
#endif


#ifdef SOCKETS_NAMESPACE
namespace SOCKETS {
#endif

#ifdef _WIN32
typedef LONG value_t;
#else
typedef unsigned int value_t;
#endif

/** pthread semaphore wrapper.
	\ingroup threading */
class Semaphore
{
public:
	Semaphore(value_t start_val = 0);
	~Semaphore();

	/** \return 0 if successful */
	int Post();
	/** Wait for Post
	    \return 0 if successful */
	int Wait();

	/** Not implemented for win32 */
	int TryWait();

	/** Not implemented for win32 */
	int GetValue(int&);

private:
	Semaphore(const Semaphore& ) {} // copy constructor
	Semaphore& operator=(const Semaphore& ) { return *this; } // assignment operator
#ifdef _WIN32
	HANDLE m_handle;
#else
	sem_t m_sem;
#endif
};




#ifdef SOCKETS_NAMESPACE
} // namespace SOCKETS {
#endif
#endif // _SOCKETS_Semaphore_H

