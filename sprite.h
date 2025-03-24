#ifndef __MYGAME_SPRITE__
#define __MYGAME_SPRITE__

#include <Windows.h>

typedef struct _SPRITE
{
	COORD Size;
	COORD Position;
	COORD Pivot;
	SHORT SortingOrder;
	CHAR_INFO* Pixels;
} SPRITE;

#endif // !__MYGAME_SPRITE__
