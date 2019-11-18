/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 37 "policy_parse.y" /* yacc.c:339  */

#include <config.h>

#include <sys/types.h>
#include <assert.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>

#include <sepol/policydb/expand.h>
#include <sepol/policydb/policydb.h>
#include <sepol/policydb/services.h>
#include <sepol/policydb/conditional.h>
#include <sepol/policydb/flask.h>
#include <sepol/policydb/hierarchy.h>
#ifdef HAVE_SEPOL_POLICYCAPS
#include <sepol/policydb/polcaps.h>
#endif

#include "queue.h"
#include <qpol/policy.h>
#include "module_compiler.h"
#include "policy_define.h"

extern policydb_t *policydbp;
extern unsigned int pass;

extern char yytext[];
extern int yylex(void);
extern int yywarn(char *msg);
extern int yyerror(char *msg);

typedef int (* require_func_t)();

/* redefine input so we can read from a string */
/* borrowed from O'Reilly lex and yacc pg 157 */
extern char qpol_src_input[];
extern char *qpol_src_inputptr;/* current position in qpol_src_input */
extern char *qpol_src_inputlim;/* end of data */


#line 114 "policy_parse.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "y.tab.h".  */
#ifndef YY_YY_POLICY_PARSE_H_INCLUDED
# define YY_YY_POLICY_PARSE_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    PATH = 258,
    FILENAME = 259,
    CLONE = 260,
    COMMON = 261,
    CLASS = 262,
    CONSTRAIN = 263,
    VALIDATETRANS = 264,
    INHERITS = 265,
    SID = 266,
    ROLE = 267,
    ROLES = 268,
    TYPEALIAS = 269,
    TYPEATTRIBUTE = 270,
    TYPEBOUNDS = 271,
    TYPE = 272,
    TYPES = 273,
    ALIAS = 274,
    ATTRIBUTE = 275,
    BOOL = 276,
    IF = 277,
    ELSE = 278,
    TYPE_TRANSITION = 279,
    TYPE_MEMBER = 280,
    TYPE_CHANGE = 281,
    ROLE_TRANSITION = 282,
    RANGE_TRANSITION = 283,
    SENSITIVITY = 284,
    DOMINANCE = 285,
    DOM = 286,
    DOMBY = 287,
    INCOMP = 288,
    CATEGORY = 289,
    LEVEL = 290,
    RANGE = 291,
    MLSCONSTRAIN = 292,
    MLSVALIDATETRANS = 293,
    USER = 294,
    NEVERALLOW = 295,
    ALLOW = 296,
    AUDITALLOW = 297,
    AUDITDENY = 298,
    DONTAUDIT = 299,
    SOURCE = 300,
    TARGET = 301,
    SAMEUSER = 302,
    FSCON = 303,
    PORTCON = 304,
    NETIFCON = 305,
    NODECON = 306,
    PIRQCON = 307,
    IOMEMCON = 308,
    IOPORTCON = 309,
    PCIDEVICECON = 310,
    FSUSEXATTR = 311,
    FSUSETASK = 312,
    FSUSETRANS = 313,
    FSUSEPSID = 314,
    GENFSCON = 315,
    U1 = 316,
    U2 = 317,
    U3 = 318,
    R1 = 319,
    R2 = 320,
    R3 = 321,
    T1 = 322,
    T2 = 323,
    T3 = 324,
    L1 = 325,
    L2 = 326,
    H1 = 327,
    H2 = 328,
    NOT = 329,
    AND = 330,
    OR = 331,
    XOR = 332,
    CTRUE = 333,
    CFALSE = 334,
    IDENTIFIER = 335,
    NUMBER = 336,
    EQUALS = 337,
    NOTEQUAL = 338,
    IPV4_ADDR = 339,
    IPV6_ADDR = 340,
    MODULE = 341,
    VERSION_IDENTIFIER = 342,
    REQUIRE = 343,
    OPTIONAL = 344,
    POLICYCAP = 345,
    PERMISSIVE = 346
  };
#endif
/* Tokens.  */
#define PATH 258
#define FILENAME 259
#define CLONE 260
#define COMMON 261
#define CLASS 262
#define CONSTRAIN 263
#define VALIDATETRANS 264
#define INHERITS 265
#define SID 266
#define ROLE 267
#define ROLES 268
#define TYPEALIAS 269
#define TYPEATTRIBUTE 270
#define TYPEBOUNDS 271
#define TYPE 272
#define TYPES 273
#define ALIAS 274
#define ATTRIBUTE 275
#define BOOL 276
#define IF 277
#define ELSE 278
#define TYPE_TRANSITION 279
#define TYPE_MEMBER 280
#define TYPE_CHANGE 281
#define ROLE_TRANSITION 282
#define RANGE_TRANSITION 283
#define SENSITIVITY 284
#define DOMINANCE 285
#define DOM 286
#define DOMBY 287
#define INCOMP 288
#define CATEGORY 289
#define LEVEL 290
#define RANGE 291
#define MLSCONSTRAIN 292
#define MLSVALIDATETRANS 293
#define USER 294
#define NEVERALLOW 295
#define ALLOW 296
#define AUDITALLOW 297
#define AUDITDENY 298
#define DONTAUDIT 299
#define SOURCE 300
#define TARGET 301
#define SAMEUSER 302
#define FSCON 303
#define PORTCON 304
#define NETIFCON 305
#define NODECON 306
#define PIRQCON 307
#define IOMEMCON 308
#define IOPORTCON 309
#define PCIDEVICECON 310
#define FSUSEXATTR 311
#define FSUSETASK 312
#define FSUSETRANS 313
#define FSUSEPSID 314
#define GENFSCON 315
#define U1 316
#define U2 317
#define U3 318
#define R1 319
#define R2 320
#define R3 321
#define T1 322
#define T2 323
#define T3 324
#define L1 325
#define L2 326
#define H1 327
#define H2 328
#define NOT 329
#define AND 330
#define OR 331
#define XOR 332
#define CTRUE 333
#define CFALSE 334
#define IDENTIFIER 335
#define NUMBER 336
#define EQUALS 337
#define NOTEQUAL 338
#define IPV4_ADDR 339
#define IPV6_ADDR 340
#define MODULE 341
#define VERSION_IDENTIFIER 342
#define REQUIRE 343
#define OPTIONAL 344
#define POLICYCAP 345
#define PERMISSIVE 346

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 85 "policy_parse.y" /* yacc.c:355  */

	unsigned int val;
	uintptr_t valptr;
	void *ptr;
        require_func_t require_func;

