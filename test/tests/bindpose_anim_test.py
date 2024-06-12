# import for visual studio intellisence
try:
    pbt
except Exception:
    import pbt
    import numpy as np

skeleton = pbt.InputSkeletonBuffer()
output = pbt.OutputAnimBuffer()

def main():

    print("bone count", skeleton.bone_count())
    print(skeleton.names)