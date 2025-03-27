#pragma once
#include <Windows.h>

extern const WCHAR flipmap[10];

// CJK ���� �� �̸��� ���� �����ڵ� ������ ������� 2ĭ ���� �Ǻ�
BOOL IsDoubleWidthCharacter(WCHAR ch) {
    // �ѱ�(Hangul Syllables), CJK �̵����׷�, �̸��� �� 2ĭ ���� ����
    return (ch >= 0x1100 && ch <= 0x115F) ||  // Hangul Jamo
        (ch >= 0x2E80 && ch <= 0xA4CF) ||  // CJK, ����, �ѱ� ����
        (ch >= 0xAC00 && ch <= 0xD7A3) ||  // Hangul Syllables
        (ch >= 0xF900 && ch <= 0xFAFF) ||  // CJK ȣȯ ����
        (ch >= 0xFE10 && ch <= 0xFE19) ||  // ���ξ��� ������
        (ch >= 0xFE30 && ch <= 0xFE6F) ||  // CJK ȣȯ ��ȣ
        (ch >= 0xFF00 && ch <= 0xFF60) ||  // ���� ���� (Ǯ���̵� ����)
        (ch >= 0xFFE0 && ch <= 0xFFE6) ||  // ���� ��ȣ
        (ch >= 0x1F300 && ch <= 0x1F64F) || // Miscellaneous Symbols and Pictographs (�̸���)
        (ch >= 0x1F900 && ch <= 0x1F9FF);  // Supplemental Symbols and Pictographs (�߰� �̸���)
}

BOOL isSpecialCharacter(WCHAR ch) {
    // ��ȣ�� ������, �齽���ø� Ư�� ���ڷ� ���
    return (ch == L'<' || ch == L'>' || ch == L'(' || ch == L')' || ch == L'{' || ch == L'}' || ch == L'[' || ch == L']' || ch == L'/' || ch == L'\\');
}


const WCHAR flipmap[10] = {
    L'<',L'>',L'(',L')',L'{', L'}',L'[',L']',L'/',L'\\',
};

LPCWSTR flipSpecialCharacter(wchar_t ch) {
    switch (ch) {
    case L'<': return &flipmap[1];  // > �� �ּ�
    case L'>': return &flipmap[0];  // < �� �ּ�
    case L'(': return &flipmap[3];  // ) �� �ּ�
    case L')': return &flipmap[2];  // ( �� �ּ�
    case L'{': return &flipmap[5];  // } �� �ּ�
    case L'}': return &flipmap[4];  // { �� �ּ�
    case L'[': return &flipmap[7];  // ] �� �ּ�
    case L']': return &flipmap[6];  // [ �� �ּ�
    case L'/': return &flipmap[9];  // \ �� �ּ�
    case L'\\': return &flipmap[8]; // / �� �ּ�
    default: return nullptr;
    }
}