#line 343 "policy_parse.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_POLICY_PARSE_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 360 "policy_parse.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   902

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  102
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  168
/* YYNRULES -- Number of rules.  */
#define YYNRULES  335
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  667

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   346

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      97,    98,   101,     2,    96,    99,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    95,    94,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    92,     2,    93,   100,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   166,   166,   167,   169,   171,   174,   169,   178,   179,
     181,   184,   185,   187,   190,   192,   193,   195,   196,   198,
     201,   202,   204,   206,   208,   211,   212,   214,   216,   217,
     221,   223,   226,   228,   230,   233,   234,   236,   237,   239,
     241,   244,   245,   247,   249,   252,   253,   255,   256,   258,
     261,   264,   265,   267,   268,   269,   270,   271,   272,   274,
     275,   276,   277,   279,   280,   281,   282,   283,   284,   285,
     286,   287,   288,   290,   293,   295,   298,   301,   304,   307,
     308,   310,   313,   315,   318,   321,   324,   325,   327,   330,
     333,   336,   339,   342,   345,   348,   352,   355,   357,   359,
     361,   364,   367,   370,   373,   377,   379,   381,   383,   386,
     390,   394,   398,   402,   404,   406,   408,   411,   413,   416,
     417,   418,   419,   420,   422,   425,   428,   431,   434,   437,
     439,   442,   444,   447,   450,   452,   455,   457,   460,   461,
     463,   464,   466,   467,   469,   472,   475,   477,   480,   483,
     486,   489,   492,   495,   498,   498,   501,   501,   504,   504,
     507,   507,   510,   510,   513,   513,   516,   516,   519,   519,
     522,   522,   525,   528,   528,   531,   531,   534,   537,   537,
     540,   540,   543,   546,   549,   552,   555,   558,   562,   564,
     567,   569,   571,   573,   576,   577,   579,   582,   583,   585,
     586,   588,   591,   591,   593,   594,   596,   597,   598,   599,
     601,   604,   606,   609,   611,   614,   617,   618,   620,   621,
     623,   626,   628,   629,   631,   632,   634,   636,   639,   640,
     642,   643,   645,   648,   649,   651,   652,   654,   656,   659,
     660,   662,   663,   665,   667,   669,   671,   674,   675,   677,
     678,   680,   682,   682,   684,   687,   690,   692,   693,   695,
     697,   700,   702,   705,   706,   708,   710,   712,   714,   716,
     719,   722,   725,   725,   728,   731,   734,   735,   736,   737,
     738,   740,   741,   743,   746,   747,   749,   751,   751,   753,
     753,   753,   753,   755,   758,   761,   764,   767,   770,   773,
     778,   783,   786,   788,   790,   792,   793,   795,   796,   797,
     798,   799,   800,   802,   804,   805,   807,   808,   810,   813,
     814,   815,   816,   817,   818,   819,   821,   823,   827,   826,
     831,   833,   835,   838,   841,   842
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "PATH", "FILENAME", "CLONE", "COMMON",
  "CLASS", "CONSTRAIN", "VALIDATETRANS", "INHERITS", "SID", "ROLE",
  "ROLES", "TYPEALIAS", "TYPEATTRIBUTE", "TYPEBOUNDS", "TYPE", "TYPES",
  "ALIAS", "ATTRIBUTE", "BOOL", "IF", "ELSE", "TYPE_TRANSITION",
  "TYPE_MEMBER", "TYPE_CHANGE", "ROLE_TRANSITION", "RANGE_TRANSITION",
  "SENSITIVITY", "DOMINANCE", "DOM", "DOMBY", "INCOMP", "CATEGORY",
  "LEVEL", "RANGE", "MLSCONSTRAIN", "MLSVALIDATETRANS", "USER",
  "NEVERALLOW", "ALLOW", "AUDITALLOW", "AUDITDENY", "DONTAUDIT", "SOURCE",
  "TARGET", "SAMEUSER", "FSCON", "PORTCON", "NETIFCON", "NODECON",
  "PIRQCON", "IOMEMCON", "IOPORTCON", "PCIDEVICECON", "FSUSEXATTR",
  "FSUSETASK", "FSUSETRANS", "FSUSEPSID", "GENFSCON", "U1", "U2", "U3",
  "R1", "R2", "R3", "T1", "T2", "T3", "L1", "L2", "H1", "H2", "NOT", "AND",
  "OR", "XOR", "CTRUE", "CFALSE", "IDENTIFIER", "NUMBER", "EQUALS",
  "NOTEQUAL", "IPV4_ADDR", "IPV6_ADDR", "MODULE", "VERSION_IDENTIFIER",
  "REQUIRE", "OPTIONAL", "POLICYCAP", "PERMISSIVE", "'{'", "'}'", "';'",
  "':'", "','", "'('", "')'", "'-'", "'~'", "'*'", "$accept", "policy",
  "base_policy", "$@1", "$@2", "$@3", "classes", "class_def",
  "initial_sids", "initial_sid_def", "access_vectors", "opt_common_perms",
  "common_perms", "common_perms_def", "av_perms", "av_perms_def",
  "opt_mls", "mls", "sensitivities", "sensitivity_def", "alias_def",
  "dominance", "opt_categories", "categories", "category_def", "levels",
  "level_def", "mlspolicy", "mlspolicy_decl", "mlsconstraint_def",
  "mlsvalidatetrans_def", "te_rbac", "te_rbac_decl", "rbac_decl",
  "te_decl", "attribute_def", "type_def", "typealias_def",
  "typeattribute_def", "typebounds_def", "opt_attr_list", "bool_def",
  "bool_val", "cond_stmt_def", "cond_else", "cond_expr", "cond_expr_prim",
  "cond_pol_list", "cond_rule_def", "cond_transition_def",
  "cond_te_avtab_def", "cond_allow_def", "cond_auditallow_def",
  "cond_auditdeny_def", "cond_dontaudit_def", "transition_def",
  "range_trans_def", "te_avtab_def", "allow_def", "auditallow_def",
  "auditdeny_def", "dontaudit_def", "neverallow_def", "role_type_def",
  "role_dominance", "role_trans_def", "role_allow_def", "roles",
  "role_def", "opt_constraints", "constraints", "constraint_decl",
  "constraint_def", "validatetrans_def", "cexpr", "cexpr_prim", "$@4",
  "$@5", "$@6", "$@7", "$@8", "$@9", "$@10", "$@11", "$@12", "$@13",
  "$@14", "$@15", "$@16", "op", "role_mls_op", "users", "user_def",
  "opt_mls_user", "initial_sid_contexts", "initial_sid_context_def",
  "opt_dev_contexts", "dev_contexts", "dev_context_def",
  "pirq_context_def", "iomem_context_def", "ioport_context_def",
  "pci_context_def", "opt_fs_contexts", "fs_contexts", "fs_context_def",
  "net_contexts", "opt_port_contexts", "port_contexts", "port_context_def",
  "opt_netif_contexts", "netif_contexts", "netif_context_def",
  "opt_node_contexts", "node_contexts", "node_context_def", "opt_fs_uses",
  "fs_uses", "fs_use_def", "opt_genfs_contexts", "genfs_contexts",
  "genfs_context_def", "$@17", "ipv4_addr_def", "security_context_def",
  "opt_mls_range_def", "mls_range_def", "mls_level_def", "id_comma_list",
  "tilde", "asterisk", "names", "$@18", "tilde_push", "asterisk_push",
  "names_push", "identifier_list_push", "identifier_push",
  "identifier_list", "nested_id_set", "nested_id_list",
  "nested_id_element", "$@19", "identifier", "path", "filename", "number",
  "ipv6_addr", "policycap_def", "permissive_def", "module_policy",
  "module_def", "version_identifier", "avrules_block", "avrule_decls",
  "avrule_decl", "require_block", "require_list", "require_decl",
  "require_class", "require_decl_def", "require_id_list", "optional_block",
  "$@20", "optional_else", "optional_decl", "else_decl",
  "avrule_user_defs", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   123,   125,    59,    58,    44,    40,    41,    45,
     126,    42
};
# endif

#define YYPACT_NINF -498

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-498)))

