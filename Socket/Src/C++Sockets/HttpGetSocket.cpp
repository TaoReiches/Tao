/** \file HttpGetSocket.cpp
 **	\date  2004-02-13
 **	\author grymse@alhem.net
**/
/*
Copyright (C) 2004-2011  Anders Hedstrom

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
#ifdef _WIN32
#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif
#else
#include <errno.h>
#endif
#include "ISocketHandler.h"
#include "HttpGetSocket.h"


#ifdef SOCKETS_NAMESPACE
namespace SOCKETS_NAMESPACE {
#endif


HttpGetSocket::HttpGetSocket(ISocketHandler& h) : HttpClientSocket(h)
{
	m_iBeginPos = 0;
	m_iSize = 0;
}


HttpGetSocket::HttpGetSocket(ISocketHandler& h,const std::string& url_in,const std::string& to_file, bool connect) : HttpClientSocket(h, url_in)
{
	m_iBeginPos = 0;
	m_iSize = 0;

	if (to_file.size())
	{
		SetFilename(to_file);
	}
	if (connect)
		DoConnect();
}


void HttpGetSocket::DoConnect()
{
	DoConnect(GetUrlHost(), GetUrlPort());
}


void HttpGetSocket::DoConnect(const std::string& host, unsigned short port)
{
	if (!Open(host, port))
	{
		if (!Connecting())
		{
			Handler().LogError(this, "HttpGetSocket", -1, "connect() failed miserably", LOG_LEVEL_FATAL);
			SetCloseAndDelete();
		}
	}
}


HttpGetSocket::HttpGetSocket(ISocketHandler& h, const std::string& host, port_t port, const std::string& url, const std::string& to_file, bool connect, int iBeginPos, int iSize) : HttpClientSocket(h, host, port, url)
{
	m_iBeginPos = iBeginPos;
	m_iSize = iSize;
	SetUrl(url);
	if (to_file.size())
	{
		SetFilename(to_file);
	}
	if (connect)
		DoConnect(host, port);
}


HttpGetSocket::~HttpGetSocket()
{
}


void HttpGetSocket::OnConnect()
{
	SetMethod( "GET" );
	AddResponseHeader( "Accept", "text/xml,application/xml,application/xhtml+xml,text/html;q=0.9,text/plain;q=0.8,video/x-mng,image/png,image/jpeg,image/gif;q=0.2,*/*;q=0.1");
	AddResponseHeader( "Accept-Language", "en-us,en;q=0.5");
	AddResponseHeader( "Accept-Encoding", "gzip,deflate");
	AddResponseHeader( "Accept-Charset", "ISO-8859-1,utf-8;q=0.7,*;q=0.7");
	AddResponseHeader( "User-agent", MyUseragent() );
	
	
	char acResponStr[64] = { 0 };
	if (m_iSize <= 0)
	{
		sprintf(acResponStr, "bytes=%d-\r\n", m_iBeginPos);
	}
	else
	{
		sprintf(acResponStr, "bytes=%d-%d\r\n", m_iBeginPos, m_iBeginPos + m_iSize - 1);
	}
		AddResponseHeader("Range", acResponStr);
	

	if (GetUrlPort() != 80 && GetUrlPort() != 443)
		AddResponseHeader( "Host", GetUrlHost() + ":" + Utility::l2string(GetUrlPort()) );
	else
		AddResponseHeader( "Host", GetUrlHost() );
	SendRequest();
}


#ifdef SOCKETS_NAMESPACE
}
#endif


