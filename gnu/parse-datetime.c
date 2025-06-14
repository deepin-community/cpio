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
#define YYPURE 1

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "parse-datetime.y" /* yacc.c:339  */

/* Parse a string into an internal timestamp.

   Copyright (C) 1999-2000, 2002-2024 Free Software Foundation, Inc.

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

/* Originally written by Steven M. Bellovin <smb@research.att.com> while
   at the University of North Carolina at Chapel Hill.  Later tweaked by
   a couple of people on Usenet.  Completely overhauled by Rich $alz
   <rsalz@bbn.com> and Jim Berets <jberets@bbn.com> in August, 1990.

   Modified by Assaf Gordon <assafgordon@gmail.com> in 2016 to add
   debug output.

   Modified by Paul Eggert <eggert@twinsun.com> in 1999 to do the
   right thing about local DST.  Also modified by Paul Eggert
   <eggert@cs.ucla.edu> in 2004 to support nanosecond-resolution
   timestamps, in 2004 to support TZ strings in dates, and in 2017 and 2020 to
   check for integer overflow and to support longer-than-'long'
   'time_t' and 'tv_nsec'.  */

#include <config.h>

#include "parse-datetime.h"

#include "idx.h"
#include "intprops.h"
#include "timespec.h"
#include "strftime.h"

/* There's no need to extend the stack, so there's no need to involve
   alloca.  */
#define YYSTACK_USE_ALLOCA 0

/* Tell Bison how much stack space is needed.  20 should be plenty for
   this grammar, which is not right recursive.  Beware setting it too
   high, since that might cause problems on machines whose
   implementations have lame stack-overflow checking.  */
#define YYMAXDEPTH 20
#define YYINITDEPTH YYMAXDEPTH

#include <inttypes.h>
#include <c-ctype.h>
#include <stdarg.h>
#include <stdckdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gettext.h"

#define _(str) gettext (str)

/* Bison's skeleton tests _STDLIB_H, while some stdlib.h headers
   use _STDLIB_H_ as witness.  Map the latter to the one bison uses.  */
/* FIXME: this is temporary.  Remove when we have a mechanism to ensure
   that the version we're using is fixed, too.  */
#ifdef _STDLIB_H_
# undef _STDLIB_H
# define _STDLIB_H 1
#endif

/* Shift A right by B bits portably, by dividing A by 2**B and
   truncating towards minus infinity.  A and B should be free of side
   effects, and B should be in the range 0 <= B <= INT_BITS - 2, where
   INT_BITS is the number of useful bits in an int.  GNU code can
   assume that INT_BITS is at least 32.

   ISO C99 says that A >> B is implementation-defined if A < 0.  Some
   implementations (e.g., UNICOS 9.0 on a Cray Y-MP EL) don't shift
   right in the usual way when A < 0, so SHR falls back on division if
   ordinary A >> B doesn't seem to be the usual signed shift.  */
#define SHR(a, b)       \
  (-1 >> 1 == -1        \
   ? (a) >> (b)         \
   : (a) / (1 << (b)) - ((a) % (1 << (b)) < 0))

#define HOUR(x) (60 * 60 * (x))

#define STREQ(a, b) (strcmp (a, b) == 0)

/* Verify that time_t is an integer as POSIX requires, and that every
   time_t value fits in intmax_t.  Please file a bug report if these
   assumptions are false on your platform.  */
static_assert (TYPE_IS_INTEGER (time_t));
static_assert (!TYPE_SIGNED (time_t) || INTMAX_MIN <= TYPE_MINIMUM (time_t));
static_assert (TYPE_MAXIMUM (time_t) <= INTMAX_MAX);

/* True if N is out of range for time_t.  */
static bool
time_overflow (intmax_t n)
{
  return ! ((TYPE_SIGNED (time_t) ? TYPE_MINIMUM (time_t) <= n : 0 <= n)
            && n <= TYPE_MAXIMUM (time_t));
}

/* Convert a possibly-signed character to an unsigned character.  This is
   a bit safer than casting to unsigned char, since it catches some type
   errors that the cast doesn't.  */
static unsigned char to_uchar (char ch) { return ch; }

static void _GL_ATTRIBUTE_FORMAT ((__printf__, 1, 2))
dbg_printf (char const *msg, ...)
{
  va_list args;
  /* TODO: use gnulib's 'program_name' instead?  */
  fputs ("date: ", stderr);

  va_start (args, msg);
  vfprintf (stderr, msg, args);
  va_end (args);
}


/* An integer value, and the number of digits in its textual
   representation.  */
typedef struct
{
  bool negative;
  intmax_t value;
  idx_t digits;
} textint;

/* An entry in the lexical lookup table.  */
typedef struct
{
  char const *name;
  int type;
  int value;
} table;

/* Meridian: am, pm, or 24-hour style.  */
enum { MERam, MERpm, MER24 };

/* A reasonable upper bound for the buffer used in debug output.  */
enum { DBGBUFSIZE = 100 };

enum { BILLION = 1000000000, LOG10_BILLION = 9 };

/* Relative times.  */
typedef struct
{
  /* Relative year, month, day, hour, minutes, seconds, and nanoseconds.  */
  intmax_t year;
  intmax_t month;
  intmax_t day;
  intmax_t hour;
  intmax_t minutes;
  intmax_t seconds;
  int ns;
} relative_time;

#if HAVE_COMPOUND_LITERALS
# define RELATIVE_TIME_0 ((relative_time) { 0, 0, 0, 0, 0, 0, 0 })
#else
static relative_time const RELATIVE_TIME_0;
#endif

/* Information passed to and from the parser.  */
typedef struct
{
  /* The input string remaining to be parsed.  */
  const char *input;

  /* N, if this is the Nth Tuesday.  */
  intmax_t day_ordinal;

  /* Day of week; Sunday is 0.  */
  int day_number;

  /* tm_isdst flag for the local zone.  */
  int local_isdst;

  /* Time zone, in seconds east of UT.  */
  int time_zone;

  /* Style used for time.  */
  int meridian;

  /* Gregorian year, month, day, hour, minutes, seconds, and nanoseconds.  */
  textint year;
  intmax_t month;
  intmax_t day;
  intmax_t hour;
  intmax_t minutes;
  struct timespec seconds; /* includes nanoseconds */

  /* Relative year, month, day, hour, minutes, seconds, and nanoseconds.  */
  relative_time rel;

  /* Presence or counts of nonterminals of various flavors parsed so far.  */
  bool timespec_seen;
  bool rels_seen;
  idx_t dates_seen;
  idx_t days_seen;
  idx_t J_zones_seen;
  idx_t local_zones_seen;
  idx_t dsts_seen;
  idx_t times_seen;
  idx_t zones_seen;
  bool year_seen;

#ifdef GNULIB_PARSE_DATETIME2
  /* Print debugging output to stderr.  */
  bool parse_datetime_debug;
#endif

  /* Which of the 'seen' parts have been printed when debugging.  */
  bool debug_dates_seen;
  bool debug_days_seen;
  bool debug_local_zones_seen;
  bool debug_times_seen;
  bool debug_zones_seen;
  bool debug_year_seen;

  /* The user specified explicit ordinal day value.  */
  bool debug_ordinal_day_seen;

  /* Table of local time zone abbreviations, terminated by a null entry.  */
  table local_time_zone_table[3];
} parser_control;

static bool
debugging (parser_control const *pc)
{
#ifdef GNULIB_PARSE_DATETIME2
  return pc->parse_datetime_debug;
#else
  return false;
#endif
}

union YYSTYPE;
static int yylex (union YYSTYPE *, parser_control *);
static int yyerror (parser_control const *, char const *);
static bool time_zone_hhmm (parser_control *, textint, intmax_t);

/* Extract into *PC any date and time info from a string of digits
   of the form e.g., YYYYMMDD, YYMMDD, HHMM, HH (and sometimes YYY,
   YYYY, ...).  */
static void
digits_to_date_time (parser_control *pc, textint text_int)
{
  if (pc->dates_seen && ! pc->year.digits
      && ! pc->rels_seen && (pc->times_seen || 2 < text_int.digits))
    {
      pc->year_seen = true;
      pc->year = text_int;
    }
  else
    {
      if (4 < text_int.digits)
        {
          pc->dates_seen++;
          pc->day = text_int.value % 100;
          pc->month = (text_int.value / 100) % 100;
          pc->year.value = text_int.value / 10000;
          pc->year.digits = text_int.digits - 4;
        }
      else
        {
          pc->times_seen++;
          if (text_int.digits <= 2)
            {
              pc->hour = text_int.value;
              pc->minutes = 0;
            }
          else
            {
              pc->hour = text_int.value / 100;
              pc->minutes = text_int.value % 100;
            }
          pc->seconds = (struct timespec) {0};
          pc->meridian = MER24;
        }
    }
}

/* Increment PC->rel by FACTOR * REL (FACTOR is 1 or -1).  Return true
   if successful, false if an overflow occurred.  */
static bool
apply_relative_time (parser_control *pc, relative_time rel, int factor)
{
  if (factor < 0
      ? (ckd_sub (&pc->rel.ns, pc->rel.ns, rel.ns)
         | ckd_sub (&pc->rel.seconds, pc->rel.seconds, rel.seconds)
         | ckd_sub (&pc->rel.minutes, pc->rel.minutes, rel.minutes)
         | ckd_sub (&pc->rel.hour, pc->rel.hour, rel.hour)
         | ckd_sub (&pc->rel.day, pc->rel.day, rel.day)
         | ckd_sub (&pc->rel.month, pc->rel.month, rel.month)
         | ckd_sub (&pc->rel.year, pc->rel.year, rel.year))
      : (ckd_add (&pc->rel.ns, pc->rel.ns, rel.ns)
         | ckd_add (&pc->rel.seconds, pc->rel.seconds, rel.seconds)
         | ckd_add (&pc->rel.minutes, pc->rel.minutes, rel.minutes)
         | ckd_add (&pc->rel.hour, pc->rel.hour, rel.hour)
         | ckd_add (&pc->rel.day, pc->rel.day, rel.day)
         | ckd_add (&pc->rel.month, pc->rel.month, rel.month)
         | ckd_add (&pc->rel.year, pc->rel.year, rel.year)))
    return false;
  pc->rels_seen = true;
  return true;
}

/* Set PC-> hour, minutes, seconds and nanoseconds members from arguments.  */
static void
set_hhmmss (parser_control *pc, intmax_t hour, intmax_t minutes,
            time_t sec, int nsec)
{
  pc->hour = hour;
  pc->minutes = minutes;
  pc->seconds = (struct timespec) { .tv_sec = sec, .tv_nsec = nsec };
}

/* Return a textual representation of the day ordinal/number values
   in the parser_control struct (e.g., "last wed", "this tues", "thu").  */
static const char *
str_days (parser_control *pc, char *buffer, int n)
{
  /* TODO: use relative_time_table for reverse lookup.  */
  static char const ordinal_values[][11] = {
     "last",
     "this",
     "next/first",
     "(SECOND)", /* SECOND is commented out in relative_time_table.  */
     "third",
     "fourth",
     "fifth",
     "sixth",
     "seventh",
     "eight",
     "ninth",
     "tenth",
     "eleventh",
     "twelfth"
  };

  static char const days_values[][4] = {
     "Sun",
     "Mon",
     "Tue",
     "Wed",
     "Thu",
     "Fri",
     "Sat"
  };

  int len;

  /* Don't add an ordinal prefix if the user didn't specify it
     (e.g., "this wed" vs "wed").  */
  if (pc->debug_ordinal_day_seen)
    {
      /* Use word description if possible (e.g., -1 = last, 3 = third).  */
      len = (-1 <= pc->day_ordinal && pc->day_ordinal <= 12
             ? snprintf (buffer, n, "%s", ordinal_values[pc->day_ordinal + 1])
             : snprintf (buffer, n, "%"PRIdMAX, pc->day_ordinal));
    }
  else
    {
      buffer[0] = '\0';
      len = 0;
    }

  /* Add the day name */
  if (0 <= pc->day_number && pc->day_number <= 6 && 0 <= len && len < n)
    snprintf (buffer + len, n - len, &" %s"[len == 0],
              days_values[pc->day_number]);
  else
    {
      /* invalid day_number value - should never happen */
    }
  return buffer;
}

