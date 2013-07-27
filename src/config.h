/* QuizBoard -- config.h
 * ========================================================================
 * Copyright 2013 Joerg Desch <github@jdesch.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * ========================================================================
 *
 *
 *
 *
 *
 * ========================================================================
 */
#ifndef __QB__CONFIG_H__
#define __QB__CONFIG_H__

// uncomment to build the DEBUGGING release
#define DEBUG 1


/*             .-----------------------------------------------.             */
/* ___________/  CONFIGURATION OF THE SOURCE                    \___________ */
/*            `-------------------------------------------------'            */
//{{{

#define CFG_MAX_ANSWERS       4
#define CFG_MAX_QUESTIONS     8
#define CFG_MAX_SOLUTIONSETS  4

//}}}

/*             .-----------------------------------------------.             */
/* ___________/  CONFIGURATION OF THE MODULES                   \___________ */
/*            `-------------------------------------------------'            */
//{{{

/*  Module "SimpleLogging"
 */
// the maximum length of the string used to build one log message
//#define SZ_QB_SIMPLELOG_BUFFER 50

/*  Module "SimpleTimer"
 */
// the maximum number of concurrently running timers.
//#define SZ_QB_MAX_TIMERS 5

//}}}

/*             .-----------------------------------------------.             */
/* ___________/  GENERAL MACRO DEFINITIONS                      \___________ */
/*            `-------------------------------------------------'            */
//{{{

#if defined(__GNUC__)
#  define UNUSED(x) x __attribute__((unused))
#  define ATTR_PACKED __attribute__((__packed__))
#  define ALLWAYS_INLINE inline __attribute__((gnu_inline, always_inline))
#  define PRINTF_STYLE_CXX(idx_fmt,idx_args) __attribute__((format (printf,(idx_fmt)+1,(idx_args)+1)))   // for C++ methods onyl
#  define PRINTF_STYLE(idx_fmt,idx_args) __attribute__((format (printf,idx_fmt,idx_args)))               // for regular functions
#else
#  define UNUSED(x) x
#  define ATTR_PACKED
#  define PRINTF_STYLE(idx_fmt,idx_args)
#  define ALWAYS_INLINE
#endif

//}}}

/*             .-----------------------------------------------.             */
/* ___________/  GENERIC STUFF                                  \___________ */
/*            `-------------------------------------------------'            */
//{{{
//}}}

#endif // __QB__CONFIG_H__
