/* ANSI-C code produced by gperf version 3.1 */
/* Command-line: gperf -m 10 ./unictype/scripts_byname.gperf  */
/* Computed positions: -k'1,3,5,8' */

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

#line 20 "./unictype/scripts_byname.gperf"
struct named_script { int name; unsigned int index; };

#define TOTAL_KEYWORDS 161
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 22
#define MIN_HASH_VALUE 2
#define MAX_HASH_VALUE 226
/* maximum key range = 225, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
scripts_hash (register const char *str, register size_t len)
{
  static const unsigned char asso_values[] =
    {
      227, 227, 227, 227, 227, 227, 227, 227, 227, 227,
      227, 227, 227, 227, 227, 227, 227, 227, 227, 227,
      227, 227, 227, 227, 227, 227, 227, 227, 227, 227,
      227, 227, 227, 227, 227, 227, 227, 227, 227, 227,
      227, 227, 227, 227, 227, 227, 227, 227, 227, 227,
      227, 227, 227, 227, 227, 227, 227, 227, 227, 227,
      227, 227, 227, 227, 227,  44,  18,   0,  94,  38,
      227,  81,  91,  34,   2,  19,  32,   3,  50,  30,
       15, 227,  77,   7,  17,  37,  51,  99, 227,   0,
        0, 227, 227, 227, 227,  77, 227,   1,  49,  97,
       30,  32,  38,  43,  76,   6,  51,   5,  51,   3,
        0,   0, 112, 227,   5,  64,  13,  16,  42,   4,
      227,  82,   8, 227, 227, 227, 227, 227, 227, 227,
      227, 227, 227, 227, 227, 227, 227, 227, 227, 227,
      227, 227, 227, 227, 227, 227, 227, 227, 227, 227,
      227, 227, 227, 227, 227, 227, 227, 227, 227, 227,
      227, 227, 227, 227, 227, 227, 227, 227, 227, 227,
      227, 227, 227, 227, 227, 227, 227, 227, 227, 227,
      227, 227, 227, 227, 227, 227, 227, 227, 227, 227,
      227, 227, 227, 227, 227, 227, 227, 227, 227, 227,
      227, 227, 227, 227, 227, 227, 227, 227, 227, 227,
      227, 227, 227, 227, 227, 227, 227, 227, 227, 227,
      227, 227, 227, 227, 227, 227, 227, 227, 227, 227,
      227, 227, 227, 227, 227, 227, 227, 227, 227, 227,
      227, 227, 227, 227, 227, 227, 227, 227, 227, 227,
      227, 227, 227, 227, 227, 227
    };
  register unsigned int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[7]];
      /*FALLTHROUGH*/
      case 7:
      case 6:
      case 5:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
      case 3:
        hval += asso_values[(unsigned char)str[2]];
      /*FALLTHROUGH*/
      case 2:
      case 1:
        hval += asso_values[(unsigned char)str[0]];
        break;
    }
  return hval;
}

