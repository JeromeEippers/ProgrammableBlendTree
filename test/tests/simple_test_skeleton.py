# import for visual studio intellisence
try:
    pbt
except Exception:
    import pbt
    import numpy as np

output = pbt.SkeletonWrite()

def main():

    output.names = ['Hips', 'LeftUpLeg', 'RightUpLeg', 'Spine', 'Chest', 'LeftArm', 'RightArm', 'Head']
    output.parents = np.array([-1, 0, 0, 0, 3, 4, 4, 4], dtype=np.int32)
    bone_count = len(output.names)
    output.pos = np.zeros([bone_count, 3], dtype=np.float32)
    output.quats = np.array([1,0,0,0], dtype=np.float32)[np.newaxis, :].repeat(bone_count, axis=0)

    output.pos[3, :] = [10, 2, 3]
    output.quats[1, :] = [-0.707,0,.707,0]
    output.quats[2, :] = [0.707,0,.707,0]
    