#define YYTABLE_NINF -191

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -44,    -3,    67,  -498,    85,  -498,   481,  -498,   183,  -498,
      -3,   112,  -498,    -3,    -3,    -3,    -3,    -3,    -3,    -3,
      64,    87,    87,    87,    87,    87,    30,    87,    87,    87,
      87,    87,    51,  -498,    -3,  -498,  -498,  -498,  -498,  -498,
    -498,  -498,  -498,  -498,  -498,  -498,  -498,  -498,  -498,  -498,
    -498,  -498,  -498,  -498,  -498,  -498,  -498,  -498,  -498,   339,
    -498,  -498,  -498,   115,  -498,  -498,  -498,    55,  -498,    -3,
    -498,   225,  -498,     7,   158,    -3,    -3,     3,   116,    91,
      64,    64,   157,  -498,  -498,   -21,  -498,  -498,   174,  -498,
      87,  -498,    86,    87,    87,    87,    87,   216,    87,    87,
      87,    87,    87,   262,   167,    -3,   213,  -498,  -498,   481,
    -498,  -498,    -3,  -498,  -498,   273,   279,  -498,    87,  -498,
      87,   198,   220,  -498,   256,    -3,   203,   234,  -498,  -498,
    -498,   237,   275,   -13,    64,    64,    64,    64,    64,  -498,
    -498,  -498,   103,  -498,  -498,  -498,  -498,   247,  -498,   254,
     267,    -3,   -29,   324,    17,  -498,   317,   364,   322,   325,
     330,    -3,  -498,  -498,  -498,  -498,  -498,  -498,  -498,    19,
    -498,   341,    -3,  -498,   434,  -498,   360,   370,   448,    -3,
     273,  -498,  -498,   396,  -498,  -498,  -498,    -3,  -498,   395,
     398,  -498,  -498,  -498,   275,   252,   261,  -498,  -498,   132,
      -3,  -498,  -498,    87,    -3,    87,    87,   400,    87,   405,
     401,   409,  -498,   278,  -498,  -498,    87,  -498,    87,    87,
      87,    87,    87,  -498,  -498,  -498,  -498,   280,    87,  -498,
      -3,    -3,   694,  -498,   431,  -498,    10,  -498,  -498,  -498,
    -498,    87,    87,    87,    87,    87,    87,    87,   487,  -498,
    -498,  -498,  -498,  -498,  -498,  -498,  -498,  -498,    -3,  -498,
      -3,    -3,  -498,    -3,  -498,    -3,    -3,   216,  -498,    87,
      87,    87,    87,    87,  -498,  -498,    -3,   477,   493,   -36,
    -498,     2,    -3,  -498,   424,  -498,  -498,  -498,  -498,  -498,
    -498,   192,  -498,   483,    -3,    -3,    87,    87,    87,    87,
      87,    87,    87,   427,  -498,     4,   426,   432,   433,  -498,
     395,    20,   435,   441,   442,   443,   444,  -498,    -3,   445,
    -498,  -498,   438,  -498,  -498,  -498,   446,   447,  -498,   123,
    -498,    -3,  -498,    -3,   507,   483,  -498,   451,   -28,   449,
     450,   452,   454,   455,   460,   466,  -498,  -498,  -498,   473,
    -498,  -498,  -498,  -498,  -498,  -498,  -498,  -498,  -498,   492,
    -498,   481,  -498,  -498,    87,    87,  -498,   461,  -498,  -498,
    -498,  -498,    79,     9,    -3,   260,  -498,  -498,    -3,  -498,
      87,    87,    87,    87,    87,    87,    87,   180,  -498,    -3,
     453,    87,   241,   560,  -498,  -498,  -498,   479,   377,    87,
      87,  -498,   436,  -498,  -498,  -498,   101,    -3,    -3,    -3,
      87,    87,    87,    87,  -498,  -498,  -498,   241,    57,   239,
     259,  -498,   393,   393,   393,    57,   393,   393,   393,   393,
     393,    57,    57,    57,   241,   241,   -38,  -498,    -3,    38,
    -498,  -498,  -498,    -3,    87,   241,  -498,  -498,     6,   480,
     486,   488,   489,   490,   494,    52,  -498,  -498,  -498,  -498,
    -498,  -498,  -498,  -498,  -498,  -498,  -498,   515,  -498,  -498,
     516,   521,  -498,  -498,   519,  -498,  -498,   384,   517,   358,
    -498,    50,   241,   241,  -498,    -3,   508,  -498,   289,   543,
    -498,   338,   241,    90,  -498,   498,  -498,  -498,  -498,  -498,
    -498,  -498,  -498,   100,   100,   100,   100,  -498,   100,   100,
     100,   100,  -498,   100,   100,  -498,   100,   100,   100,  -498,
    -498,  -498,  -498,  -498,  -498,  -498,  -498,   518,  -498,   500,
    -498,   508,    -3,    -3,    -3,    -3,   545,   289,  -498,  -498,
    -498,   122,  -498,  -498,   324,  -498,  -498,   197,  -498,  -498,
    -498,  -498,  -498,  -498,  -498,  -498,  -498,  -498,  -498,  -498,
    -498,  -498,  -498,    -3,    -3,    -3,    -3,    -3,   514,    -3,
     563,   545,  -498,  -498,  -498,   162,  -498,   324,  -498,   520,
      -3,   522,   523,   524,  -498,   611,    -3,   348,   569,   563,
    -498,  -498,  -498,  -498,   170,    -3,  -498,  -498,  -498,  -498,
    -498,   -45,   508,   508,   508,   508,   508,  -498,   348,  -498,
    -498,  -498,  -498,  -498,    -3,   570,   569,  -498,  -498,  -498,
     532,    83,  -498,   110,    -3,   138,   139,    -3,  -498,    -3,
     394,  -498,   570,  -498,  -498,    -3,  -498,  -498,    -3,   508,
    -498,  -498,   508,  -498,   508,  -498,  -498,    -3,  -498,   546,
     544,  -498,  -498,    -3,  -498,    -3,    -3,    -3,  -498,    -3,
      -3,  -498,  -498,  -498,  -498,  -498,  -498
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       4,     0,     0,     2,     0,     3,     0,   293,     0,     1,
       0,     0,     8,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   332,     0,   312,   307,   308,    63,    64,
      65,    66,    67,    68,   309,    69,    70,    71,   119,   120,
     121,   122,   123,    59,    60,    61,    62,    72,   300,   335,
     306,   310,   311,     0,   255,   302,   303,     0,    10,     0,
       9,    16,    11,     0,     0,     0,     0,    80,     0,     0,
       0,     0,     0,    94,    95,     0,   265,   266,     0,   269,
       0,   268,   267,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   335,   305,   304,     0,
     301,    13,     0,    12,     5,     0,    15,    17,     0,   130,
       0,     0,     0,   263,     0,     0,    80,     0,    73,    82,
      83,     0,    88,     0,     0,     0,     0,     0,     0,    97,
     290,   292,     0,   287,   289,   271,   270,     0,   272,     0,
       0,     0,     0,     0,     0,   134,     0,     0,     0,     0,
       0,     0,   319,   320,   321,   323,   324,   325,   322,     0,
     315,     0,     0,   299,     0,   334,     0,     0,    26,     0,
      14,    20,    18,     0,    32,    76,    77,     0,    78,    79,
       0,    75,    81,    87,    89,    90,    91,    92,    93,     0,
       0,   286,   288,     0,     0,     0,     0,     0,     0,     0,
     260,   262,   283,     0,   131,   135,     0,   133,     0,     0,
       0,     0,     0,   313,   314,   316,   326,     0,     0,   328,
       0,     0,     0,    25,     0,    28,     0,    21,   129,   264,
      74,     0,     0,     0,     0,     0,     0,     0,    86,    96,
      98,    99,   105,   106,   107,   108,   100,   291,     0,   273,
       0,     0,   132,     0,   117,     0,     0,     0,   136,     0,
       0,     0,     0,     0,   318,   317,     0,   198,   331,     0,
     284,     0,     0,    58,     0,    51,    54,    53,    55,    57,
      56,     0,    29,    36,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    84,     0,     0,     0,     0,   259,
     261,     0,     0,     0,     0,     0,     0,   327,     0,     0,
     333,   329,     0,    19,   285,    31,     0,     0,    52,   139,
     194,     0,    33,     0,     0,    35,    37,    23,     0,     0,
       0,     0,     0,     0,     0,     0,    97,   295,   114,     0,
     115,   116,   118,   137,   128,   124,   125,   126,   127,     0,
     196,     0,    30,   298,     0,     0,     6,   138,   140,   142,
     143,   195,     0,     0,     0,     0,    41,    38,     0,    22,
       0,     0,     0,     0,     0,     0,     0,     0,   113,     0,
       0,     0,     0,     0,   141,    34,    40,     0,     0,     0,
       0,    42,    27,    45,    47,    48,     0,     0,     0,     0,
       0,     0,     0,     0,    85,   197,   330,     0,     0,     0,
       0,   172,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   150,     0,   217,
     199,    39,    44,     0,     0,     0,    46,    24,     0,     0,
       0,     0,     0,     0,     0,     0,   191,   192,   193,   188,
     189,   190,   177,   173,   178,   175,   180,   154,   156,   158,
     160,     0,   162,   164,   166,   168,   170,     0,     0,     0,
     147,     0,     0,     0,   145,     0,     0,   200,   240,   216,
     218,     0,     0,     0,   102,     0,   103,   104,   109,   110,
     111,   112,   144,     0,     0,     0,     0,   151,     0,     0,
       0,     0,   152,     0,     0,   153,     0,     0,     0,   182,
     186,   183,   187,   184,   185,   146,   148,   149,   201,     0,
     296,     0,     0,     0,     0,     0,   248,   239,   241,   219,
      43,     0,    50,   101,     0,   274,   275,     0,   278,   174,
     276,   179,   176,   181,   155,   157,   159,   161,   163,   165,
     167,   169,   171,     0,     0,     0,     0,     0,     0,     0,
     223,   247,   249,   242,    49,     0,   281,     0,   279,     0,
       0,     0,     0,     0,   246,     0,     0,   203,   229,   222,
     224,   250,   277,   282,     0,     0,   220,   243,   244,   245,
     294,     0,     0,     0,     0,     0,     0,     7,   202,   204,
     206,   207,   208,   209,     0,   234,   228,   230,   225,   280,
     258,     0,   254,     0,     0,     0,     0,     0,   205,     0,
       0,   221,   233,   235,   231,     0,   256,   252,     0,     0,
     226,   210,     0,   211,     0,   213,   215,     0,   297,     0,
       0,   236,   257,     0,   251,     0,     0,     0,   232,     0,
       0,   253,   227,   212,   214,   237,   238
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -498,  -498,  -498,  -498,  -498,  -498,  -498,   620,  -498,   561,
    -498,  -498,  -498,   525,  -498,   456,  -498,  -498,  -498,   399,
     -73,  -498,  -498,  -498,   300,  -498,   263,  -498,   238,  -498,
    -498,  -498,   355,  -202,  -191,  -498,  -498,  -498,  -498,  -498,
     527,  -498,  -498,  -189,  -498,   -20,  -498,   296,  -498,  -498,
    -498,  -498,  -498,  -498,  -498,  -498,  -498,  -498,  -498,  -498,
    -498,  -498,  -498,  -498,  -498,  -498,  -498,   376,  -147,  -498,
    -498,   277,  -498,  -498,  -362,  -498,  -498,  -498,  -498,  -498,
    -498,  -498,  -498,  -498,  -498,  -498,  -498,  -498,  -498,  -104,
     -92,  -498,  -238,  -498,  -498,   206,  -498,  -498,    39,  -498,
    -498,  -498,  -498,  -498,  -498,   159,  -498,  -498,  -498,    60,
    -498,  -498,    34,  -498,  -498,    25,  -498,  -498,   114,  -498,
    -498,    89,  -498,  -403,  -412,  -498,  -260,  -242,   -75,  -119,
      93,   458,  -498,  -498,  -498,   -95,    88,  -151,  -284,   -61,
    -498,   526,  -498,    -1,  -498,   214,  -497,    21,  -498,  -498,
    -498,  -498,  -498,  -103,  -498,   608,  -194,  -498,   501,  -498,
    -498,  -498,  -187,  -498,  -498,  -498,  -498,   566
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     4,   178,   393,    11,    12,    71,    72,
     114,   115,   116,   117,   180,   181,   232,   233,   234,   235,
     121,   293,   334,   335,   336,   375,   376,   402,   403,   404,
     405,   284,   285,    36,    37,    38,    39,    40,    41,    42,
     127,    43,   131,    44,   304,    82,    83,   199,   249,   250,
     251,   252,   253,   254,   255,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    54,    55,    56,   154,   155,   366,
     367,   368,   369,   370,   436,   437,   508,   509,   510,   511,
     513,   514,   516,   517,   518,   503,   505,   504,   506,   461,
     462,   329,   106,   319,   439,   440,   607,   608,   609,   610,
     611,   612,   613,   488,   489,   490,   587,   588,   589,   590,
     615,   616,   617,   631,   632,   633,   536,   537,   538,   570,
     571,   572,   653,    66,   528,   636,   209,   210,   122,    88,
      89,    90,   204,   547,   548,   549,   575,   550,   279,    91,
     142,   143,   200,    92,   601,   349,   531,   650,   289,    57,
       5,     6,    67,    58,    59,    60,    61,   169,   170,   171,
     172,   227,    62,   278,   321,    63,   322,   108
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       8,   124,   213,   308,   126,   256,   176,   215,   347,    68,
     347,   338,    73,    74,    75,    76,    77,    78,    79,    84,
     294,   120,   120,   309,   141,   118,   161,   145,   120,   153,
     286,   162,   153,   104,   564,     7,   163,   482,   483,   164,
     165,   287,     1,   288,     7,   290,   330,   372,   166,   438,
     189,     7,     7,   167,   621,   455,   484,   323,   168,     7,
     132,   133,   134,   135,   136,   379,   208,     9,   111,   137,
     138,    85,   480,   481,   123,   123,   359,     7,   140,    84,
      84,   141,   286,   493,   144,   193,   486,   146,   456,   457,
     458,   371,    10,   287,   406,   288,   325,   290,   348,   125,
     494,   119,   295,   396,   174,   623,   624,   625,   626,   627,
     214,   177,   223,   353,   194,   195,   196,   197,   198,    10,
     526,   527,    97,    69,   123,   482,   483,   482,   483,   415,
     541,   364,   365,    84,    84,    84,    84,    84,    80,   459,
     460,   144,   655,   103,     7,   656,   502,   657,   525,   110,
     207,   211,   580,   581,   582,   583,   241,   242,   243,     7,
     222,    81,   105,     7,   215,   482,   483,     7,   596,   129,
     130,   226,   395,   244,   245,   246,   247,   120,   236,    85,
     212,     7,   637,     7,   542,   148,   239,    86,    87,   622,
       7,   310,   544,   256,   447,    85,   201,   482,   483,   257,
      86,    87,   140,   259,   241,   242,   243,   109,   326,   639,
     128,   640,   641,   643,   645,   646,   574,   647,     7,     7,
      32,   244,   245,   246,   247,   248,   654,   649,   153,   280,
     281,   112,   134,   135,   136,   658,    69,   642,   644,   137,
     138,   661,   212,   662,   663,   664,   659,   665,   666,   139,
     212,   463,   105,   418,     7,   592,   464,   305,   390,   306,
     307,   173,   211,   619,   211,   123,    85,    64,    32,   161,
      65,   465,     7,   414,   162,   317,   466,   212,   324,   163,
     179,   327,   164,   165,   331,   112,   419,   420,   421,   577,
     332,   166,   185,   337,   280,   374,   167,   399,   400,   125,
     397,   168,   422,   423,   424,   425,   426,   427,   428,   429,
     430,   431,   432,   433,   186,   434,   187,   211,   467,   468,
     469,   470,   472,   473,   474,   475,   476,   134,   191,   136,
     280,   192,   373,   471,   137,   138,   134,   324,   435,   477,
     478,   479,   203,   137,   138,   532,   533,   534,   535,   205,
     188,    13,   187,    14,    15,    16,    17,   137,   138,    18,
      19,    20,   206,    21,    22,    23,    24,    25,   491,    26,
     267,   324,   268,   398,   275,   652,   276,   280,   105,    27,
      28,    29,    30,    31,   545,   545,   545,   545,   211,   545,
     545,   545,   545,   576,   545,   545,   578,   545,   545,   545,
     603,   604,   605,   606,   212,   324,   448,   449,   450,   551,
     552,   553,   216,   554,   555,   556,   557,   219,   558,   559,
     220,   560,   561,   562,   593,   221,   576,    32,    33,   523,
      34,   524,   540,    35,   187,   225,    13,   485,    14,    15,
      16,    17,   123,   593,    18,    19,    20,   228,    21,    22,
      23,    24,    25,   229,    26,   519,   520,   521,   217,   218,
     231,   291,   230,   105,    27,    28,    29,    30,    31,   364,
     365,   442,   443,   399,   400,   459,   460,   231,    64,   648,
      93,    94,    95,    96,   529,    98,    99,   100,   101,   102,
     238,   187,   240,    13,   262,    14,    15,    16,    17,   264,
     265,    18,    19,    20,   266,    21,    22,    23,    24,    25,
     303,    26,   318,    33,   282,    34,   320,   333,   283,   346,
     350,    27,    28,    29,    30,    31,   351,   352,   389,   354,
     361,   565,   566,   567,   568,   355,   356,   357,   358,   360,
     362,   363,   374,   378,   380,   381,   416,   382,   147,   383,
     384,   149,   150,   151,   152,   385,   156,   157,   158,   159,
     160,   386,   579,   529,   529,   529,   529,   388,   585,    32,
      33,   438,    34,   441,   496,    35,   183,   507,   184,   529,
     497,  -190,   498,   499,   500,   602,   512,   515,   501,   530,
     522,   486,   543,   482,   620,   563,   546,   546,   546,   546,
     529,   546,   546,   546,   546,   569,   546,   546,   584,   546,
     546,   546,   586,   629,   600,   595,   597,   598,   599,   614,
     638,   630,   529,   529,   529,   529,   529,   635,   529,   648,
      64,    70,   113,   292,   211,   377,   237,   529,   401,   328,
     446,   182,   387,   311,   394,   487,   529,   628,   539,   618,
     634,   573,   529,   190,   529,   529,   529,   651,   529,   529,
     591,   258,   495,   260,   261,   594,   263,   107,   202,     0,
     224,   660,   175,     0,   269,     0,   270,   271,   272,   273,
     274,     0,     0,     0,     0,     0,   277,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   296,
     297,   298,   299,   300,   301,   302,    13,     0,    14,    15,
      16,    17,     0,     0,    18,    19,    20,     0,    21,    22,
      23,    24,    25,     0,    26,     0,     0,   312,   313,   314,
     315,   316,     0,     0,    27,    28,    29,    30,    31,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   339,   340,   341,   342,   343,   344,
     345,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    33,   282,    34,     0,     0,   283,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   391,   392,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   407,   408,
     409,   410,   411,   412,   413,     0,     0,     0,     0,   417,
       0,     0,     0,     0,     0,     0,     0,   444,   445,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   451,   452,
     453,   454,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   492
};

