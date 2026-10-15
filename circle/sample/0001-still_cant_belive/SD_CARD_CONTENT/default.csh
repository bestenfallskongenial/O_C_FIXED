#extension GL_OES_EGL_image_external : require

precision mediump float;

uniform samplerExternalOES vid;
varying vec2 tcoord;

void main(void)
{
    gl_FragColor = texture2D(vid, tcoord);
}