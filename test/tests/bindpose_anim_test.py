# import for visual studio intellisence
try:
    pbt
except Exception:
    import pbt
    import numpy as np

skeleton = pbt.InputSkeletonBuffer()
output = pbt.OutputAnimation()

def main():

    output.pos = skeleton.pos.copy()[np.newaxis, ...]
    output.quats = skeleton.quats.copy()[np.newaxis, ...]