static const yytype_int16 yycheck[] =
{
       1,    76,   153,   263,    77,   199,   109,   154,     4,    10,
       4,   295,    13,    14,    15,    16,    17,    18,    19,    20,
      10,    19,    19,   265,    85,    18,     7,    88,    19,    12,
     232,    12,    12,    34,   531,    80,    17,    75,    76,    20,
      21,   232,    86,   232,    80,   232,   284,   331,    29,    11,
     125,    80,    80,    34,    99,   417,    94,    93,    39,    80,
      80,    81,    75,    76,    77,    93,    95,     0,    69,    82,
      83,    92,   434,   435,    75,    76,   318,    80,    99,    80,
      81,   142,   284,   445,    85,    98,    48,    88,    31,    32,
      33,   329,     7,   284,   378,   284,    94,   284,    94,    96,
      94,    94,    92,    94,   105,   602,   603,   604,   605,   606,
      93,   112,    93,    93,   134,   135,   136,   137,   138,     7,
     482,   483,    92,    11,   125,    75,    76,    75,    76,   389,
     492,     8,     9,   134,   135,   136,   137,   138,    74,    82,
      83,   142,   639,    92,    80,   642,    94,   644,    98,    94,
     151,   152,   564,   565,   566,   567,    24,    25,    26,    80,
     161,    97,    39,    80,   311,    75,    76,    80,   580,    78,
      79,   172,    93,    41,    42,    43,    44,    19,   179,    92,
      80,    80,    99,    80,    94,    99,   187,   100,   101,   601,
      80,   266,    92,   387,    93,    92,    93,    75,    76,   200,
     100,   101,    99,   204,    24,    25,    26,    92,   281,    99,
      94,   623,   624,   625,   626,   627,    94,   629,    80,    80,
      88,    41,    42,    43,    44,    93,   638,   630,    12,   230,
     231,     6,    75,    76,    77,   647,    11,    99,    99,    82,
      83,   653,    80,   655,   656,   657,   649,   659,   660,    92,
      80,    12,    39,    12,    80,    93,    17,   258,   361,   260,
     261,    94,   263,    93,   265,   266,    92,    84,    88,     7,
      87,    12,    80,    93,    12,   276,    17,    80,   279,    17,
       7,   282,    20,    21,    92,     6,    45,    46,    47,    92,
     291,    29,    94,   294,   295,    35,    34,    37,    38,    96,
     373,    39,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    94,    74,    96,   318,   422,   423,
     424,   425,   426,   427,   428,   429,   430,    75,    94,    77,
     331,    94,   333,   425,    82,    83,    75,   338,    97,   431,
     432,   433,    95,    82,    83,    56,    57,    58,    59,    95,
      94,    12,    96,    14,    15,    16,    17,    82,    83,    20,
      21,    22,    95,    24,    25,    26,    27,    28,   443,    30,
      92,   372,    94,   374,    94,   635,    96,   378,    39,    40,
      41,    42,    43,    44,   503,   504,   505,   506,   389,   508,
     509,   510,   511,   544,   513,   514,   547,   516,   517,   518,
      52,    53,    54,    55,    80,   406,   407,   408,   409,   504,
     505,   506,    95,   508,   509,   510,   511,    95,   513,   514,
      95,   516,   517,   518,   575,    95,   577,    88,    89,    71,
      91,    73,    94,    94,    96,    94,    12,   438,    14,    15,
      16,    17,   443,   594,    20,    21,    22,    13,    24,    25,
      26,    27,    28,    93,    30,    71,    72,    73,    94,    95,
      29,    30,    92,    39,    40,    41,    42,    43,    44,     8,
       9,    94,    95,    37,    38,    82,    83,    29,    84,    85,
      22,    23,    24,    25,   485,    27,    28,    29,    30,    31,
      94,    96,    94,    12,    94,    14,    15,    16,    17,    94,
      99,    20,    21,    22,    95,    24,    25,    26,    27,    28,
      23,    30,    35,    89,    90,    91,    23,    34,    94,    92,
      94,    40,    41,    42,    43,    44,    94,    94,    36,    94,
      92,   532,   533,   534,   535,    94,    94,    94,    94,    94,
      94,    94,    35,    92,    95,    95,    93,    95,    90,    95,
      95,    93,    94,    95,    96,    95,    98,    99,   100,   101,
     102,    95,   563,   564,   565,   566,   567,    94,   569,    88,
      89,    11,    91,    94,    94,    94,   118,    62,   120,   580,
      94,    65,    94,    94,    94,   586,    65,    68,    94,    81,
      73,    48,    94,    75,   595,    95,   503,   504,   505,   506,
     601,   508,   509,   510,   511,    60,   513,   514,    94,   516,
     517,   518,    49,   614,     3,    95,    94,    94,    94,    50,
     621,    51,   623,   624,   625,   626,   627,    95,   629,    85,
      84,    11,    71,   234,   635,   335,   180,   638,   375,   284,
     402,   116,   346,   267,   367,   439,   647,   608,   489,   589,
     616,   537,   653,   126,   655,   656,   657,   632,   659,   660,
     571,   203,   448,   205,   206,   577,   208,    59,   142,    -1,
     169,   650,   106,    -1,   216,    -1,   218,   219,   220,   221,
     222,    -1,    -1,    -1,    -1,    -1,   228,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   241,
     242,   243,   244,   245,   246,   247,    12,    -1,    14,    15,
      16,    17,    -1,    -1,    20,    21,    22,    -1,    24,    25,
      26,    27,    28,    -1,    30,    -1,    -1,   269,   270,   271,
     272,   273,    -1,    -1,    40,    41,    42,    43,    44,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   296,   297,   298,   299,   300,   301,
     302,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    89,    90,    91,    -1,    -1,    94,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   364,   365,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   380,   381,
     382,   383,   384,   385,   386,    -1,    -1,    -1,    -1,   391,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   399,   400,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   410,   411,
     412,   413,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   444
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,    86,   103,   104,   105,   252,   253,    80,   245,     0,
       7,   108,   109,    12,    14,    15,    16,    17,    20,    21,
      22,    24,    25,    26,    27,    28,    30,    40,    41,    42,
      43,    44,    88,    89,    91,    94,   135,   136,   137,   138,
     139,   140,   141,   143,   145,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   251,   255,   256,
     257,   258,   264,   267,    84,    87,   225,   254,   245,    11,
     109,   110,   111,   245,   245,   245,   245,   245,   245,   245,
      74,    97,   147,   148,   245,    92,   100,   101,   231,   232,
     233,   241,   245,   233,   233,   233,   233,    92,   233,   233,
     233,   233,   233,    92,   245,    39,   194,   257,   269,    92,
      94,   245,     6,   111,   112,   113,   114,   115,    18,    94,
      19,   122,   230,   245,   230,    96,   122,   142,    94,    78,
      79,   144,   147,   147,    75,    76,    77,    82,    83,    92,
      99,   241,   242,   243,   245,   241,   245,   233,    99,   233,
     233,   233,   233,    12,   169,   170,   233,   233,   233,   233,
     233,     7,    12,    17,    20,    21,    29,    34,    39,   259,
     260,   261,   262,    94,   245,   269,   255,   245,   106,     7,
     116,   117,   115,   233,   233,    94,    94,    96,    94,   230,
     142,    94,    94,    98,   147,   147,   147,   147,   147,   149,
     244,    93,   243,    95,   234,    95,    95,   245,    95,   228,
     229,   245,    80,   239,    93,   170,    95,    94,    95,    95,
      95,    95,   245,    93,   260,    94,   245,   263,    13,    93,
      92,    29,   118,   119,   120,   121,   245,   117,    94,   245,
      94,    24,    25,    26,    41,    42,    43,    44,    93,   150,
     151,   152,   153,   154,   155,   156,   258,   245,   233,   245,
     233,   233,    94,   233,    94,    99,    95,    92,    94,   233,
     233,   233,   233,   233,   233,    94,    96,   233,   265,   240,
     245,   245,    90,    94,   133,   134,   135,   136,   145,   250,
     264,    30,   121,   123,    10,    92,   233,   233,   233,   233,
     233,   233,   233,    23,   146,   245,   245,   245,   228,   229,
     230,   169,   233,   233,   233,   233,   233,   245,    35,   195,
      23,   266,   268,    93,   245,    94,   122,   245,   134,   193,
     194,    92,   245,    34,   124,   125,   126,   245,   240,   233,
     233,   233,   233,   233,   233,   233,    92,     4,    94,   247,
      94,    94,    94,    93,    94,    94,    94,    94,    94,   229,
      94,    92,    94,    94,     8,     9,   171,   172,   173,   174,
     175,   194,   240,   245,    35,   127,   128,   126,    92,    93,
      95,    95,    95,    95,    95,    95,    95,   149,    94,    36,
     255,   233,   233,   107,   173,    93,    94,   122,   245,    37,
      38,   128,   129,   130,   131,   132,   240,   233,   233,   233,
     233,   233,   233,   233,    93,   228,    93,   233,    12,    45,
      46,    47,    61,    62,    63,    64,    65,    66,    67,    68,
      69,    70,    71,    72,    74,    97,   176,   177,    11,   196,
     197,    94,    94,    95,   233,   233,   130,    93,   245,   245,
     245,   233,   233,   233,   233,   176,    31,    32,    33,    82,
      83,   191,   192,    12,    17,    12,    17,   191,   191,   191,
     191,   192,   191,   191,   191,   191,   191,   192,   192,   192,
     176,   176,    75,    76,    94,   245,    48,   197,   205,   206,
     207,   230,   233,   176,    94,   247,    94,    94,    94,    94,
      94,    94,    94,   187,   189,   188,   190,    62,   178,   179,
     180,   181,    65,   182,   183,    68,   184,   185,   186,    71,
      72,    73,    73,    71,    73,    98,   176,   176,   226,   245,
      81,   248,    56,    57,    58,    59,   218,   219,   220,   207,
      94,   176,    94,    94,    92,   231,   232,   235,   236,   237,
     239,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237,   237,   237,    95,   248,   245,   245,   245,   245,    60,
     221,   222,   223,   220,    94,   238,   239,    92,   239,   245,
     226,   226,   226,   226,    94,   245,    49,   208,   209,   210,
     211,   223,    93,   239,   238,    95,   226,    94,    94,    94,
       3,   246,   245,    52,    53,    54,    55,   198,   199,   200,
     201,   202,   203,   204,    50,   212,   213,   214,   211,    93,
     245,    99,   226,   248,   248,   248,   248,   248,   200,   245,
      51,   215,   216,   217,   214,    95,   227,    99,   245,    99,
     226,   226,    99,   226,    99,   226,   226,   226,    85,   225,
     249,   217,   228,   224,   226,   248,   248,   248,   226,   225,
     249,   226,   226,   226,   226,   226,   226
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   102,   103,   103,   105,   106,   107,   104,   108,   108,
     109,   110,   110,   111,   112,   113,   113,   114,   114,   115,
     116,   116,   117,   117,   117,   118,   118,   119,   120,   120,
     121,   121,   122,   123,   123,   124,   124,   125,   125,   126,
     126,   127,   127,   128,   128,   129,   129,   130,   130,   131,
     132,   133,   133,   134,   134,   134,   134,   134,   134,   135,
     135,   135,   135,   136,   136,   136,   136,   136,   136,   136,
     136,   136,   136,   137,   138,   138,   139,   140,   141,   142,
     142,   143,   144,   144,   145,   146,   146,   147,   147,   147,
     147,   147,   147,   147,   147,   148,   149,   149,   150,   150,
     150,   151,   151,   151,   151,   152,   152,   152,   152,   153,
     154,   155,   156,   157,   157,   157,   157,   158,   158,   159,
     159,   159,   159,   159,   160,   161,   162,   163,   164,   165,
     165,   166,   167,   168,   169,   169,   170,   170,   171,   171,
     172,   172,   173,   173,   174,   175,   176,   176,   176,   176,
     176,   177,   177,   177,   178,   177,   179,   177,   180,   177,
     181,   177,   182,   177,   183,   177,   184,   177,   185,   177,
     186,   177,   177,   187,   177,   188,   177,   177,   189,   177,
     190,   177,   177,   177,   177,   177,   177,   177,   191,   191,
     192,   192,   192,   192,   193,   193,   194,   195,   195,   196,
     196,   197,   198,   198,   199,   199,   200,   200,   200,   200,
     201,   202,   202,   203,   203,   204,   205,   205,   206,   206,
     207,   208,   209,   209,   210,   210,   211,   211,   212,   212,
     213,   213,   214,   215,   215,   216,   216,   217,   217,   218,
     218,   219,   219,   220,   220,   220,   220,   221,   221,   222,
     222,   223,   224,   223,   223,   225,   226,   227,   227,   228,
     228,   229,   229,   230,   230,   231,   232,   233,   233,   233,
     233,   233,   234,   233,   235,   236,   237,   237,   237,   237,
     237,   238,   238,   239,   240,   240,   241,   242,   242,   243,
     244,   243,   243,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   254,   255,   256,   256,   257,   257,   257,
     257,   257,   257,   258,   259,   259,   260,   260,   261,   262,
     262,   262,   262,   262,   262,   262,   263,   263,   265,   264,
     266,   266,   267,   268,   269,   269
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     0,     0,     0,    16,     1,     2,
       2,     1,     2,     2,     2,     1,     0,     1,     2,     5,
       1,     2,     5,     4,     7,     1,     0,     5,     1,     2,
       4,     3,     2,     2,     4,     1,     0,     1,     2,     4,
       3,     1,     2,     5,     3,     1,     2,     1,     1,     5,
       4,     1,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     5,     4,     4,     4,     4,     2,
       0,     4,     1,     1,     6,     4,     0,     3,     2,     3,
       3,     3,     3,     3,     1,     1,     2,     0,     1,     1,
       1,     8,     7,     7,     7,     1,     1,     1,     1,     7,
       7,     7,     7,     8,     7,     7,     7,     5,     7,     1,
       1,     1,     1,     1,     7,     7,     7,     7,     7,     5,
       3,     4,     5,     4,     1,     2,     3,     5,     1,     0,
       1,     2,     1,     1,     5,     4,     3,     2,     3,     3,
       1,     3,     3,     3,     0,     4,     0,     4,     0,     4,
       0,     4,     0,     4,     0,     4,     0,     4,     0,     4,
       0,     4,     1,     0,     4,     0,     4,     2,     0,     4,
       0,     4,     3,     3,     3,     3,     3,     3,     1,     1,
       1,     1,     1,     1,     1,     2,     6,     4,     0,     1,
       2,     3,     1,     0,     1,     2,     1,     1,     1,     1,
       3,     3,     5,     3,     5,     3,     1,     0,     1,     2,
       5,     3,     1,     0,     1,     2,     4,     6,     1,     0,
       1,     2,     4,     1,     0,     1,     2,     4,     4,     1,
       0,     1,     2,     4,     4,     4,     3,     1,     0,     1,
       2,     6,     0,     7,     4,     1,     6,     2,     0,     3,
       1,     3,     1,     1,     3,     1,     1,     1,     1,     1,
       2,     2,     0,     4,     1,     1,     1,     3,     1,     2,
       4,     1,     2,     1,     1,     2,     3,     1,     2,     1,
       0,     3,     1,     1,     1,     1,     1,     1,     3,     3,
       2,     4,     1,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     4,     2,     1,     2,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     0,     6,
       4,     0,     1,     1,     2,     0
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 4:
#line 169 "policy_parse.y" /* yacc.c:1646  */
    { if (define_policy(pass, 0) == -1) return -1; }
#line 2007 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 5:
#line 171 "policy_parse.y" /* yacc.c:1646  */
    { if (pass == 1) { if (policydb_index_classes(policydbp)) return -1; }
                            else if (pass == 2) { if (policydb_index_others(NULL, policydbp, 0)) return -1; }}
#line 2014 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 6:
#line 174 "policy_parse.y" /* yacc.c:1646  */
    { if (pass == 1) { if (policydb_index_bools(policydbp)) return -1;}
			   else if (pass == 2) { if (policydb_index_others(NULL, policydbp, 0)) return -1;}}
#line 2021 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 10:
#line 182 "policy_parse.y" /* yacc.c:1646  */
    {if (define_class()) return -1;}
#line 2027 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 13:
#line 188 "policy_parse.y" /* yacc.c:1646  */
    {if (define_initial_sid()) return -1;}
#line 2033 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 19:
#line 199 "policy_parse.y" /* yacc.c:1646  */
    {if (define_common_perms()) return -1;}
#line 2039 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 22:
#line 205 "policy_parse.y" /* yacc.c:1646  */
    {if (define_av_perms(FALSE)) return -1;}
#line 2045 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 23:
#line 207 "policy_parse.y" /* yacc.c:1646  */
    {if (define_av_perms(TRUE)) return -1;}
#line 2051 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 24:
#line 209 "policy_parse.y" /* yacc.c:1646  */
    {if (define_av_perms(TRUE)) return -1;}
#line 2057 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 30:
#line 222 "policy_parse.y" /* yacc.c:1646  */
    {if (define_mls() | define_sens()) return -1;}
#line 2063 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 31:
#line 224 "policy_parse.y" /* yacc.c:1646  */
    {if (define_mls() | define_sens()) return -1;}
#line 2069 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 33:
#line 229 "policy_parse.y" /* yacc.c:1646  */
    {if (define_dominance()) return -1;}
#line 2075 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 34:
#line 231 "policy_parse.y" /* yacc.c:1646  */
    {if (define_dominance()) return -1;}
#line 2081 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 39:
#line 240 "policy_parse.y" /* yacc.c:1646  */
    {if (define_category()) return -1;}
#line 2087 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 40:
#line 242 "policy_parse.y" /* yacc.c:1646  */
    {if (define_category()) return -1;}
#line 2093 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 43:
#line 248 "policy_parse.y" /* yacc.c:1646  */
    {if (define_level()) return -1;}
#line 2099 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 44:
#line 250 "policy_parse.y" /* yacc.c:1646  */
    {if (define_level()) return -1;}
#line 2105 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 49:
#line 259 "policy_parse.y" /* yacc.c:1646  */
    { if (define_constraint((constraint_expr_t*)(yyvsp[-1].valptr))) return -1; }
#line 2111 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 50:
#line 262 "policy_parse.y" /* yacc.c:1646  */
    { if (define_validatetrans((constraint_expr_t*)(yyvsp[-1].valptr))) return -1; }
#line 2117 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 73:
#line 291 "policy_parse.y" /* yacc.c:1646  */
    { if (define_attrib()) return -1;}
#line 2123 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 74:
#line 294 "policy_parse.y" /* yacc.c:1646  */
    {if (define_type(1)) return -1;}
#line 2129 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 75:
#line 296 "policy_parse.y" /* yacc.c:1646  */
    {if (define_type(0)) return -1;}
#line 2135 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 76:
#line 299 "policy_parse.y" /* yacc.c:1646  */
    {if (define_typealias()) return -1;}
#line 2141 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 77:
#line 302 "policy_parse.y" /* yacc.c:1646  */
    {if (define_typeattribute()) return -1;}
#line 2147 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 78:
#line 305 "policy_parse.y" /* yacc.c:1646  */
    {if (define_typebounds()) return -1;}
#line 2153 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 81:
#line 311 "policy_parse.y" /* yacc.c:1646  */
    {if (define_bool()) return -1;}
#line 2159 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 82:
#line 314 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_id("T",0)) return -1; }
#line 2165 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 83:
#line 316 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_id("F",0)) return -1; }
#line 2171 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 84:
#line 319 "policy_parse.y" /* yacc.c:1646  */
    { if (pass == 2) { if (define_conditional((cond_expr_t*)(yyvsp[-4].ptr), (avrule_t*)(yyvsp[-2].ptr), (avrule_t*)(yyvsp[0].ptr)) < 0) return -1;  }}
#line 2177 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 85:
#line 322 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = (yyvsp[-1].ptr); }
#line 2183 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 86:
#line 324 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = NULL; }
#line 2189 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 87:
#line 326 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = (yyvsp[-1].ptr);}
#line 2195 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 88:
#line 328 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = define_cond_expr(COND_NOT, (yyvsp[0].ptr), 0);
			  if ((yyval.ptr) == 0) return -1; }