/* Convert a time zone to its string representation.  */

enum { TIME_ZONE_BUFSIZE = INT_STRLEN_BOUND (intmax_t) + sizeof ":MM:SS" } ;

static char const *
time_zone_str (int time_zone, char time_zone_buf[TIME_ZONE_BUFSIZE])
{
  char *p = time_zone_buf;
  char sign = time_zone < 0 ? '-' : '+';
  int hour = abs (time_zone / (60 * 60));
  p += sprintf (time_zone_buf, "%c%02d", sign, hour);
  int offset_from_hour = abs (time_zone % (60 * 60));
  if (offset_from_hour != 0)
    {
      int mm = offset_from_hour / 60;
      int ss = offset_from_hour % 60;
      *p++ = ':';
      *p++ = '0' + mm / 10;
      *p++ = '0' + mm % 10;
      if (ss)
        {
          *p++ = ':';
          *p++ = '0' + ss / 10;
          *p++ = '0' + ss % 10;
        }
      *p = '\0';
    }
  return time_zone_buf;
}

/* debugging: print the current time in the parser_control structure.
   The parser will increment "*_seen" members for those which were parsed.
   This function will print only newly seen parts.  */
static void
debug_print_current_time (char const *item, parser_control *pc)
{
  bool space = false;

  if (!debugging (pc))
    return;

  /* no newline, more items printed below */
  dbg_printf (_("parsed %s part: "), item);

  if (pc->dates_seen && !pc->debug_dates_seen)
    {
      /*TODO: use pc->year.negative?  */
      fprintf (stderr, "(Y-M-D) %04"PRIdMAX"-%02"PRIdMAX"-%02"PRIdMAX,
              pc->year.value, pc->month, pc->day);
      pc->debug_dates_seen = true;
      space = true;
    }

  if (pc->year_seen != pc->debug_year_seen)
    {
      if (space)
        fputc (' ', stderr);
      fprintf (stderr, _("year: %04"PRIdMAX), pc->year.value);

      pc->debug_year_seen = pc->year_seen;
      space = true;
    }

  if (pc->times_seen && !pc->debug_times_seen)
    {
      intmax_t sec = pc->seconds.tv_sec;
      fprintf (stderr, &" %02"PRIdMAX":%02"PRIdMAX":%02"PRIdMAX[!space],
               pc->hour, pc->minutes, sec);
      if (pc->seconds.tv_nsec != 0)
        {
          int nsec = pc->seconds.tv_nsec;
          fprintf (stderr, ".%09d", nsec);
        }
      if (pc->meridian == MERpm)
        fputs ("pm", stderr);

      pc->debug_times_seen = true;
      space = true;
    }

  if (pc->days_seen && !pc->debug_days_seen)
    {
      if (space)
        fputc (' ', stderr);
      char tmp[DBGBUFSIZE];
      fprintf (stderr, _("%s (day ordinal=%"PRIdMAX" number=%d)"),
               str_days (pc, tmp, sizeof tmp),
               pc->day_ordinal, pc->day_number);
      pc->debug_days_seen = true;
      space = true;
    }

  /* local zone strings only change the DST settings,
     not the timezone value.  If seen, inform about the DST.  */
  if (pc->local_zones_seen && !pc->debug_local_zones_seen)
    {
      fprintf (stderr, &" isdst=%d%s"[!space],
               pc->local_isdst, pc->dsts_seen ? " DST" : "");
      pc->debug_local_zones_seen = true;
      space = true;
    }

  if (pc->zones_seen && !pc->debug_zones_seen)
    {
      char time_zone_buf[TIME_ZONE_BUFSIZE];
      fprintf (stderr, &" UTC%s"[!space],
               time_zone_str (pc->time_zone, time_zone_buf));
      pc->debug_zones_seen = true;
      space = true;
    }

  if (pc->timespec_seen)
    {
      intmax_t sec = pc->seconds.tv_sec;
      if (space)
        fputc (' ', stderr);
      fprintf (stderr, _("number of seconds: %"PRIdMAX), sec);
    }

  fputc ('\n', stderr);
}

/* Debugging: print the current relative values.  */

static bool
print_rel_part (bool space, intmax_t val, char const *name)
{
  if (val == 0)
    return space;
  fprintf (stderr, &" %+"PRIdMAX" %s"[!space], val, name);
  return true;
}

static void
debug_print_relative_time (char const *item, parser_control const *pc)
{
  bool space = false;

  if (!debugging (pc))
    return;

  /* no newline, more items printed below */
  dbg_printf (_("parsed %s part: "), item);

  if (pc->rel.year == 0 && pc->rel.month == 0 && pc->rel.day == 0
      && pc->rel.hour == 0 && pc->rel.minutes == 0 && pc->rel.seconds == 0
      && pc->rel.ns == 0)
    {
      /* Special case: relative time of this/today/now */
      fputs (_("today/this/now\n"), stderr);
      return;
    }

  space = print_rel_part (space, pc->rel.year, "year(s)");
  space = print_rel_part (space, pc->rel.month, "month(s)");
  space = print_rel_part (space, pc->rel.day, "day(s)");
  space = print_rel_part (space, pc->rel.hour, "hour(s)");
  space = print_rel_part (space, pc->rel.minutes, "minutes");
  space = print_rel_part (space, pc->rel.seconds, "seconds");
  print_rel_part (space, pc->rel.ns, "nanoseconds");

  fputc ('\n', stderr);
}




#line 620 "parse-datetime.c" /* yacc.c:339  */

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
   by #include "parse-datetime-gen.h".  */
#ifndef YY_YY_PARSE_DATETIME_TAB_H_INCLUDED
# define YY_YY_PARSE_DATETIME_TAB_H_INCLUDED
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
    tAGO = 258,
    tDST = 259,
    tYEAR_UNIT = 260,
    tMONTH_UNIT = 261,
    tHOUR_UNIT = 262,
    tMINUTE_UNIT = 263,
    tSEC_UNIT = 264,
    tDAY_UNIT = 265,
    tDAY_SHIFT = 266,
    tDAY = 267,
    tDAYZONE = 268,
    tLOCAL_ZONE = 269,
    tMERIDIAN = 270,
    tMONTH = 271,
    tORDINAL = 272,
    tZONE = 273,
    tSNUMBER = 274,
    tUNUMBER = 275,
    tSDECIMAL_NUMBER = 276,
    tUDECIMAL_NUMBER = 277
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 565 "parse-datetime.y" /* yacc.c:355  */

  intmax_t intval;
  textint textintval;
  struct timespec timespec;
  relative_time rel;

#line 690 "parse-datetime.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (parser_control *pc);

#endif /* !YY_YY_PARSE_DATETIME_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 706 "parse-datetime.c" /* yacc.c:358  */

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
#define YYFINAL  12
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   114

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  29
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  26
/* YYNRULES -- Number of rules.  */
#define YYNRULES  92
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  115

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   277

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
       2,     2,     2,     2,    27,     2,     2,    28,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    26,     2,
       2,     2,     2,     2,    23,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    24,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    25,     2,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   592,   592,   593,   597,   605,   607,   611,   616,   621,
     626,   631,   636,   641,   646,   650,   654,   661,   665,   669,
     674,   679,   684,   688,   693,   698,   705,   707,   711,   736,
     738,   748,   750,   752,   757,   762,   765,   767,   772,   777,
     782,   788,   797,   802,   835,   843,   851,   856,   862,   867,
     873,   877,   887,   889,   891,   896,   898,   900,   902,   904,
     906,   908,   911,   914,   916,   918,   920,   922,   924,   926,
     928,   930,   932,   934,   936,   938,   942,   944,   946,   949,
     951,   953,   958,   962,   962,   965,   966,   972,   973,   979,
     984,   995,   996
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "tAGO", "tDST", "tYEAR_UNIT",
  "tMONTH_UNIT", "tHOUR_UNIT", "tMINUTE_UNIT", "tSEC_UNIT", "tDAY_UNIT",
  "tDAY_SHIFT", "tDAY", "tDAYZONE", "tLOCAL_ZONE", "tMERIDIAN", "tMONTH",
  "tORDINAL", "tZONE", "tSNUMBER", "tUNUMBER", "tSDECIMAL_NUMBER",
  "tUDECIMAL_NUMBER", "'@'", "'J'", "'T'", "':'", "','", "'/'", "$accept",
  "spec", "timespec", "items", "item", "datetime", "iso_8601_datetime",
  "time", "iso_8601_time", "o_zone_offset", "zone_offset", "local_zone",
  "zone", "day", "date", "iso_8601_date", "rel", "relunit",
  "relunit_snumber", "dayshift", "seconds", "signed_seconds",
  "unsigned_seconds", "number", "hybrid", "o_colon_minutes", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,    64,    74,    84,    58,    44,    47
};
# endif

#define YYPACT_NINF -91

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-91)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -14,     7,    39,   -91,    37,   -91,   -91,   -91,   -91,   -91,
     -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,
      14,   -91,    64,    47,    67,     6,    82,    -4,    74,    75,
     -91,    76,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,
     -91,    69,   -91,    93,   -91,   -91,   -91,   -91,   -91,   -91,
      79,    72,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,
      26,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,
     -91,   -91,   -91,   -91,   -91,   -91,    62,    11,    80,    81,
     -91,   -91,   -91,   -91,   -91,    83,   -91,   -91,    84,    85,
     -91,   -91,   -91,   -91,   -91,    45,    86,   -12,   -91,   -91,
     -91,   -91,    87,    18,   -91,   -91,    88,    89,    78,   -91,
      59,   -91,   -91,    18,    91
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     0,     0,     2,     3,    86,    88,    85,    87,     4,
      83,    84,     1,    57,    60,    66,    69,    74,    63,    82,
      38,    36,    29,     0,     0,    31,     0,    89,     0,     0,
      10,    32,     6,     7,    17,     8,    22,     9,    11,    13,
      12,    50,    14,    53,    75,    54,    15,    16,    39,    30,
       0,    46,    55,    58,    64,    67,    70,    61,    40,    37,
      91,    33,    76,    77,    79,    80,    81,    78,    56,    59,
      65,    68,    71,    62,    41,    19,    48,    91,     0,     0,
      23,    90,    72,    73,    34,     0,    52,    45,     0,     0,
      35,    44,    49,    51,    28,    26,    42,     0,    18,    47,
      92,    20,    91,     0,    24,    27,     0,     0,    26,    43,
      26,    21,    25,     0,    26
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,    17,   -28,
     -27,   -91,   -91,   -91,   -91,   -91,   -91,   -91,    38,   -91,
     -91,   -91,   -90,   -91,   -91,    46
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     2,     3,     4,    32,    33,    34,    35,    36,   104,
     105,    37,    38,    39,    40,    41,    42,    43,    44,    45,
       9,    10,    11,    46,    47,    94
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      80,    68,    69,    70,    71,    72,    73,   102,    74,     1,
      59,    75,    76,   108,   107,    77,    62,    63,    64,    65,
      66,    67,    78,   114,    79,    60,     5,     6,     7,     8,
      93,    62,    63,    64,    65,    66,    67,    89,     6,    12,
       8,    48,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    89,    23,    24,    25,    26,    27,    28,    29,
     101,    30,    31,    61,   102,    81,    50,    51,    49,    84,
      80,   103,    52,    53,    54,    55,    56,    57,   102,    58,
     112,    91,    92,    82,    83,   113,   112,    62,    63,    64,
      65,    66,    67,   111,    85,    26,    86,   102,    87,    88,
      95,    96,    98,    97,    99,   100,    90,     0,   109,   110,
     102,     0,     0,    89,   106
};

