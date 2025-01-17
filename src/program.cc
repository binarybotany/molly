#include "program.h"

#include <stdexcept>

const char *vss = R"(
#version 330 core

layout (location = 0) in vec2 a_position;
layout (location = 1) in vec2 a_texture_coordinates;

out vec2 texture_coordinates;

void main() {
  gl_Position = vec4(a_position, 0.0, 1.0);
  texture_coordinates = a_texture_coordinates;
}
)";

const char *fss = R"(
#version 330 core

// by Timothy Lottes https://www.shadertoy.com/view/MsjXzh

out vec4 out_color;

in vec2 texture_coordinates;
uniform sampler2D screen_texture;
uniform vec2 viewport_resolution;

// Emulated input resolution
#define resolution (viewport_resolution / 6.0)

// Hardness of scanline
// -8.0  = soft
// -16.0 = medium
float hardness_scanline = -10.0;

// Hardness of pixels in scanline
// -2.0 = soft
// -4.0 = hard
float hardness_pixel = -4.0;

// Hardness of short vertical bloom
// -1.0 = wide to the point of clipping (bad)
// -1.5 = wide
// -4.0 = not very wide at all
float hardness_bloom_scan = -2.0;

// Hardness of short horizontal bloom
// -0.5 = wide to the point of clipping (bad)
// -1.0 = wide
// -2.0 = not very wide at all
float hardness_bloom_pixel = -1.5;

// Amount of small bloom effect
// 1.0 / 1.0  = only bloom
// 1.0 / 16.0 = what I think is a good amount of small bloom
// 0.0        = no bloom
float bloom_amount = 1.0 / 16.0; 

// Display warp
// 0.0       = none
// 1.0 / 8.0 = extreme
vec2 warp_amount = vec2(0.0, 0.0); 

// Amount of shadow mask
float mask_dark = 0.5;
float mask_light = 1.5;

float srgb_to_linear(float c) {
  return(c <= 0.04045) ? c / 12.92 : pow((c + 0.055) / 1.055, 2.4);
}

vec3 srgb_to_linear(vec3 c) {
  return vec3(srgb_to_linear(c.r), srgb_to_linear(c.g), srgb_to_linear(c.b));
}

float linear_to_srgb(float c) {
  return(c < 0.0031308 ? c * 12.92 : 1.055 * pow(c, 0.41666) - 0.055);
}

vec3 linear_to_srgb(vec3 c) {
  return vec3(linear_to_srgb(c.r), linear_to_srgb(c.g), linear_to_srgb(c.b));
}


// Nearest emulated sample given floating point position and texel offset.
// Also zero's offscreen
vec3 fetch(vec2 position, vec2 offset) {
  position = floor(position * resolution + offset) / resolution;
  if(max(
    abs(position.x - 0.5),
    abs(position.y - 0.5)
  ) > 0.5) return vec3(0.0, 0.0, 0.0);
  return srgb_to_linear(texture(screen_texture, position.xy, -16.0).rgb);
}

// Distance in emulated pixels to nearest texel
vec2 distance(vec2 position) {
  position = position * resolution;
  return -((position - floor(position)) - vec2(0.5));
}

// 1D Gaussian
float gaussian(float position, float scale) {
  return exp2(scale * position * position);
}

// 3-tap Gaussian filter along horizontal line
vec3 horizontal3(vec2 position, float offset) {
  vec3 b = fetch(position, vec2(-1.0, offset));
  vec3 c = fetch(position, vec2( 0.0, offset));
  vec3 d = fetch(position, vec2( 1.0, offset));
  float dist = distance(position).x;
  float scale = hardness_pixel;
  
  // Convert distance to weight
  float wb = gaussian(dist - 1.0, scale);
  float wc = gaussian(dist + 0.0, scale);
  float wd = gaussian(dist + 1.0, scale);

  // Return filtered sample
  return (b * wb + c * wc + d * wd) / (wb + wc + wd);
}

// 5-tap Gaussian filter along horz line.
vec3 horizontal5(vec2 position, float offset) {
  vec3 a = fetch(position, vec2(-2.0, offset));
  vec3 b = fetch(position, vec2(-1.0, offset));
  vec3 c = fetch(position, vec2( 0.0, offset));
  vec3 d = fetch(position, vec2( 1.0, offset));
  vec3 e = fetch(position, vec2( 2.0, offset));
  float dist = distance(position).x;
  
  // Convert distance to weight.
  float scale = hardness_pixel;
  float wa = gaussian(dist - 2.0, scale);
  float wb = gaussian(dist - 1.0, scale);
  float wc = gaussian(dist + 0.0, scale);
  float wd = gaussian(dist + 1.0, scale);
  float we = gaussian(dist + 2.0, scale);
  
  // Return filtered sample.
  return (a * wa + b * wb + c * wc + d * wd + e * we) 
    / (wa + wb + wc + wd + we);
}

