//
// This file is part of the aMule Project.
//
// Copyright (c) 2006-2008 Marcelo Roberto Jimenez ( phoenix@amule.org )
// Copyright (c) 2006-2008 aMule Team ( admin@amule.org / http://www.amule.org )
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


//
// Country flags are from FAMFAMFAM (http://www.famfamfam.com)
//
// Flag icons - http://www.famfamfam.com
// 
// These icons are public domain, and as such are free for any use (attribution appreciated but not required).
// 
// Note that these flags are named using the ISO3166-1 alpha-2 country codes where appropriate.
// A list of codes can be found at http://en.wikipedia.org/wiki/ISO_3166-1_alpha-2
//
// If you find these icons useful, please donate via paypal to mjames@gmail.com
// (or click the donate button available at http://www.famfamfam.com/lab/icons/silk)
// 
// Contact: mjames@gmail.com
//

#include "IP2Country.h"


#include "Logger.h"			// For AddLogLineM()
#include <common/Format.h>		// For CFormat()
#include <common/StringFunctions.h>	// For unicode2char()
#include "pixmaps/flags_xpm/CountryFlags.h"

#include <wx/intl.h>
#include <wx/image.h>

#ifdef __WXMAC__
	#include <CoreFoundation/CFBundle.h>
	#include <wx/mac/corefoundation/cfstring.h>
#endif

CIP2Country::CIP2Country()
{
	m_geoip = NULL;

#ifdef __WXMAC__
	// For the Mac GUI application, look for GeoIP database in the bundle
	CFURLRef GeoIPDBUrl = CFBundleCopyResourceURL(
		CFBundleGetMainBundle(),
		CFSTR("GeoIP"), CFSTR("dat"), CFSTR("GeoIP")
		);
	if (GeoIPDBUrl) {
		CFURLRef absoluteURL =
			CFURLCopyAbsoluteURL(GeoIPDBUrl);
		CFRelease(GeoIPDBUrl);
		if (absoluteURL) {
			CFStringRef pathString =
				CFURLCopyFileSystemPath(
					absoluteURL,
					kCFURLPOSIXPathStyle);
			CFRelease(absoluteURL);
			wxString GeoIPDB = wxMacCFStringHolder(pathString).
				AsString(wxLocale::GetSystemEncoding());

			m_geoip = GeoIP_open(unicode2char(GeoIPDB),
					GEOIP_STANDARD);
		}
	}
#endif
	if (m_geoip == NULL)
		m_geoip = GeoIP_new(GEOIP_STANDARD);
		
	// Load data from xpm files
	for (int i = 0; i < FLAGS_XPM_SIZE; ++i) {
		CountryData countrydata;
		countrydata.Name = char2unicode(flagXPMCodeVector[i].code);
		countrydata.Flag = wxBitmap(flagXPMCodeVector[i].xpm);
		
		if (countrydata.Flag.IsOk()) {
			m_CountryDataMap[countrydata.Name] = countrydata;
		} else {
			AddLogLineM(true, _("CIP2Country::CIP2Country(): Failed to load country data from ") + countrydata.Name);
			continue;
		}
	}
	
	AddLogLineM(false, CFormat(wxPLURAL("Loaded %d flag bitmap.", "Loaded %d flag bitmaps.", m_CountryDataMap.size())) % m_CountryDataMap.size());
}


CIP2Country::~CIP2Country()
{
	GeoIP_delete(m_geoip);
}


const CountryData& CIP2Country::GetCountryData(const wxString &ip)
{
	// Should prevent the crash if the GeoIP database does not exists
	if (m_geoip == NULL) {
		CountryDataMap::iterator it = m_CountryDataMap.find(wxString(wxT("unknown")));
		it->second.Name = wxT("?");
		return it->second;	
	}
	
	const wxString CCode = wxString(char2unicode(GeoIP_country_code_by_addr(m_geoip, unicode2char(ip)))).MakeLower();
	
	CountryDataMap::iterator it = m_CountryDataMap.find(CCode);
	if (it == m_CountryDataMap.end()) { 
		// Show the code and ?? flag
		it = m_CountryDataMap.find(wxString(wxT("unknown")));
		wxASSERT(it != m_CountryDataMap.end());
		if (CCode.IsEmpty()) {
			it->second.Name = wxT("?");
		} else{
			it->second.Name = CCode;			
		}
	}
	
	return it->second;	
}

