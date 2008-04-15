//								-*- C++ -*-
// This file is part of the aMule Project.
//
// Copyright (c) 2004-2008 Angel Vidal (Kry) ( kry@amule.org )
// Copyright (c) 2004-2008 aMule Team ( admin@amule.org / http://www.amule.org )
// Copyright (c) 2003 Barry Dunne (http://www.emule-project.net)
//
// Any parts of this program derived from the xMule, lMule or eMule project,
// or contributed by third-party developers are copyrighted by their
// respective authors.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301, USA
//

// Note To Mods //
/*
Please do not change anything here and release it..
There is going to be a new forum created just for the Kademlia side of the client..
If you feel there is an error or a way to improve something, please
post it in the forum first and let us look at it.. If it is a real improvement,
it will be added to the offical client.. Changing something without knowing
what all it does can cause great harm to the network if released in mass form..
Any mod that changes anything within the Kademlia side will not be allowed to advertise
there client on the eMule forum..
*/

#ifndef __KAD_ENTRY_H__
#define __KAD_ENTRY_H__


#include "../utils/UInt128.h"
#include <time.h>

////////////////////////////////////////
namespace Kademlia {
////////////////////////////////////////

class CEntry
{
public:
	CEntry()
	{
		m_uIP = 0;
		m_uTCPport = 0;
		m_uUDPport = 0;
		m_uSize = 0;
		m_tLifeTime = time(NULL);
		m_bSource = false;
	}

	~CEntry()
	{
		for (TagPtrList::iterator it = m_lTagList.begin(); it != m_lTagList.end(); ++it) {
			delete *it;
		}
	}
	
	uint32_t GetIntTagValue(const wxString& tagname) const
	{
		CTag* tag;
		for (TagPtrList::const_iterator it = m_lTagList.begin(); it != m_lTagList.end(); ++it) {
			tag = *it;
			if ((tag->GetName() == tagname) && tag->IsInt()) {
				return tag->GetInt();
			}
		}
		return 0;
	}

	wxString GetStrTagValue(const wxString& tagname) const
	{
		CTag* tag;
		for (TagPtrList::const_iterator it = m_lTagList.begin(); it != m_lTagList.end(); ++it) {
			tag = *it;
			if ((tag->GetName() == tagname) && tag->IsStr()) {
				return tag->GetStr();
			}
		}
		return wxEmptyString;
	}	
	
	uint32_t m_uIP;
	uint16_t m_uTCPport;
	uint16_t m_uUDPport;
	CUInt128 m_uKeyID;
	CUInt128 m_uSourceID;
	wxString m_sFileName; // NOTE: this always holds the string in LOWERCASE!!!
	uint64_t m_uSize;
	TagPtrList m_lTagList;
	time_t m_tLifeTime;
	bool m_bSource;
};

}

#endif // __KAD_ENTRY_H__
// File_checked_for_headers
