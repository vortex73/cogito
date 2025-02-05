#include "token.h"
#include <span>

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
		&& ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
		&& (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
		&& ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
		&& ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
		&& ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
		&& ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
		&& ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
		&& ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
		&& ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
		&& ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
		&& ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
		&& ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
		&& ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
		&& ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
		&& ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
		&& ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
		&& ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
		&& ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
		&& ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
		&& ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
		&& ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
		&& ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
#endif

struct KeywordEntry {
	const char* name;
	Type tokenType;
};

class Perfect_Hash {
	private:
		static constexpr unsigned char asso_values[] = {
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 20, 55, 20, 15, 30,
			20, 20,  0, 10, 55,  5, 55, 55, 40,  0,
			0,  0,  5, 55,  0,  5,  0, 10,  0,  5,
			55, 55, 30, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55, 55, 55, 55, 55,
			55, 55, 55, 55, 55, 55
		};

		static constexpr size_t TOTAL_KEYWORDS = 37;
		static constexpr size_t MIN_WORD_LENGTH = 2;
		static constexpr size_t MAX_WORD_LENGTH = 10;
		static constexpr size_t MIN_HASH_VALUE = 3;
		static constexpr size_t MAX_HASH_VALUE = 54;

		static inline unsigned int hash(std::span<const char> span);
		static inline bool equals(std::span<const char> span, const char* keyword);

	public:
		static const KeywordEntry* in_word_set(std::span<const char> span);
};

inline unsigned int Perfect_Hash::hash(std::span<const char> span) {
	const char* str = span.data();
	unsigned int hval = span.size();

	switch (span.size()) {
		default:
			hval += asso_values[static_cast<unsigned char>(str[2])];
		case 2:
		case 1:
			hval += asso_values[static_cast<unsigned char>(str[0])];
			break;
	}
	return hval;
}

inline bool Perfect_Hash::equals(std::span<const char> span, const char* keyword) {
	if (span.empty() || !keyword || !keyword[0]) return false;

	if (span[0] != keyword[0]) return false;

	size_t i = 1;
	while (i < span.size() && keyword[i] != '\0') {
		if (span[i] != keyword[i]) return false;
		i++;
	}
	return i == span.size() && keyword[i] == '\0';
}

inline const KeywordEntry* Perfect_Hash::in_word_set(std::span<const char> span) {
	static constexpr KeywordEntry wordlist[] = {
		{""}, {""}, {""},
		{"for", Type::tok_for},
		{""},
		{"float", Type::tok_float},
		{"return", Type::tok_return},
		{"if", Type::tok_if},
		{"int", Type::tok_int},
		{"void", Type::tok_void},
		{"short", Type::tok_short},
		{"struct", Type::tok_struct},
		{"typedef", Type::tok_typedef},
		{"restrict", Type::tok_restrict},
		{"goto", Type::tok_goto},
		{"while", Type::tok_while},
		{"switch", Type::tok_switch},
		{""},
		{"register", Type::tok_register},
		{""},
		{"union", Type::tok_union},
		{"signed", Type::tok_signed},
		{"do", Type::tok_do},
		{"unsigned", Type::tok_unsigned},
		{"auto", Type::tok_auto},
		{"_Bool", Type::tok_bool},
		{"extern", Type::tok_extern},
		{"default", Type::tok_default},
		{"_Complex", Type::tok_complex},
		{"else", Type::tok_else},
		{"_Imaginary", Type::tok_imaginary},
		{"static", Type::tok_static},
		{""}, {""},
		{"enum", Type::tok_enum},
		{"const", Type::tok_const},
		{"double", Type::tok_double},
		{""},
		{"continue", Type::tok_continue},
		{"case", Type::tok_case},
		{"break", Type::tok_break},
		{"sizeof", Type::tok_sizeof},
		{""}, {""},
		{"long", Type::tok_long},
		{""}, {""}, {""},
		{"volatile", Type::tok_volatile},
		{""}, {""},
		{"inline", Type::tok_inline},
		{""}, {""},
		{"char", Type::tok_char}
	};

	if (span.size() <= MAX_WORD_LENGTH && span.size() >= MIN_WORD_LENGTH) {
		unsigned int key = hash(span);
		if (key <= MAX_HASH_VALUE) {
			const char* s = wordlist[key].name;
			if (equals(span, s)) {
				return &wordlist[key];
			}
		}
	}
	return nullptr;
}
