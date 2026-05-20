#include <stdlib.h>
#include <string.h>

static const char table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

char *base64_encode(const char *input) {
    size_t len = strlen(input);
    size_t out_len = 4 * ((len + 2) / 3);
    char *out = malloc(out_len + 1);

    if (!out)
        return NULL;

    size_t i = 0;
    size_t j = 0;

    while (i < len) {
        unsigned octet_a = i < len ? (unsigned char)input[i++] : 0;
        unsigned octet_b = i < len ? (unsigned char)input[i++] : 0;
        unsigned octet_c = i < len ? (unsigned char)input[i++] : 0;
        unsigned triple = (octet_a << 16) | (octet_b << 8) | octet_c;

        out[j++] = table[(triple >> 18) & 0x3F];
        out[j++] = table[(triple >> 12) & 0x3F];
        out[j++] = table[(triple >> 6) & 0x3F];
        out[j++] = table[triple & 0x3F];
    }

    for (size_t k = 0; k < (3 - len % 3) % 3; k++) {
        out[out_len - 1 - k] = '=';
    }

    out[out_len] = '\0';

    return out;
}