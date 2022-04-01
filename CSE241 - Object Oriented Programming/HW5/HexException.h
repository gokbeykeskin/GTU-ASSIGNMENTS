#ifndef HexException_H
#define HexException_H

#include <exception>
#include <stdexcept>
using namespace std;
namespace gokbeysHex{
	class HexException:public runtime_error{ //operator() indexes are out of are range.
	public:
		HexException():runtime_error("THESE INDEXES DOESN'T EXIST"){/*INTENTIONALLY EMPTY*/}
	};

	class HexException2:public runtime_error{ //lastMove()
		public:
			HexException2():runtime_error("THERE IS NO LAST MOVE"){}
	};
}





#endif //HexException_H