#line 2202 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 89:
#line 331 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = define_cond_expr(COND_AND, (yyvsp[-2].ptr), (yyvsp[0].ptr));
			  if ((yyval.ptr) == 0) return  -1; }
#line 2209 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 90:
#line 334 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = define_cond_expr(COND_OR, (yyvsp[-2].ptr), (yyvsp[0].ptr));
			  if ((yyval.ptr) == 0) return   -1; }
#line 2216 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 91:
#line 337 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = define_cond_expr(COND_XOR, (yyvsp[-2].ptr), (yyvsp[0].ptr));
			  if ((yyval.ptr) == 0) return  -1; }
#line 2223 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 92:
#line 340 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = define_cond_expr(COND_EQ, (yyvsp[-2].ptr), (yyvsp[0].ptr));
			  if ((yyval.ptr) == 0) return  -1; }
#line 2230 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 93:
#line 343 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = define_cond_expr(COND_NEQ, (yyvsp[-2].ptr), (yyvsp[0].ptr));
			  if ((yyval.ptr) == 0) return  -1; }
#line 2237 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 94:
#line 346 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2243 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 95:
#line 349 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = define_cond_expr(COND_BOOL,0, 0);
			  if ((yyval.ptr) == COND_ERR) return   -1; }
#line 2250 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 96:
#line 353 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = define_cond_pol_list((avrule_t *)(yyvsp[-1].ptr), (avrule_t *)(yyvsp[0].ptr)); }
#line 2256 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 97:
#line 355 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = NULL; }
#line 2262 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 98:
#line 358 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2268 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 99:
#line 360 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2274 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 100:
#line 362 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = NULL; }
#line 2280 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 101:
#line 365 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = define_cond_filename_trans() ;
                          if ((yyval.ptr) == COND_ERR) return -1;}
