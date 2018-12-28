in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture1;
uniform sampler2D ourTexture2;
uniform float coef;
void main()
{
	color = mix(texture2D(ourTexture1, TexCoord), texture2D(ourTexture2, TexCoord), coef);
}