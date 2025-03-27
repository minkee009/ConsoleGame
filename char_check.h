#pragma once
#include <Windows.h>

extern const WCHAR flipmap[10];

// CJK 문자 및 이모지 등의 유니코드 범위를 기반으로 2칸 문자 판별
BOOL IsDoubleWidthCharacter(WCHAR ch) {
    // 한글(Hangul Syllables), CJK 이데오그램, 이모지 등 2칸 문자 범위
    return (ch >= 0x1100 && ch <= 0x115F) ||  // Hangul Jamo
        (ch >= 0x2E80 && ch <= 0xA4CF) ||  // CJK, 한자, 한글 포함
        (ch >= 0xAC00 && ch <= 0xD7A3) ||  // Hangul Syllables
        (ch >= 0xF900 && ch <= 0xFAFF) ||  // CJK 호환 한자
        (ch >= 0xFE10 && ch <= 0xFE19) ||  // 세로쓰기 구두점
        (ch >= 0xFE30 && ch <= 0xFE6F) ||  // CJK 호환 기호
        (ch >= 0xFF00 && ch <= 0xFF60) ||  // 전각 문자 (풀와이드 문자)
        (ch >= 0xFFE0 && ch <= 0xFFE6) ||  // 전각 기호
        (ch >= 0x1F300 && ch <= 0x1F64F) || // Miscellaneous Symbols and Pictographs (이모지)
        (ch >= 0x1F900 && ch <= 0x1F9FF);  // Supplemental Symbols and Pictographs (추가 이모지)
}

BOOL isSpecialCharacter(WCHAR ch) {
    // 괄호와 슬래시, 백슬래시를 특수 문자로 취급
    return (ch == L'<' || ch == L'>' || ch == L'(' || ch == L')' || ch == L'{' || ch == L'}' || ch == L'[' || ch == L']' || ch == L'/' || ch == L'\\');
}


const WCHAR flipmap[10] = {
    L'<',L'>',L'(',L')',L'{', L'}',L'[',L']',L'/',L'\\',
};

LPCWSTR flipSpecialCharacter(wchar_t ch) {
    switch (ch) {
    case L'<': return &flipmap[1];  // > 의 주소
    case L'>': return &flipmap[0];  // < 의 주소
    case L'(': return &flipmap[3];  // ) 의 주소
    case L')': return &flipmap[2];  // ( 의 주소
    case L'{': return &flipmap[5];  // } 의 주소
    case L'}': return &flipmap[4];  // { 의 주소
    case L'[': return &flipmap[7];  // ] 의 주소
    case L']': return &flipmap[6];  // [ 의 주소
    case L'/': return &flipmap[9];  // \ 의 주소
    case L'\\': return &flipmap[8]; // / 의 주소
    default: return nullptr;
    }
}