static const yytype_int8 yycheck[] =
{
      27,     5,     6,     7,     8,     9,    10,    19,    12,    23,
       4,    15,    16,   103,    26,    19,     5,     6,     7,     8,
       9,    10,    26,   113,    28,    19,    19,    20,    21,    22,
      19,     5,     6,     7,     8,     9,    10,    26,    20,     0,
      22,    27,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    26,    16,    17,    18,    19,    20,    21,    22,
      15,    24,    25,    25,    19,    27,    19,    20,     4,    31,
      97,    26,     5,     6,     7,     8,     9,    10,    19,    12,
     108,    19,    20,     9,     9,    26,   114,     5,     6,     7,
       8,     9,    10,    15,    25,    19,     3,    19,    19,    27,
      20,    20,    85,    20,    20,    20,    60,    -1,    20,    20,
      19,    -1,    -1,    26,    28
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    23,    30,    31,    32,    19,    20,    21,    22,    49,
      50,    51,     0,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    16,    17,    18,    19,    20,    21,    22,
      24,    25,    33,    34,    35,    36,    37,    40,    41,    42,
      43,    44,    45,    46,    47,    48,    52,    53,    27,     4,
      19,    20,     5,     6,     7,     8,     9,    10,    12,     4,
      19,    47,     5,     6,     7,     8,     9,    10,     5,     6,
       7,     8,     9,    10,    12,    15,    16,    19,    26,    28,
      39,    47,     9,     9,    47,    25,     3,    19,    27,    26,
      54,    19,    20,    19,    54,    20,    20,    20,    37,    20,
      20,    15,    19,    26,    38,    39,    28,    26,    51,    20,
      20,    15,    38,    26,    51
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    29,    30,    30,    31,    32,    32,    33,    33,    33,
      33,    33,    33,    33,    33,    33,    33,    34,    35,    36,
      36,    36,    36,    37,    37,    37,    38,    38,    39,    40,
      40,    41,    41,    41,    41,    41,    41,    41,    42,    42,
      42,    42,    43,    43,    43,    43,    43,    43,    43,    43,
      43,    44,    45,    45,    45,    46,    46,    46,    46,    46,
      46,    46,    46,    46,    46,    46,    46,    46,    46,    46,
      46,    46,    46,    46,    46,    46,    47,    47,    47,    47,
      47,    47,    48,    49,    49,    50,    50,    51,    51,    52,
      53,    54,    54
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     1,     2,     0,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     2,
       4,     6,     1,     2,     4,     6,     0,     1,     2,     1,
       2,     1,     1,     2,     2,     3,     1,     2,     1,     2,
       2,     2,     3,     5,     3,     3,     2,     4,     2,     3,
       1,     3,     2,     1,     1,     2,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       2,     2,     2,     2,     1,     1,     2,     2,     2,     2,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     0,     2
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
      yyerror (pc, YY_("syntax error: cannot back up")); \
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
                  Type, Value, pc); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, parser_control *pc)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (pc);
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
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, parser_control *pc)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, pc);
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
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule, parser_control *pc)
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
                                              , pc);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule, pc); \
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
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, parser_control *pc)
{
  YYUSE (yyvaluep);
  YYUSE (pc);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/*----------.
| yyparse.  |
`----------*/

int
yyparse (parser_control *pc)
{
/* The lookahead symbol.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

    /* Number of syntax errors so far.  */
    int yynerrs;

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
      yychar = yylex (&yylval, pc);
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
#line 598 "parse-datetime.y" /* yacc.c:1646  */
    {
        pc->seconds = (yyvsp[0].timespec);
        pc->timespec_seen = true;
        debug_print_current_time (_("number of seconds"), pc);
      }
#line 1879 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 7:
#line 612 "parse-datetime.y" /* yacc.c:1646  */
    {
        pc->times_seen++; pc->dates_seen++;
        debug_print_current_time (_("datetime"), pc);
      }
#line 1888 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 8:
#line 617 "parse-datetime.y" /* yacc.c:1646  */
    {
        pc->times_seen++;
        debug_print_current_time (_("time"), pc);
      }
#line 1897 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 9:
#line 622 "parse-datetime.y" /* yacc.c:1646  */
    {
        pc->local_zones_seen++;
        debug_print_current_time (_("local_zone"), pc);
      }
#line 1906 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 10:
#line 627 "parse-datetime.y" /* yacc.c:1646  */
    {
        pc->J_zones_seen++;
        debug_print_current_time ("J", pc);
      }
#line 1915 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 11:
#line 632 "parse-datetime.y" /* yacc.c:1646  */
    {
        pc->zones_seen++;
        debug_print_current_time (_("zone"), pc);
      }
#line 1924 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 12:
#line 637 "parse-datetime.y" /* yacc.c:1646  */
    {
        pc->dates_seen++;
        debug_print_current_time (_("date"), pc);
      }
#line 1933 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 13:
#line 642 "parse-datetime.y" /* yacc.c:1646  */
    {
        pc->days_seen++;
        debug_print_current_time (_("day"), pc);
      }
#line 1942 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 14:
#line 647 "parse-datetime.y" /* yacc.c:1646  */
    {
        debug_print_relative_time (_("relative"), pc);
      }
#line 1950 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 15:
#line 651 "parse-datetime.y" /* yacc.c:1646  */
    {
        debug_print_current_time (_("number"), pc);
      }
#line 1958 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 16:
#line 655 "parse-datetime.y" /* yacc.c:1646  */
    {
        debug_print_relative_time (_("hybrid"), pc);
      }
#line 1966 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 19:
#line 670 "parse-datetime.y" /* yacc.c:1646  */
    {
        set_hhmmss (pc, (yyvsp[-1].textintval).value, 0, 0, 0);
        pc->meridian = (yyvsp[0].intval);
      }
#line 1975 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 20:
#line 675 "parse-datetime.y" /* yacc.c:1646  */
    {
        set_hhmmss (pc, (yyvsp[-3].textintval).value, (yyvsp[-1].textintval).value, 0, 0);
        pc->meridian = (yyvsp[0].intval);
      }
#line 1984 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 21:
#line 680 "parse-datetime.y" /* yacc.c:1646  */
    {
        set_hhmmss (pc, (yyvsp[-5].textintval).value, (yyvsp[-3].textintval).value, (yyvsp[-1].timespec).tv_sec, (yyvsp[-1].timespec).tv_nsec);
        pc->meridian = (yyvsp[0].intval);
      }
#line 1993 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 23:
#line 689 "parse-datetime.y" /* yacc.c:1646  */
    {
        set_hhmmss (pc, (yyvsp[-1].textintval).value, 0, 0, 0);
        pc->meridian = MER24;
      }
#line 2002 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 24:
#line 694 "parse-datetime.y" /* yacc.c:1646  */
    {
        set_hhmmss (pc, (yyvsp[-3].textintval).value, (yyvsp[-1].textintval).value, 0, 0);
        pc->meridian = MER24;
      }
#line 2011 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 25:
#line 699 "parse-datetime.y" /* yacc.c:1646  */
    {
        set_hhmmss (pc, (yyvsp[-5].textintval).value, (yyvsp[-3].textintval).value, (yyvsp[-1].timespec).tv_sec, (yyvsp[-1].timespec).tv_nsec);
        pc->meridian = MER24;
      }
#line 2020 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 28:
#line 712 "parse-datetime.y" /* yacc.c:1646  */
    {
        pc->zones_seen++;
        if (! time_zone_hhmm (pc, (yyvsp[-1].textintval), (yyvsp[0].intval))) YYABORT;
      }
#line 2029 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 29:
#line 737 "parse-datetime.y" /* yacc.c:1646  */
    { pc->local_isdst = (yyvsp[0].intval); }
#line 2035 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 30:
#line 739 "parse-datetime.y" /* yacc.c:1646  */
    {
        pc->local_isdst = 1;
        pc->dsts_seen++;
      }
#line 2044 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 31:
#line 749 "parse-datetime.y" /* yacc.c:1646  */
    { pc->time_zone = (yyvsp[0].intval); }
#line 2050 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 32:
#line 751 "parse-datetime.y" /* yacc.c:1646  */
    { pc->time_zone = -HOUR (7); }
#line 2056 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 33:
#line 753 "parse-datetime.y" /* yacc.c:1646  */
    { pc->time_zone = (yyvsp[-1].intval);
        if (! apply_relative_time (pc, (yyvsp[0].rel), 1)) YYABORT;
        debug_print_relative_time (_("relative"), pc);
      }
#line 2065 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 34:
#line 758 "parse-datetime.y" /* yacc.c:1646  */
    { pc->time_zone = -HOUR (7);
        if (! apply_relative_time (pc, (yyvsp[0].rel), 1)) YYABORT;
        debug_print_relative_time (_("relative"), pc);
      }
#line 2074 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 35:
#line 763 "parse-datetime.y" /* yacc.c:1646  */
    { if (! time_zone_hhmm (pc, (yyvsp[-1].textintval), (yyvsp[0].intval))) YYABORT;
        if (ckd_add (&pc->time_zone, pc->time_zone, (yyvsp[-2].intval))) YYABORT; }
#line 2081 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 36:
#line 766 "parse-datetime.y" /* yacc.c:1646  */
    { pc->time_zone = (yyvsp[0].intval) + 60 * 60; }
#line 2087 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 37:
#line 768 "parse-datetime.y" /* yacc.c:1646  */
    { pc->time_zone = (yyvsp[-1].intval) + 60 * 60; }
#line 2093 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 38:
#line 773 "parse-datetime.y" /* yacc.c:1646  */
    {
        pc->day_ordinal = 0;
        pc->day_number = (yyvsp[0].intval);
      }
#line 2102 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 39:
#line 778 "parse-datetime.y" /* yacc.c:1646  */
    {
        pc->day_ordinal = 0;
        pc->day_number = (yyvsp[-1].intval);
      }
#line 2111 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 40:
#line 783 "parse-datetime.y" /* yacc.c:1646  */
    {
        pc->day_ordinal = (yyvsp[-1].intval);
        pc->day_number = (yyvsp[0].intval);
        pc->debug_ordinal_day_seen = true;
      }
#line 2121 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 41:
#line 789 "parse-datetime.y" /* yacc.c:1646  */
    {
        pc->day_ordinal = (yyvsp[-1].textintval).value;
        pc->day_number = (yyvsp[0].intval);
        pc->debug_ordinal_day_seen = true;
      }
#line 2131 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 42:
#line 798 "parse-datetime.y" /* yacc.c:1646  */
    {
        pc->month = (yyvsp[-2].textintval).value;
        pc->day = (yyvsp[0].textintval).value;
      }
#line 2140 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 43:
#line 803 "parse-datetime.y" /* yacc.c:1646  */
    {
        /* Interpret as YYYY/MM/DD if the first value has 4 or more digits,
           otherwise as MM/DD/YY.
           The goal in recognizing YYYY/MM/DD is solely to support legacy
           machine-generated dates like those in an RCS log listing.  If
           you want portability, use the ISO 8601 format.  */
        if (4 <= (yyvsp[-4].textintval).digits)
          {
            if (debugging (pc))
              {
                intmax_t digits = (yyvsp[-4].textintval).digits;
                dbg_printf (_("warning: value %"PRIdMAX" has %"PRIdMAX" digits. "
                              "Assuming YYYY/MM/DD\n"),
                            (yyvsp[-4].textintval).value, digits);
              }

            pc->year = (yyvsp[-4].textintval);
            pc->month = (yyvsp[-2].textintval).value;
            pc->day = (yyvsp[0].textintval).value;
          }
        else
          {
            if (debugging (pc))
              dbg_printf (_("warning: value %"PRIdMAX" has less than 4 digits. "
                            "Assuming MM/DD/YY[YY]\n"),
                          (yyvsp[-4].textintval).value);

            pc->month = (yyvsp[-4].textintval).value;
            pc->day = (yyvsp[-2].textintval).value;
            pc->year = (yyvsp[0].textintval);
          }
      }
#line 2177 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 44:
#line 836 "parse-datetime.y" /* yacc.c:1646  */
    {
        /* E.g., 17-JUN-1992.  */
        pc->day = (yyvsp[-2].textintval).value;
        pc->month = (yyvsp[-1].intval);
        if (ckd_sub (&pc->year.value, 0, (yyvsp[0].textintval).value)) YYABORT;
        pc->year.digits = (yyvsp[0].textintval).digits;
      }
#line 2189 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 45:
#line 844 "parse-datetime.y" /* yacc.c:1646  */
    {
        /* E.g., JUN-17-1992.  */
        pc->month = (yyvsp[-2].intval);
        if (ckd_sub (&pc->day, 0, (yyvsp[-1].textintval).value)) YYABORT;
        if (ckd_sub (&pc->year.value, 0, (yyvsp[0].textintval).value)) YYABORT;
        pc->year.digits = (yyvsp[0].textintval).digits;
      }
#line 2201 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 46:
#line 852 "parse-datetime.y" /* yacc.c:1646  */
    {
        pc->month = (yyvsp[-1].intval);
        pc->day = (yyvsp[0].textintval).value;
      }
#line 2210 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 47:
#line 857 "parse-datetime.y" /* yacc.c:1646  */
    {
        pc->month = (yyvsp[-3].intval);
        pc->day = (yyvsp[-2].textintval).value;
        pc->year = (yyvsp[0].textintval);
      }
#line 2220 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 48:
#line 863 "parse-datetime.y" /* yacc.c:1646  */
    {
        pc->day = (yyvsp[-1].textintval).value;
        pc->month = (yyvsp[0].intval);
      }
#line 2229 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 49:
#line 868 "parse-datetime.y" /* yacc.c:1646  */
    {
        pc->day = (yyvsp[-2].textintval).value;
        pc->month = (yyvsp[-1].intval);
        pc->year = (yyvsp[0].textintval);
      }
#line 2239 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 51:
#line 878 "parse-datetime.y" /* yacc.c:1646  */
    {
        /* ISO 8601 format.  YYYY-MM-DD.  */
        pc->year = (yyvsp[-2].textintval);
        if (ckd_sub (&pc->month, 0, (yyvsp[-1].textintval).value)) YYABORT;
        if (ckd_sub (&pc->day, 0, (yyvsp[0].textintval).value)) YYABORT;
      }
#line 2250 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 52:
#line 888 "parse-datetime.y" /* yacc.c:1646  */
    { if (! apply_relative_time (pc, (yyvsp[-1].rel), (yyvsp[0].intval))) YYABORT; }
#line 2256 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 53:
#line 890 "parse-datetime.y" /* yacc.c:1646  */
    { if (! apply_relative_time (pc, (yyvsp[0].rel), 1)) YYABORT; }
#line 2262 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 54:
#line 892 "parse-datetime.y" /* yacc.c:1646  */
    { if (! apply_relative_time (pc, (yyvsp[0].rel), 1)) YYABORT; }
#line 2268 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 55:
#line 897 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).year = (yyvsp[-1].intval); }
#line 2274 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 56:
#line 899 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).year = (yyvsp[-1].textintval).value; }
#line 2280 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 57:
#line 901 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).year = 1; }
#line 2286 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 58:
#line 903 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).month = (yyvsp[-1].intval); }
#line 2292 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 59:
#line 905 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).month = (yyvsp[-1].textintval).value; }
#line 2298 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 60:
#line 907 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).month = 1; }
#line 2304 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 61:
#line 909 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0;
        if (ckd_mul (&(yyval.rel).day, (yyvsp[-1].intval), (yyvsp[0].intval))) YYABORT; }
#line 2311 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 62:
#line 912 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0;
        if (ckd_mul (&(yyval.rel).day, (yyvsp[-1].textintval).value, (yyvsp[0].intval))) YYABORT; }
