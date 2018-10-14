# sleek-surface
Sleek surface building without misplaced extremes based on the finite discrete point set.

It is an implementation of an algorithm described in <URL>

This algorithm gets finite discrete set of 3D points distributed by regular grid as input and builds 3D surface that looks sleek and in the same time has no misplaced extremes and no oscillations.

The core of this algorithm is [tbezierSO0](https://github.com/icosaeder/tbezier) routine that builds sleek curve without misplaced extremes. This routine is used to create a set of intersecting curves and then the surface is built as a set of joint bicubically blended [Coons patches](https://en.wikipedia.org/wiki/Coons_patch). The normals are calculated using [smoothing groups](https://en.wikipedia.org/wiki/Smoothing_group) algorithm and then additionally smoothed using [Gaussian filter](https://en.wikipedia.org/wiki/Gaussian_filter). The result surface has both high visual quality and high accuracy.

Written by Konstantin Ryabinin under terms of MIT license.

## Testing

You can compile the test application just by calling `make`.

The file `main.cpp` contains the hardcoded testing dataset described in the original paper. For testing purposes, the application prints to console the surface vertex data in the [Stanford Triangle Format](http://paulbourke.net/dataformats/ply/). So, you can run it as follows:
```
./main > out.ply
```
After that, you can view `out.ply` in some 3D model viewer, for example, import it in [Blender](https://www.blender.org/).
