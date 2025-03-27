#include "char_check.h"

// 전역 배열 정의
const wchar_t flipmap[10] = {
    L'<', L'>', L'(', L')', L'{', L'}', L'[', L']', L'/', L'\\'
};

// CJK 문자 및 이모지 등의 유니코드 범위를 기반으로 2칸 문자 판별
int IsDoubleWidthCharacter(wchar_t ch) {
    return (ch >= 0x1100 && ch <= 0x115F) ||  // Hangul Jamo
        (ch >= 0x2E80 && ch <= 0xA4CF) ||  // CJK, 한자, 한글 포함
        (ch >= 0xAC00 && ch <= 0xD7A3) ||  // Hangul Syllables
        (ch >= 0xF900 && ch <= 0xFAFF) ||  // CJK 호환 한자
        (ch >= 0xFE10 && ch <= 0xFE19) ||  // 세로쓰기 구두점
        (ch >= 0xFE30 && ch <= 0xFE6F) ||  // CJK 호환 기호
        (ch >= 0xFF00 && ch <= 0xFF60) ||  // 전각 문자 (풀와이드 문자)
        (ch >= 0xFFE0 && ch <= 0xFFE6) ||  // 전각 기호
        (ch >= 0x1F300 && ch <= 0x1F64F) || // 이모지
        (ch >= 0x1F900 && ch <= 0x1F9FF);  // 추가 이모지
}

// 특수 문자 확인
int isSpecialCharacter(wchar_t ch) {
    return (ch == L'<' || ch == L'>' || ch == L'(' || ch == L')' ||
        ch == L'{' || ch == L'}' || ch == L'[' || ch == L']' ||
        ch == L'/' || ch == L'\\');
}

// 대응되는 특수 문자 반환
const wchar_t* flipSpecialCharacter(wchar_t ch) {
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
    default: return NULL;
    }
}
