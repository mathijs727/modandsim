#version 330

in vec2 v_texCoord;

out vec3 color;

uniform sampler2D u_sampler;

void main()
{
	color = texture(u_sampler, v_texCoord).rgb;
}