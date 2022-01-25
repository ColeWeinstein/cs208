/*
    bits.c
    Assignment for Carleton's CS208
    Performs a multitude of functions, from converting a string to upper or lower case, returning x bits from the the middle of an integer, or converting an Unicode codepoint to utf_8 or vice versa.

    Written by Cole Weinstein, 1/23/2022
*/

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "bits.h"

void to_upper(char *s) {
    if (s == NULL) {
        return;
    }

    int s_length = strlen(s);
    int index;
    for (index = 0; index < s_length; index++) {
        if (s[index] > 0x60 && s[index] < 0x7B) {
            s[index] = s[index] & 0xDF;
        }
    }

    return;
}

void to_lower(char *s) {
    if (s == NULL) {
        return;
    }

    int s_length = strlen(s);
    int index;
    for (index = 0; index < s_length; index++) {
        if (s[index > 0x40 && s[index] < 0x5B]) {
            s[index] = s[index] | 0x20;
        }
    }

    return;
}

int middle_bits(int n, int bit_index, int bit_count) {
    if (n < 0 || bit_index < 0 || bit_index > 30 || bit_count < 1 || bit_count > (31 - bit_index)) {
        return n;
    }

    // shifts the bits in n so that the highest order bit to return is at index 31
    int return_val = n << (31 - (bit_index + bit_count));
    // if there was a 1 at the bit at index (bit_index + bit_count), it would override the sign of return_val and, when shifted right again, make return_val negative. Thus, we bitwise & it with 0x7FFFFFFF to make the sign positive, always.
    return_val = return_val & 0x7FFFFFFF;
    // shifts the bits in n so that the lowest order bit to return is at index 0
    return_val = return_val >> (31 - bit_count);

    return return_val;
}

bool to_utf8(int codepoint, char *utf8_buffer) {
    if (utf8_buffer == NULL || codepoint < 0 || codepoint > 0x10FFFF) {
        return false;
    }
    // 007F 07FF FFFF 10FFFF
    if (codepoint < 0x0080) {
        utf8_buffer[0] = (char) codepoint;
        utf8_buffer[1] = 0;
    }
    else if (codepoint < 0x0800) {
        utf8_buffer[0] = (char) middle_bits(codepoint, 6, 5) | 0xC0;
        utf8_buffer[1] = (char) middle_bits(codepoint, 0, 6) | 0x80;
        utf8_buffer[2] = 0;
    }
    else if (codepoint < 0x10000) {
        utf8_buffer[0] = (char) middle_bits(codepoint, 12, 4) | 0xE0;
        utf8_buffer[1] = (char) middle_bits(codepoint, 6, 6) | 0x80;
        utf8_buffer[2] = (char) middle_bits(codepoint, 0, 6) | 0x80;
        utf8_buffer[3] = 0;
    }
    else {
        utf8_buffer[0] = (char) middle_bits(codepoint, 18, 3) | 0xF0;
        utf8_buffer[1] = (char) middle_bits(codepoint, 12, 6) | 0x80;
        utf8_buffer[2] = (char) middle_bits(codepoint, 6, 6) | 0x80;
        utf8_buffer[3] = (char) middle_bits(codepoint, 0, 6) | 0x80;
        utf8_buffer[4] = 0;
    }
    return true;
}

int from_utf8(char *utf8_buffer) {
    if (utf8_buffer == NULL) {
        return -1;
    }

    int codepoint;
    int utf8_length = strlen(utf8_buffer);

    if (utf8_length == 0) {
        return 0;
    }
    else if (utf8_length == 1) {
        // utf8 encoding for 1 byte: 0xxxxxxx
        if (utf8_buffer[0] >> 7 != 0) {
            return -1;
        }
        codepoint = (int) utf8_buffer[0];
    }
    else if (utf8_length == 2) {
        //utf8 encoding for 2 bytes: 110xxxxx 10xxxxxx
        if ((unsigned char) utf8_buffer[0] >> 5 != 6
        || (unsigned char) utf8_buffer[1] >> 6 != 2) {
            return -1;
        }
        int first_bits = utf8_buffer[0] & 0x1F;
        int second_bits = utf8_buffer[1] & 0x3F;
        codepoint = (first_bits << 6) | second_bits;
    }
    else if (utf8_length == 3) {
        //utf8 encoding for 2 bytes: 1110xxxx 10xxxxxx 10xxxxxx
        if ((unsigned char) utf8_buffer[0] >> 4 != 0xE
        || (unsigned char) utf8_buffer[1] >> 6 != 2
        || (unsigned char) utf8_buffer[2] >> 6 != 2) {
            return -1;
        }
        int first_bits = utf8_buffer[0] & 0x0F;
        int second_bits = utf8_buffer[1] & 0x3F;
        int third_bits = utf8_buffer[2] & 0x3F;
        codepoint = (first_bits << 12) | (second_bits << 6) | third_bits;
    }
    else if (utf8_length == 4) {
        //utf8 encoding for 2 bytes: 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        if ((unsigned char) utf8_buffer[0] >> 3 != 0x1E
        || (unsigned char) utf8_buffer[1] >> 6 != 2
        || (unsigned char) utf8_buffer[2] >> 6 != 2
        || (unsigned char) utf8_buffer[3] >> 6 != 2) {
            return -1;
        }
        int first_bits = utf8_buffer[0] & 0x07;
        int second_bits = utf8_buffer[1] & 0x3F;
        int third_bits = utf8_buffer[2] & 0x3F;
        int fourth_bits = utf8_buffer[3] & 0x3F;
        codepoint = (first_bits << 18) | (second_bits << 12) | (third_bits << 6) | fourth_bits;
    }
    else {
        // if utf8 encoding is too long or utf8_buffer did not have a null terminator
        return -1;
    }

    return codepoint;
}