#line 2287 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 102:
#line 368 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = define_cond_compute_type(AVRULE_TRANSITION) ;
                          if ((yyval.ptr) == COND_ERR) return -1;}
#line 2294 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 103:
#line 371 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = define_cond_compute_type(AVRULE_MEMBER) ;
                          if ((yyval.ptr) ==  COND_ERR) return -1;}
#line 2301 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 104:
#line 374 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = define_cond_compute_type(AVRULE_CHANGE) ;
                          if ((yyval.ptr) == COND_ERR) return -1;}
#line 2308 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 105:
#line 378 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2314 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 106:
#line 380 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2320 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 107:
#line 382 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2326 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 108:
#line 384 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2332 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 109:
#line 387 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = define_cond_te_avtab(AVRULE_ALLOWED) ;
                          if ((yyval.ptr) == COND_ERR) return -1; }
#line 2339 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 110:
#line 391 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = define_cond_te_avtab(AVRULE_AUDITALLOW) ;
                          if ((yyval.ptr) == COND_ERR) return -1; }
#line 2346 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 111:
#line 395 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = define_cond_te_avtab(AVRULE_AUDITDENY) ;
                          if ((yyval.ptr) == COND_ERR) return -1; }
#line 2353 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 112:
#line 399 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = define_cond_te_avtab(AVRULE_DONTAUDIT);
                          if ((yyval.ptr) == COND_ERR) return -1; }
