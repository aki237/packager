#include "Dibba.hpp"
#include <iostream>
#include <stdint.h>
#include <sstream>

namespace Dibba {
    std::string string_to_hex(const std::string& input) {
	static const char* const lut = "0123456789abcdef";
	size_t len = input.length();

	std::string output;
	output.reserve(2 * len);
	for (size_t i = 0; i < len; ++i) {
	    const unsigned char c = input[i];
	    output.push_back(lut[c >> 4]);
	    output.push_back(lut[c & 15]);
	}
	return output;
    }
    
    DibbaReader::DibbaReader(std::istream& f) : rd(f) {}
    
    Option<Error> DibbaReader::Parse() {
	if (!checkIntegrity()) {
	    return Option<Error>(ErrMalformed);
	}
	rd.seekg(4, std::ios::beg);
	while (true) {
	    char readh;
	    rd.get(readh);
	    if (readh != TypeFile) {
		if (readh == TypeDibbaEnder) {
		    return Option<Error>();
		}
		return Option<Error>(ErrMalformed);
	    }
	    rd.get(readh);
	    int length = int(readh);
	    char filename[length+1];
	    rd.get(filename, length + 1);
	    std::string sfilename = std::string(filename);
	    int i = 0;
	    std::string len;
	    while (i < 8) {
		char filelen;
		rd.get(filelen);
		len += filelen;
		i++;
	    }
	    int64_t x;
	    std::stringstream ss;
	    ss << std::hex << string_to_hex(len);
	    ss >> x;
	    int64_t start = int64_t(rd.tellg());
	    rd.seekg(x, std::ios::cur);
	    fileBounds tfb;
	    tfb.filename = sfilename;
	    tfb.start = start;
	    tfb.length = x;
	    fb.push_back(tfb);
	}
    }
    
    Option<std::string> DibbaReader::Read(std::string rfile) {
	for (auto const& val : fb) {
	    if (rfile != val.filename) {
		continue;
	    }
	    rd.seekg(val.start, std::ios::beg);
	    char read;
	    std::string fileContents;
	    int64_t count = 0;
	    while (rd.get(read)) {
		fileContents += read;
		count += 1;
		if (count == val.length) {
		    break;
		}
	    }
	    return Option<std::string>(fileContents);
	}
	return Option<std::string>();
    }

    bool DibbaReader::checkIntegrity() {
	rd.seekg(0, std::ios::beg);
	char read;
	rd.get(read);
	if (read != TypeDibbaHeader) {
	    return false;
	}
	rd.get(read);
	if (read != 'D') {
	    return false;
	}
	rd.get(read);
	if (read != 'I') {
	    return false;
	}
	rd.get(read);
	if (read != 'B') {
	    return false;
	}
	rd.seekg(-1, std::ios::end);
	rd.get(read);
	if (read != TypeDibbaEnder) {
	    return false;
	}
	return true;
    }

    std::vector<std::string> DibbaReader::FileList() {
	std::vector<std::string> fl;
	for (auto const& val : fb) {
	    fl.push_back(val.filename);
	}
	return fl;
    }
}
