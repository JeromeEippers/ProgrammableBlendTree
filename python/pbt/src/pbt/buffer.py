import numpy as np

class AnimBuffer:

    def __init__(self, stack_index):
        self.stack_index = stack_index
        self.pos = None
        self.quats = None

    def bone_count(self):
        return self.pos.shape[1]
    
    def frame_count(self):
        return self.pos.shape[0]

    def _get_positions(self):
        return self.pos.flatten().tolist()
    
    def _get_quats(self):
        return self.quats.flatten().tolist()
    
    def _set_positions(self, pos, bone_count):
        self.pos = np.array(pos, dtype=np.float32).reshape(-1, bone_count, 3)

    def _set_quats(self, quats, bone_count):
        self.quats = np.array(quats, dtype=np.float32).reshape(-1, bone_count, 4)


class InputAnimBuffer (AnimBuffer):
    pass

class OutputAnimBuffer (AnimBuffer):
    def __init__(self):
        super(OutputAnimBuffer, self).__init__(0)

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
    
    def _set_names(self, names):
        self.names = names

    def _set_parents(self, parents):
        self.parents = np.array(parents, dtype=np.int32)

    def _set_positions(self, pos):
        self.pos = np.array(pos, dtype=np.float32).reshape(-1, 3)

    def _set_quats(self, quats):
        self.quats = np.array(quats, dtype=np.float32).reshape(-1, 4)
    

class InputSkeletonBuffer (SkeletonBuffer):
    pass

class OutputSkeletonBuffer (SkeletonBuffer):
    pass
