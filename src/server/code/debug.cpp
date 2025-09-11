# include "debug.hpp"

#include <time.h>
#include <string.h>
#include <unistd.h>
# include <stdio.h>

static const char* get_time(void) {
  static char timeTxt[55];
  time_t rawtime;
  struct tm * timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  snprintf(timeTxt, 54,"%s", asctime(timeinfo));
  int len = strlen(timeTxt);
  while (len > 0) {
    if (timeTxt[len] == '\n') {
      timeTxt[len] = '\0';
      break ;
    }
    len--;
  }
  return (timeTxt);
}

void set_time(char* const txt, size_t len) {
  const char* t = get_time();
  snprintf(txt, len, "%s", t);
}

void print_error(t_setting* setting, const char* msg, ...) {
  char buff[BUFFER_SIZE + 1];
  char output[BUFFER_SIZE * 2 + 1];
  va_list list;
  va_start(list, msg);
  vsnprintf(buff, BUFFER_SIZE, msg, list);
  va_end(list);
  const char* time = get_time();
  if (setting->color)
    snprintf(output, BUFFER_SIZE * 2, "%s[%s]%s%s\n", REDCS, time, buff, RESETCS);
  else
    snprintf(output, BUFFER_SIZE * 2, "[%s]%s\n", time, buff);
  write(STDERR_FILENO, output, strlen(output));
}

void print_warning(t_setting* setting, const char* msg, ...) { 
  char buff[BUFFER_SIZE + 1];
  char output[BUFFER_SIZE * 2 + 1];
  va_list list;
  va_start(list, msg);
  vsnprintf(buff, BUFFER_SIZE, msg, list);
  va_end(list);
  const char* time = get_time();
  if (setting->color)
    snprintf(output, BUFFER_SIZE * 2, "%s[%s]%s%s\n", YELCS, time, buff, RESETCS);
  else
    snprintf(output, BUFFER_SIZE * 2, "[%s]%s\n", time, buff);
  write(STDERR_FILENO, output, strlen(output));
}

void print_sucsses(t_setting* setting, const char* msg, ...) {
  char buff[BUFFER_SIZE + 1];
  char output[BUFFER_SIZE * 2 + 1];
  va_list list;
  va_start(list, msg);
  vsnprintf(buff, BUFFER_SIZE, msg, list);
  va_end(list);
  const char* time = get_time();
  if (setting->color)
    snprintf(output, BUFFER_SIZE * 2, "%s[%s]%s%s\n", GRNCS, time, buff, RESETCS);
  else
    snprintf(output, BUFFER_SIZE * 2, "[%s]%s\n", time, buff);
  write(STDERR_FILENO, output, strlen(output));
}


void print_notify(t_setting* setting, const char* msg, ...) {
  char buff[BUFFER_SIZE + 1];
  char output[BUFFER_SIZE * 2 + 1];
  va_list list;
  va_start(list, msg);
  vsnprintf(buff, BUFFER_SIZE, msg, list);
  va_end(list);
  const char* time = get_time();
  if (setting->color)
    snprintf(output, BUFFER_SIZE * 2, "%s[%s]%s%s\n", CYNCS, time, buff, RESETCS);
  else
    snprintf(output, BUFFER_SIZE * 2, "[%s]%s\n", time, buff);
  write(STDERR_FILENO, output, strlen(output));
}