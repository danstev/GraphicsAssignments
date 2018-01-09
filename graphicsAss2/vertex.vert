#version 420
layout(location = 0) in vec3 position;
layout(location = 1) in vec4 colour;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 texcoord;

uniform mat4 model, view, projection, tex_matrix;
uniform mat3 normalmatrix;
uniform vec4 lightpos;
uniform uint disp;

out vec3 fnormal, flightdir, fposition;
out vec4 fdiffusecolour;
out vec2 ftexcoord;
flat out uint displace;

vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);

void main()
{
	vec4 position_h = vec4(position, 1.0);
	vec3 light_pos3 = lightpos.xyz;		

	fdiffusecolour = colour;

	mat4 mv_matrix = view * model;
	fposition = (mv_matrix * position_h).xyz;	
	fnormal = normalize(normalmatrix * normal);
	flightdir = light_pos3 - fposition;	

	ftexcoord = texcoord.xy;
	uint displace = disp;
	gl_Position = (projection * view * model) * position_h;
}