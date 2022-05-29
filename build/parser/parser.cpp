/* A Bison parser, made by GNU Bison 3.8.1.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30801

/* Bison version string.  */
#define YYBISON_VERSION "3.8.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "src/parser/parser.y"

#include "ast/node.hpp"
#include "ast/generate/generate.hpp"
#include <iostream>
#include <cstdlib>
#include "parser.hpp"
#include "config.hpp"

//AST抽象语法树root，在generate.hpp中定义
using sysy::ast::root;

extern int yylex();
extern int yyget_lineno();
extern int yylex_destroy();

void yyerror(const char* s)
{
    std::cerr << sysy::config::filename << ":" <<yylloc.first_line << \
    ":" << yylloc.first_column << " error :" << s << std::endl;
    yylex_destroy();
    std::exit(1);
}
// BISON的location操作default定义
#define YYLLOC_DEFAULT(Current, Rhs, N)                               \
    do {                                                              \
        if (N) {                                                      \
            (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;    \
            (Current).first_column = YYRHSLOC (Rhs, 1).first_column;  \
            (Current).last_line    = YYRHSLOC (Rhs, N).last_line;     \
            (Current).last_column  = YYRHSLOC (Rhs, N).last_column;   \
        } else {                                                      \
            (Current).first_line   = (Current).last_line   =          \
              YYRHSLOC (Rhs, 0).last_line;                            \
            (Current).first_column = (Current).last_column =          \
              YYRHSLOC (Rhs, 0).last_column;                          \
        }                                                             \
        yylloc = Current;                                             \
    } while (0)
//?
#define yytnamerr(_yyres, _yystr)                                         \
        ([](char* yyres, const char* yystr) {                             \
            if (*yystr == '"') {                                          \
                if (yyres) return yystpcpy(yyres, yystr + 1) - yyres - 1; \
                else return yystrlen(yystr) - 2;                          \
            } else {                                                      \
                if (yyres) return yystpcpy(yyres, yystr) - yyres;         \
                else return yystrlen(yystr);                              \
            }                                                             \
        })(_yyres, _yystr)

#line 122 "/home/lucar/compiler/build/parser/parser.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INT_VALUE = 3,                  /* "integer"  */
  YYSYMBOL_IDENTIFIER = 4,                 /* "identifier"  */
  YYSYMBOL_IF = 5,                         /* "if"  */
  YYSYMBOL_ELSE = 6,                       /* "else"  */
  YYSYMBOL_WHILE = 7,                      /* "while"  */
  YYSYMBOL_BREAK = 8,                      /* "break"  */
  YYSYMBOL_CONTINUE = 9,                   /* "continue"  */
  YYSYMBOL_RETURN = 10,                    /* "return"  */
  YYSYMBOL_CONST = 11,                     /* "const"  */
  YYSYMBOL_INT = 12,                       /* "int"  */
  YYSYMBOL_VOID = 13,                      /* "void"  */
  YYSYMBOL_LPARENT = 14,                   /* "("  */
  YYSYMBOL_RPARENT = 15,                   /* ")"  */
  YYSYMBOL_LBRACKET = 16,                  /* "["  */
  YYSYMBOL_RBRACKET = 17,                  /* "]"  */
  YYSYMBOL_LBRACE = 18,                    /* "{"  */
  YYSYMBOL_RBRACE = 19,                    /* "}"  */
  YYSYMBOL_ASSIGN = 20,                    /* "="  */
  YYSYMBOL_EQ = 21,                        /* "=="  */
  YYSYMBOL_NQ = 22,                        /* "!="  */
  YYSYMBOL_LT = 23,                        /* "<"  */
  YYSYMBOL_LEQ = 24,                       /* "<="  */
  YYSYMBOL_GT = 25,                        /* ">"  */
  YYSYMBOL_GEQ = 26,                       /* ">="  */
  YYSYMBOL_AND = 27,                       /* "&&"  */
  YYSYMBOL_OR = 28,                        /* "||"  */
  YYSYMBOL_SEMI = 29,                      /* ";"  */
  YYSYMBOL_COMMA = 30,                     /* ","  */
  YYSYMBOL_PLUSPLUS = 31,                  /* "++"  */
  YYSYMBOL_MINUSMINUS = 32,                /* "--"  */
  YYSYMBOL_PLUS = 33,                      /* "+"  */
  YYSYMBOL_MINUS = 34,                     /* "-"  */
  YYSYMBOL_MUL = 35,                       /* "*"  */
  YYSYMBOL_DIV = 36,                       /* "/"  */
  YYSYMBOL_MOD = 37,                       /* "%"  */
  YYSYMBOL_NOT = 38,                       /* "!"  */
  YYSYMBOL_YYACCEPT = 39,                  /* $accept  */
  YYSYMBOL_CompUnit = 40,                  /* CompUnit  */
  YYSYMBOL_Decl = 41,                      /* Decl  */
  YYSYMBOL_ConstStmt = 42,                 /* ConstStmt  */
  YYSYMBOL_ConstDecl = 43,                 /* ConstDecl  */
  YYSYMBOL_BType = 44,                     /* BType  */
  YYSYMBOL_VarStmt = 45,                   /* VarStmt  */
  YYSYMBOL_VarDecl = 46,                   /* VarDecl  */
  YYSYMBOL_VarDef = 47,                    /* VarDef  */
  YYSYMBOL_ConstDef = 48,                  /* ConstDef  */
  YYSYMBOL_DefOne = 49,                    /* DefOne  */
  YYSYMBOL_DefArray = 50,                  /* DefArray  */
  YYSYMBOL_ArrayName = 51,                 /* ArrayName  */
  YYSYMBOL_ArrayInit = 52,                 /* ArrayInit  */
  YYSYMBOL_ArrayInner = 53,                /* ArrayInner  */
  YYSYMBOL_ConstDefOne = 54,               /* ConstDefOne  */
  YYSYMBOL_ConstDefArray = 55,             /* ConstDefArray  */
  YYSYMBOL_Exp = 56,                       /* Exp  */
  YYSYMBOL_Cond = 57,                      /* Cond  */
  YYSYMBOL_LOrExp = 58,                    /* LOrExp  */
  YYSYMBOL_LAndExp = 59,                   /* LAndExp  */
  YYSYMBOL_EqExp = 60,                     /* EqExp  */
  YYSYMBOL_RelExp = 61,                    /* RelExp  */
  YYSYMBOL_AddExp = 62,                    /* AddExp  */
  YYSYMBOL_MulExp = 63,                    /* MulExp  */
  YYSYMBOL_UnaryExp = 64,                  /* UnaryExp  */
  YYSYMBOL_FuncCall = 65,                  /* FuncCall  */
  YYSYMBOL_PrimaryExp = 66,                /* PrimaryExp  */
  YYSYMBOL_Lval = 67,                      /* Lval  */
  YYSYMBOL_ArrayItem = 68,                 /* ArrayItem  */
  YYSYMBOL_FuncDef = 69,                   /* FuncDef  */
  YYSYMBOL_FuncFParams = 70,               /* FuncFParams  */
  YYSYMBOL_FuncRParams = 71,               /* FuncRParams  */
  YYSYMBOL_FuncFParam = 72,                /* FuncFParam  */
  YYSYMBOL_FuncFParamOne = 73,             /* FuncFParamOne  */
  YYSYMBOL_FuncFParamArray = 74,           /* FuncFParamArray  */
  YYSYMBOL_Block = 75,                     /* Block  */
  YYSYMBOL_BlockItems = 76,                /* BlockItems  */
  YYSYMBOL_BlockItem = 77,                 /* BlockItem  */
  YYSYMBOL_Stmt = 78,                      /* Stmt  */
  YYSYMBOL_AssignStmt = 79,                /* AssignStmt  */
  YYSYMBOL_AssignStmtNoSEMI = 80,          /* AssignStmtNoSEMI  */
  YYSYMBOL_IfStmt = 81,                    /* IfStmt  */
  YYSYMBOL_ReturnStmt = 82,                /* ReturnStmt  */
  YYSYMBOL_WhileStmt = 83,                 /* WhileStmt  */
  YYSYMBOL_BreakStmt = 84,                 /* BreakStmt  */
  YYSYMBOL_ContinueStmt = 85,              /* ContinueStmt  */
  YYSYMBOL_RelOp = 86,                     /* RelOp  */
  YYSYMBOL_AddOp = 87,                     /* AddOp  */
  YYSYMBOL_MulOp = 88,                     /* MulOp  */
  YYSYMBOL_UnaryOp = 89,                   /* UnaryOp  */
  YYSYMBOL_Ident = 90,                     /* Ident  */
  YYSYMBOL_Number = 91                     /* Number  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  15
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   248

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  53
/* YYNRULES -- Number of rules.  */
#define YYNRULES  115
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  192

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   293


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
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
      35,    36,    37,    38
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   113,   113,   114,   115,   116,   119,   120,   123,   126,
     127,   130,   133,   136,   137,   140,   141,   144,   145,   148,
     149,   152,   153,   156,   157,   160,   161,   164,   165,   166,
     167,   170,   173,   176,   179,   182,   183,   186,   187,   190,
     191,   192,   195,   196,   199,   200,   203,   204,   207,   208,
     209,   212,   213,   216,   217,   218,   219,   222,   223,   226,
     227,   230,   231,   232,   233,   236,   237,   240,   241,   244,
     245,   248,   252,   258,   265,   266,   269,   270,   273,   274,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   288,
     294,   295,   300,   305,   306,   309,   310,   313,   314,   317,
     320,   323,   326,   327,   328,   329,   332,   333,   336,   337,
     338,   341,   342,   343,   345,   347
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "\"integer\"",
  "\"identifier\"", "\"if\"", "\"else\"", "\"while\"", "\"break\"",
  "\"continue\"", "\"return\"", "\"const\"", "\"int\"", "\"void\"",
  "\"(\"", "\")\"", "\"[\"", "\"]\"", "\"{\"", "\"}\"", "\"=\"", "\"==\"",
  "\"!=\"", "\"<\"", "\"<=\"", "\">\"", "\">=\"", "\"&&\"", "\"||\"",
  "\";\"", "\",\"", "\"++\"", "\"--\"", "\"+\"", "\"-\"", "\"*\"", "\"/\"",
  "\"%\"", "\"!\"", "$accept", "CompUnit", "Decl", "ConstStmt",
  "ConstDecl", "BType", "VarStmt", "VarDecl", "VarDef", "ConstDef",
  "DefOne", "DefArray", "ArrayName", "ArrayInit", "ArrayInner",
  "ConstDefOne", "ConstDefArray", "Exp", "Cond", "LOrExp", "LAndExp",
  "EqExp", "RelExp", "AddExp", "MulExp", "UnaryExp", "FuncCall",
  "PrimaryExp", "Lval", "ArrayItem", "FuncDef", "FuncFParams",
  "FuncRParams", "FuncFParam", "FuncFParamOne", "FuncFParamArray", "Block",
  "BlockItems", "BlockItem", "Stmt", "AssignStmt", "AssignStmtNoSEMI",
  "IfStmt", "ReturnStmt", "WhileStmt", "BreakStmt", "ContinueStmt",
  "RelOp", "AddOp", "MulOp", "UnaryOp", "Ident", "Number", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-89)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     143,     7,   -89,    60,    65,   -89,   -89,   152,    60,   -89,
     154,   -89,    60,   -89,    53,   -89,   -89,   -89,   -89,    60,
     -89,   -89,   -89,    74,    75,   -89,    60,   -89,    80,   -89,
     -89,    85,     9,   -89,   201,    51,    35,   201,   201,   -89,
      99,    51,   201,    61,    60,    -4,   -89,    68,   100,   -89,
     201,    60,    60,   -89,   -89,   -89,    18,   141,   -89,   -89,
     -89,    43,   119,   -89,   201,    67,   -89,    54,   -89,    61,
       2,    22,    70,   -89,    70,   129,   -89,   -89,    61,     7,
     106,   201,   138,   116,   130,   107,   126,    70,   158,   -89,
     158,   -89,   -89,   -89,   201,   -89,   -89,   -89,   201,   201,
     201,   -89,   -89,   201,   -89,   209,   -89,   -89,     1,    70,
     -89,    61,   -89,   145,   175,   162,   177,    11,   -89,   -89,
     -89,    60,   181,    70,   -89,   161,   -89,   -89,   -89,   185,
     -89,   -89,   -89,   -89,   -89,   -89,   -89,   -89,   199,   -89,
     201,   201,   201,   201,   -89,   -89,   -89,   -89,   201,   141,
     -89,   200,    70,   202,   201,    70,     8,   -89,    79,   -89,
     201,   201,   -89,   -89,   -89,   189,   -89,   -89,   -89,   -89,
     -89,   130,   107,   126,   126,    70,   -89,   -89,   -89,   201,
     -89,    70,   205,   206,   -89,    70,   193,   193,   222,   -89,
     193,   -89
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,    11,     0,     0,     4,     6,     0,     0,     7,
       0,     5,     0,   114,     0,     1,     2,     3,     8,     0,
      13,    15,    16,    22,    20,    12,     0,     9,     0,    17,
      18,     0,     0,    10,     0,     0,     0,     0,     0,    14,
      20,     0,     0,     0,     0,     0,    66,    69,    70,   115,
       0,     0,     0,   111,   112,   113,     0,    45,    47,    49,
      50,    53,    58,    56,     0,    57,    54,     0,    21,     0,
       0,     0,    19,    32,    31,     0,    64,    71,     0,     0,
       0,     0,     0,    34,    36,    38,    41,    43,    91,    57,
      92,    23,   106,   107,     0,   108,   109,   110,     0,     0,
       0,    93,    94,     0,    48,     0,    26,    29,     0,    30,
      62,     0,    24,     0,     0,     0,     0,     0,    74,    88,
      78,     0,     0,    33,    80,     0,    76,    79,    81,    56,
      83,    82,    84,    85,    86,    63,    65,    72,     0,    55,
       0,     0,     0,     0,   104,   105,   102,   103,     0,    44,
      46,     0,    90,     0,    52,    68,     0,    25,     0,    61,
       0,     0,   100,   101,    98,     0,    87,    75,    77,    89,
      73,    35,    37,    40,    39,    42,    59,    60,    51,     0,
      28,    27,     0,     0,    97,    67,     0,     0,    95,    99,
       0,    96
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -89,   -89,   142,   -89,   -89,     5,   -89,   -89,   203,   211,
     -89,   -89,    34,   -40,   -89,   -89,   -89,   -69,    25,   -89,
      96,    97,    45,   -34,   150,   -57,   -89,   -89,   157,   -89,
     233,   210,   -89,   166,   -89,   -89,   -41,   -89,   123,   -88,
     -89,   -65,   -89,   -89,   -89,   -89,   -89,   -89,   -89,   -89,
     -89,    10,   -89
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     4,   120,     6,     7,    44,     9,    10,    20,    27,
      21,    22,    23,    68,   108,    29,    30,   122,    82,    83,
      84,    85,    86,   123,    57,    58,    59,    60,    61,    62,
      11,    45,   156,    46,    47,    48,   124,   125,   126,   127,
     128,    63,   130,   131,   132,   133,   134,   148,    94,    98,
      64,    65,    66
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      56,    73,    76,    71,    72,     8,    12,   104,    74,     8,
     129,    78,   138,    14,    49,    13,    87,   111,    24,     2,
     157,     2,    31,   178,    43,    50,    79,   107,   110,    31,
     151,   158,    79,   109,   153,    91,    40,   135,   179,   112,
     164,   150,    51,    52,    53,    54,    28,     2,   165,    55,
      69,    92,    93,    28,    77,    92,    93,    49,    13,    99,
     129,    89,    89,   100,    13,    15,   152,    32,    50,    67,
     159,   155,    67,   106,   101,   102,     1,     2,     3,    75,
     121,   105,    49,    13,    80,    51,    52,    53,    54,    36,
      34,    37,    55,    50,    35,    38,    34,    67,   188,   189,
      41,    37,   191,    92,    93,    42,    87,    87,    87,    87,
      51,    52,    53,    54,   175,    37,    81,    55,   180,    38,
      87,   129,   129,   137,   181,   129,    87,    87,   142,   143,
     121,    40,    49,    13,   113,   103,   114,   115,   116,   117,
       1,     2,     5,    50,   140,   185,    16,    75,   118,   144,
     145,   146,   147,   139,     1,     2,     3,   141,   119,   160,
      51,    52,    53,    54,    49,    13,   113,    55,   114,   115,
     116,   117,     1,     2,    99,    50,    95,    96,    97,    75,
     167,    18,    19,    25,    26,   182,   183,   173,   174,   161,
     119,   162,    51,    52,    53,    54,    49,    13,   113,    55,
     114,   115,   116,   117,    49,    13,   163,    50,    88,    90,
     166,    75,    49,    13,   169,    50,   170,   176,   184,   177,
     186,   187,   119,   154,    51,    52,    53,    54,   190,    39,
      33,    55,    51,    52,    53,    54,   171,    17,   172,    55,
      51,    52,    53,    54,   149,   136,    70,    55,   168
};

