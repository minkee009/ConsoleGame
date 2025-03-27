#ifndef __MYGAME_CHAR_CHECK__
#define __MYGAME_CHAR_CHECK__

#include <wchar.h>

#ifdef __cplusplus
extern "C" {  
#endif

// ���� �迭 ���� (���Ǵ� .c ���Ͽ��� ����)
extern const wchar_t flipmap[10];

// �Լ� ����
int IsDoubleWidthCharacter(wchar_t ch);
int isSpecialCharacter(wchar_t ch);
const wchar_t* flipSpecialCharacter(wchar_t ch);

#ifdef __cplusplus
}
#endif

#endif // !__MYGAME_CHAR_CHECK__