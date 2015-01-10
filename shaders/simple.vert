attribute highp vec4 vertex;
attribute mediump vec2 texCoord;
attribute mediump vec3 solidColor;

varying mediump vec2 texc;
varying mediump vec3 color;

uniform mediump mat4 modelMatrix;
uniform mediump mat4 cameraToClipMatrix;

void main(void)
{
    vec4 modelPos = modelMatrix * vertex;
    gl_Position = cameraToClipMatrix * modelPos;
    texc = texCoord;
    color = solidColor;
}