struct script_stringpool_t
  {
    char script_stringpool_str2[sizeof("Yi")];
    char script_stringpool_str5[sizeof("Cham")];
    char script_stringpool_str6[sizeof("Mro")];
    char script_stringpool_str8[sizeof("Miao")];
    char script_stringpool_str9[sizeof("Common")];
    char script_stringpool_str10[sizeof("Chakma")];
    char script_stringpool_str11[sizeof("Mandaic")];
    char script_stringpool_str12[sizeof("Carian")];
    char script_stringpool_str13[sizeof("Mongolian")];
    char script_stringpool_str14[sizeof("Myanmar")];
    char script_stringpool_str15[sizeof("Sinhala")];
    char script_stringpool_str16[sizeof("Sharada")];
    char script_stringpool_str17[sizeof("Chorasmian")];
    char script_stringpool_str19[sizeof("Syriac")];
    char script_stringpool_str20[sizeof("Cuneiform")];
    char script_stringpool_str21[sizeof("Shavian")];
    char script_stringpool_str22[sizeof("Thai")];
    char script_stringpool_str24[sizeof("Thaana")];
    char script_stringpool_str25[sizeof("Samaritan")];
    char script_stringpool_str26[sizeof("Bengali")];
    char script_stringpool_str27[sizeof("Kannada")];
    char script_stringpool_str28[sizeof("Brahmi")];
    char script_stringpool_str29[sizeof("Bamum")];
    char script_stringpool_str30[sizeof("Khojki")];
    char script_stringpool_str31[sizeof("Phoenician")];
    char script_stringpool_str32[sizeof("Khmer")];
    char script_stringpool_str33[sizeof("Takri")];
    char script_stringpool_str34[sizeof("Toto")];
    char script_stringpool_str35[sizeof("Lao")];
    char script_stringpool_str36[sizeof("Caucasian_Albanian")];
    char script_stringpool_str37[sizeof("Modi")];
    char script_stringpool_str38[sizeof("Bhaiksuki")];
    char script_stringpool_str39[sizeof("Tangut")];
    char script_stringpool_str40[sizeof("Osmanya")];
    char script_stringpool_str41[sizeof("Batak")];
    char script_stringpool_str42[sizeof("Oriya")];
    char script_stringpool_str43[sizeof("Kharoshthi")];
    char script_stringpool_str44[sizeof("Yezidi")];
    char script_stringpool_str45[sizeof("Tirhuta")];
    char script_stringpool_str46[sizeof("Katakana")];
    char script_stringpool_str47[sizeof("Saurashtra")];
    char script_stringpool_str48[sizeof("Ahom")];
    char script_stringpool_str49[sizeof("Canadian_Aboriginal")];
    char script_stringpool_str50[sizeof("Latin")];
    char script_stringpool_str51[sizeof("Tai_Tham")];
    char script_stringpool_str52[sizeof("Khitan_Small_Script")];
    char script_stringpool_str53[sizeof("Nko")];
    char script_stringpool_str54[sizeof("Mende_Kikakui")];
    char script_stringpool_str55[sizeof("Armenian")];
    char script_stringpool_str56[sizeof("Limbu")];
    char script_stringpool_str57[sizeof("Arabic")];
    char script_stringpool_str58[sizeof("Newa")];
    char script_stringpool_str59[sizeof("Linear_B")];
    char script_stringpool_str60[sizeof("Vai")];
    char script_stringpool_str61[sizeof("Tai_Le")];
    char script_stringpool_str62[sizeof("Multani")];
    char script_stringpool_str63[sizeof("Sogdian")];
    char script_stringpool_str66[sizeof("Zanabazar_Square")];
    char script_stringpool_str67[sizeof("Anatolian_Hieroglyphs")];
    char script_stringpool_str68[sizeof("Osage")];
    char script_stringpool_str69[sizeof("Lydian")];
    char script_stringpool_str70[sizeof("Tagbanwa")];
    char script_stringpool_str72[sizeof("Cherokee")];
    char script_stringpool_str75[sizeof("Khudawadi")];
    char script_stringpool_str76[sizeof("Tamil")];
    char script_stringpool_str77[sizeof("Braille")];
    char script_stringpool_str79[sizeof("Makasar")];
    char script_stringpool_str81[sizeof("Sundanese")];
    char script_stringpool_str82[sizeof("Meetei_Mayek")];
    char script_stringpool_str83[sizeof("Vithkuqi")];
    char script_stringpool_str84[sizeof("Javanese")];
    char script_stringpool_str85[sizeof("Linear_A")];
    char script_stringpool_str86[sizeof("Tibetan")];
    char script_stringpool_str87[sizeof("Tangsa")];
    char script_stringpool_str88[sizeof("Old_Permic")];
    char script_stringpool_str89[sizeof("Phags_Pa")];
    char script_stringpool_str91[sizeof("Marchen")];
    char script_stringpool_str92[sizeof("Old_Turkic")];
    char script_stringpool_str93[sizeof("Bassa_Vah")];
    char script_stringpool_str94[sizeof("Han")];
    char script_stringpool_str95[sizeof("Tai_Viet")];
    char script_stringpool_str96[sizeof("Avestan")];
    char script_stringpool_str97[sizeof("Old_South_Arabian")];
    char script_stringpool_str98[sizeof("Hanunoo")];
    char script_stringpool_str99[sizeof("Soyombo")];
    char script_stringpool_str100[sizeof("Lisu")];
    char script_stringpool_str101[sizeof("Buginese")];
    char script_stringpool_str102[sizeof("Grantha")];
    char script_stringpool_str103[sizeof("Adlam")];
    char script_stringpool_str104[sizeof("Sora_Sompeng")];
    char script_stringpool_str105[sizeof("Glagolitic")];
    char script_stringpool_str106[sizeof("Gothic")];
    char script_stringpool_str107[sizeof("Kaithi")];
    char script_stringpool_str108[sizeof("Old_Sogdian")];
    char script_stringpool_str109[sizeof("Balinese")];
    char script_stringpool_str110[sizeof("Nandinagari")];
    char script_stringpool_str111[sizeof("Hatran")];
    char script_stringpool_str112[sizeof("Medefaidrin")];
    char script_stringpool_str113[sizeof("Hangul")];
    char script_stringpool_str114[sizeof("Ogham")];
    char script_stringpool_str115[sizeof("Warang_Citi")];
    char script_stringpool_str116[sizeof("Gurmukhi")];
    char script_stringpool_str117[sizeof("Telugu")];
    char script_stringpool_str118[sizeof("Tagalog")];
    char script_stringpool_str119[sizeof("Pau_Cin_Hau")];
    char script_stringpool_str120[sizeof("Siddham")];
    char script_stringpool_str121[sizeof("Psalter_Pahlavi")];
    char script_stringpool_str122[sizeof("Nabataean")];
    char script_stringpool_str123[sizeof("Greek")];
    char script_stringpool_str124[sizeof("Coptic")];
    char script_stringpool_str125[sizeof("Cypriot")];
    char script_stringpool_str126[sizeof("Nyiakeng_Puachue_Hmong")];
    char script_stringpool_str127[sizeof("Meroitic_Cursive")];
    char script_stringpool_str128[sizeof("Elymaic")];
    char script_stringpool_str129[sizeof("Buhid")];
    char script_stringpool_str130[sizeof("Cypro_Minoan")];
    char script_stringpool_str131[sizeof("Meroitic_Hieroglyphs")];
    char script_stringpool_str132[sizeof("Georgian")];
    char script_stringpool_str133[sizeof("Syloti_Nagri")];
    char script_stringpool_str134[sizeof("Rejang")];
    char script_stringpool_str135[sizeof("Nushu")];
    char script_stringpool_str136[sizeof("Lycian")];
    char script_stringpool_str137[sizeof("Inscriptional_Pahlavi")];
    char script_stringpool_str138[sizeof("Inscriptional_Parthian")];
    char script_stringpool_str139[sizeof("Tifinagh")];
    char script_stringpool_str140[sizeof("Old_North_Arabian")];
    char script_stringpool_str141[sizeof("Bopomofo")];
    char script_stringpool_str142[sizeof("Manichaean")];
    char script_stringpool_str143[sizeof("Dogra")];
    char script_stringpool_str144[sizeof("Mahajani")];
    char script_stringpool_str146[sizeof("Malayalam")];
    char script_stringpool_str147[sizeof("Devanagari")];
    char script_stringpool_str148[sizeof("Hiragana")];
    char script_stringpool_str149[sizeof("Ugaritic")];
    char script_stringpool_str150[sizeof("Old_Persian")];
    char script_stringpool_str151[sizeof("Gujarati")];
    char script_stringpool_str153[sizeof("Egyptian_Hieroglyphs")];
    char script_stringpool_str155[sizeof("Old_Italic")];
    char script_stringpool_str156[sizeof("Inherited")];
    char script_stringpool_str157[sizeof("Palmyrene")];
    char script_stringpool_str158[sizeof("Elbasan")];
    char script_stringpool_str159[sizeof("New_Tai_Lue")];
    char script_stringpool_str161[sizeof("Cyrillic")];
    char script_stringpool_str162[sizeof("Masaram_Gondi")];
    char script_stringpool_str170[sizeof("Deseret")];
    char script_stringpool_str172[sizeof("Gunjala_Gondi")];
    char script_stringpool_str173[sizeof("SignWriting")];
    char script_stringpool_str178[sizeof("Hebrew")];
    char script_stringpool_str179[sizeof("Runic")];
    char script_stringpool_str181[sizeof("Wancho")];
    char script_stringpool_str183[sizeof("Old_Uyghur")];
    char script_stringpool_str191[sizeof("Kayah_Li")];
    char script_stringpool_str197[sizeof("Ol_Chiki")];
    char script_stringpool_str198[sizeof("Pahawh_Hmong")];
    char script_stringpool_str207[sizeof("Old_Hungarian")];
    char script_stringpool_str214[sizeof("Duployan")];
    char script_stringpool_str216[sizeof("Dives_Akuru")];
    char script_stringpool_str218[sizeof("Imperial_Aramaic")];
    char script_stringpool_str219[sizeof("Ethiopic")];
    char script_stringpool_str221[sizeof("Hanifi_Rohingya")];
    char script_stringpool_str226[sizeof("Lepcha")];
  };
