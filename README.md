### Requirements list:
1. Project triangles
2. Rasterize triangle
3. Interpolate vertex attributes
4. Pass fragment to the fragment shader
5. Render to a software target, a continuous region of memory (e.g. array or vector) of a parametric type target_t which holds the rendered image in row-major format
6. Pipeline must be agnostic with respect to type (target_t) and dimensions of the target. 
7. The fragment shader, when producing a fragment, must provide a target_t.
8. Allow for generic vertex information, i.e., the combination of object-shader determines the data flow from rasterizer and shader.
9. Implement the shader in terms of a strategy pattern, i.e., a call to a (statically) polymorphic function in a class held by the pipeline, but hot-swappable.
10. add management of objects, their properties (i.e., textures), and the shaders to be applied to those objects.
11. Visibility Culling: object keeps its bounding box and the system verifies its visibility before sending it to the pipeline.
12. Transformations and animations: objects expose their transformations for the physics engine and can contain multiple meshes for animations. The physics engine (not to be implemented, but design its interaction with the pipeline) can then change mesh and position of the object according to the logic of the program.
13. Apply a data-oriented design to the full pipeline.