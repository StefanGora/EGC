#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;


void main()
{
    // TODO(student): Compute world space vectors
    vec3 vertex_position_world_space = vec3(Model * vec4(v_position,1)); 
	vec3 light_direction = normalize(light_position - vertex_position_world_space); //L
	vec3 vertex_normal_world_space = normalize(vec3(Model * vec4(v_normal,0))); // N
	vec3 viz_world_space = normalize(eye_position - vertex_position_world_space ); //V
	vec3 median_world_space = normalize (light_direction + viz_world_space); //H

    // TODO(student): Define ambient light component
    float ambient_light = 0.25;

    // TODO(student): Compute diffuse light component
    float light_intensity = 1 .0;
	float diffuse_light = material_kd * light_intensity * max(dot(vertex_normal_world_space, light_direction), 0);

    // TODO(student): Compute specular light component
    float specular_light;



    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Gouraud (1971) shading method. There is also the Phong (1975) shading
    // method, which we'll use in the future. Don't mix them up!
    
    if (diffuse_light > 0)
	{
	//V1 - Phong
	//vec3 R = reflect(-light_direction, vertex_normal_world_space);
	//specular_light = material_ks * light_intensity * pow(max(dot(viz_world_space, R), 0), material_shininess);

	//V2 cu vectorul median - Blinn
	specular_light = material_ks * light_intensity * pow(max(dot(vertex_normal_world_space, median_world_space), 0), material_shininess);
	}

    // TODO(student): Compute light
    float d = distance(light_position,vertex_position_world_space);
	float aFactor = 1/(d*d+1);
	float light	= ambient_light + aFactor * (diffuse_light + specular_light);

    // TODO(student): Send color light output to fragment shader
    color = object_color * light;

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
