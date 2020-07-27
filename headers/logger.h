/*
 * logger.h
 *
 *  Created on: Nov 27, 2019
 *      Author: khubaibumer
 */

#ifndef SRC_INCLUDE_LOGGER_H_
#define SRC_INCLUDE_LOGGER_H_

#ifndef __KERNEL__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#else
#include <stdarg.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#endif

#define DECLARE_SYMBOL(__type,  __sym) __type __sym
#define DECLARE_STATIC_SYMBOL(__type, __sym) static __type __sym

typedef int (*log_f) (const char *fmt, ...);

typedef enum {
	fatal = 1, ///< log.f() is enabled
	error_,	///< log.e() and higher is enabled
	critical,	///< log.c() and higher is enabled
	warning, 	///< log.w() and higher is enabled
	alert,	///< log.a() and higher is enabled
	info,	///< log.i() and higher is enabled
	debug, 	///< log.d() and higher is enabled
	verbose 	///< log.v() and higher is enabled
} kLoggingLevel;

static __always_inline const char *loglvl_to_str(kLoggingLevel __lvl__) {
	switch(__lvl__) {
	case fatal:
		return "fatal";
	case error_:
		return "error";
	case critical:
		return "critical";
	case warning:
		return "warning";
	case alert:
		return "alert";
	case info:
		return "info";
	case debug:
		return "debug";
	case verbose:
		return "verbose";
	default:
		return "unknown";
	};
}

typedef struct {
	log_f v;
	log_f d;
	log_f i;
	log_f a;
	log_f w;
	log_f c;
	log_f e;
	log_f f;
	FILE* (*get) (void);
	void (*set_file) (const char *);
} logger_t;

extern logger_t get_logger_instance( void );

extern void set_logging_level(kLoggingLevel _level);

static __always_inline logger_t __get_logger_instance( void ) {
	return get_logger_instance();
}

#endif /* SRC_INCLUDE_LOGGER_H_ */

/*	Using same trick being used in asm/current.h	*/
#define log __get_logger_instance()

