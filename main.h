/* C++ code produced by gperf version 3.1 */
/* Command-line: gperf -L C++ -E -t main.gperf  */
/* Computed positions: -k'' */

#line 1 "main.gperf"

#include <string>
#line 5 "main.gperf"
struct printMap {
	Type type;
	std::string;
};
/* maximum key range = 1, duplicates = 0 */

class Perfect_Hash
{
private:
  static inline unsigned int hash (const char *str, size_t len);
public:
  static struct printMap *in_word_set (const char *str, size_t len);
};

inline /*ARGSUSED*/
unsigned int
Perfect_Hash::hash (const char *str, size_t len)
{
  return 0;
}

struct printMap *
Perfect_Hash::in_word_set (const char *str, size_t len)
{
  enum
    {
      TOTAL_KEYWORDS = 1,
      MIN_WORD_LENGTH = 10,
      MAX_WORD_LENGTH = 10,
      MIN_HASH_VALUE = 0,
      MAX_HASH_VALUE = 0
    };

  static struct printMap wordlist[] =
    {
#line 11 "main.gperf"
      {"tok_string", "tok_string"}
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
#line 12 "main.gperf"

