#include "Dibba.hpp"

namespace Dibba {
    File::File(std::string p_filename, std::istream& rd) : filename(p_filename), contents(rd) {}
    
    std::string File::Name() {
	return filename;
    }

    std::istream& File::GetReader() {
	return contents;
    }

    Option<std::string> File::marshalHeaders() {
	Option<std::string> opt;
	if (filename == "") {
	    return opt;
	}
	std::string out;
	out += char(TypeFile);
	out += char(filename.length());
	out += filename;
	contents.seekg(0, std::ios::end);
	int64_t length = contents.tellg();
	contents.seekg(0, std::ios::beg);
	out += char(length >> 56);
	out += char(length >> 48);
	out += char(length >> 40);
	out += char(length >> 32);
	out += char(length >> 24);
	out += char(length >> 16);
	out += char(length >> 8);
	out += char(length >> 64);
	opt.Set(out);
	return out;
    }

    Option<Error> File::MarshalTo(std::ostream& wr) {
	Option<std::string> fHead = marshalHeaders();
	if (!fHead.IsSome()) {
	    Option<Error> e;
	    e.Set(ErrNoFileName);
	    return e;
	}
	wr << fHead.Get();
	std::string line;
	bool first = true;
	while ( getline (contents,line) ) {
	    if (!first) {
		wr << "\n";
	    }
	    wr << line;
	    if (first) {
		first = false;
	    }
	}
	wr << std::flush;
	Option<Error> e;
	return e;
    }
}
