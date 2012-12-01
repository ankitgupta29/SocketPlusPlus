#include "include.hpp"

class AddrError : public exception {
	string msg;

public:
	AddrError(const string& _msg) throw()
	: msg(_msg){
		msg.append(": ");
		msg.append( strerror(errno) );
	}

	AddrError( const string& _msg, int family_type ) throw()
	: msg(_msg){
		stringstream s;
		s << family_type;
		msg.append( ": Unknown family type (" + s.str() + ").");
	}

  ~AddrError() throw() {};

};

