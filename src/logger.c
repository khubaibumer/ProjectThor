/*
 * logger.c
 *
 *  Created on: Nov 27, 2019
 *      Author: khubaibumer
 */

#pragma GCC diagnostic ignored "-Wvarargs"
#pragma GCC diagnostic ignored "-Wdeclaration-after-statement"

#include <logger.h>
#include <thor.h>

#define PMODNAM "THOR: "

DECLARE_STATIC_SYMBOL(logger_t, logger) = { };
/*	Default Log Level	*/
DECLARE_STATIC_SYMBOL(kLoggingLevel, level) = verbose;

DECLARE_STATIC_SYMBOL(FILE *, logfile) = NULL;

DECLARE_STATIC_SYMBOL(volatile int, __enc_bit) = 0;

int _logv(const char *fmt, ...) {

	if (level < verbose)
		return 0;

	DECLARE_SYMBOL(va_list, args);
	DECLARE_SYMBOL(int, r) = 0;
	DECLARE_SYMBOL(int, fmtlen) = strlen(fmt) + strlen(PMODNAM) + 2;
	DECLARE_SYMBOL(char, *_fmt) = kmalloc((fmtlen * sizeof(char)) + 15,
			GFP_KERNEL);

	strcat(_fmt, " [Verbose] ");
	strcat(_fmt, PMODNAM);
	strcat(_fmt, fmt);
	va_start(args, _fmt);

	r = vfprintf(logfile, _fmt, args);
	va_end(args);

	kfree(_fmt);
	return r;
}

int _logd(const char *fmt, ...) {

	if (level < debug)
		return 0;

	DECLARE_SYMBOL(va_list, args);
	DECLARE_SYMBOL(int, r) = 0;
	DECLARE_SYMBOL(int, fmtlen) = strlen(fmt) + strlen(PMODNAM) + 2;
	DECLARE_SYMBOL(char, *_fmt) = kmalloc((fmtlen * sizeof(char)) + 15,
			GFP_KERNEL);

	strcat(_fmt, " [Debug] ");
	strcat(_fmt, PMODNAM);
	strcat(_fmt, fmt);
	va_start(args, _fmt);
	r = vfprintf(logfile, _fmt, args);
	va_end(args);

	kfree(_fmt);
	return r;
}

int _logi(const char *fmt, ...) {

	if (level < info)
		return 0;

	DECLARE_SYMBOL(va_list, args);
	DECLARE_SYMBOL(int, r) = 0;
	DECLARE_SYMBOL(int, fmtlen) = strlen(fmt) + strlen(PMODNAM) + 2;
	DECLARE_SYMBOL(char, *_fmt) = kmalloc((fmtlen * sizeof(char)) + 15,
			GFP_KERNEL);

	strcat(_fmt, " [Info] ");
	strcat(_fmt, PMODNAM);
	strcat(_fmt, fmt);
	va_start(args, _fmt);
	r = vfprintf(logfile, _fmt, args);
	va_end(args);

	kfree(_fmt);
	return r;
}

int _loga(const char *fmt, ...) {

	if (level < alert)
		return 0;

	DECLARE_SYMBOL(va_list, args);
	DECLARE_SYMBOL(int, r) = 0;
	DECLARE_SYMBOL(int, fmtlen) = strlen(fmt) + strlen(PMODNAM) + 2;
	DECLARE_SYMBOL(char, *_fmt) = kmalloc((fmtlen * sizeof(char)) + 15,
			GFP_KERNEL);

	strcat(_fmt, " [Alert] ");
	strcat(_fmt, PMODNAM);
	strcat(_fmt, fmt);
	va_start(args, _fmt);
	r = vfprintf(logfile, _fmt, args);
	va_end(args);

	kfree(_fmt);
	return r;
}

int _logw(const char *fmt, ...) {

	if (level < warning)
		return 0;

	DECLARE_SYMBOL(va_list, args);
	DECLARE_SYMBOL(int, r) = 0;
	DECLARE_SYMBOL(int, fmtlen) = strlen(fmt) + strlen(PMODNAM) + 2;
	DECLARE_SYMBOL(char, *_fmt) = kmalloc((fmtlen * sizeof(char)) + 15,
			GFP_KERNEL);

	strcat(_fmt, " [Warn] ");
	strcat(_fmt, PMODNAM);
	strcat(_fmt, fmt);
	va_start(args, _fmt);
	r = vfprintf(logfile, _fmt, args);
	va_end(args);

	kfree(_fmt);
	return r;
}

int _logc(const char *fmt, ...) {

	if (level < critical)
		return 0;

	DECLARE_SYMBOL(va_list, args);
	DECLARE_SYMBOL(int, r) = 0;
	DECLARE_SYMBOL(int, fmtlen) = strlen(fmt) + strlen(PMODNAM) + 2;
	DECLARE_SYMBOL(char, *_fmt) = kmalloc((fmtlen * sizeof(char)) + 15,
			GFP_KERNEL);

	strcat(_fmt, " [Critical] ");
	strcat(_fmt, PMODNAM);
	strcat(_fmt, fmt);
	va_start(args, _fmt);
	r = vfprintf(logfile, _fmt, args);
	va_end(args);

	kfree(_fmt);
	return r;
}

int _loge(const char *fmt, ...) {

	if (level < error_)
		return 0;

	DECLARE_SYMBOL(va_list, args);
	DECLARE_SYMBOL(int, r) = 0;
	DECLARE_SYMBOL(int, fmtlen) = strlen(fmt) + strlen(PMODNAM) + 2;
	DECLARE_SYMBOL(char, *_fmt) = kmalloc((fmtlen * sizeof(char)) + 15,
			GFP_KERNEL);

	strcat(_fmt, " [Error] ");
	strcat(_fmt, PMODNAM);
	strcat(_fmt, fmt);
	va_start(args, _fmt);
	r = vfprintf(logfile, _fmt, args);
	va_end(args);

	kfree(_fmt);
	return r;
}

int _logf(const char *fmt, ...) {

	if (level < fatal)
		return 0;

	DECLARE_SYMBOL(va_list, args);
	DECLARE_SYMBOL(int, r) = 0;
	DECLARE_SYMBOL(int, fmtlen) = strlen(fmt) + strlen(PMODNAM) + 2;
	DECLARE_SYMBOL(char, *_fmt) = kmalloc((fmtlen * sizeof(char)) + 15,
			GFP_KERNEL);

	strcat(_fmt, " [Fatal] ");
	strcat(_fmt, PMODNAM);
	strcat(_fmt, fmt);
	va_start(args, _fmt);
	r = vfprintf(logfile, _fmt, args);
	va_end(args);

	kfree(_fmt);
	return r;
}

void set_logging_level(kLoggingLevel _level) {
	level = _level;
}

FILE* __get_logfile(void) {
	if (logfile)
		return logfile;
	return stderr;
}

void __set_file(const char *file) {

	if (logfile) {
		fclose(logfile);
		logfile = NULL;
	}

	if (logfile == NULL) {
		logfile = fopen(file, "aw+");
		setvbuf(logfile, NULL, _IOLBF, 1024);
	}

}

logger_t get_logger_instance(void) {

	if (logfile == NULL) {
		logfile = fopen("thor-logs", "aw+");
		setvbuf(logfile, NULL, _IOLBF, 1024);
	}

	logger.a = _loga;
	logger.c = _logc;
	logger.d = _logd;
	logger.e = _loge;
	logger.f = _logf;
	logger.i = _logi;
	logger.v = _logv;
	logger.w = _logw;
	logger.get = __get_logfile;
	logger.set_file = __set_file;

	return logger;
}
