# import for visual studio intellisence
try:
    pbt
except Exception:
    import pbt
    import numpy as np

output = pbt.OutputSkeletonBuffer()

def main():

    output.names = ['Hips', 'LeftUpLeg', 'RightUpLeg', 'Spine', 'Chest', 'LeftArm', 'RightArm', 'Head']
    output.parents = np.array([-1, 0, 0, 0, 3, 4, 4, 4], dtype=np.int32)
    output.pos = np.zeros([output.bone_count(), 3], dtype=np.float32)
    output.quats = np.array([1,0,0,0], dtype=np.float32)[np.newaxis, :].repeat(output.bone_count(), axis=0)

