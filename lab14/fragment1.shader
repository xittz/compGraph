uniform vec4 objColor;
in vec4 ourColor;
out vec4 color;
void main()
{
	color = ourColor * objColor;
}