#include "types.h"
#include "ReferenceCounted.h"

namespace kst {
	class Node : public ReferenceCounted {
	public:
		float3 position;
	};
}