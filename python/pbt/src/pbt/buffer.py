import numpy as np



class OutputAnimation:

    def __init__(self):
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


class AnimStackBuffer:
    def __init__(self):
        self.pos = None
        self.quats = None

    def bone_count(self):
        return self.pos.shape[0]
    
    def _get_positions(self):
        return self.pos.flatten().tolist()
    
    def _get_quats(self):
        return self.quats.flatten().tolist()

    def _set_positions(self, pos):
        self.pos = np.array(pos, dtype=np.float32).reshape(-1, 3)

    def _set_quats(self, quats):
        self.quats = np.array(quats, dtype=np.float32).reshape(-1, 4)


class InputAnimBuffer (AnimStackBuffer):
    pass

class OutputAnimBuffer (AnimStackBuffer):
    pass


class SkeletonBuffer (AnimStackBuffer):

    def __init__(self):
        super(SkeletonBuffer, self).__init__()
        self.names = []
        self.parents = None

    def bone_index(self, name):
        return self.names.index(name)
    
    def _get_names(self):
        return self.names
    
    def _get_parents(self):
        return self.parents.flatten().tolist()
    
    def _set_names(self, names):
        self.names = names

    def _set_parents(self, parents):
        self.parents = np.array(parents, dtype=np.int32)

class InputSkeletonBuffer (SkeletonBuffer):
    pass

class OutputSkeletonBuffer (SkeletonBuffer):
    pass
