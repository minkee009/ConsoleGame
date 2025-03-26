#ifndef __MYGAME_SPRITE__
#define __MYGAME_SPRITE__

#include <Windows.h>

typedef struct _SPRITE
{
	COORD Size;
	COORD Pivot;
	SHORT SortingOrder;
	const WCHAR** ShapeString;
	WORD Attribute;
	BOOL Flip;
} SPRITE;

#endif // !__MYGAME_SPRITE__