#line 2318 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 63:
#line 915 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).day = (yyvsp[0].intval); }
#line 2324 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 64:
#line 917 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).hour = (yyvsp[-1].intval); }
#line 2330 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 65:
#line 919 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).hour = (yyvsp[-1].textintval).value; }
#line 2336 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 66:
#line 921 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).hour = 1; }
#line 2342 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 67:
#line 923 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).minutes = (yyvsp[-1].intval); }
#line 2348 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 68:
#line 925 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).minutes = (yyvsp[-1].textintval).value; }
#line 2354 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 69:
#line 927 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).minutes = 1; }
#line 2360 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 70:
#line 929 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).seconds = (yyvsp[-1].intval); }
#line 2366 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 71:
#line 931 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).seconds = (yyvsp[-1].textintval).value; }
#line 2372 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 72:
#line 933 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).seconds = (yyvsp[-1].timespec).tv_sec; (yyval.rel).ns = (yyvsp[-1].timespec).tv_nsec; }
#line 2378 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 73:
#line 935 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).seconds = (yyvsp[-1].timespec).tv_sec; (yyval.rel).ns = (yyvsp[-1].timespec).tv_nsec; }
#line 2384 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 74:
#line 937 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).seconds = 1; }
#line 2390 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 76:
#line 943 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).year = (yyvsp[-1].textintval).value; }
#line 2396 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 77:
#line 945 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).month = (yyvsp[-1].textintval).value; }
#line 2402 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 78:
#line 947 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0;
        if (ckd_mul (&(yyval.rel).day, (yyvsp[-1].textintval).value, (yyvsp[0].intval))) YYABORT; }
#line 2409 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 79:
#line 950 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).hour = (yyvsp[-1].textintval).value; }
#line 2415 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 80:
#line 952 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).minutes = (yyvsp[-1].textintval).value; }
#line 2421 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 81:
#line 954 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).seconds = (yyvsp[-1].textintval).value; }
#line 2427 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 82:
#line 959 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.rel) = RELATIVE_TIME_0; (yyval.rel).day = (yyvsp[0].intval); }
#line 2433 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 86:
#line 967 "parse-datetime.y" /* yacc.c:1646  */
    { if (time_overflow ((yyvsp[0].textintval).value)) YYABORT;
        (yyval.timespec) = (struct timespec) { .tv_sec = (yyvsp[0].textintval).value }; }
#line 2440 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 88:
#line 974 "parse-datetime.y" /* yacc.c:1646  */
    { if (time_overflow ((yyvsp[0].textintval).value)) YYABORT;
        (yyval.timespec) = (struct timespec) { .tv_sec = (yyvsp[0].textintval).value }; }
#line 2447 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 89:
#line 980 "parse-datetime.y" /* yacc.c:1646  */
    { digits_to_date_time (pc, (yyvsp[0].textintval)); }
#line 2453 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 90:
#line 985 "parse-datetime.y" /* yacc.c:1646  */
    {
        /* Hybrid all-digit and relative offset, so that we accept e.g.,
           "YYYYMMDD +N days" as well as "YYYYMMDD N days".  */
        digits_to_date_time (pc, (yyvsp[-1].textintval));
        if (! apply_relative_time (pc, (yyvsp[0].rel), 1)) YYABORT;
      }
#line 2464 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 91:
#line 995 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.intval) = -1; }
#line 2470 "parse-datetime.c" /* yacc.c:1646  */
    break;

  case 92:
#line 997 "parse-datetime.y" /* yacc.c:1646  */
    { (yyval.intval) = (yyvsp[0].textintval).value; }
#line 2476 "parse-datetime.c" /* yacc.c:1646  */
    break;


#line 2480 "parse-datetime.c" /* yacc.c:1646  */
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
      yyerror (pc, YY_("syntax error"));
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
        yyerror (pc, yymsgp);
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
                      yytoken, &yylval, pc);
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
                  yystos[yystate], yyvsp, pc);
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
  yyerror (pc, YY_("memory exhausted"));
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
                  yytoken, &yylval, pc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, pc);
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
#line 1000 "parse-datetime.y" /* yacc.c:1906  */


static table const meridian_table[] =
{
  { "AM",   tMERIDIAN, MERam },
  { "A.M.", tMERIDIAN, MERam },
  { "PM",   tMERIDIAN, MERpm },
  { "P.M.", tMERIDIAN, MERpm },
  { NULL, 0, 0 }
};

static table const dst_table[] =
{
  { "DST", tDST, 0 }
};

static table const month_and_day_table[] =
{
  { "JANUARY",  tMONTH,  1 },
  { "FEBRUARY", tMONTH,  2 },
  { "MARCH",    tMONTH,  3 },
  { "APRIL",    tMONTH,  4 },
  { "MAY",      tMONTH,  5 },
  { "JUNE",     tMONTH,  6 },
  { "JULY",     tMONTH,  7 },
  { "AUGUST",   tMONTH,  8 },
  { "SEPTEMBER",tMONTH,  9 },
  { "SEPT",     tMONTH,  9 },
  { "OCTOBER",  tMONTH, 10 },
  { "NOVEMBER", tMONTH, 11 },
  { "DECEMBER", tMONTH, 12 },
  { "SUNDAY",   tDAY,    0 },
  { "MONDAY",   tDAY,    1 },
  { "TUESDAY",  tDAY,    2 },
  { "TUES",     tDAY,    2 },
  { "WEDNESDAY",tDAY,    3 },
  { "WEDNES",   tDAY,    3 },
  { "THURSDAY", tDAY,    4 },
  { "THUR",     tDAY,    4 },
  { "THURS",    tDAY,    4 },
  { "FRIDAY",   tDAY,    5 },
  { "SATURDAY", tDAY,    6 },
  { NULL, 0, 0 }
};

static table const time_units_table[] =
{
  { "YEAR",     tYEAR_UNIT,      1 },
  { "MONTH",    tMONTH_UNIT,     1 },
  { "FORTNIGHT",tDAY_UNIT,      14 },
  { "WEEK",     tDAY_UNIT,       7 },
  { "DAY",      tDAY_UNIT,       1 },
  { "HOUR",     tHOUR_UNIT,      1 },
  { "MINUTE",   tMINUTE_UNIT,    1 },
  { "MIN",      tMINUTE_UNIT,    1 },
  { "SECOND",   tSEC_UNIT,       1 },
  { "SEC",      tSEC_UNIT,       1 },
  { NULL, 0, 0 }
};

/* Assorted relative-time words.  */
static table const relative_time_table[] =
{
  { "TOMORROW", tDAY_SHIFT,      1 },
  { "YESTERDAY",tDAY_SHIFT,     -1 },
  { "TODAY",    tDAY_SHIFT,      0 },
  { "NOW",      tDAY_SHIFT,      0 },
  { "LAST",     tORDINAL,       -1 },
  { "THIS",     tORDINAL,        0 },
  { "NEXT",     tORDINAL,        1 },
  { "FIRST",    tORDINAL,        1 },
/*{ "SECOND",   tORDINAL,        2 }, */
  { "THIRD",    tORDINAL,        3 },
  { "FOURTH",   tORDINAL,        4 },
  { "FIFTH",    tORDINAL,        5 },
  { "SIXTH",    tORDINAL,        6 },
  { "SEVENTH",  tORDINAL,        7 },
  { "EIGHTH",   tORDINAL,        8 },
  { "NINTH",    tORDINAL,        9 },
  { "TENTH",    tORDINAL,       10 },
  { "ELEVENTH", tORDINAL,       11 },
  { "TWELFTH",  tORDINAL,       12 },
  { "AGO",      tAGO,           -1 },
  { "HENCE",    tAGO,            1 },
  { NULL, 0, 0 }
};

/* The universal time zone table.  These labels can be used even for
   timestamps that would not otherwise be valid, e.g., GMT timestamps
   oin London during summer.  */
static table const universal_time_zone_table[] =
{
  { "GMT",      tZONE,     HOUR ( 0) }, /* Greenwich Mean */
  { "UT",       tZONE,     HOUR ( 0) }, /* Universal (Coordinated) */
  { "UTC",      tZONE,     HOUR ( 0) },
  { NULL, 0, 0 }
};

/* The time zone table.  This table is necessarily incomplete, as time
   zone abbreviations are ambiguous; e.g., Australians interpret "EST"
   as Eastern time in Australia, not as US Eastern Standard Time.
   You cannot rely on parse_datetime to handle arbitrary time zone
   abbreviations; use numeric abbreviations like "-0500" instead.  */
