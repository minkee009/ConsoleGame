#ifndef __MYGAME_CHAR_CHECK__
#define __MYGAME_CHAR_CHECK__

#include <wchar.h>

#ifdef __cplusplus
extern "C" {  
#endif

// 전역 배열 선언 (정의는 .c 파일에서 수행)
extern const wchar_t flipmap[10];

// 함수 선언
int IsDoubleWidthCharacter(wchar_t ch);
int isSpecialCharacter(wchar_t ch);
const wchar_t* flipSpecialCharacter(wchar_t ch);

#ifdef __cplusplus
}
#endif

#endif // !__MYGAME_CHAR_CHECK__