static const struct script_stringpool_t script_stringpool_contents =
  {
    "Yi",
    "Cham",
    "Mro",
    "Miao",
    "Common",
    "Chakma",
    "Mandaic",
    "Carian",
    "Mongolian",
    "Myanmar",
    "Sinhala",
    "Sharada",
    "Chorasmian",
    "Syriac",
    "Cuneiform",
    "Shavian",
    "Thai",
    "Thaana",
    "Samaritan",
    "Bengali",
    "Kannada",
    "Brahmi",
    "Bamum",
    "Khojki",
    "Phoenician",
    "Khmer",
    "Takri",
    "Toto",
    "Lao",
    "Caucasian_Albanian",
    "Modi",
    "Bhaiksuki",
    "Tangut",
    "Osmanya",
    "Batak",
    "Oriya",
    "Kharoshthi",
    "Yezidi",
    "Tirhuta",
    "Katakana",
    "Saurashtra",
    "Ahom",
    "Canadian_Aboriginal",
    "Latin",
    "Tai_Tham",
    "Khitan_Small_Script",
    "Nko",
    "Mende_Kikakui",
    "Armenian",
    "Limbu",
    "Arabic",
    "Newa",
    "Linear_B",
    "Vai",
    "Tai_Le",
    "Multani",
    "Sogdian",
    "Zanabazar_Square",
    "Anatolian_Hieroglyphs",
    "Osage",
    "Lydian",
    "Tagbanwa",
    "Cherokee",
    "Khudawadi",
    "Tamil",
    "Braille",
    "Makasar",
    "Sundanese",
    "Meetei_Mayek",
    "Vithkuqi",
    "Javanese",
    "Linear_A",
    "Tibetan",
    "Tangsa",
    "Old_Permic",
    "Phags_Pa",
    "Marchen",
    "Old_Turkic",
    "Bassa_Vah",
    "Han",
    "Tai_Viet",
    "Avestan",
    "Old_South_Arabian",
    "Hanunoo",
    "Soyombo",
    "Lisu",
    "Buginese",
    "Grantha",
    "Adlam",
    "Sora_Sompeng",
    "Glagolitic",
    "Gothic",
    "Kaithi",
    "Old_Sogdian",
    "Balinese",
    "Nandinagari",
    "Hatran",
    "Medefaidrin",
    "Hangul",
    "Ogham",
    "Warang_Citi",
    "Gurmukhi",
    "Telugu",
    "Tagalog",
    "Pau_Cin_Hau",
    "Siddham",
    "Psalter_Pahlavi",
    "Nabataean",
    "Greek",
    "Coptic",
    "Cypriot",
    "Nyiakeng_Puachue_Hmong",
    "Meroitic_Cursive",
    "Elymaic",
    "Buhid",
    "Cypro_Minoan",
    "Meroitic_Hieroglyphs",
    "Georgian",
    "Syloti_Nagri",
    "Rejang",
    "Nushu",
    "Lycian",
    "Inscriptional_Pahlavi",
    "Inscriptional_Parthian",
    "Tifinagh",
    "Old_North_Arabian",
    "Bopomofo",
    "Manichaean",
    "Dogra",
    "Mahajani",
    "Malayalam",
    "Devanagari",
    "Hiragana",
    "Ugaritic",
    "Old_Persian",
    "Gujarati",
    "Egyptian_Hieroglyphs",
    "Old_Italic",
    "Inherited",
    "Palmyrene",
    "Elbasan",
    "New_Tai_Lue",
    "Cyrillic",
    "Masaram_Gondi",
    "Deseret",
    "Gunjala_Gondi",
    "SignWriting",
    "Hebrew",
    "Runic",
    "Wancho",
    "Old_Uyghur",
    "Kayah_Li",
    "Ol_Chiki",
    "Pahawh_Hmong",
    "Old_Hungarian",
    "Duployan",
    "Dives_Akuru",
    "Imperial_Aramaic",
    "Ethiopic",
    "Hanifi_Rohingya",
    "Lepcha"
  };