static table const time_zone_table[] =
{
  { "WET",      tZONE,     HOUR ( 0) }, /* Western European */
  { "WEST",     tDAYZONE,  HOUR ( 0) }, /* Western European Summer */
  { "BST",      tDAYZONE,  HOUR ( 0) }, /* British Summer */
  { "ART",      tZONE,    -HOUR ( 3) }, /* Argentina */
  { "BRT",      tZONE,    -HOUR ( 3) }, /* Brazil */
  { "BRST",     tDAYZONE, -HOUR ( 3) }, /* Brazil Summer */
  { "NST",      tZONE,   -(HOUR ( 3) + 30 * 60) }, /* Newfoundland Standard */
  { "NDT",      tDAYZONE,-(HOUR ( 3) + 30 * 60) }, /* Newfoundland Daylight */
  { "AST",      tZONE,    -HOUR ( 4) }, /* Atlantic Standard */
  { "ADT",      tDAYZONE, -HOUR ( 4) }, /* Atlantic Daylight */
  { "CLT",      tZONE,    -HOUR ( 4) }, /* Chile */
  { "CLST",     tDAYZONE, -HOUR ( 4) }, /* Chile Summer */
  { "EST",      tZONE,    -HOUR ( 5) }, /* Eastern Standard */
  { "EDT",      tDAYZONE, -HOUR ( 5) }, /* Eastern Daylight */
  { "CST",      tZONE,    -HOUR ( 6) }, /* Central Standard */
  { "CDT",      tDAYZONE, -HOUR ( 6) }, /* Central Daylight */
  { "MST",      tZONE,    -HOUR ( 7) }, /* Mountain Standard */
  { "MDT",      tDAYZONE, -HOUR ( 7) }, /* Mountain Daylight */
  { "PST",      tZONE,    -HOUR ( 8) }, /* Pacific Standard */
  { "PDT",      tDAYZONE, -HOUR ( 8) }, /* Pacific Daylight */
  { "AKST",     tZONE,    -HOUR ( 9) }, /* Alaska Standard */
  { "AKDT",     tDAYZONE, -HOUR ( 9) }, /* Alaska Daylight */
  { "HST",      tZONE,    -HOUR (10) }, /* Hawaii Standard */
  { "HAST",     tZONE,    -HOUR (10) }, /* Hawaii-Aleutian Standard */
  { "HADT",     tDAYZONE, -HOUR (10) }, /* Hawaii-Aleutian Daylight */
  { "SST",      tZONE,    -HOUR (12) }, /* Samoa Standard */
  { "WAT",      tZONE,     HOUR ( 1) }, /* West Africa */
  { "CET",      tZONE,     HOUR ( 1) }, /* Central European */
  { "CEST",     tDAYZONE,  HOUR ( 1) }, /* Central European Summer */
  { "MET",      tZONE,     HOUR ( 1) }, /* Middle European */
  { "MEZ",      tZONE,     HOUR ( 1) }, /* Middle European */
  { "MEST",     tDAYZONE,  HOUR ( 1) }, /* Middle European Summer */
  { "MESZ",     tDAYZONE,  HOUR ( 1) }, /* Middle European Summer */
  { "EET",      tZONE,     HOUR ( 2) }, /* Eastern European */
  { "EEST",     tDAYZONE,  HOUR ( 2) }, /* Eastern European Summer */
  { "CAT",      tZONE,     HOUR ( 2) }, /* Central Africa */
  { "SAST",     tZONE,     HOUR ( 2) }, /* South Africa Standard */
  { "EAT",      tZONE,     HOUR ( 3) }, /* East Africa */
  { "MSK",      tZONE,     HOUR ( 3) }, /* Moscow */
  { "MSD",      tDAYZONE,  HOUR ( 3) }, /* Moscow Daylight */
  { "IST",      tZONE,    (HOUR ( 5) + 30 * 60) }, /* India Standard */
  { "SGT",      tZONE,     HOUR ( 8) }, /* Singapore */
  { "KST",      tZONE,     HOUR ( 9) }, /* Korea Standard */
  { "JST",      tZONE,     HOUR ( 9) }, /* Japan Standard */
  { "GST",      tZONE,     HOUR (10) }, /* Guam Standard */
  { "NZST",     tZONE,     HOUR (12) }, /* New Zealand Standard */
  { "NZDT",     tDAYZONE,  HOUR (12) }, /* New Zealand Daylight */
  { NULL, 0, 0 }
};

/* Military time zone table.

   RFC 822 got these backwards, but RFC 5322 makes the incorrect
   treatment optional, so do them the right way here.

   'J' is special, as it is local time.
   'T' is also special, as it is the separator in ISO
   8601 date and time of day representation.  */
static table const military_table[] =
{
  { "A", tZONE,  HOUR ( 1) },
  { "B", tZONE,  HOUR ( 2) },
  { "C", tZONE,  HOUR ( 3) },
  { "D", tZONE,  HOUR ( 4) },
  { "E", tZONE,  HOUR ( 5) },
  { "F", tZONE,  HOUR ( 6) },
  { "G", tZONE,  HOUR ( 7) },
  { "H", tZONE,  HOUR ( 8) },
  { "I", tZONE,  HOUR ( 9) },
  { "J", 'J',    0 },
  { "K", tZONE,  HOUR (10) },
  { "L", tZONE,  HOUR (11) },
  { "M", tZONE,  HOUR (12) },
  { "N", tZONE, -HOUR ( 1) },
  { "O", tZONE, -HOUR ( 2) },
  { "P", tZONE, -HOUR ( 3) },
  { "Q", tZONE, -HOUR ( 4) },
  { "R", tZONE, -HOUR ( 5) },
  { "S", tZONE, -HOUR ( 6) },
  { "T", 'T',    0 },
  { "U", tZONE, -HOUR ( 8) },
  { "V", tZONE, -HOUR ( 9) },
  { "W", tZONE, -HOUR (10) },
  { "X", tZONE, -HOUR (11) },
  { "Y", tZONE, -HOUR (12) },
  { "Z", tZONE,  HOUR ( 0) },
  { NULL, 0, 0 }
};



/* Convert a time zone expressed as HH:MM into an integer count of
   seconds.  If MM is negative, then S is of the form HHMM and needs
   to be picked apart; otherwise, S is of the form HH.  As specified in
   https://pubs.opengroup.org/onlinepubs/9699919799/basedefs/V1_chap08.html#tag_08_03, allow
   only valid TZ range, and consider first two digits as hours, if no
   minutes specified.  Return true if successful.  */

static bool
time_zone_hhmm (parser_control *pc, textint s, intmax_t mm)
{
  intmax_t n_minutes;
  bool overflow = false;

  /* If the length of S is 1 or 2 and no minutes are specified,
     interpret it as a number of hours.  */
  if (s.digits <= 2 && mm < 0)
    s.value *= 100;

  if (mm < 0)
    n_minutes = (s.value / 100) * 60 + s.value % 100;
  else
    {
      overflow |= ckd_mul (&n_minutes, s.value, 60);
      overflow |= (s.negative
                   ? ckd_sub (&n_minutes, n_minutes, mm)
                   : ckd_add (&n_minutes, n_minutes, mm));
    }

  if (overflow || ! (-24 * 60 <= n_minutes && n_minutes <= 24 * 60))
    return false;
  pc->time_zone = n_minutes * 60;
  return true;
}

static int
to_hour (intmax_t hours, int meridian)
{
  switch (meridian)
    {
    default: /* Pacify GCC.  */
    case MER24:
      return 0 <= hours && hours < 24 ? hours : -1;
    case MERam:
      return 0 < hours && hours < 12 ? hours : hours == 12 ? 0 : -1;
    case MERpm:
      return 0 < hours && hours < 12 ? hours + 12 : hours == 12 ? 12 : -1;
    }
}

enum { TM_YEAR_BASE = 1900 };
enum { TM_YEAR_BUFSIZE = INT_BUFSIZE_BOUND (int) + 1 };

/* Convert TM_YEAR, a year minus 1900, to a string that is numerically
   correct even if subtracting 1900 would overflow.  */

static char const *
tm_year_str (int tm_year, char buf[TM_YEAR_BUFSIZE])
{
  static_assert (TM_YEAR_BASE % 100 == 0);
  sprintf (buf, &"-%02d%02d"[-TM_YEAR_BASE <= tm_year],
           abs (tm_year / 100 + TM_YEAR_BASE / 100),
           abs (tm_year % 100));
  return buf;
}

/* Convert a text year number to a year minus 1900, working correctly
   even if the input is in the range INT_MAX .. INT_MAX + 1900 - 1.  */

static bool
to_tm_year (textint textyear, bool debug, int *tm_year)
{
  intmax_t year = textyear.value;

  /* XPG4 suggests that years 00-68 map to 2000-2068, and
     years 69-99 map to 1969-1999.  */
  if (0 <= year && textyear.digits == 2)
    {
      year += year < 69 ? 2000 : 1900;
      if (debug)
        dbg_printf (_("warning: adjusting year value %"PRIdMAX
                      " to %"PRIdMAX"\n"),
                    textyear.value, year);
    }

  if (year < 0
      ? ckd_sub (tm_year, -TM_YEAR_BASE, year)
      : ckd_sub (tm_year, year, TM_YEAR_BASE))
    {
      if (debug)
        dbg_printf (_("error: out-of-range year %"PRIdMAX"\n"), year);
      return false;
    }

  return true;
}

static table const * _GL_ATTRIBUTE_PURE
lookup_zone (parser_control const *pc, char const *name)
{
  table const *tp;

  for (tp = universal_time_zone_table; tp->name; tp++)
    if (strcmp (name, tp->name) == 0)
      return tp;

  /* Try local zone abbreviations before those in time_zone_table, as
     the local ones are more likely to be right.  */
  for (tp = pc->local_time_zone_table; tp->name; tp++)
    if (strcmp (name, tp->name) == 0)
      return tp;

  for (tp = time_zone_table; tp->name; tp++)
    if (strcmp (name, tp->name) == 0)
      return tp;

  return NULL;
}

#if ! HAVE_TM_GMTOFF
/* Yield the difference between *A and *B,
   measured in seconds, ignoring leap seconds.
   The body of this function is taken directly from the GNU C Library;
   see strftime.c.  */
static int
tm_diff (const struct tm *a, const struct tm *b)
{
  /* Compute intervening leap days correctly even if year is negative.
     Take care to avoid int overflow in leap day calculations,
     but it's OK to assume that A and B are close to each other.  */
  int a4 = SHR (a->tm_year, 2) + SHR (TM_YEAR_BASE, 2) - ! (a->tm_year & 3);
  int b4 = SHR (b->tm_year, 2) + SHR (TM_YEAR_BASE, 2) - ! (b->tm_year & 3);
  int a100 = a4 / 25 - (a4 % 25 < 0);
  int b100 = b4 / 25 - (b4 % 25 < 0);
  int a400 = SHR (a100, 2);
  int b400 = SHR (b100, 2);
  int intervening_leap_days = (a4 - b4) - (a100 - b100) + (a400 - b400);
  int years = a->tm_year - b->tm_year;
  int days = (365 * years + intervening_leap_days
              + (a->tm_yday - b->tm_yday));
  return (60 * (60 * (24 * days + (a->tm_hour - b->tm_hour))
                + (a->tm_min - b->tm_min))
          + (a->tm_sec - b->tm_sec));
}
#endif /* ! HAVE_TM_GMTOFF */

static table const *
lookup_word (parser_control const *pc, char *word)
{
  char *p;
  char *q;
  idx_t wordlen;
  table const *tp;
  bool period_found;
  bool abbrev;

  /* Make it uppercase.  */
  for (p = word; *p; p++)
    *p = c_toupper (to_uchar (*p));

  for (tp = meridian_table; tp->name; tp++)
    if (strcmp (word, tp->name) == 0)
      return tp;

  /* See if we have an abbreviation for a month.  */
  wordlen = strlen (word);
  abbrev = wordlen == 3 || (wordlen == 4 && word[3] == '.');

  for (tp = month_and_day_table; tp->name; tp++)
    if ((abbrev ? strncmp (word, tp->name, 3) : strcmp (word, tp->name)) == 0)
      return tp;

  if ((tp = lookup_zone (pc, word)))
    return tp;

  if (strcmp (word, dst_table[0].name) == 0)
    return dst_table;

  for (tp = time_units_table; tp->name; tp++)
    if (strcmp (word, tp->name) == 0)
      return tp;

  /* Strip off any plural and try the units table again.  */
  if (word[wordlen - 1] == 'S')
    {
      word[wordlen - 1] = '\0';
      for (tp = time_units_table; tp->name; tp++)
        if (strcmp (word, tp->name) == 0)
          return tp;
      word[wordlen - 1] = 'S';  /* For "this" in relative_time_table.  */
    }

  for (tp = relative_time_table; tp->name; tp++)
    if (strcmp (word, tp->name) == 0)
      return tp;

  /* Military time zones.  */
  if (wordlen == 1)
    for (tp = military_table; tp->name; tp++)
      if (word[0] == tp->name[0])
        return tp;

  /* Drop out any periods and try the time zone table again.  */
  for (period_found = false, p = q = word; (*p = *q); q++)
    if (*q == '.')
      period_found = true;
    else
      p++;
  if (period_found && (tp = lookup_zone (pc, word)))
    return tp;

  return NULL;
}

