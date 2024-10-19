#include <stdarg.h>
#include <stddef.h>

int snprintf(char *str, size_t size, const char *format, ...) {
    va_list args;
    va_start(args, format);
    int written = 0;
    const char *p = format;

    while (*p != '\0' && written < size - 1) {
        if (*p == '%' && *(p + 1) != '\0') {
            p++;
            if (*p == 's') {
                char *s = va_arg(args, char *);
                while (*s != '\0' && written < size - 1) {
                    str[written++] = *s++;
                }
            } else if (*p == 'd' || *p == 'i') {
                int num = va_arg(args, int);
                char buffer[20];
                int len = 0;
                if (num < 0) {
                    str[written++] = '-';
                    num = -num;
                }
                do {
                    buffer[len++] = '0' + (num % 10);
                    num /= 10;
                } while (num > 0);
                for (int i = len - 1; i >= 0 && written < size - 1; i--) {
                    str[written++] = buffer[i];
                }
            } else if (*p == 'z' && *(p + 1) == 'u') {
                p++;
                size_t num = va_arg(args, size_t);
                char buffer[20];
                int len = 0;
                do {
                    buffer[len++] = '0' + (num % 10);
                    num /= 10;
                } while (num > 0);
                for (int i = len - 1; i >= 0 && written < size - 1; i--) {
                    str[written++] = buffer[i];
                }
            }
        } else {
            str[written++] = *p;
        }
        p++;
    }

    str[written] = '\0';
    va_end(args);
    return written;
}