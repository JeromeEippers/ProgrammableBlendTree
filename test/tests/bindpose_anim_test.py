# import for visual studio intellisence
try:
    pbt
except Exception:
    import pbt
    import numpy as np

skeleton = pbt.SkeletonRead()
output = pbt.AnimationWrite()

def main():

    output.pos = skeleton.pos.copy()[np.newaxis, ...]
    output.quats = skeleton.quats.copy()[np.newaxis, ...]