static int
yylex (union YYSTYPE *lvalp, parser_control *pc)
{
  unsigned char c;

  for (;;)
    {
      while (c = *pc->input, c_isspace (c))
        pc->input++;

      if (c_isdigit (c) || c == '-' || c == '+')
        {
          char const *p = pc->input;
          int sign;
          if (c == '-' || c == '+')
            {
              sign = c == '-' ? -1 : 1;
              while (c = *(pc->input = ++p), c_isspace (c))
                continue;
              if (! c_isdigit (c))
                /* skip the '-' sign */
                continue;
            }
          else
            sign = 0;

          time_t value = 0;
          do
            {
              if (ckd_mul (&value, value, 10))
                return '?';
              if (ckd_add (&value, value, sign < 0 ? '0' - c : c - '0'))
                return '?';
              c = *++p;
            }
          while (c_isdigit (c));

          if ((c == '.' || c == ',') && c_isdigit (p[1]))
            {
              time_t s = value;
              int digits;

              /* Accumulate fraction, to ns precision.  */
              p++;
              int ns = *p++ - '0';
              for (digits = 2; digits <= LOG10_BILLION; digits++)
                {
                  ns *= 10;
                  if (c_isdigit (*p))
                    ns += *p++ - '0';
                }

              /* Skip excess digits, truncating toward -Infinity.  */
              if (sign < 0)
                for (; c_isdigit (*p); p++)
                  if (*p != '0')
                    {
                      ns++;
                      break;
                    }
              while (c_isdigit (*p))
                p++;

              /* Adjust to the timespec convention, which is that
                 tv_nsec is always a positive offset even if tv_sec is
                 negative.  */
              if (sign < 0 && ns)
                {
                  if (ckd_sub (&s, s, 1))
                    return '?';
                  ns = BILLION - ns;
                }

              lvalp->timespec = (struct timespec) { .tv_sec = s,
                                                    .tv_nsec = ns };
              pc->input = p;
              return sign ? tSDECIMAL_NUMBER : tUDECIMAL_NUMBER;
            }
          else
            {
              lvalp->textintval.negative = sign < 0;
              lvalp->textintval.value = value;
              lvalp->textintval.digits = p - pc->input;
              pc->input = p;
              return sign ? tSNUMBER : tUNUMBER;
            }
        }

      if (c_isalpha (c))
        {
          char buff[20];
          char *p = buff;
          table const *tp;

          do
            {
              if (p < buff + sizeof buff - 1)
                *p++ = c;
              c = *++pc->input;
            }
          while (c_isalpha (c) || c == '.');

          *p = '\0';
          tp = lookup_word (pc, buff);
          if (! tp)
            {
              if (debugging (pc))
                dbg_printf (_("error: unknown word '%s'\n"), buff);
              return '?';
            }
          lvalp->intval = tp->value;
          return tp->type;
        }

      if (c != '(')
        return to_uchar (*pc->input++);

      idx_t count = 0;
      do
        {
          c = *pc->input++;
          if (c == '\0')
            return c;
          if (c == '(')
            count++;
          else if (c == ')')
            count--;
        }
      while (count != 0);
    }
}

/* Do nothing if the parser reports an error.  */
static int
yyerror (_GL_UNUSED parser_control const *pc,
         _GL_UNUSED char const *s)
{
  return 0;
}

/* If *TM0 is the old and *TM1 is the new value of a struct tm after
   passing it to mktime_z, return true if it's OK.  It's not OK if
   mktime failed or if *TM0 has out-of-range mainline members.
   The caller should set TM1->tm_wday to -1 before calling mktime,
   as a negative tm_wday is how mktime failure is inferred.  */

static bool
mktime_ok (struct tm const *tm0, struct tm const *tm1)
{
  if (tm1->tm_wday < 0)
    return false;

  return ! ((tm0->tm_sec ^ tm1->tm_sec)
            | (tm0->tm_min ^ tm1->tm_min)
            | (tm0->tm_hour ^ tm1->tm_hour)
            | (tm0->tm_mday ^ tm1->tm_mday)
            | (tm0->tm_mon ^ tm1->tm_mon)
            | (tm0->tm_year ^ tm1->tm_year));
}

/* Debugging: format a 'struct tm' into a buffer, taking the parser's
   timezone information into account (if pc != NULL).  */
static char const *
debug_strfdatetime (struct tm const *tm, parser_control const *pc,
                    char *buf, int n)
{
  /* TODO:
     1. find an optimal way to print date string in a clear and unambiguous
        format.  Currently, always add '(Y-M-D)' prefix.
        Consider '2016y01m10d'  or 'year(2016) month(01) day(10)'.

        If the user needs debug printing, it means he/she already having
        issues with the parsing - better to avoid formats that could
        be mis-interpreted (e.g., just YYYY-MM-DD).

     2. Can strftime be used instead?
        depends if it is portable and can print invalid dates on all systems.

     3. Print timezone information ?

     4. Print DST information ?

     5. Print nanosecond information ?

     NOTE:
     Printed date/time values might not be valid, e.g., '2016-02-31'
     or '2016-19-2016' .  These are the values as parsed from the user
     string, before validation.
  */
  int m = nstrftime (buf, n, "(Y-M-D) %Y-%m-%d %H:%M:%S", tm, 0, 0);

  /* If parser_control information was provided (for timezone),
     and there's enough space in the buffer, add timezone info.  */
  if (pc && m < n && pc->zones_seen)
    {
      int tz = pc->time_zone;

      /* Account for DST if tLOCAL_ZONE was seen.  */
      if (pc->local_zones_seen && !pc->zones_seen && 0 < pc->local_isdst)
        tz += 60 * 60;

      char time_zone_buf[TIME_ZONE_BUFSIZE];
      snprintf (&buf[m], n - m, " TZ=%s", time_zone_str (tz, time_zone_buf));
    }
  return buf;
}

static char const *
debug_strfdate (struct tm const *tm, char *buf, int n)
{
  char tm_year_buf[TM_YEAR_BUFSIZE];
  snprintf (buf, n, "(Y-M-D) %s-%02d-%02d",
            tm_year_str (tm->tm_year, tm_year_buf),
            tm->tm_mon + 1, tm->tm_mday);
  return buf;
}

static char const *
debug_strftime (struct tm const *tm, char *buf, int n)
{
  snprintf (buf, n, "%02d:%02d:%02d", tm->tm_hour, tm->tm_min, tm->tm_sec);
  return buf;
}

/* If mktime_ok failed, display the failed time values,
   and provide possible hints.  Example output:

    date: error: invalid date/time value:
    date:     user provided time: '(Y-M-D) 2006-04-02 02:45:00'
    date:        normalized time: '(Y-M-D) 2006-04-02 03:45:00'
    date:                                             __
    date:      possible reasons:
    date:        nonexistent due to daylight-saving time;
    date:        numeric values overflow;
    date:        missing timezone;
 */
static void
debug_mktime_not_ok (struct tm const *tm0, struct tm const *tm1,
                     parser_control const *pc, bool time_zone_seen)
{
  /* TODO: handle t == -1 (as in 'mktime_ok').  */
  char tmp[DBGBUFSIZE];
  int i;
  const bool eq_sec   = (tm0->tm_sec  == tm1->tm_sec);
  const bool eq_min   = (tm0->tm_min  == tm1->tm_min);
  const bool eq_hour  = (tm0->tm_hour == tm1->tm_hour);
  const bool eq_mday  = (tm0->tm_mday == tm1->tm_mday);
  const bool eq_month = (tm0->tm_mon  == tm1->tm_mon);
  const bool eq_year  = (tm0->tm_year == tm1->tm_year);

  const bool dst_shift = eq_sec && eq_min && !eq_hour
                         && eq_mday && eq_month && eq_year;

  if (!debugging (pc))
    return;

  dbg_printf (_("error: invalid date/time value:\n"));
  dbg_printf (_("    user provided time: '%s'\n"),
              debug_strfdatetime (tm0, pc, tmp, sizeof tmp));
  dbg_printf (_("       normalized time: '%s'\n"),
              debug_strfdatetime (tm1, pc, tmp, sizeof tmp));
  /* The format must be aligned with debug_strfdatetime and the two
     DEBUG statements above.  This string is not translated.  */
  i = snprintf (tmp, sizeof tmp,
                "                                 %4s %2s %2s %2s %2s %2s",
                eq_year ? "" : "----",
                eq_month ? "" : "--",
                eq_mday ? "" : "--",
                eq_hour ? "" : "--",
                eq_min ? "" : "--",
                eq_sec ? "" : "--");
  /* Trim trailing whitespace.  */
  if (0 <= i)
    {
      if (sizeof tmp - 1 < i)
        i = sizeof tmp - 1;
      while (0 < i && tmp[i - 1] == ' ')
        --i;
      tmp[i] = '\0';
    }
  dbg_printf ("%s\n", tmp);

  dbg_printf (_("     possible reasons:\n"));
  if (dst_shift)
    dbg_printf (_("       nonexistent due to daylight-saving time;\n"));
  if (!eq_mday && !eq_month)
    dbg_printf (_("       invalid day/month combination;\n"));
  dbg_printf (_("       numeric values overflow;\n"));
  dbg_printf ("       %s\n", (time_zone_seen ? _("incorrect timezone")
                              : _("missing timezone")));
}

/* Parse a date/time string, storing the resulting time value into *RESULT.
   The string itself is pointed to by P.  Return true if successful.
   P can be an incomplete or relative time specification; if so, use
   *NOW as the basis for the returned time.  Default to timezone
   TZDEFAULT, which corresponds to tzalloc (TZSTRING).  */