static const yytype_uint8 yycheck[] =
{
      34,    41,    43,    37,    38,     0,     1,    64,    42,     4,
      75,    15,    81,     3,     3,     4,    50,    15,     8,    12,
      19,    12,    12,    15,    15,    14,    30,    67,    69,    19,
      99,    30,    30,    67,   103,    17,    26,    78,    30,    17,
      29,    98,    31,    32,    33,    34,    12,    12,   117,    38,
      15,    33,    34,    19,    44,    33,    34,     3,     4,    16,
     125,    51,    52,    20,     4,     0,   100,    14,    14,    18,
     111,   105,    18,    19,    31,    32,    11,    12,    13,    18,
      75,    14,     3,     4,    16,    31,    32,    33,    34,    14,
      16,    16,    38,    14,    20,    20,    16,    18,   186,   187,
      20,    16,   190,    33,    34,    20,   140,   141,   142,   143,
      31,    32,    33,    34,   148,    16,    16,    38,   158,    20,
     154,   186,   187,    17,   158,   190,   160,   161,    21,    22,
     125,   121,     3,     4,     5,    16,     7,     8,     9,    10,
      11,    12,     0,    14,    28,   179,     4,    18,    19,    23,
      24,    25,    26,    15,    11,    12,    13,    27,    29,    14,
      31,    32,    33,    34,     3,     4,     5,    38,     7,     8,
       9,    10,    11,    12,    16,    14,    35,    36,    37,    18,
      19,    29,    30,    29,    30,   160,   161,   142,   143,    14,
      29,    29,    31,    32,    33,    34,     3,     4,     5,    38,
       7,     8,     9,    10,     3,     4,    29,    14,    51,    52,
      29,    18,     3,     4,    29,    14,    17,    17,    29,    17,
      15,    15,    29,    14,    31,    32,    33,    34,     6,    26,
      19,    38,    31,    32,    33,    34,   140,     4,   141,    38,
      31,    32,    33,    34,    94,    79,    36,    38,   125
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    11,    12,    13,    40,    41,    42,    43,    44,    45,
      46,    69,    44,     4,    90,     0,    41,    69,    29,    30,
      47,    49,    50,    51,    90,    29,    30,    48,    51,    54,
      55,    90,    14,    48,    16,    20,    14,    16,    20,    47,
      90,    20,    20,    15,    44,    70,    72,    73,    74,     3,
      14,    31,    32,    33,    34,    38,    62,    63,    64,    65,
      66,    67,    68,    80,    89,    90,    91,    18,    52,    15,
      70,    62,    62,    52,    62,    18,    75,    90,    15,    30,
      16,    16,    57,    58,    59,    60,    61,    62,    67,    90,
      67,    17,    33,    34,    87,    35,    36,    37,    88,    16,
      20,    31,    32,    16,    64,    14,    19,    52,    53,    62,
      75,    15,    17,     5,     7,     8,     9,    10,    19,    29,
      41,    44,    56,    62,    75,    76,    77,    78,    79,    80,
      81,    82,    83,    84,    85,    75,    72,    17,    56,    15,
      28,    27,    21,    22,    23,    24,    25,    26,    86,    63,
      64,    56,    62,    56,    14,    62,    71,    19,    30,    75,
      14,    14,    29,    29,    29,    56,    29,    19,    77,    29,
      17,    59,    60,    61,    61,    62,    17,    17,    15,    30,
      52,    62,    57,    57,    29,    62,    15,    15,    78,    78,
       6,    78
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    39,    40,    40,    40,    40,    41,    41,    42,    43,
      43,    44,    45,    46,    46,    47,    47,    48,    48,    49,
      49,    50,    50,    51,    51,    52,    52,    53,    53,    53,
      53,    54,    55,    56,    57,    58,    58,    59,    59,    60,
      60,    60,    61,    61,    62,    62,    63,    63,    64,    64,
      64,    65,    65,    66,    66,    66,    66,    67,    67,    68,
      68,    69,    69,    69,    69,    70,    70,    71,    71,    72,
      72,    73,    74,    74,    75,    75,    76,    76,    77,    77,
      78,    78,    78,    78,    78,    78,    78,    78,    78,    79,
      80,    80,    80,    80,    80,    81,    81,    82,    82,    83,
      84,    85,    86,    86,    86,    86,    87,    87,    88,    88,
      88,    89,    89,    89,    90,    91
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     2,     1,     1,     1,     1,     2,     3,
       3,     1,     2,     2,     3,     1,     1,     1,     1,     3,
       1,     3,     1,     4,     4,     3,     2,     3,     3,     1,
       1,     3,     3,     1,     1,     3,     1,     3,     1,     3,
       3,     1,     3,     1,     3,     1,     3,     1,     2,     1,
       1,     4,     3,     1,     1,     3,     1,     1,     1,     4,
       4,     6,     5,     6,     5,     3,     1,     3,     1,     1,
       1,     2,     3,     4,     2,     3,     1,     2,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     1,     2,
       3,     2,     2,     2,     2,     5,     7,     3,     2,     5,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* CompUnit: CompUnit Decl  */
#line 113 "src/parser/parser.y"
                            {(yyval.root)->body.push_back((yyvsp[0].decl));}
#line 1468 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 3: /* CompUnit: CompUnit FuncDef  */
#line 114 "src/parser/parser.y"
                               {(yyval.root)->body.push_back((yyvsp[0].funcdef));}
#line 1474 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 4: /* CompUnit: Decl  */
#line 115 "src/parser/parser.y"
                   {root=new sysy::ast::node::Root();(yyval.root)=root;(yyval.root)->body.push_back((yyvsp[0].decl));}
#line 1480 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 5: /* CompUnit: FuncDef  */
#line 116 "src/parser/parser.y"
                      {root=new sysy::ast::node::Root();(yyval.root)=root;(yyval.root)->body.push_back((yyvsp[0].funcdef));}
#line 1486 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 8: /* ConstStmt: ConstDecl ";"  */
#line 123 "src/parser/parser.y"
                             {(yyval.declare_stmt)=(yyvsp[-1].declare_stmt);}
#line 1492 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 9: /* ConstDecl: "const" BType ConstDef  */
#line 126 "src/parser/parser.y"
                                   {(yyval.declare_stmt)=new sysy::ast::node::DeclareStmt((yyvsp[-1].token));(yyval.declare_stmt)->list.push_back((yyvsp[0].decl));}
#line 1498 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 10: /* ConstDecl: ConstDecl "," ConstDef  */
#line 127 "src/parser/parser.y"
                                       {(yyval.declare_stmt)->list.push_back((yyvsp[0].decl));}
#line 1504 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 12: /* VarStmt: VarDecl ";"  */
#line 133 "src/parser/parser.y"
                           {(yyval.declare_stmt)=(yyvsp[-1].declare_stmt);}
#line 1510 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 13: /* VarDecl: BType VarDef  */
#line 136 "src/parser/parser.y"
                           {(yyval.declare_stmt)=new sysy::ast::node::DeclareStmt((yyvsp[-1].token));(yyval.declare_stmt)->list.push_back((yyvsp[0].decl));}
#line 1516 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 14: /* VarDecl: VarDecl "," VarDef  */
#line 137 "src/parser/parser.y"
                                   {(yyval.declare_stmt)->list.push_back((yyvsp[0].decl));}
#line 1522 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 19: /* DefOne: Ident "=" AddExp  */
#line 148 "src/parser/parser.y"
                                  {(yyval.decl)=new sysy::ast::node::VarDeclWithInitVal(*(yyvsp[-2].ident),*(yyvsp[0].expr));}
#line 1528 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 20: /* DefOne: Ident  */
#line 149 "src/parser/parser.y"
                    {(yyval.decl)=new sysy::ast::node::VarDecl(*(yyvsp[0].ident));}
#line 1534 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 21: /* DefArray: ArrayName "=" ArrayInit  */
#line 152 "src/parser/parser.y"
                                         {(yyval.decl)=new sysy::ast::node::ArrayDeclWithInit(*(yyvsp[-2].arrayident),*(yyvsp[0].array_init_val)); }
#line 1540 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 22: /* DefArray: ArrayName  */
#line 153 "src/parser/parser.y"
                        {(yyval.decl)=new sysy::ast::node::ArrayDecl(*(yyvsp[0].arrayident));}
#line 1546 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 23: /* ArrayName: ArrayName "[" AddExp "]"  */
#line 156 "src/parser/parser.y"
                                                 {(yyval.arrayident)->shape.push_back((yyvsp[-1].expr));}
#line 1552 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 24: /* ArrayName: Ident "[" AddExp "]"  */
#line 157 "src/parser/parser.y"
                                             {(yyval.arrayident) = new sysy::ast::node::ArrayIdentifier(*(yyvsp[-3].ident)); (yyval.arrayident)->shape.push_back((yyvsp[-1].expr));}
#line 1558 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 25: /* ArrayInit: "{" ArrayInner "}"  */
#line 160 "src/parser/parser.y"
                                       {(yyval.array_init_val)=(yyvsp[-1].array_init_val);}
#line 1564 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 26: /* ArrayInit: "{" "}"  */
#line 161 "src/parser/parser.y"
                            {(yyval.array_init_val)=new sysy::ast::node::ArrayDeclWithInitVal(false, nullptr);}
#line 1570 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 27: /* ArrayInner: ArrayInner "," AddExp  */
#line 164 "src/parser/parser.y"
                                      {(yyval.array_init_val)=(yyvsp[-2].array_init_val); (yyval.array_init_val)->list.push_back(new sysy::ast::node::ArrayDeclWithInitVal(true,(yyvsp[0].expr))); }
#line 1576 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 28: /* ArrayInner: ArrayInner "," ArrayInit  */
#line 165 "src/parser/parser.y"
                                         {(yyval.array_init_val)=(yyvsp[-2].array_init_val); (yyval.array_init_val)->list.push_back((yyvsp[0].array_init_val));}
#line 1582 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 29: /* ArrayInner: ArrayInit  */
#line 166 "src/parser/parser.y"
                        {(yyval.array_init_val)=new sysy::ast::node::ArrayDeclWithInitVal(false,nullptr);(yyval.array_init_val)->list.push_back((yyvsp[0].array_init_val));}
#line 1588 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 30: /* ArrayInner: AddExp  */
#line 167 "src/parser/parser.y"
                     {(yyval.array_init_val)=new sysy::ast::node::ArrayDeclWithInitVal(false,nullptr);(yyval.array_init_val)->list.push_back(new sysy::ast::node::ArrayDeclWithInitVal(true,(yyvsp[0].expr)));}
#line 1594 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 31: /* ConstDefOne: Ident "=" AddExp  */
#line 170 "src/parser/parser.y"
                                  {(yyval.decl)=new sysy::ast::node::VarDeclWithInitVal(*(yyvsp[-2].ident),*(yyvsp[0].expr),true);}
#line 1600 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 32: /* ConstDefArray: ArrayName "=" ArrayInit  */
#line 173 "src/parser/parser.y"
                                             {(yyval.decl) = new sysy::ast::node::ArrayDeclWithInit(*(yyvsp[-2].arrayident), *(yyvsp[0].array_init_val), true);}
#line 1606 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 35: /* LOrExp: LOrExp "||" LAndExp  */
#line 182 "src/parser/parser.y"
                            {(yyval.expr)=new sysy::ast::node::BinaryExpr((yyvsp[-1].token),*(yyvsp[-2].expr),*(yyvsp[0].expr));}
#line 1612 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 37: /* LAndExp: LAndExp "&&" EqExp  */
#line 186 "src/parser/parser.y"
                            {(yyval.expr)=new sysy::ast::node::BinaryExpr((yyvsp[-1].token),*(yyvsp[-2].expr),*(yyvsp[0].expr));}
#line 1618 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 39: /* EqExp: EqExp "!=" RelExp  */
#line 190 "src/parser/parser.y"
                          {(yyval.expr)=new sysy::ast::node::BinaryExpr((yyvsp[-1].token),*(yyvsp[-2].expr),*(yyvsp[0].expr));}
#line 1624 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 40: /* EqExp: EqExp "==" RelExp  */
#line 191 "src/parser/parser.y"
                          {(yyval.expr)=new sysy::ast::node::BinaryExpr((yyvsp[-1].token),*(yyvsp[-2].expr),*(yyvsp[0].expr));}
#line 1630 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 42: /* RelExp: RelExp RelOp AddExp  */
#line 195 "src/parser/parser.y"
                              {(yyval.expr)=new sysy::ast::node::BinaryExpr((yyvsp[-1].token),*(yyvsp[-2].expr),*(yyvsp[0].expr));}
#line 1636 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 44: /* AddExp: AddExp AddOp MulExp  */
#line 199 "src/parser/parser.y"
                              {(yyval.expr)=new sysy::ast::node::BinaryExpr((yyvsp[-1].token),*(yyvsp[-2].expr),*(yyvsp[0].expr));}
#line 1642 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 46: /* MulExp: MulExp MulOp UnaryExp  */
#line 203 "src/parser/parser.y"
                                {(yyval.expr)=new sysy::ast::node::BinaryExpr((yyvsp[-1].token),*(yyvsp[-2].expr),*(yyvsp[0].expr));}
#line 1648 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 48: /* UnaryExp: UnaryOp UnaryExp  */
#line 207 "src/parser/parser.y"
                           {(yyval.expr)=new sysy::ast::node::UnaryExpr((yyvsp[-1].token),*(yyvsp[0].expr));}
#line 1654 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 51: /* FuncCall: Ident "(" FuncRParams ")"  */
#line 212 "src/parser/parser.y"
                                            {(yyval.expr)=new sysy::ast::node::FunctionCall(*(yyvsp[-3].ident),*(yyvsp[-1].functionCallArgList));}
#line 1660 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 52: /* FuncCall: Ident "(" "("  */
#line 213 "src/parser/parser.y"
                                {(yyval.expr)=new sysy::ast::node::FunctionCall(*(yyvsp[-2].ident),*(new sysy::ast::node::FunctionCallArgList()));}
#line 1666 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 55: /* PrimaryExp: "(" Cond ")"  */
#line 218 "src/parser/parser.y"
                                   {(yyval.expr)=(yyvsp[-1].condexpr);}
#line 1672 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 59: /* ArrayItem: Lval "[" Exp "]"  */
#line 226 "src/parser/parser.y"
                                         {(yyval.arrayident)=new sysy::ast::node::ArrayIdentifier(*(yyvsp[-3].ident));(yyval.arrayident)->shape.push_back((yyvsp[-1].expr));}
#line 1678 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 60: /* ArrayItem: ArrayItem "[" Exp "]"  */
#line 227 "src/parser/parser.y"
                                              {(yyval.arrayident)=(yyvsp[-3].arrayident); (yyval.arrayident)->shape.push_back((yyvsp[-1].expr));}
#line 1684 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 61: /* FuncDef: BType Ident "(" FuncFParams ")" Block  */
#line 230 "src/parser/parser.y"
                                                        {(yyval.funcdef)=new sysy::ast::node::FuncDefine((yyvsp[-5].token),*(yyvsp[-4].ident),*(yyvsp[-2].funcdefarglist),*(yyvsp[0].block));}
#line 1690 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 62: /* FuncDef: BType Ident "(" ")" Block  */
#line 231 "src/parser/parser.y"
                                            {(yyval.funcdef)=new sysy::ast::node::FuncDefine((yyvsp[-4].token),*(yyvsp[-3].ident),*(new sysy::ast::node::FuncArgList()),*(yyvsp[0].block));}
#line 1696 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 63: /* FuncDef: "void" Ident "(" FuncFParams ")" Block  */
#line 232 "src/parser/parser.y"
                                                       {(yyval.funcdef)=new sysy::ast::node::FuncDefine((yyvsp[-5].token),*(yyvsp[-4].ident),*(yyvsp[-2].funcdefarglist),*(yyvsp[0].block));}
#line 1702 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 64: /* FuncDef: "void" Ident "(" ")" Block  */
#line 233 "src/parser/parser.y"
                                           {(yyval.funcdef)=new sysy::ast::node::FuncDefine((yyvsp[-4].token),*(yyvsp[-3].ident),*(new sysy::ast::node::FuncArgList()),*(yyvsp[0].block));}
#line 1708 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 65: /* FuncFParams: FuncFParams "," FuncFParam  */
#line 236 "src/parser/parser.y"
                                           {(yyval.funcdefarglist)=(yyvsp[-2].funcdefarglist);(yyval.funcdefarglist)->list.push_back((yyvsp[0].funcdefarg));}
#line 1714 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 66: /* FuncFParams: FuncFParam  */
#line 237 "src/parser/parser.y"
                         {(yyval.funcdefarglist)=new sysy::ast::node::FuncArgList();(yyval.funcdefarglist)->list.push_back((yyvsp[0].funcdefarg));}
#line 1720 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 67: /* FuncRParams: FuncRParams "," AddExp  */
#line 240 "src/parser/parser.y"
                                       {(yyval.functionCallArgList)=(yyvsp[-2].functionCallArgList);(yyval.functionCallArgList)->args.push_back((yyvsp[0].expr));}
#line 1726 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 68: /* FuncRParams: AddExp  */
#line 241 "src/parser/parser.y"
                     {(yyval.functionCallArgList)=new sysy::ast::node::FunctionCallArgList();(yyval.functionCallArgList)->args.push_back((yyvsp[0].expr));}
#line 1732 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 71: /* FuncFParamOne: BType Ident  */
#line 248 "src/parser/parser.y"
                              {(yyval.funcdefarg)=new sysy::ast::node::FuncArg((yyvsp[-1].token),*(yyvsp[0].ident));}
#line 1738 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 72: /* FuncFParamArray: FuncFParamOne "[" "]"  */
#line 253 "src/parser/parser.y"
                {
                        (yyval.funcdefarg)=new sysy::ast::node::FuncArg((yyvsp[-2].funcdefarg)->type,
                        *new sysy::ast::node::ArrayIdentifier(*(new sysy::ast::node::ArrayIdentifier((yyvsp[-2].funcdefarg)->name))));
                        ((sysy::ast::node::ArrayIdentifier*)&((yyval.funcdefarg)->name))->shape.push_back(new sysy::ast::node::Number(1));
                }
#line 1748 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 73: /* FuncFParamArray: FuncFParamArray "[" Exp "]"  */
#line 259 "src/parser/parser.y"
                {
                        (yyval.funcdefarg)=(yyvsp[-3].funcdefarg);
                        ((sysy::ast::node::ArrayIdentifier*)&((yyval.funcdefarg)->name))->shape.push_back((yyvsp[-1].expr));
                }
#line 1757 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 74: /* Block: "{" "}"  */
#line 265 "src/parser/parser.y"
                        {(yyval.block)=new sysy::ast::node::Block();}
#line 1763 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 75: /* Block: "{" BlockItems "}"  */
#line 266 "src/parser/parser.y"
                                   {(yyval.block)=(yyvsp[-1].block);}
#line 1769 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 76: /* BlockItems: BlockItem  */
#line 269 "src/parser/parser.y"
                            {(yyval.block)=new sysy::ast::node::Block();(yyval.block)->Stmts.push_back((yyvsp[0].stmt));}
#line 1775 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 77: /* BlockItems: BlockItems BlockItem  */
#line 270 "src/parser/parser.y"
                                       {(yyval.block)=(yyvsp[-1].block);(yyval.block)->Stmts.push_back((yyvsp[0].stmt));}
#line 1781 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 87: /* Stmt: Exp ";"  */
#line 284 "src/parser/parser.y"
                   {(yyval.stmt)=new sysy::ast::node::ValueExpr(*(yyvsp[-1].expr));}
#line 1787 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 88: /* Stmt: ";"  */
#line 285 "src/parser/parser.y"
               {(yyval.stmt)=new sysy::ast::node::VoidStmt();}
#line 1793 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 89: /* AssignStmt: AssignStmtNoSEMI ";"  */
#line 288 "src/parser/parser.y"
                                        {(yyval.stmt)=(yyvsp[-1].stmt);}
#line 1799 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 90: /* AssignStmtNoSEMI: Lval "=" AddExp  */
#line 294 "src/parser/parser.y"
                                     {(yyval.stmt)=new sysy::ast::node::AssignStmt(*(yyvsp[-2].ident),*(yyvsp[0].expr));}
#line 1805 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 91: /* AssignStmtNoSEMI: "++" Lval  */
#line 296 "src/parser/parser.y"
                {
                        (yyval.stmt)=new sysy::ast::node::AssignStmt(*(yyvsp[0].ident),
                        *(new sysy::ast::node::BinaryExpr(PLUS,*(yyvsp[0].ident),*new sysy::ast::node::Number(1))));
                }
#line 1814 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 92: /* AssignStmtNoSEMI: "--" Lval  */
#line 301 "src/parser/parser.y"
                {
                        (yyval.stmt)=new sysy::ast::node::AssignStmt(*(yyvsp[0].ident),
                        *(new sysy::ast::node::BinaryExpr(MINUS,*(yyvsp[0].ident),*new sysy::ast::node::Number(1))));
                }
#line 1823 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 93: /* AssignStmtNoSEMI: Lval "++"  */
#line 305 "src/parser/parser.y"
                                {(yyval.stmt)=new sysy::ast::node::AfterInc(*(yyvsp[-1].ident),PLUSPLUS);}
#line 1829 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 94: /* AssignStmtNoSEMI: Lval "--"  */
#line 306 "src/parser/parser.y"
                                  {(yyval.stmt)=new sysy::ast::node::AfterInc(*(yyvsp[-1].ident),MINUSMINUS);}
#line 1835 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 95: /* IfStmt: "if" "(" Cond ")" Stmt  */
#line 309 "src/parser/parser.y"
                                       {(yyval.stmt)=new sysy::ast::node::IfStmt(*(yyvsp[-2].condexpr),*(yyvsp[0].stmt),*(new sysy::ast::node::VoidStmt()));}
#line 1841 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 96: /* IfStmt: "if" "(" Cond ")" Stmt "else" Stmt  */
#line 310 "src/parser/parser.y"
                                                 {(yyval.stmt)=new sysy::ast::node::IfStmt(*(yyvsp[-4].condexpr),*(yyvsp[-2].stmt),*(yyvsp[0].stmt));}
#line 1847 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 97: /* ReturnStmt: "return" Exp ";"  */
#line 313 "src/parser/parser.y"
                                  {(yyval.stmt)=new sysy::ast::node::ReturnStmt((yyvsp[-1].expr));}
#line 1853 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 98: /* ReturnStmt: "return" ";"  */
#line 314 "src/parser/parser.y"
                              {(yyval.stmt)=new sysy::ast::node::ReturnStmt();}
#line 1859 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 99: /* WhileStmt: "while" "(" Cond ")" Stmt  */
#line 317 "src/parser/parser.y"
                                                  {(yyval.stmt)=new sysy::ast::node::WhileStmt(*(yyvsp[-2].condexpr),*(yyvsp[0].stmt));}
#line 1865 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 100: /* BreakStmt: "break" ";"  */
#line 320 "src/parser/parser.y"
                             {(yyval.stmt)=new sysy::ast::node::BreakStmt();}
#line 1871 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 101: /* ContinueStmt: "continue" ";"  */
#line 323 "src/parser/parser.y"
                                {(yyval.stmt)=new sysy::ast::node::ContinueStmt();}
#line 1877 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 114: /* Ident: "identifier"  */
#line 345 "src/parser/parser.y"
                         {(yyval.ident)=new sysy::ast::node::Identifier(*(yyvsp[0].string));}
#line 1883 "/home/lucar/compiler/build/parser/parser.cpp"
    break;

  case 115: /* Number: "integer"  */
#line 347 "src/parser/parser.y"
                        {(yyval.expr)=new sysy::ast::node::Number(*(yyvsp[0].string));}
#line 1889 "/home/lucar/compiler/build/parser/parser.cpp"
    break;


#line 1893 "/home/lucar/compiler/build/parser/parser.cpp"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc);
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 349 "src/parser/parser.y"

