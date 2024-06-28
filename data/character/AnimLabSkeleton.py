# import for visual studio intellisence
try:
    pbt
except Exception:
    import pbt
    import numpy as np

output = pbt.SkeletonWrite()

def main():

    vbuffer, ibuffer, materials, sibuffer, swbuffer, bindXforms, restXforms, names, parents, name = pbt.usd.import_usd_asset('data/character/AnimLabSimpleMale.usd')
    q, p = pbt.utils.m4x4_to_qp(restXforms[np.newaxis, ...])

    output.names = names
    output.parents = parents
    output.pos = p[0]
    output.quats = q[0]

