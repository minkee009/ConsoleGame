#ifndef __MYGAME__SPRITE__
#define __MYGAME__SPRITE__

#include <Windows.h>

typedef struct _SPRITE
{
	COORD Size;
	COORD Position;
	COORD Pivot;
	SHORT SortingOrder;
	CHAR_INFO* Pixels;
} SPRITE;

#endif // !__MYGAME__SPRITE__
