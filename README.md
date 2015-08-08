### RayTracer

![Raw Content](http://i.imgur.com/c61Pfsc.png)

A side project I learned from Caleb Piercy to understand about lighting and shadows. 

As noted, raytracer works by emitting rays the size of (width) x (height) of the photo, reflecting the rays if they hit a surface, and determining the colors of the surface if the rays hit the light source.

In this case, the image shown above is 320x240 (image is magnified for clarity), therefore the raytracer emits a total of 76800 rays onto the frustum. All surfaces that are hit by the 76800 rays emitted from the camera view will reflect accordingly. This is calculated by obtaining the normal vector of the surface, followed by finding the reflected vector of the light using the following algorithm:

![Algorithm](http://i.imgur.com/QkFjfZS.png)

Where ![RayDirection](http://i.imgur.com/lUQbj59.png) is the ray direction, or a point with a directional vector, and ![Normal](https://i.imgur.com/AylQdSP.png) is the surface normal.

Calculating the surface normal requires using the following algorithm:

![NormalCalculation](http://i.imgur.com/g07aJbC.png)

Where ![A](https://i.imgur.com/cfS5LhR.png), ![B](https://i.imgur.com/W465ySO.png), and ![C](https://i.imgur.com/jSOrhAr.png) are all points on a 3D plane, and ![magnitude](https://i.imgur.com/KnVnh2D.png) is the magnitude or length of the cross product.

Once the rays have been reflected and intersects a light source, the rays closest to the light source will have a brighter hue color on the reflected surface, where the initial ray intersection is at.

Rays with multiple reflections will have the Nth intersection color be mapped onto the N-1th intersection.

Rays with two or more intersections (and no reflections) will emit a shadow, or a darker hue color of the surface, starting from the 3rd intersection. There will be no light reflected after the 4th intersection.

The end result will give an illusion of showing objects with a reflective surface and shadows.

Credits:

[Caleb Piercy - Raytracer C++ Tutorial](https://www.youtube.com/watch?v=k_aRiYSXcyo)
