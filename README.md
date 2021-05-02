### Requirements list:

1. Project triangles
2. Rasterize triangle
3. Interpolate vertex attributes
4. Pass fragment to the fragment shader
5. Render to a software target, a continuous region of memory (e.g. array or vector) of a parametric type target_t which
   holds the rendered image in row-major format
6. Pipeline must be agnostic with respect to type (target_t) and dimensions of the target.
7. The fragment shader, when producing a fragment, must provide a target_t.
8. Allow for generic vertex information, i.e., the combination of object-shader determines the data flow from rasterizer
   and shader.
9. Implement the shader in terms of a strategy pattern, i.e., a call to a (statically) polymorphic function in a class
   held by the pipeline, but hot-swappable.
10. add management of objects, their properties (i.e., textures), and the shaders to be applied to those objects.
11. Visibility Culling: object keeps its bounding box and the system verifies its visibility before sending it to the
    pipeline.
12. Transformations and animations: objects expose their transformations for the physics engine and can contain multiple
    meshes for animations. The physics engine (not to be implemented, but design its interaction with the pipeline) can
    then change mesh and position of the object according to the logic of the program.
13. Apply a data-oriented design to the full pipeline.

#### Assignment 1: Polymorphic Raster Pipeline

- Implement a software 3D rendering pipeline with programmable fragment shader.

- Given the triangles, transform them in view coordinates, project them, rasterize the triangles, interpolate the vertex
  attributes, and for each fragment pass the interpolated vertex to the fragment shader.

- Render to a software target, a continuous region of memory (e.g. array or vector) of a parametric type target_t which
  holds the rendered image in row-major format and can be used to display or save the rendered image. The pipeline must
  be agnostic with respect to type (target_t) and dimensions of the target. The fragment shader, when producing a
  fragment, must provide a target_t.

<!--    Implement the shader in terms of a strategy pattern, i.e., a call to a polymorphic function in a class held by the pipeline, but hot-swappable. 
        Assume a single known vertex definition with pre-specified attributes (3D coordinates, 3D normals and u,v coordinates) known by the polymorphic interface.
        We will relax these assumptions as well as move away from the polymorphic call in later iterations. Do not worry about textures at this stage.
-->

#### Assignment 2:

Complete the pipeline:

- Add management of objects, their properties (i.e., textures), and the shaders to be applied to those objects (the
  vertex + the texture file + the fragment shader (TODO: Only one or more that one?));

- Transform the dynamically polymorphic call to the shader into a static one;
- Allow for generic vertex information, i.e., the combination of object-shader determines the data flow from rasterizer
  and shader, and should not be a pre-determined property of the rasterizer as was in the previous assignment (i.e., the
  Vertex struct in my solution). Therefore, *each vertex* may contain several other information.
- Try and apply a data-oriented design to the full pipeline.

#### Project assignment:

- Visibility culling: un oggetto tiene la sua bounding box e il sistema verifica la visibilità dell'oggetto prima di
  mandarlo alla pipeline. (Se parzialmente visibile, decidere se tagliare e aggiungere vertici per creare i triangoli
  oppure passare il triangolo intero)
- Trasformazioni e animazione: ogni oggetto espone le sue trasformazioni per il motore fisico e può contenere più mesh
  per le animazioni. il motore fisico (da non implementare, ma da prevederne l'interazione con la pipeline) può così
  cambiare mesh e posizione dell'oggetto in seguito alla logica del programma grafico.

#### Various resources:
- https://stackoverflow.com/questions/24441631/how-exactly-does-opengl-do-perspectively-correct-linear-interpolation
- https://www.3dgep.com/understanding-the-view-matrix/
- http://www.songho.ca/opengl/
- https://en.wikibooks.org/wiki/GLSL_Programming/Rasterization
- https://joshbeam.com/articles/triangle_rasterization/
- https://www.youtube.com/watch?v=S3EVSxtsRl0
- https://www.khronos.org/registry/OpenGL/specs/gl/glspec44.core.pdf pag 427 IMPORTANTE