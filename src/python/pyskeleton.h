#ifndef _INC_PBT_PYTHONSKELETON_
#define _INC_PBT_PYTHONSKELETON_

#include "python.h"
#include "../skeleton.h"

bool bpt_python_fill_skeleton_struct_from_output_buffer(PyObject* instance, PbtSkeleton* skeleton);
bool bpt_python_fill_input_buffer_with_skeleton(PbtSkeleton* skeleton, PyObject* instance);

#endif