#define script_stringpool ((const char *) &script_stringpool_contents)

static const struct named_script script_names[] =
  {
    {-1}, {-1},
#line 67 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str2, 36},
    {-1}, {-1},
#line 107 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str5, 76},
#line 145 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str6, 114},
    {-1},
#line 129 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str8, 98},
#line 31 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str9, 0},
#line 126 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str10, 95},
#line 125 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str11, 94},
#line 105 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str12, 74},
#line 62 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str13, 31},
#line 53 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str14, 22},
#line 49 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str15, 18},
#line 130 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str16, 99},
#line 183 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str17, 152},
    {-1},
#line 38 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str19, 7},
#line 93 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str20, 62},
#line 80 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str21, 49},
#line 50 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str22, 19},
    {-1},
#line 39 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str24, 8},
#line 112 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str25, 81},
#line 41 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str26, 10},
#line 47 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str27, 16},
#line 124 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str28, 93},
#line 114 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str29, 83},
#line 139 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str30, 108},
#line 94 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str31, 63},
#line 61 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str32, 30},
#line 132 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str33, 101},
#line 190 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str34, 159},
#line 51 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str35, 20},
#line 133 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str36, 102},
#line 144 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str37, 113},
#line 163 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str38, 132},
#line 167 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str39, 136},
#line 81 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str40, 50},
#line 123 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str41, 92},
#line 44 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str42, 13},
#line 91 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str43, 60},
#line 186 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str44, 155},
#line 154 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str45, 123},
#line 64 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str46, 33},
#line 101 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str47, 70},
#line 156 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str48, 125},
#line 58 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str49, 27},
#line 32 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str50, 1},
#line 108 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str51, 77},
#line 185 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str52, 154},
#line 96 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str53, 65},
#line 143 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str54, 112},
#line 35 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str55, 4},
#line 76 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str56, 45},
#line 37 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str57, 6},
#line 165 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str58, 134},
#line 78 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str59, 47},
#line 100 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str60, 69},
#line 77 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str61, 46},
#line 159 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str62, 128},
#line 177 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str63, 146},
    {-1}, {-1},
#line 171 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str66, 140},
#line 157 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str67, 126},
#line 166 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str68, 135},
#line 106 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str69, 75},
#line 75 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str70, 44},
    {-1},
#line 57 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str72, 26},
    {-1}, {-1},
#line 153 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str75, 122},
#line 45 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str76, 14},
#line 83 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str77, 52},
    {-1},
#line 174 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str79, 143},
    {-1},
#line 97 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str81, 66},
#line 116 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str82, 85},
#line 191 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str83, 160},
#line 115 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str84, 84},
#line 140 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str85, 109},
#line 52 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str86, 21},
#line 189 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str87, 158},
#line 150 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str88, 119},
#line 95 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str89, 64},
    {-1},
#line 164 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str91, 133},
#line 121 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str92, 90},
#line 134 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str93, 103},
#line 66 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str94, 35},
#line 109 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str95, 78},
#line 110 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str96, 79},
#line 118 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str97, 87},
#line 73 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str98, 42},
#line 170 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str99, 139},
#line 113 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str100, 82},
#line 84 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str101, 53},
#line 137 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str102, 106},
#line 162 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str103, 131},
#line 131 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str104, 100},
#line 87 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str105, 56},
#line 69 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str106, 38},
#line 122 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str107, 91},
#line 178 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str108, 147},
#line 92 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str109, 61},
#line 180 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str110, 149},
#line 158 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str111, 127},
#line 175 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str112, 144},
#line 55 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str113, 24},
#line 59 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str114, 28},
#line 155 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str115, 124},
#line 42 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str116, 11},
#line 46 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str117, 15},
#line 72 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str118, 41},
#line 149 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str119, 118},
#line 152 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str120, 121},
#line 151 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str121, 120},
#line 147 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str122, 116},
#line 33 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str123, 2},
#line 85 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str124, 54},
#line 82 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str125, 51},
#line 181 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str126, 150},
#line 127 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str127, 96},
#line 179 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str128, 148},
#line 74 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str129, 43},
#line 187 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str130, 156},
#line 128 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str131, 97},
#line 54 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str132, 23},
#line 89 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str133, 58},
#line 103 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str134, 72},
#line 169 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str135, 138},
#line 104 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str136, 73},
#line 120 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str137, 89},
#line 119 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str138, 88},
#line 88 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str139, 57},
#line 146 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str140, 115},
#line 65 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str141, 34},
#line 142 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str142, 111},
#line 172 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str143, 141},
#line 141 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str144, 110},
    {-1},
#line 48 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str146, 17},
#line 40 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str147, 9},
#line 63 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str148, 32},
#line 79 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str149, 48},
#line 90 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str150, 59},
#line 43 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str151, 12},
    {-1},
#line 111 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str153, 80},
    {-1},
#line 68 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str155, 37},
#line 71 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str156, 40},
#line 148 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str157, 117},
#line 136 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str158, 105},
#line 86 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str159, 55},
    {-1},
#line 34 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str161, 3},
#line 168 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str162, 137},
    {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
#line 70 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str170, 39},
    {-1},
#line 173 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str172, 142},
#line 161 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str173, 130},
    {-1}, {-1}, {-1}, {-1},
#line 36 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str178, 5},
#line 60 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str179, 29},
    {-1},
#line 182 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str181, 151},
    {-1},
#line 188 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str183, 157},
    {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
#line 102 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str191, 71},
    {-1}, {-1}, {-1}, {-1}, {-1},
#line 99 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str197, 68},
#line 138 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str198, 107},
    {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
#line 160 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str207, 129},
    {-1}, {-1}, {-1}, {-1}, {-1}, {-1},
#line 135 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str214, 104},
    {-1},
#line 184 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str216, 153},
    {-1},
#line 117 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str218, 86},
#line 56 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str219, 25},
    {-1},
#line 176 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str221, 145},
    {-1}, {-1}, {-1}, {-1},
#line 98 "./unictype/scripts_byname.gperf"
    {(int)(size_t)&((struct script_stringpool_t *)0)->script_stringpool_str226, 67}
  };

static const struct named_script *
uc_script_lookup (register const char *str, register size_t len)
{
  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register unsigned int key = scripts_hash (str, len);

      if (key <= MAX_HASH_VALUE)
        {
          register int o = script_names[key].name;
          if (o >= 0)
            {
              register const char *s = o + script_stringpool;

              if (*str == *s && !strcmp (str + 1, s + 1))
                return &script_names[key];
            }
        }
    }
  return 0;
}
