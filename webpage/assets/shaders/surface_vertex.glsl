// default vertex shader, based on the userland example codes
//   all right reserved and all artistic liberties taken.
//
//      DieAnthropologischeKonstante 2026
//
// Declares an attribute variable named vertex of type vec4
// to hold the position of each vertex in the object space.

attribute vec4 vertex;

// Declares a varying variable named tcoord of type vec2.

varying vec2 tcoord;

void main(void) // entry point for the vertex shader.
{
// Copies the vertex position to a temporary variable pos.
// This is typically done to apply transformations or manipulations
// to the vertex position before assigning it to gl_Position.

vec4 pos = vertex;

// Sets the final position of the vertex in homogeneous coordinates.
// This is a required step in the vertex shader to determine where the
// vertex will appear on the screen after projection.

gl_Position = pos;

// Calculates the texture coordinates (tcoord) by scaling
// and shifting the xy coordinates of the vertex.
// This step ensures that the texture coordinates are in the range [0,1],

tcoord = vertex.xy * 0.5 + 0.5;
}
