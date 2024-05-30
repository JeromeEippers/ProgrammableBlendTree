#ifndef _INC_PBT_PYTHONSKELETON_
#define _INC_PBT_PYTHONSKELETON_

#include "python.h"
#include "../skeleton.h"

bool bpt_python_fill_skeleton_struct_from_input_buffer(PyObject* instance, PbtSkeleton* skeleton);

#endif