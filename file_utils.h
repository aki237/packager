#ifndef FILE_UTILS_H
#define FILE_UTILS_H
#include <magic.h>
#include <iostream>

bool get_encoding(const char* contents, size_t len) {
    const char *magic_full;
    magic_t magic_cookie;
    magic_cookie = magic_open(MAGIC_MIME_ENCODING);
    if (magic_cookie == NULL) {
        return false;
    }
    if (magic_load(magic_cookie, NULL) != 0) {
        magic_close(magic_cookie);
        return false;
    }
    magic_full = magic_buffer(magic_cookie, contents, len);
    std::cout << std::string(magic_full) << std::endl;
    if (std::string (magic_full) == "binary") {
        magic_close(magic_cookie);
        return false;
    }
    magic_close(magic_cookie);
    return true;
}

std::string get_mime_type(const char* contents, size_t len) {
    const char *magic_full;
    magic_t magic_cookie;
    magic_cookie = magic_open(MAGIC_MIME_TYPE);
    if (magic_cookie == NULL) {
        return "";
    }
    if (magic_load(magic_cookie, NULL) != 0) {
        magic_close(magic_cookie);
        return "";
    }
    magic_full = magic_buffer(magic_cookie, contents, len);
    std::cout << std::string(magic_full) << std::endl;
    std::string mime = std::string(magic_full);
    magic_close(magic_cookie);
    return mime;
}

#endif // FILE_UTILS_H
