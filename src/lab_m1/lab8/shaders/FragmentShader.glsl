#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms

uniform vec3 object_color;
uniform int type_light;
uniform float cut_off_angle;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec3 N = normalize(world_normal);
	vec3 L = normalize(light_position - world_position);
	vec3 V = normalize(eye_position - world_position);
	vec3 H = normalize(L + V);
	vec3 R = normalize(reflect(L, world_normal));

    // TODO(student): Define ambient, diffuse and specular light components
    float ambient_light = 0.25;
    float diffuse_light = material_kd * max(dot(normalize(N), L), 0);
    float specular_light = 0;

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(N, H), 0), material_shininess);
    }


    float light = 0; // daca nu avem spot lumina vine de la alte obiecte

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.
    if (type_light == 1) {   
        float cut_off = radians(cut_off_angle);
        float spot_limit = cos(cut_off);
        float spotlight = dot(-L, light_direction);

        if (spotlight > spot_limit) { 
            float linear_att = (spotlight - spot_limit) / (1.f - spot_limit);
            float light_att_factor = pow(linear_att, 2);
            light = ambient_light + light_att_factor * (diffuse_light + specular_light);
        } else {
            light = ambient_light;
        }
    }
    else {
        float d = distance(light_position, world_position);
        float attenuation_factor = 1 / (1 + 0.1 * d * d);
        light = ambient_light + attenuation_factor * (diffuse_light + specular_light);
    }

    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.

    // TODO(student): Write pixel out color
    out_color = vec4(object_color * light, 1.f);

}