#line 2360 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 113:
#line 403 "policy_parse.y" /* yacc.c:1646  */
    {if (define_filename_trans()) return -1; }
#line 2366 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 114:
#line 405 "policy_parse.y" /* yacc.c:1646  */
    {if (define_compute_type(AVRULE_TRANSITION)) return -1;}
#line 2372 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 115:
#line 407 "policy_parse.y" /* yacc.c:1646  */
    {if (define_compute_type(AVRULE_MEMBER)) return -1;}
#line 2378 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 116:
#line 409 "policy_parse.y" /* yacc.c:1646  */
    {if (define_compute_type(AVRULE_CHANGE)) return -1;}
#line 2384 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 117:
#line 412 "policy_parse.y" /* yacc.c:1646  */
    { if (define_range_trans(0)) return -1; }
#line 2390 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 118:
#line 414 "policy_parse.y" /* yacc.c:1646  */
    { if (define_range_trans(1)) return -1; }
#line 2396 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 124:
#line 423 "policy_parse.y" /* yacc.c:1646  */
    {if (define_te_avtab(AVRULE_ALLOWED)) return -1; }
#line 2402 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 125:
#line 426 "policy_parse.y" /* yacc.c:1646  */
    {if (define_te_avtab(AVRULE_AUDITALLOW)) return -1; }
#line 2408 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 126:
#line 429 "policy_parse.y" /* yacc.c:1646  */
    {if (define_te_avtab(AVRULE_AUDITDENY)) return -1; }
#line 2414 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 127:
#line 432 "policy_parse.y" /* yacc.c:1646  */
    {if (define_te_avtab(AVRULE_DONTAUDIT)) return -1; }
#line 2420 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 128:
#line 435 "policy_parse.y" /* yacc.c:1646  */
    {if (define_te_avtab(AVRULE_NEVERALLOW)) return -1; }
#line 2426 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 129:
#line 438 "policy_parse.y" /* yacc.c:1646  */
    {if (define_role_types()) return -1;}
#line 2432 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 130:
#line 440 "policy_parse.y" /* yacc.c:1646  */
    {if (define_role_types()) return -1;}
#line 2438 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 132:
#line 445 "policy_parse.y" /* yacc.c:1646  */
    {if (define_role_trans()) return -1; }
#line 2444 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 133:
#line 448 "policy_parse.y" /* yacc.c:1646  */
    {if (define_role_allow()) return -1; }
#line 2450 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 134:
#line 451 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = (yyvsp[0].ptr); }
#line 2456 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 135:
#line 453 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.ptr) = merge_roles_dom((role_datum_t*)(yyvsp[-1].ptr), (role_datum_t*)(yyvsp[0].ptr)); if ((yyval.ptr) == 0) return -1;}
#line 2462 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 136:
#line 456 "policy_parse.y" /* yacc.c:1646  */
    {(yyval.ptr) = define_role_dom(NULL); if ((yyval.ptr) == 0) return -1;}
#line 2468 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 137:
#line 458 "policy_parse.y" /* yacc.c:1646  */
    {(yyval.ptr) = define_role_dom((role_datum_t*)(yyvsp[-1].ptr)); if ((yyval.ptr) == 0) return -1;}
#line 2474 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 144:
#line 470 "policy_parse.y" /* yacc.c:1646  */
    { if (define_constraint((constraint_expr_t*)(yyvsp[-1].valptr))) return -1; }
#line 2480 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 145:
#line 473 "policy_parse.y" /* yacc.c:1646  */
    { if (define_validatetrans((constraint_expr_t*)(yyvsp[-1].valptr))) return -1; }
#line 2486 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 146:
#line 476 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = (yyvsp[-1].valptr); }
#line 2492 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 147:
#line 478 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_NOT, (yyvsp[0].valptr), 0);
			  if ((yyval.valptr) == 0) return -1; }
#line 2499 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 148:
#line 481 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_AND, (yyvsp[-2].valptr), (yyvsp[0].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2506 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 149:
#line 484 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_OR, (yyvsp[-2].valptr), (yyvsp[0].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2513 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 150:
#line 487 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = (yyvsp[0].valptr); }
#line 2519 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 151:
#line 490 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_USER, (yyvsp[-1].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2526 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 152:
#line 493 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_ROLE, (yyvsp[-1].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2533 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 153:
#line 496 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_TYPE, (yyvsp[-1].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2540 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 154:
#line 498 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_separator(1)) return -1; }
#line 2546 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 155:
#line 499 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_NAMES, CEXPR_USER, (yyvsp[-2].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2553 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 156:
#line 501 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_separator(1)) return -1; }
#line 2559 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 157:
#line 502 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_NAMES, (CEXPR_USER | CEXPR_TARGET), (yyvsp[-2].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2566 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 158:
#line 504 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_separator(1)) return -1; }
#line 2572 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 159:
#line 505 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_NAMES, (CEXPR_USER | CEXPR_XTARGET), (yyvsp[-2].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2579 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 160:
#line 507 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_separator(1)) return -1; }
#line 2585 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 161:
#line 508 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_NAMES, CEXPR_ROLE, (yyvsp[-2].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2592 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 162:
#line 510 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_separator(1)) return -1; }
#line 2598 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 163:
#line 511 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_NAMES, (CEXPR_ROLE | CEXPR_TARGET), (yyvsp[-2].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2605 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 164:
#line 513 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_separator(1)) return -1; }
#line 2611 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 165:
#line 514 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_NAMES, (CEXPR_ROLE | CEXPR_XTARGET), (yyvsp[-2].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2618 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 166:
#line 516 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_separator(1)) return -1; }
#line 2624 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 167:
#line 517 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_NAMES, CEXPR_TYPE, (yyvsp[-2].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2631 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 168:
#line 519 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_separator(1)) return -1; }
#line 2637 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 169:
#line 520 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_NAMES, (CEXPR_TYPE | CEXPR_TARGET), (yyvsp[-2].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2644 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 170:
#line 522 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_separator(1)) return -1; }
#line 2650 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 171:
#line 523 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_NAMES, (CEXPR_TYPE | CEXPR_XTARGET), (yyvsp[-2].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2657 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 172:
#line 526 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_USER, CEXPR_EQ);
			  if ((yyval.valptr) == 0) return -1; }
#line 2664 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 173:
#line 528 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_separator(1)) return -1; }
#line 2670 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 174:
#line 529 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_NAMES, CEXPR_ROLE, CEXPR_EQ);
			  if ((yyval.valptr) == 0) return -1; }
#line 2677 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 175:
#line 531 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_separator(1)) return -1; }
#line 2683 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 176:
#line 532 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_NAMES, (CEXPR_ROLE | CEXPR_TARGET), CEXPR_EQ);
			  if ((yyval.valptr) == 0) return -1; }
#line 2690 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 177:
#line 535 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_ROLE, (yyvsp[0].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2697 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 178:
#line 537 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_separator(1)) return -1; }
#line 2703 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 179:
#line 538 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_NAMES, CEXPR_TYPE, CEXPR_EQ);
			  if ((yyval.valptr) == 0) return -1; }
#line 2710 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 180:
#line 540 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_separator(1)) return -1; }
#line 2716 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 181:
#line 541 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_NAMES, (CEXPR_TYPE | CEXPR_TARGET), CEXPR_EQ);
			  if ((yyval.valptr) == 0) return -1; }
#line 2723 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 182:
#line 544 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_L1L2, (yyvsp[-1].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2730 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 183:
#line 547 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_L1H2, (yyvsp[-1].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2737 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 184:
#line 550 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_H1L2, (yyvsp[-1].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2744 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 185:
#line 553 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_H1H2, (yyvsp[-1].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2751 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 186:
#line 556 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_L1H1, (yyvsp[-1].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2758 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 187:
#line 559 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = define_cexpr(CEXPR_ATTR, CEXPR_L2H2, (yyvsp[-1].valptr));
			  if ((yyval.valptr) == 0) return -1; }
