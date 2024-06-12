#ifndef _INC_PBT_PYTHONANIMATION_
#define _INC_PBT_PYTHONANIMATION_

#include "python.h"
#include "../skeleton.h"
#include "../animation.h"

PbtAnimation * pbt_python_create_animation_from_file(const char* python_script_path, PbtSkeleton* skeleton);
PbtAnimation * pbt_python_create_animation_from_string(const char* python_script, PbtSkeleton* skeleton);

bool bpt_python_fill_animation_struct_from_output_buffer(PyObject* instance, PbtSkeleton* skeleton, PbtAnimation* animation);

#endif