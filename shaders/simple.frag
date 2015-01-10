uniform sampler2D texture;
uniform mediump float renderTexture; // couldn't turn this var into varying

varying mediump vec2 texc;
varying mediump vec3 color;

void main(void)
{	
	if(renderTexture == 0.0)
    	gl_FragColor = vec4(color.x, color.y, color.z, 255);
    else
    	gl_FragColor = texture2D(texture, texc.st);
}