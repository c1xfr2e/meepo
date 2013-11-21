/***********************************************************************
    filename:   InputEvent.h
    created:    2011/10/3
    author:     ZhaoHao

    purpose:	
*************************************************************************/
#pragma once

#include "PreHeader.h"
#include "Vector.h"
#include <string>

namespace Meepo
{

// Enumeration of mouse buttons
enum EMouseButton
{
	MouseButton_Left,   // The left mouse button.
	MouseButton_Right,  // The right mouse button.
	MouseButton_Middle, // The middle mouse button.
    MouseButton_None
};

// System key flag values
enum ESystemKey
{
	SystemKey_LeftMouse		= 0x0001,	// The left mouse button.
	SystemKey_RightMouse	= 0x0002,	// The right mouse button.
	SystemKey_Shift			= 0x0004,	// Either shift key.
	SystemKey_Control		= 0x0008,	// Either control key.
	SystemKey_MiddleMouse	= 0x0010,	// The middle mouse button.
	SystemKey_Alt			= 0x0080	// Either alt key.
};

class CEventArgs
{
public:
	CEventArgs(CElement* element) : m_pElement(0), m_bHandled(false) {}
	virtual ~CEventArgs(void) {}

	CElement* m_pElement;
	bool m_bHandled; //!< handlers should set this to true if they handled the event, or false otherwise.
};


// EventArgs based class that is used for objects passed to input event handlers
// concerning mouse input.
class CMouseEventArgs : public CEventArgs
{
public:
	CMouseEventArgs(CElement* element) : CEventArgs(element) {}

	CVector2		m_position;		// holds current mouse position.
	CVector2		m_moveDelta;	// holds variation of mouse position from last mouse input
	EMouseButton	m_button;		// one of the MouseButton enumerated values describing the mouse button causing the event (for button inputs only)
	unsigned int	m_systemKeys;	// current state of the system keys and mouse buttons.
	float			m_wheelChange;	// Holds the amount the scroll wheel has changed.
	unsigned int	m_clickCount;   // Holds number of mouse button down events currently counted in a multi-click sequence (for button inputs only).
};

} // namespace Meepo

