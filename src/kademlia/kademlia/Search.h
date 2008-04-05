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

#ifndef __SEARCH_H__
#define __SEARCH_H__

#include "SearchManager.h"

class CKnownFile;
class CMemFile;
class CTag;

////////////////////////////////////////
namespace Kademlia {
////////////////////////////////////////

void deleteTagPtrListEntries(TagPtrList* taglist);

class CSearch
{
	friend class CSearchManager;

public:
	uint32 GetSearchID() const {return m_searchID;}
	uint32 GetSearchTypes() const {return m_type;}
	void SetSearchTypes( uint32 val ) {m_type = val;}
	void SetTargetID( CUInt128 val ) {m_target = val;}

	uint32 GetKadPacketSent() const {return m_kadPacketSent;}
	uint32 GetRequestAnswer() const {return m_totalRequestAnswers;}
	void StorePacket();

	CUInt128 m_keywordPublish; //Need to make this private...
	const wxString& GetFileName(void) const { return m_fileName; }
	void SetFileName(const wxString& fileName) { m_fileName = fileName; }
	CUInt128 GetTarget(void) const {return m_target;}
	void AddFileID(const CUInt128& id);
	void PreparePacketForTags( CMemFile* packet, CKnownFile* file );
	bool Stoping(void) const {return m_stoping;}
	uint32 GetNodeLoad() const;
	uint32 GetNodeLoadResonse() const {return m_totalLoadResponses;}
	uint32 GetNodeLoadTotal() const {return m_totalLoad;}
	void UpdateNodeLoad( uint8 load ){ m_totalLoad += load; m_totalLoadResponses++; }
	uint32 GetAnswers() const;
	
	enum
	{
		NODE,
		NODECOMPLETE,
		FILE,
		KEYWORD,
		NOTES,
		STOREFILE,
		STOREKEYWORD,
		STORENOTES,
		FINDBUDDY,
		FINDSOURCE
	};

	CSearch();
	~CSearch();

private:
	void Go(void);
	void ProcessResponse(uint32 fromIP, uint16 fromPort, ContactList *results);
	void ProcessResult(uint32 fromIP, uint16 fromPort, const CUInt128 &answer, TagPtrList *info);
	void ProcessResultFile(uint32 fromIP, uint16 fromPort, const CUInt128 &answer, TagPtrList *info);
	void ProcessResultKeyword(uint32 fromIP, uint16 fromPort, const CUInt128 &answer, TagPtrList *info);
	void ProcessResultNotes(uint32 fromIP, uint16 fromPort, const CUInt128 &answer, TagPtrList *info);
	void JumpStart(void);
	void SendFindValue(const CUInt128 &check, uint32 ip, uint16 port);
	void PrepareToStop(void);

	bool		m_stoping;
	time_t		m_created;
	uint32		m_type;
	uint32		m_uAnswers;
	uint32		m_totalRequestAnswers;
	uint32		m_kadPacketSent; //Used for gui reasons.. May not be needed later..
	uint32		m_totalLoad;
	uint32		m_totalLoadResponses;
	uint32		m_lastResponse;

	uint32		m_searchID;
	CUInt128	m_target;
	CMemFile*	m_searchTerms;
	WordList	m_words;
	wxString		m_fileName;
	UIntList	m_fileIDs;

	ContactMap	m_possible;
	ContactMap	m_tried;
	ContactMap	m_responded;
	ContactMap	m_best;
	ContactList	m_delete;
	ContactMap	m_inUse;
};

} // End namespace

#endif //__SEARCH_H__
// File_checked_for_headers
