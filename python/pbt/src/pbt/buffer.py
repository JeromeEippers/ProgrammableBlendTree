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
    
    def bone_index(self, name):
        return self.names.index(name)
    
    def _get_names(self):
        return self.names
    
    def _get_parents(self):
        return self.parents.flatten().tolist()
    
    def _get_positions(self):
        return self.pos.flatten().tolist()
    
    def _get_quats(self):
        return self.quats.flatten().tolist()
    

class InputSkeletonBuffer (SkeletonBuffer):
    pass

class OutputSkeletonBuffer (SkeletonBuffer):
    pass