static bool
parse_datetime_body (struct timespec *result, char const *p,
                     struct timespec const *now, unsigned int flags,
                     timezone_t tzdefault, char const *tzstring)
{
  struct tm tm;
  struct tm tm0;
  char time_zone_buf[TIME_ZONE_BUFSIZE];
  char dbg_tm[DBGBUFSIZE];
  bool ok = false;
  char const *input_sentinel = p + strlen (p);
  char *tz1alloc = NULL;

  /* A reasonable upper bound for the size of ordinary TZ strings.
     Use heap allocation if TZ's length exceeds this.  */
  enum { TZBUFSIZE = 100 };
  char tz1buf[TZBUFSIZE];

  struct timespec gettime_buffer;
  if (! now)
    {
      gettime (&gettime_buffer);
      now = &gettime_buffer;
    }

  time_t Start = now->tv_sec;
  int Start_ns = now->tv_nsec;

  unsigned char c;
  while (c = *p, c_isspace (c))
    p++;

  timezone_t tz = tzdefault;

  /* Store a local copy prior to first "goto".  Without this, a prior use
     below of RELATIVE_TIME_0 on the RHS might translate to an assignment-
     to-temporary, which would trigger a -Wjump-misses-init warning.  */
  const relative_time rel_time_0 = RELATIVE_TIME_0;

  if (strncmp (p, "TZ=\"", 4) == 0)
    {
      char const *tzbase = p + 4;
      idx_t tzsize = 1;
      char const *s;

      for (s = tzbase; *s; s++, tzsize++)
        if (*s == '\\')
          {
            s++;
            if (! (*s == '\\' || *s == '"'))
              break;
          }
        else if (*s == '"')
          {
            timezone_t tz1;
            char *tz1string = tz1buf;
            char *z;
            if (TZBUFSIZE < tzsize)
              {
                tz1alloc = malloc (tzsize);
                if (!tz1alloc)
                  goto fail;
                tz1string = tz1alloc;
              }
            z = tz1string;
            for (s = tzbase; *s != '"'; s++)
              *z++ = *(s += *s == '\\');
            *z = '\0';
            tz1 = tzalloc (tz1string);
            if (!tz1)
              goto fail;
            tz = tz1;
            tzstring = tz1string;

            p = s + 1;
            while (c = *p, c_isspace (c))
              p++;

            break;
          }
    }

  struct tm tmp;
  if (! localtime_rz (tz, &now->tv_sec, &tmp))
    goto fail;

  /* As documented, be careful to treat the empty string just like
     a date string of "0".  Without this, an empty string would be
     declared invalid when parsed during a DST transition.  */
  if (*p == '\0')
    p = "0";

  parser_control pc;
  pc.input = p;
#ifdef GNULIB_PARSE_DATETIME2
  pc.parse_datetime_debug = (flags & PARSE_DATETIME_DEBUG) != 0;
#endif
  if (ckd_add (&pc.year.value, tmp.tm_year, TM_YEAR_BASE))
    {
      if (debugging (&pc))
        dbg_printf (_("error: initial year out of range\n"));
      goto fail;
    }
  pc.year.digits = 0;
  pc.month = tmp.tm_mon + 1;
  pc.day = tmp.tm_mday;
  pc.hour = tmp.tm_hour;
  pc.minutes = tmp.tm_min;
  pc.seconds = (struct timespec) { .tv_sec = tmp.tm_sec, .tv_nsec = Start_ns };
  tm.tm_isdst = tmp.tm_isdst;

  pc.meridian = MER24;
  pc.rel = rel_time_0;
  pc.timespec_seen = false;
  pc.rels_seen = false;
  pc.dates_seen = 0;
  pc.days_seen = 0;
  pc.times_seen = 0;
  pc.J_zones_seen = 0;
  pc.local_zones_seen = 0;
  pc.dsts_seen = 0;
  pc.zones_seen = 0;
  pc.year_seen = false;
  pc.debug_dates_seen = false;
  pc.debug_days_seen = false;
  pc.debug_times_seen = false;
  pc.debug_local_zones_seen = false;
  pc.debug_zones_seen = false;
  pc.debug_year_seen = false;
  pc.debug_ordinal_day_seen = false;

#if HAVE_STRUCT_TM_TM_ZONE
  pc.local_time_zone_table[0].name = tmp.tm_zone;
  pc.local_time_zone_table[0].type = tLOCAL_ZONE;
  pc.local_time_zone_table[0].value = tmp.tm_isdst;
  pc.local_time_zone_table[1].name = NULL;

  /* Probe the names used in the next three calendar quarters, looking
     for a tm_isdst different from the one we already have.  */
  {
    int quarter;
    for (quarter = 1; quarter <= 3; quarter++)
      {
        time_t probe;
        if (ckd_add (&probe, Start, quarter * (90 * 24 * 60 * 60)))
          break;
        struct tm probe_tm;
        if (localtime_rz (tz, &probe, &probe_tm) && probe_tm.tm_zone
            && probe_tm.tm_isdst != pc.local_time_zone_table[0].value)
          {
              {
                pc.local_time_zone_table[1].name = probe_tm.tm_zone;
                pc.local_time_zone_table[1].type = tLOCAL_ZONE;
                pc.local_time_zone_table[1].value = probe_tm.tm_isdst;
                pc.local_time_zone_table[2].name = NULL;
              }
            break;
          }
      }
  }
#else
#if HAVE_TZNAME
  {
# if !HAVE_DECL_TZNAME
    extern char *tzname[];
# endif
    int i;
    for (i = 0; i < 2; i++)
      {
        pc.local_time_zone_table[i].name = tzname[i];
        pc.local_time_zone_table[i].type = tLOCAL_ZONE;
        pc.local_time_zone_table[i].value = i;
      }
    pc.local_time_zone_table[i].name = NULL;
  }
#else
  pc.local_time_zone_table[0].name = NULL;
#endif
#endif

  if (pc.local_time_zone_table[0].name && pc.local_time_zone_table[1].name
      && ! strcmp (pc.local_time_zone_table[0].name,
                   pc.local_time_zone_table[1].name))
    {
      /* This locale uses the same abbreviation for standard and
         daylight times.  So if we see that abbreviation, we don't
         know whether it's daylight time.  */
      pc.local_time_zone_table[0].value = -1;
      pc.local_time_zone_table[1].name = NULL;
    }

  if (yyparse (&pc) != 0)
    {
      if (debugging (&pc))
        dbg_printf ((input_sentinel <= pc.input
                     ? _("error: parsing failed\n")
                     : _("error: parsing failed, stopped at '%s'\n")),
                    pc.input);
      goto fail;
    }


  /* Determine effective timezone source.  */

  if (debugging (&pc))
    {
      dbg_printf (_("input timezone: "));

      if (pc.timespec_seen)
        fprintf (stderr, _("'@timespec' - always UTC"));
      else if (pc.zones_seen)
        fprintf (stderr, _("parsed date/time string"));
      else if (tzstring)
        {
          if (tz != tzdefault)
            fprintf (stderr, _("TZ=\"%s\" in date string"), tzstring);
          else if (STREQ (tzstring, "UTC0"))
            {
              /* Special case: 'date -u' sets TZ="UTC0".  */
              fprintf (stderr, _("TZ=\"UTC0\" environment value or -u"));
            }
          else
            fprintf (stderr, _("TZ=\"%s\" environment value"), tzstring);
        }
      else
        fprintf (stderr, _("system default"));

      /* Account for DST changes if tLOCAL_ZONE was seen.
         local timezone only changes DST and is relative to the
         default timezone.*/
      if (pc.local_zones_seen && !pc.zones_seen && 0 < pc.local_isdst)
        fprintf (stderr, ", dst");

      if (pc.zones_seen)
        fprintf (stderr, " (%s)", time_zone_str (pc.time_zone, time_zone_buf));

      fputc ('\n', stderr);
    }

  if (pc.timespec_seen)
    *result = pc.seconds;
  else
    {
      if (1 < (pc.times_seen | pc.dates_seen | pc.days_seen | pc.dsts_seen
               | (pc.J_zones_seen + pc.local_zones_seen + pc.zones_seen)))
        {
          if (debugging (&pc))
            {
              if (pc.times_seen > 1)
                dbg_printf ("error: seen multiple time parts\n");
              if (pc.dates_seen > 1)
                dbg_printf ("error: seen multiple date parts\n");
              if (pc.days_seen > 1)
                dbg_printf ("error: seen multiple days parts\n");
              if (pc.dsts_seen > 1)
                dbg_printf ("error: seen multiple daylight-saving parts\n");
              if ((pc.J_zones_seen + pc.local_zones_seen + pc.zones_seen) > 1)
                dbg_printf ("error: seen multiple time-zone parts\n");
            }
          goto fail;
        }

      if (! to_tm_year (pc.year, debugging (&pc), &tm.tm_year)
          || ckd_add (&tm.tm_mon, pc.month, -1)
          || ckd_add (&tm.tm_mday, pc.day, 0))
        {
          if (debugging (&pc))
            dbg_printf (_("error: year, month, or day overflow\n"));
          goto fail;
        }
      if (pc.times_seen || (pc.rels_seen && ! pc.dates_seen && ! pc.days_seen))
        {
          tm.tm_hour = to_hour (pc.hour, pc.meridian);
          if (tm.tm_hour < 0)
            {
              char const *mrd = (pc.meridian == MERam ? "am"
                                 : pc.meridian == MERpm ?"pm" : "");
              if (debugging (&pc))
                dbg_printf (_("error: invalid hour %"PRIdMAX"%s\n"),
                            pc.hour, mrd);
              goto fail;
            }
          tm.tm_min = pc.minutes;
          tm.tm_sec = pc.seconds.tv_sec;
          if (debugging (&pc))
            dbg_printf ((pc.times_seen
                         ? _("using specified time as starting value: '%s'\n")
                         : _("using current time as starting value: '%s'\n")),
                        debug_strftime (&tm, dbg_tm, sizeof dbg_tm));
        }
      else
        {
          tm.tm_hour = tm.tm_min = tm.tm_sec = 0;
          pc.seconds.tv_nsec = 0;
          if (debugging (&pc))
            dbg_printf ("warning: using midnight as starting time: 00:00:00\n");
        }

      /* Let mktime deduce tm_isdst if we have an absolute timestamp.  */
      if (pc.dates_seen | pc.days_seen | pc.times_seen)
        tm.tm_isdst = -1;

      /* But if the input explicitly specifies local time with or without
         DST, give mktime that information.  */
      if (pc.local_zones_seen)
        tm.tm_isdst = pc.local_isdst;

      tm0.tm_sec = tm.tm_sec;
      tm0.tm_min = tm.tm_min;
      tm0.tm_hour = tm.tm_hour;
      tm0.tm_mday = tm.tm_mday;
      tm0.tm_mon = tm.tm_mon;
      tm0.tm_year = tm.tm_year;
      tm0.tm_isdst = tm.tm_isdst;
      tm.tm_wday = -1;

      Start = mktime_z (tz, &tm);

      if (! mktime_ok (&tm0, &tm))
        {
          bool repaired = false;
          bool time_zone_seen = pc.zones_seen != 0;
          if (time_zone_seen)
            {
              /* Guard against falsely reporting errors near the time_t
                 boundaries when parsing times in other time zones.  For
                 example, suppose the input string "1969-12-31 23:00:00 -0100",
                 the current time zone is 8 hours ahead of UTC, and the min
                 time_t value is 1970-01-01 00:00:00 UTC.  Then the min
                 localtime value is 1970-01-01 08:00:00, and mktime will
                 therefore fail on 1969-12-31 23:00:00.  To work around the
                 problem, set the time zone to 1 hour behind UTC temporarily
                 by setting TZ="XXX1:00" and try mktime again.  */

              char tz2buf[sizeof "XXX" - 1 + TIME_ZONE_BUFSIZE];
              tz2buf[0] = tz2buf[1] = tz2buf[2] = 'X';
              time_zone_str (pc.time_zone, &tz2buf[3]);
              timezone_t tz2 = tzalloc (tz2buf);
              if (!tz2)
                {
                  if (debugging (&pc))
                    dbg_printf (_("error: tzalloc (\"%s\") failed\n"), tz2buf);
                  goto fail;
                }
              tm.tm_sec = tm0.tm_sec;
              tm.tm_min = tm0.tm_min;
              tm.tm_hour = tm0.tm_hour;
              tm.tm_mday = tm0.tm_mday;
              tm.tm_mon = tm0.tm_mon;
              tm.tm_year = tm0.tm_year;
              tm.tm_isdst = tm0.tm_isdst;
              tm.tm_wday = -1;
              Start = mktime_z (tz2, &tm);
              repaired = mktime_ok (&tm0, &tm);
              tzfree (tz2);
            }

          if (! repaired)
            {
              debug_mktime_not_ok (&tm0, &tm, &pc, time_zone_seen);
              goto fail;
            }
        }

      char dbg_ord[DBGBUFSIZE];

      if (pc.days_seen && ! pc.dates_seen)
        {
          intmax_t dayincr;
          tm.tm_yday = -1;
          intmax_t day_ordinal = (pc.day_ordinal
                                  - (0 < pc.day_ordinal
                                     && tm.tm_wday != pc.day_number));
          if (! (ckd_mul (&dayincr, day_ordinal, 7)
                 || ckd_add (&dayincr, (pc.day_number - tm.tm_wday + 7) % 7,
                             dayincr)
                 || ckd_add (&tm.tm_mday, dayincr, tm.tm_mday)))
            {
              tm.tm_isdst = -1;
              Start = mktime_z (tz, &tm);
            }

          if (tm.tm_yday < 0)
            {
              if (debugging (&pc))
                dbg_printf (_("error: day '%s' "
                              "(day ordinal=%"PRIdMAX" number=%d) "
                              "resulted in an invalid date: '%s'\n"),
                            str_days (&pc, dbg_ord, sizeof dbg_ord),
                            pc.day_ordinal, pc.day_number,
                            debug_strfdatetime (&tm, &pc, dbg_tm,
                                                sizeof dbg_tm));
              goto fail;
            }

          if (debugging (&pc))
            dbg_printf (_("new start date: '%s' is '%s'\n"),
                        str_days (&pc, dbg_ord, sizeof dbg_ord),
                        debug_strfdatetime (&tm, &pc, dbg_tm, sizeof dbg_tm));

        }

      if (debugging (&pc))
        {
          if (!pc.dates_seen && !pc.days_seen)
            dbg_printf (_("using current date as starting value: '%s'\n"),
                        debug_strfdate (&tm, dbg_tm, sizeof dbg_tm));

          if (pc.days_seen && pc.dates_seen)
            dbg_printf (_("warning: day (%s) ignored when explicit dates "
                          "are given\n"),
                        str_days (&pc, dbg_ord, sizeof dbg_ord));

          dbg_printf (_("starting date/time: '%s'\n"),
                      debug_strfdatetime (&tm, &pc, dbg_tm, sizeof dbg_tm));
        }

      /* Add relative date.  */
      if (pc.rel.year | pc.rel.month | pc.rel.day)
        {
          if (debugging (&pc))
            {
              if ((pc.rel.year != 0 || pc.rel.month != 0) && tm.tm_mday != 15)
                dbg_printf (_("warning: when adding relative months/years, "
                              "it is recommended to specify the 15th of the "
                              "months\n"));

              if (pc.rel.day != 0 && tm.tm_hour != 12)
                dbg_printf (_("warning: when adding relative days, "
                              "it is recommended to specify noon\n"));
            }

          int year, month, day;
          if (ckd_add (&year, tm.tm_year, pc.rel.year)
              || ckd_add (&month, tm.tm_mon, pc.rel.month)
              || ckd_add (&day, tm.tm_mday, pc.rel.day))
            {
              if (debugging (&pc))
                dbg_printf (_("error: %s:%d\n"), __FILE__, __LINE__);
              goto fail;
            }
          tm.tm_year = year;
          tm.tm_mon = month;
          tm.tm_mday = day;
          tm.tm_hour = tm0.tm_hour;
          tm.tm_min = tm0.tm_min;
          tm.tm_sec = tm0.tm_sec;
          tm.tm_isdst = tm0.tm_isdst;
          tm.tm_wday = -1;
          Start = mktime_z (tz, &tm);
          if (tm.tm_wday < 0)
            {
              if (debugging (&pc))
                dbg_printf (_("error: adding relative date resulted "
                              "in an invalid date: '%s'\n"),
                            debug_strfdatetime (&tm, &pc, dbg_tm,
                                                sizeof dbg_tm));
              goto fail;
            }

          if (debugging (&pc))
            {
              dbg_printf (_("after date adjustment "
                            "(%+"PRIdMAX" years, %+"PRIdMAX" months, "
                            "%+"PRIdMAX" days),\n"),
                          pc.rel.year, pc.rel.month, pc.rel.day);
              dbg_printf (_("    new date/time = '%s'\n"),
                          debug_strfdatetime (&tm, &pc, dbg_tm,
                                              sizeof dbg_tm));

              /* Warn about crossing DST due to time adjustment.
                 Example: https://bugs.gnu.org/8357
                 env TZ=Europe/Helsinki \
                   date --debug \
                        -d 'Mon Mar 28 00:36:07 2011 EEST 1 day ago'

                 This case is different than DST changes due to time adjustment,
                 i.e., "1 day ago" vs "24 hours ago" are calculated in different
                 places.

                 'tm0.tm_isdst' contains the DST of the input date,
                 'tm.tm_isdst' is the normalized result after calling
                 mktime (&tm).
              */
              if (tm0.tm_isdst != -1 && tm.tm_isdst != tm0.tm_isdst)
                dbg_printf (_("warning: daylight saving time changed after "
                              "date adjustment\n"));

              /* Warn if the user did not ask to adjust days but mday changed,
                 or
                 user did not ask to adjust months/days but the month changed.

                 Example for first case:
                 2016-05-31 + 1 month => 2016-06-31 => 2016-07-01.
                 User asked to adjust month, but the day changed from 31 to 01.

                 Example for second case:
                 2016-02-29 + 1 year => 2017-02-29 => 2017-03-01.
                 User asked to adjust year, but the month changed from 02 to 03.
              */
              if (pc.rel.day == 0
                  && (tm.tm_mday != day
                      || (pc.rel.month == 0 && tm.tm_mon != month)))
                {
                  dbg_printf (_("warning: month/year adjustment resulted in "
                                "shifted dates:\n"));
                  char tm_year_buf[TM_YEAR_BUFSIZE];
                  dbg_printf (_("     adjusted Y M D: %s %02d %02d\n"),
                              tm_year_str (year, tm_year_buf), month + 1, day);
                  dbg_printf (_("   normalized Y M D: %s %02d %02d\n"),
                              tm_year_str (tm.tm_year, tm_year_buf),
                              tm.tm_mon + 1, tm.tm_mday);
                }
            }

        }

      /* The only "output" of this if-block is an updated Start value,
         so this block must follow others that clobber Start.  */
      if (pc.zones_seen)
        {
          bool overflow = false;
#ifdef HAVE_TM_GMTOFF
          long int utcoff = tm.tm_gmtoff;
#else
          time_t t = Start;
          struct tm gmt;
          int utcoff = (gmtime_r (&t, &gmt)
                        ? tm_diff (&tm, &gmt)
                        : (overflow = true, 0));
#endif
          intmax_t delta;
          overflow |= ckd_sub (&delta, pc.time_zone, utcoff);
          time_t t1;
          overflow |= ckd_sub (&t1, Start, delta);
          if (overflow)
            {
              if (debugging (&pc))
                dbg_printf (_("error: timezone %d caused time_t overflow\n"),
                            pc.time_zone);
              goto fail;
            }
          Start = t1;
        }

      if (debugging (&pc))
        {
          intmax_t Starti = Start;
          dbg_printf (_("'%s' = %"PRIdMAX" epoch-seconds\n"),
                      debug_strfdatetime (&tm, &pc, dbg_tm, sizeof dbg_tm),
                      Starti);
        }


      /* Add relative hours, minutes, and seconds.  On hosts that support
         leap seconds, ignore the possibility of leap seconds; e.g.,
         "+ 10 minutes" adds 600 seconds, even if one of them is a
         leap second.  Typically this is not what the user wants, but it's
         too hard to do it the other way, because the time zone indicator
         must be applied before relative times, and if mktime is applied
         again the time zone will be lost.  */
      {
        intmax_t orig_ns = pc.seconds.tv_nsec;
        intmax_t sum_ns = orig_ns + pc.rel.ns;
        int normalized_ns = (sum_ns % BILLION + BILLION) % BILLION;
        int d4 = (sum_ns - normalized_ns) / BILLION;
        intmax_t d1, t1, d2, t2, t3;
        time_t t4;
        if (ckd_mul (&d1, pc.rel.hour, 60 * 60)
            || ckd_add (&t1, Start, d1)
            || ckd_mul (&d2, pc.rel.minutes, 60)
            || ckd_add (&t2, t1, d2)
            || ckd_add (&t3, t2, pc.rel.seconds)
            || ckd_add (&t4, t3, d4))
          {
            if (debugging (&pc))
              dbg_printf (_("error: adding relative time caused an "
                            "overflow\n"));
            goto fail;
          }

        result->tv_sec = t4;
        result->tv_nsec = normalized_ns;

        if (debugging (&pc)
            && (pc.rel.hour | pc.rel.minutes | pc.rel.seconds | pc.rel.ns))
          {
            dbg_printf (_("after time adjustment (%+"PRIdMAX" hours, "
                          "%+"PRIdMAX" minutes, "
                          "%+"PRIdMAX" seconds, %+d ns),\n"),
                        pc.rel.hour, pc.rel.minutes, pc.rel.seconds,
                        pc.rel.ns);
            intmax_t t4i = t4;
            dbg_printf (_("    new time = %"PRIdMAX" epoch-seconds\n"), t4i);

            /* Warn about crossing DST due to time adjustment.
               Example: https://bugs.gnu.org/8357
               env TZ=Europe/Helsinki           \
               date --debug                                             \
               -d 'Mon Mar 28 00:36:07 2011 EEST 24 hours ago'

               This case is different than DST changes due to days adjustment,
               i.e., "1 day ago" vs "24 hours ago" are calculated in different
               places.

               'tm.tm_isdst' contains the date after date adjustment.  */
            struct tm lmt;
            if (tm.tm_isdst != -1 && localtime_rz (tz, &result->tv_sec, &lmt)
                && tm.tm_isdst != lmt.tm_isdst)
              dbg_printf (_("warning: daylight saving time changed after "
                            "time adjustment\n"));
          }
      }
    }

  if (debugging (&pc))
    {
      /* Special case: using 'date -u' simply set TZ=UTC0 */
      if (! tzstring)
        dbg_printf (_("timezone: system default\n"));
      else if (STREQ (tzstring, "UTC0"))
        dbg_printf (_("timezone: Universal Time\n"));
      else
        dbg_printf (_("timezone: TZ=\"%s\" environment value\n"), tzstring);

      intmax_t sec = result->tv_sec;
      int nsec = result->tv_nsec;
      dbg_printf (_("final: %"PRIdMAX".%09d (epoch-seconds)\n"),
                  sec, nsec);

      struct tm gmt, lmt;
      bool got_utc = !!gmtime_r (&result->tv_sec, &gmt);
      if (got_utc)
        dbg_printf (_("final: %s (UTC)\n"),
                    debug_strfdatetime (&gmt, NULL,
                                        dbg_tm, sizeof dbg_tm));
      if (localtime_rz (tz, &result->tv_sec, &lmt))
        {
#ifdef HAVE_TM_GMTOFF
          bool got_utcoff = true;
          long int utcoff = lmt.tm_gmtoff;
#else
          bool got_utcoff = got_utc;
          int utcoff;
          if (got_utcoff)
            utcoff = tm_diff (&lmt, &gmt);
#endif
          if (got_utcoff)
            dbg_printf (_("final: %s (UTC%s)\n"),
                        debug_strfdatetime (&lmt, NULL, dbg_tm, sizeof dbg_tm),
                        time_zone_str (utcoff, time_zone_buf));
          else
            dbg_printf (_("final: %s (unknown time zone offset)\n"),
                        debug_strfdatetime (&lmt, NULL, dbg_tm, sizeof dbg_tm));
        }
    }

  ok = true;

 fail:
  if (tz != tzdefault)
    tzfree (tz);
  free (tz1alloc);
  return ok;
}

