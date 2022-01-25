/*
    bits.h
    Jeff Ondich, 19 January 2022
*/

#include <stdbool.h>

// Replaces each char in the specified null-terminated 
// string with its uppercase equivalent if it's a lowercase
// Latin letter (ASCII 97-122). Any other char remains
// unchanged. If s is NULL, to_upper does nothing.
// 
// The trick, however, is that to_upper may only use
// bitwise operations on the chars in s. That is, when
// computing the new char for a particular location,
// you may only use >>, <<, &, |, ~, or ^. No +, no -, and
// no calling the toupper library function.
//
// (You may, however, test for whether a char is a letter by
// using <, <=, >, and >=, or even the library functions
// islower, isupper, and isalpha.)
//
// Hint: take a look at the binary ASCII values for corresponding
// uppercase and lowercase letters. Do they have a difference
// in common? (Spoiler: yes.) Can you exploit that difference in
// to_upper and to_lower? (Spoiler: also yes.)
void to_upper(char *s);

// Replaces each char in the specified null-terminated 
// string with its lowercase equivalent if it's an uppercase
// Latin letter (ASCII 65-90). Any other char remains
// unchanged. If s is NULL, to_upper does nothing.
// 
// Same implementation restrictions as for to_upper.
void to_lower(char *s);

// For this function, bits are numbered starting at
// 0 at the lowest-order bit up to 31 for the highest-
// order bit. So, for example, the 1 bit in the N
// shown below is located at bit index 23.
//
//     3322 2222 2222 1111 1111 1100 0000 0000
//     1098 7654 3210 9876 5432 1098 7654 3210
//
//   N 0000 0000 1000 0000 0000 0000 0000 0000
// 
// Given a non-negative integer n, bit_index between
// 0 and 30 inclusive, and a bit_count between 1 and
// (31 - bit_index) inclusive, middle_bits returns
// the value that the specified set of bits of n
// represent.
//
// For example:
//     n    0000 0000 0101 1100 0000 0000 0000 0000
//     bit_index 18
//     bit_count 4
//
// should result in a return value of 7, because bits
// 21, 20, 19, and 18 are 0111.
//
// If n, bit_index, and bit_count do not satisfy the
// constraints specified above, middle_bits should
// return n.
int middle_bits(int n, int bit_index, int bit_count);

// Given a 32-bit integer codepoint between 0 and 0x10FFFF
// and a pointer to a char buffer containing enough
// space to contain at least 5 chars, to_utf8 fills
// the buffer with the bytes in the UTF-8 encoding of the
// codepoint followed by a null byte.
//
// For example, if the codepoint is 0xE9 (that is,
// the Unicode codepoint for é), the first three
// chars in utf8_buffer should be filled by:
//
//      0xC3 0xA9 0x00
//
// If utf8_buffer is NULL or codepoint < 0 or
// codepoint > 0x10FFFF, to_utf8 returns false. Otherwise,
// it returns true.
//
// Suggestions:
// - Consult the chart at https://en.wikipedia.org/wiki/UTF-8#Encoding
// - Consider using your middle_bits function to help you
// - You might find this helpful: https://sandbox.jeffondich.com/encoder/
bool to_utf8(int codepoint, char *utf8_buffer);

// Given a char buffer containing the (null-terminated)
// bytes in the UTF-8 encoding of a codepoint between
// 0 and 0x10FFFF, from_utf8 returns the codepoint.
//
// If utf8_buffer is NULL or if the bytes contained
// there do not correspond to a valid UTF-8 encoding,
// from_utf8 returns -1.
//
// For example, if utf8_buffer contains:
//
//      0xC3 0xA9 0x00
//
// from_utf8 returns 0xE9 (also known as 233).
int from_utf8(char *utf8_buffer);