#line 2765 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 188:
#line 563 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = CEXPR_EQ; }
#line 2771 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 189:
#line 565 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = CEXPR_NEQ; }
#line 2777 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 190:
#line 568 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = (yyvsp[0].valptr); }
#line 2783 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 191:
#line 570 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = CEXPR_DOM; }
#line 2789 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 192:
#line 572 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = CEXPR_DOMBY; }
#line 2795 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 193:
#line 574 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.valptr) = CEXPR_INCOMP; }
#line 2801 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 196:
#line 580 "policy_parse.y" /* yacc.c:1646  */
    {if (define_user()) return -1;}
#line 2807 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 201:
#line 589 "policy_parse.y" /* yacc.c:1646  */
    {if (define_initial_sid_context()) return -1;}
#line 2813 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 210:
#line 602 "policy_parse.y" /* yacc.c:1646  */
    {if (define_pirq_context((yyvsp[-1].val))) return -1;}
#line 2819 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 211:
#line 605 "policy_parse.y" /* yacc.c:1646  */
    {if (define_iomem_context((yyvsp[-1].val),(yyvsp[-1].val))) return -1;}
#line 2825 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 212:
#line 607 "policy_parse.y" /* yacc.c:1646  */
    {if (define_iomem_context((yyvsp[-3].val),(yyvsp[-1].val))) return -1;}
#line 2831 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 213:
#line 610 "policy_parse.y" /* yacc.c:1646  */
    {if (define_ioport_context((yyvsp[-1].val),(yyvsp[-1].val))) return -1;}
#line 2837 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 214:
#line 612 "policy_parse.y" /* yacc.c:1646  */
    {if (define_ioport_context((yyvsp[-3].val),(yyvsp[-1].val))) return -1;}
#line 2843 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 215:
#line 615 "policy_parse.y" /* yacc.c:1646  */
    {if (define_pcidevice_context((yyvsp[-1].val))) return -1;}
#line 2849 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 220:
#line 624 "policy_parse.y" /* yacc.c:1646  */
    {if (define_fs_context((yyvsp[-3].val),(yyvsp[-2].val))) return -1;}
#line 2855 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 226:
#line 635 "policy_parse.y" /* yacc.c:1646  */
    {if (define_port_context((yyvsp[-1].val),(yyvsp[-1].val))) return -1;}
#line 2861 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 227:
#line 637 "policy_parse.y" /* yacc.c:1646  */
    {if (define_port_context((yyvsp[-3].val),(yyvsp[-1].val))) return -1;}
#line 2867 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 232:
#line 646 "policy_parse.y" /* yacc.c:1646  */
    {if (define_netif_context()) return -1;}
#line 2873 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 237:
#line 655 "policy_parse.y" /* yacc.c:1646  */
    {if (define_ipv4_node_context()) return -1;}
#line 2879 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 238:
#line 657 "policy_parse.y" /* yacc.c:1646  */
    {if (define_ipv6_node_context()) return -1;}
#line 2885 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 243:
#line 666 "policy_parse.y" /* yacc.c:1646  */
    {if (define_fs_use(SECURITY_FS_USE_XATTR)) return -1;}
#line 2891 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 244:
#line 668 "policy_parse.y" /* yacc.c:1646  */
    {if (define_fs_use(SECURITY_FS_USE_TASK)) return -1;}
#line 2897 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 245:
#line 670 "policy_parse.y" /* yacc.c:1646  */
    {if (define_fs_use(SECURITY_FS_USE_TRANS)) return -1;}
#line 2903 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 246:
#line 672 "policy_parse.y" /* yacc.c:1646  */
    {if (define_fs_use(SECURITY_FS_USE_PSIDS)) return -1;}
#line 2909 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 251:
#line 681 "policy_parse.y" /* yacc.c:1646  */
    {if (define_genfs_context(1)) return -1;}
#line 2915 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 252:
#line 682 "policy_parse.y" /* yacc.c:1646  */
    {insert_id("-", 0);}
#line 2921 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 253:
#line 683 "policy_parse.y" /* yacc.c:1646  */
    {if (define_genfs_context(1)) return -1;}
#line 2927 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 254:
#line 685 "policy_parse.y" /* yacc.c:1646  */
    {if (define_genfs_context(0)) return -1;}
#line 2933 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 255:
#line 688 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_id(yytext,0)) return -1; }
#line 2939 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 259:
#line 696 "policy_parse.y" /* yacc.c:1646  */
    {if (insert_separator(0)) return -1;}
#line 2945 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 260:
#line 698 "policy_parse.y" /* yacc.c:1646  */
    {if (insert_separator(0)) return -1;}
#line 2951 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 261:
#line 701 "policy_parse.y" /* yacc.c:1646  */
    {if (insert_separator(0)) return -1;}
#line 2957 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 262:
#line 703 "policy_parse.y" /* yacc.c:1646  */
    {if (insert_separator(0)) return -1;}
#line 2963 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 267:
#line 713 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_separator(0)) return -1; }
#line 2969 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 268:
#line 715 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_separator(0)) return -1; }
#line 2975 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 269:
#line 717 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_id("*", 0)) return -1; 
			  if (insert_separator(0)) return -1; }
#line 2982 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 270:
#line 720 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_id("~", 0)) return -1;
			  if (insert_separator(0)) return -1; }
#line 2989 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 271:
#line 723 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_id("~", 0)) return -1; 
			  if (insert_separator(0)) return -1; }
#line 2996 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 272:
#line 725 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_id("-", 0)) return -1; }
#line 3002 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 273:
#line 726 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_separator(0)) return -1; }
#line 3008 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 274:
#line 729 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_id("~", 1)) return -1; }
#line 3014 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 275:
#line 732 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_id("*", 1)) return -1; }
#line 3020 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 283:
#line 744 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_id(yytext, 1)) return -1; }
#line 3026 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 290:
#line 753 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_id("-", 0)) return -1; }
#line 3032 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 293:
#line 756 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_id(yytext,0)) return -1; }
#line 3038 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 294:
#line 759 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_id(yytext,0)) return -1; }
#line 3044 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 295:
#line 762 "policy_parse.y" /* yacc.c:1646  */
    { yytext[strlen(yytext) - 1] = '\0'; if (insert_id(yytext + 1,0)) return -1; }
#line 3050 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 296:
#line 765 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.val) = strtoul(yytext,NULL,0); }
#line 3056 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 297:
#line 768 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_id(yytext,0)) return -1; }
#line 3062 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 298:
#line 771 "policy_parse.y" /* yacc.c:1646  */
    {if (define_polcap()) return -1;}
#line 3068 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 299:
#line 774 "policy_parse.y" /* yacc.c:1646  */
    {if (define_permissive()) return -1;}
#line 3074 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 300:
#line 779 "policy_parse.y" /* yacc.c:1646  */
    { if (end_avrule_block(pass) == -1) return -1;
                          if (policydb_index_others(NULL, policydbp, 0)) return -1;
                        }
#line 3082 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 301:
#line 784 "policy_parse.y" /* yacc.c:1646  */
    { if (define_policy(pass, 1) == -1) return -1; }
#line 3088 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 302:
#line 787 "policy_parse.y" /* yacc.c:1646  */
    { if (insert_id(yytext,0)) return -1; }
#line 3094 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 318:
#line 811 "policy_parse.y" /* yacc.c:1646  */
    { if (require_class(pass)) return -1; }
#line 3100 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 319:
#line 813 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.require_func) = require_role; }
#line 3106 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 320:
#line 814 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.require_func) = require_type; }
#line 3112 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 321:
#line 815 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.require_func) = require_attribute; }
#line 3118 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 322:
#line 816 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.require_func) = require_user; }
#line 3124 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 323:
#line 817 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.require_func) = require_bool; }
#line 3130 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 324:
#line 818 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.require_func) = require_sens; }
#line 3136 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 325:
#line 819 "policy_parse.y" /* yacc.c:1646  */
    { (yyval.require_func) = require_cat; }
#line 3142 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 326:
#line 822 "policy_parse.y" /* yacc.c:1646  */
    { if ((yyvsp[-1].require_func) (pass)) return -1; }
#line 3148 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 327:
#line 824 "policy_parse.y" /* yacc.c:1646  */
    { if ((yyvsp[-3].require_func) (pass)) return -1; }
#line 3154 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 328:
#line 827 "policy_parse.y" /* yacc.c:1646  */
    { if (end_avrule_block(pass) == -1) return -1; }
#line 3160 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 329:
#line 829 "policy_parse.y" /* yacc.c:1646  */
    { if (end_optional(pass) == -1) return -1; }
#line 3166 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 330:
#line 832 "policy_parse.y" /* yacc.c:1646  */
    { if (end_avrule_block(pass) == -1) return -1; }
#line 3172 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 332:
#line 836 "policy_parse.y" /* yacc.c:1646  */
    { if (begin_optional(pass) == -1) return -1; }
#line 3178 "policy_parse.c" /* yacc.c:1646  */
    break;

  case 333:
#line 839 "policy_parse.y" /* yacc.c:1646  */
    { if (begin_optional_else(pass) == -1) return -1; }
#line 3184 "policy_parse.c" /* yacc.c:1646  */
    break;


#line 3188 "policy_parse.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