#ifdef GNULIB_PARSE_DATETIME2
/* Parse a date/time string, storing the resulting time value into *RESULT.
   The string itself is pointed to by P.  Return true if successful.
   P can be an incomplete or relative time specification; if so, use
   *NOW as the basis for the returned time.  Default to timezone
   TZDEFAULT, which corresponds to tzalloc (TZSTRING).  */
bool
parse_datetime2 (struct timespec *result, char const *p,
                 struct timespec const *now, unsigned int flags,
                 timezone_t tzdefault, char const *tzstring)
{
  return parse_datetime_body (result, p, now, flags, tzdefault, tzstring);
}
#endif


/* The plain interface: run with debug=false and the default timezone.   */
bool
parse_datetime (struct timespec *result, char const *p,
                struct timespec const *now)
{
  char const *tzstring = getenv ("TZ");
  timezone_t tz = tzalloc (tzstring);
  if (!tz)
    return false;
  bool ok = parse_datetime_body (result, p, now, 0, tz, tzstring);
  tzfree (tz);
  return ok;
}

#if TEST

int
main (int ac, char **av)
{
  char buff[BUFSIZ];

  printf ("Enter date, or blank line to exit.\n\t> ");
  fflush (stdout);

  buff[BUFSIZ - 1] = '\0';
  while (fgets (buff, BUFSIZ - 1, stdin) && buff[0])
    {
      struct timespec d;
      struct tm const *tm;
      if (! parse_datetime (&d, buff, NULL))
        printf ("Bad format - couldn't convert.\n");
      else if (! (tm = localtime (&d.tv_sec)))
        {
          intmax_t sec = d.tv_sec;
          printf ("localtime (%"PRIdMAX") failed\n", sec);
        }
      else
        {
          int ns = d.tv_nsec;
          char tm_year_buf[TM_YEAR_BUFSIZE];
          printf ("%s-%02d-%02d %02d:%02d:%02d.%09d\n",
                  tm_year_str (tm->tm_year, tm_year_buf),
                  tm->tm_mon + 1, tm->tm_mday,
                  tm->tm_hour, tm->tm_min, tm->tm_sec, ns);
        }
      printf ("\t> ");
      fflush (stdout);
    }
  return 0;
}
#endif /* TEST */
