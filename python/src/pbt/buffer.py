class AnimBuffer:

    def __init__(self, stack_index):
        self.stack_index = stack_index


class InputAnimBuffer (AnimBuffer):
    pass

class OutputAnimBuffer (AnimBuffer):
    pass

class SkeletonBuffer:

    def __init__(self):
        self.names = []
        self.parents = None
        self.pos = None
        self.quats = None

    def bone_count(self):
        return len(self.names)
    
    def _get_names(self):
        return self.names
    

class InputSkeletonBuffer (SkeletonBuffer):
    pass

class OutputSkeletonBuffer (SkeletonBuffer):
    pass