#ifndef MULECOLOR_H
#define MULECOLOR_H

#include <wx/colour.h>
#include <wx/settings.h>

class CMuleColour : public wxColour 
{
public:

	enum ColourComponent { COLOUR_R = 1, COLOUR_G = 2, COLOUR_B = 4 };

	CMuleColour() { }	
	CMuleColour(const wxColour& colour) : wxColour(colour) { }
	CMuleColour(byte r, byte g, byte b) : wxColour(r,g,b) {  }
	CMuleColour(unsigned long rgb) : wxColour() { wxColour::Set(rgb); }
	CMuleColour(wxSystemColour colour) : wxColour(wxSystemSettings::GetColour(colour)) { }

	~CMuleColour() { }
	
	const CMuleColour& Blend(byte percentage, ColourComponent flags = (ColourComponent)(COLOUR_R | COLOUR_G | COLOUR_B) )
	{
		unsigned int red = (unsigned int)(Red() * ((flags & COLOUR_R) ? ((float)percentage/(float)100) : (float)1));
		unsigned int green = (unsigned int)(Green() * ((flags & COLOUR_G) ? ((float)percentage/(float)100) : (float)1));
		unsigned int blue = (unsigned int)(Blue() * ((flags & COLOUR_B) ? ((float)percentage/(float)100) : (float)1));
		Set((red < 255) ? red : 255, (green < 255) ? green : 255, (blue < 255) ? blue : 255, Alpha());
		return *this;
	}
	
	const CMuleColour& BlendWith(const CMuleColour& colour, double covered)
	{
		unsigned int red = (unsigned int)(Red() + (colour.Red() * covered) + 0.5);
		unsigned int green = (unsigned int)(Green() + (colour.Green() * covered) + 0.5);
		unsigned int blue = (unsigned int)(Blue() + (colour.Blue() * covered) + 0.5);
		Set((red < 255) ? red : 255, (green < 255) ? green : 255, (blue < 255) ? blue : 255, Alpha());
		return *this;
	}
	
	unsigned long GetULong() const { return (Blue() << 16) | (Green() << 8) | Red(); }
		
	bool IsBlack() const { return !Red() && !Blue() && !Green(); }
	
	bool IsSameAs(const CMuleColour& colour) const { return GetULong() == colour.GetULong(); }
	
private:
	
};

#endif
