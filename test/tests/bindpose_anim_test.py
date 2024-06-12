# import for visual studio intellisence
try:
    pbt
except Exception:
    import pbt
    import numpy as np

skeleton = pbt.InputSkeletonBuffer()
output = pbt.OutputAnimBuffer()

def main():

    output.pos = skeleton.pos.copy()[np.newaxis, ...]
    output.quats = skeleton.quats.copy()[np.newaxis, ...]

    print('frame count', output.frame_count())