# Programmable blend tree

A c99 skeletal animation engine to experiment new ideas on the way we build animation graphs for games.

Usually a game engine will have :
* A blendtree with a lot of specialized nodes: 
    * blending space, layering, transitions, ...
* A few stages your animations goes through:
    * animation graph
    * blendtree
    * procedural animation, like IK
    * control rig

But what if we remove most of this and just give one type of node in the blendtree?  

## Programmable blend node

With a programmable blend node, a technical animator can write what he wants to compute exactly. He can write a blendspace, a transition, a layer with bonemask or a mix of all of these. He can compute an ik or a lookat in the middle of the blendtree if he needs it, right where he needs it. And not as a post process after the blendtree.

### What language ?
Python.  
* Because the language is usually known and used by the technical team already.
* Because it can be run easily when developping the feature. To get a 'ground truth' for the feature.
* It has already a builtin AST module (see performance)

### Performance.
Of course python can not be used in the final blendtree due to speed. The blendtree will need to be compiled. And we can do that compilation in python using the builtin AST module to parse the code. As a first test, I will use a virtual machine that will probably not have a lot of commands, but they will be vectorized with SIMD as much as possible. When compiling the code we will know the skeleton and we can try to optimize the needed calls and the memory layout.

## Development
### Dependencies

* python 3.10
* raylib

### Rough Task List

- [x] integrate python
- [x] load skeleton
- [x] create procedural animation
- [x] animation stack
- [ ] load animation
- [ ] blend tree
- [ ] virtual machine
- [ ] python to vm bytecode
- [ ] viewer

### Setup
Compiled on ubuntu, with a python3.10 virtual environment that must be in the folder python_venv

```
python3 -m venv python_venv

./python_venv/bin/pip3 install -r python/requirements.txt
```

