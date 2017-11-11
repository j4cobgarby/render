# My first renderer!

## What it does

It renders a vector of AABBs, each with their own colour, position and size, to an image.

## How it does it

Given an output resolution; a camera origin, focal length, plane size, and direction; and a vector of AABBs, 
it sends `resolutionx * resolutiony` rays, each coming from the camera origin and passing through evenly
distributed points on the focal plane. Each of these points is then checked to see if it hits a box.

If a ray hits a box, the colour of the first box it hits is used, and the current ray's corresponding position
in the image is set to the given colour.

However before this colour is set in the image, it's darkened by a value based on how far away the point of
contact is from the origin of the camera.

## Dependencies

 - SFML >2

## How to use it

Make sure you have the required depencies, and then..

```
git clone https://github.com/j4cobgarby/render.git
cd render
mkdir build && cd build
cmake ../src -G <Whichever project type you want. Don't use the -G flag to use the system default>
make
./render
```