// 7-tap Gaussian filter along horz line.
vec3 horizontal7(vec2 position, float offset) {
  vec3 a = fetch(position, vec2(-3.0, offset));
  vec3 b = fetch(position, vec2(-2.0, offset));
  vec3 c = fetch(position, vec2(-1.0, offset));
  vec3 d = fetch(position, vec2( 0.0, offset));
  vec3 e = fetch(position, vec2( 1.0, offset));
  vec3 f = fetch(position, vec2( 2.0, offset));
  vec3 g = fetch(position, vec2( 3.0, offset));
  float dist=distance(position).x;

  // Convert distance to weight.
  float scale = hardness_bloom_pixel;
  float wa = gaussian(dist - 3.0, scale);
  float wb = gaussian(dist - 2.0, scale);
  float wc = gaussian(dist - 1.0, scale);
  float wd = gaussian(dist + 0.0, scale);
  float we = gaussian(dist + 1.0, scale);
  float wf = gaussian(dist + 2.0, scale);
  float wg = gaussian(dist + 3.0, scale);
  
  // Return filtered sample.
  return (a * wa + b * wb + c * wc + d * wd + e * we + f * wf + g * wg) 
    / (wa + wb + wc + wd + we + wf + wg);
}

float scanline_weight(vec2 position, float offset) {
  float dist = distance(position).y;
  return gaussian(dist + offset, hardness_scanline);
}

float scanline_weight_bloom(vec2 position, float offset) {
  float dist = distance(position).y;
  return gaussian(dist + offset, hardness_bloom_scan);
}

// Allow nearest three lines to effect pixel
vec3 tri(vec2 position) {
  vec3 a = horizontal3(position,-1.0);
  vec3 b = horizontal5(position, 0.0);
  vec3 c = horizontal3(position, 1.0);
  float wa = scanline_weight(position,-1.0);
  float wb = scanline_weight(position, 0.0);
  float wc = scanline_weight(position, 1.0);
  return a * wa + b * wb + c * wc;
}

vec3 bloom(vec2 position){
  vec3 a = horizontal5(position, -2.0);
  vec3 b = horizontal7(position, -1.0);
  vec3 c = horizontal7(position,  0.0);
  vec3 d = horizontal7(position,  1.0);
  vec3 e = horizontal5(position,  2.0);
  float wa = scanline_weight_bloom(position, -2.0);
  float wb = scanline_weight_bloom(position, -1.0);
  float wc = scanline_weight_bloom(position,  0.0);
  float wd = scanline_weight_bloom(position,  1.0);
  float we = scanline_weight_bloom(position,  2.0);
  return a * wa + b * wb + c * wc + d * wd + e * we;
}

vec2 warp(vec2 position) {
  position = position * 2.0 - 1.0;    
  position *= vec2(
    1.0 + (position.y * position.y) * warp_amount.x, 
    1.0 + (position.x * position.x) * warp_amount.y);

  return position * 0.5 + 0.5;
}


vec3 aperture_grille_mask(vec2 position) {
  position.x = fract(position.x / 3.0);
  vec3 mask = vec3(mask_dark, mask_dark, mask_dark);
  
  if (position.x < 0.333) {
    mask.r = mask_light;
  }
  else if (position.x < 0.666) {
    mask.g = mask_light;
  }
  else { 
    mask.b = mask_light;
  }
  
  return mask;
}

void main() {
  vec2 position = warp(gl_FragCoord.xy / viewport_resolution.xy);
  vec3 color = tri(position) * aperture_grille_mask(position);

  color += bloom(position) * bloom_amount;

  out_color = vec4(linear_to_srgb(color), 1.0);
}
)";

uint32_t add_shader(program_t &program, uint32_t type, const char *source) {
  uint32_t shader = glCreateShader(type);
  glShaderSource(shader, 1, &source, nullptr);
  glCompileShader(shader);

#if DEBUG
  int32_t compiled;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
  if (!compiled) {
    int32_t log_length = 0;
    char message[1024];
    glGetShaderInfoLog(shader, 1024, &log_length, message);
    throw std::runtime_error(message);
  }
#endif

  glAttachShader(program.id, shader);
  return shader;
}

void program_controller::create(program_t &program) {
  program.id = glCreateProgram();
  uint32_t vs = add_shader(program, GL_VERTEX_SHADER, vss);
  uint32_t fs = add_shader(program, GL_FRAGMENT_SHADER, fss);
  glLinkProgram(program.id);

#if DEBUG
  int32_t linked;
  glGetProgramiv(program.id, GL_LINK_STATUS, &linked);
  if (!linked) {
    int32_t log_length = 0;
    char message[1024];
    glGetProgramInfoLog(program.id, 1024, &log_length, message);
    throw std::runtime_error(message);
  }
#endif
}

void program_controller::destroy(program_t &program) {
  glDeleteProgram(program.id);
}

void program_controller::use(program_t &program) { glUseProgram(program.id); }