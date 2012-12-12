#include <stdlib.h>

#include "entities/landscape.h"
#include "assets/terrain.h"

landscape* landscape_new() {
  
  landscape* l = malloc(sizeof(landscape));
  
  l->heightmap = asset_hndl_null();
  l->attribmap = asset_hndl_null();
  
  l->scale = 0.25;
  l->size_x = 128;
  l->size_y = 128;
  
  l->ground0 = asset_hndl_null();
  l->ground1 = asset_hndl_null();
  l->ground2 = asset_hndl_null();
  l->ground3 = asset_hndl_null();
  
  l->ground0_nm = asset_hndl_null();
  l->ground1_nm = asset_hndl_null();
  l->ground2_nm = asset_hndl_null();
  l->ground3_nm = asset_hndl_null();
  
  return l;
  
}

void landscape_delete(landscape* l) {
  free(l);
}

float landscape_height(landscape* l, vec2 pos) {
  
  terrain* t = asset_hndl_ptr(l->heightmap);
  
  pos.x = (1 - ((pos.x / l->size_x) + 0.5)) * t->width;
  pos.y = (1 - ((pos.y / l->size_y) + 0.5)) * t->height;
  
  return l->scale * terrain_height(t, pos);
  
}

vec3  landscape_normal(landscape* l, vec2 pos) {
  
  terrain* t = asset_hndl_ptr(l->heightmap);

  pos.x = (1 - ((pos.x / l->size_x) + 0.5)) * t->width;
  pos.y = (1 - ((pos.y / l->size_y) + 0.5)) * t->height;
  
  return terrain_normal(t, pos);

}

void landscape_paint_height(landscape* l, vec2 pos, float radius, float value) {

  terrain* t = asset_hndl_ptr(l->heightmap);

  pos.x = (1 - ((pos.x / l->size_x) + 0.5)) * t->width;
  pos.y = (1 - ((pos.y / l->size_y) + 0.5)) * t->height;
  
  int base_x = roundf(pos.x);
  int base_y = roundf(pos.y);
  
  for (int x = base_x - radius - 1; x < base_x + radius + 1; x++)
  for (int y = base_y - radius - 1; y < base_y + radius + 1; y++) {
    
    if (x < 0) continue;
    if (y < 0) continue;
    if (x >= t->width) continue;
    if (y >= t->height) continue;
    
    float dist = saturate(1 - vec2_dist(pos, vec2_new(x, y)) / radius);
    
    t->heightmap[x + y * t->width] += value * dist;
  }
  
  int chunk_x = base_x / t->chunk_width;
  int chunk_y = base_y / t->chunk_height;
  
  for(int x = chunk_x - 1; x < chunk_x + 2; x++)
  for(int y = chunk_y - 1; y < chunk_y + 2; y++) {
    int chunk = clamp(x + y * t->chunk_width, 0, t->num_chunks-1);
    terrain_reload_chunk(t, chunk);
  }
  
}

void landscape_paint_color(landscape* l, vec2 pos, float radius, int type) {

}
