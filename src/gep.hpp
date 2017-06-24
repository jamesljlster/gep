#ifndef __GEP_H__
#define __GEP_H__

namespace gep
{
	struct GEP_TERMINAL
	{
		int type;
		double value;
	};

	struct GEP_OPERATOR
	{
		int type;
		int prefixOp;
		int op;
	};

	union GEP_NODE
	{
		int type;
		struct GEP_TERMINAL t;
		struct GEP_OPERATOR op;
	};
}

#endif
