// zerop().

// General includes.
#include "base/cl_sysdep.h"

// Specification.
#include "cln/lfloat.h"


// Implementation.

#include "float/lfloat/cl_LF.h"

namespace cln {

CL_INLINE bool CL_INLINE_DECL(zerop) (const cl_LF& x)
{
	return TheLfloat(x)->expo == 0;
}

}  // namespace cln
