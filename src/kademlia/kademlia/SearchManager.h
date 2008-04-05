//
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

#ifndef __SEARCHMANAGER_H__
#define __SEARCHMANAGER_H__

#include "../utils/UInt128.h"
#include "../routing/Maps.h"
#include "../../Tag.h"

class CMemFile;

////////////////////////////////////////
namespace Kademlia {
////////////////////////////////////////

class CSearch;
class CRoutingZone;

// If type is unknown it will be an empty string
// If there are any properties about the file to report, there will follow LPCSTR key/value pairs.
//typedef void (CALLBACK *SEARCH_KEYWORD_CALLBACK)(uint32 searchID, CUInt128 fileID, wxString name, uint32 size, wxString type, uint16 numProperties, ...);
//typedef void (CALLBACK *SEARCH_ID_CALLBACK)(uint32 searchID, CUInt128 contactID, uint8 type, uint32 ip, uint16 tcp, uint16 udp, uint32 serverip, uint16 port);

typedef std::list<wxString> WordList;
typedef std::map<CUInt128, CSearch*> SearchMap;

#define SEARCH_IMAGE	"-image"
#define SEARCH_AUDIO	"-audio"
#define SEARCH_VIDEO	"-video"
#define SEARCH_DOC		"-doc"
#define SEARCH_PRO		"-pro"

class CSearchManager
{
	friend class CRoutingZone;
	friend class CKademlia;

public:

	static void StopSearch(uint32 searchID, bool delayDelete);
	static void StopAllSearches(void);

	// Search for a particular file
	// Will return unique search id, returns zero if already searching for this file.
	static CSearch* PrepareLookup(uint32 type, bool start, const CUInt128 &id);

	// Will return unique search id, returns zero if already searching for this keyword.
	static CSearch* PrepareFindKeywords(const wxString& keyword, CMemFile* ed2k_packet, uint32 searchid);

	static bool StartSearch(CSearch* pSearch);
	static void DeleteSearch(CSearch* pSearch);

	static void ProcessResponse(const CUInt128 &target, uint32 fromIP, uint16 fromPort, ContactList *results);
	static void ProcessResult(const CUInt128 &target, uint32 fromIP, uint16 fromPort, const CUInt128 &answer, TagPtrList *info);
	static void ProcessPublishResult(const CUInt128 &target, const uint8 load, const bool loadResponse);

	static void GetWords(const wxString& str, WordList *words);

	static void UpdateStats(void);

	static bool IsNodeSearch(const CUInt128 &target);

	static bool AlreadySearchingFor(const CUInt128 &target);

	static const wxChar* GetInvalidKeywordChars() { return wxT(" ()[]{}<>,._-!?:;\\/"); }

private:

	static void FindNode(const CUInt128 &id);
	static void FindNodeComplete(const CUInt128 &id);

	static uint32 m_nextID;
	static SearchMap m_searches;

	static void JumpStart(void);
};

} // End namespace

#endif // __SEARCHMANAGER_H__
// File_checked_for_headers
