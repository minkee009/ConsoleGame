#pragma once

// CJK 문자 및 이모지 등의 유니코드 범위를 기반으로 2칸 문자 판별
bool IsDoubleWidthCharacter(wchar_t ch) {
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