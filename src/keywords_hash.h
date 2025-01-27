/* C++ code produced by gperf version 3.1 */
/* Command-line: gperf -L C++ -E -t keyword.gperf  */
/* Computed positions: -k'1,3' */

#include "token.h"
#include <cstring>

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
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gperf@gnu.org>."
#endif

#line 1 "keyword.gperf"
struct KeywordEntry {
    const char* name;
    Type tokenType;
};
/* maximum key range = 52, duplicates = 0 */

class Perfect_Hash
{
private:
  static inline unsigned int hash (const char *str, size_t len);
public:
  static struct KeywordEntry *in_word_set (const char *str, size_t len);
};

inline unsigned int
Perfect_Hash::hash (const char *str, size_t len)
{
  static unsigned char asso_values[] =
    {
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
  unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[static_cast<unsigned char>(str[2])];
      /*FALLTHROUGH*/
      case 2:
      case 1:
        hval += asso_values[static_cast<unsigned char>(str[0])];
        break;
    }
  return hval;
}

inline struct KeywordEntry *
Perfect_Hash::in_word_set (const char *str, size_t len)
{
  enum
    {
      TOTAL_KEYWORDS = 37,
      MIN_WORD_LENGTH = 2,
      MAX_WORD_LENGTH = 10,
      MIN_HASH_VALUE = 3,
      MAX_HASH_VALUE = 54
    };

  static struct KeywordEntry wordlist[] =
    {
      {""}, {""}, {""},
#line 20 "keyword.gperf"
      {"for", Type::tok_for},
      {""},
#line 16 "keyword.gperf"
      {"float", Type::tok_float},
#line 13 "keyword.gperf"
      {"return", Type::tok_return},
#line 32 "keyword.gperf"
      {"if", Type::tok_if},
#line 42 "keyword.gperf"
      {"int", Type::tok_int},
#line 14 "keyword.gperf"
      {"void", Type::tok_void},
#line 17 "keyword.gperf"
      {"short", Type::tok_short},
#line 29 "keyword.gperf"
      {"struct", Type::tok_struct},
#line 37 "keyword.gperf"
      {"typedef", Type::tok_typedef},
#line 9 "keyword.gperf"
      {"restrict", Type::tok_restrict},
#line 24 "keyword.gperf"
      {"goto", Type::tok_goto},
#line 22 "keyword.gperf"
      {"while", Type::tok_while},
#line 43 "keyword.gperf"
      {"switch", Type::tok_switch},
      {""},
#line 39 "keyword.gperf"
      {"register", Type::tok_register},
      {""},
#line 40 "keyword.gperf"
      {"union", Type::tok_union},
#line 21 "keyword.gperf"
      {"signed", Type::tok_signed},
#line 41 "keyword.gperf"
      {"do", Type::tok_do},
#line 10 "keyword.gperf"
      {"unsigned", Type::tok_unsigned},
#line 7 "keyword.gperf"
      {"auto", Type::tok_auto},
#line 26 "keyword.gperf"
      {"_Bool", Type::tok_bool},
#line 12 "keyword.gperf"
      {"extern", Type::tok_extern},
#line 27 "keyword.gperf"
      {"default", Type::tok_default},
#line 34 "keyword.gperf"
      {"_Complex", Type::tok_complex},
#line 38 "keyword.gperf"
      {"else", Type::tok_else},
#line 30 "keyword.gperf"
      {"_Imaginary", Type::tok_imaginary},
#line 33 "keyword.gperf"
      {"static", Type::tok_static},
      {""}, {""},
#line 8 "keyword.gperf"
      {"enum", Type::tok_enum},
#line 23 "keyword.gperf"
      {"const", Type::tok_const},
#line 35 "keyword.gperf"
      {"double", Type::tok_double},
      {""},
#line 31 "keyword.gperf"
      {"continue", Type::tok_continue},
#line 15 "keyword.gperf"
      {"case", Type::tok_case},
#line 11 "keyword.gperf"
      {"break", Type::tok_break},
#line 25 "keyword.gperf"
      {"sizeof", Type::tok_sizeof},
      {""}, {""},
#line 36 "keyword.gperf"
      {"long", Type::tok_long},
      {""}, {""}, {""},
#line 18 "keyword.gperf"
      {"volatile", Type::tok_volatile},
      {""}, {""},
#line 28 "keyword.gperf"
      {"inline", Type::tok_inline},
      {""}, {""},
#line 19 "keyword.gperf"
      {"char", Type::tok_char}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      unsigned int key = hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          const char *s = wordlist[key].name;

          if (*str == *s && !strcmp (str + 1, s + 1))
            return &wordlist[key];
        }
    }
  return 0;
}
#line 44 "keyword.gperf"

