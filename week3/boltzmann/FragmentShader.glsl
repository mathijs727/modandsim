#version 120

varying vec2 v_texCoord;

uniform sampler2D u_sampler;

void main()
{
	gl_FragColor = vec4(texture2D(u_sampler, v_texCoord).rgb, 1.0);
}
