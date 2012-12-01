#include "include.hpp"

class SockError : public exception {
	string msg;

public:
	SockError(const string& _msg) throw()
	: msg(_msg){
		msg.append(": ");
		msg.append(strerror(errno));
	}

  ~SockError() throw() {};
};

