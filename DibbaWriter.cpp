#include "Dibba.hpp"

namespace Dibba {
    DibbaWriter::DibbaWriter(std::ostream& output) : out(output) , fresh(true){}

    Option<Error> DibbaWriter::Init() {
	if (!fresh) {
	    return Option<Error>(ErrAlreadyCommitted);
	}
	fresh = false;
	out << char(TypeDibbaHeader) << 'D' << 'I' << 'B' ;
	return Option<Error>();
    }

    Option<Error> DibbaWriter::Add(File& f) {
	auto oe = f.MarshalTo(out);
	if (oe.IsSome()) {
	    return oe;
	}
	return Option<Error>();
    }
    
    Option<Error> DibbaWriter::Commit() {
	if (fresh) {
	    return Option<Error>(ErrNotInit);
	}
	out << char(TypeDibbaEnder);
	Option<Error> e;
	return e;
    }
}
