attribute highp vec4 vertex;
attribute mediump vec4 texCoord;
attribute highp vec3 color;
varying mediump vec4 fragColor;

uniform mediump mat4 modelMatrix;
uniform mediump mat4 cameraToClipMatrix;

void main(void)
{
    vec4 modelPos = modelMatrix * vertex;
    gl_Position = cameraToClipMatrix * modelPos;

	fragColor = vec4(color.x,color.y,color.z,255);
}