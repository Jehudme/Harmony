#ifndef SCREEN_VERT_H
#define SCREEN_VERT_H

#ifdef __cplusplus
extern "C" {
#endif

static const char SCREEN_VERT[] = "#version 330 core\nconst vec2 positions[3]=vec2[](vec2(-1.,-1.),vec2(3.,-1.),vec2(-1.,3.));noperspective out vec2 vTexCoord;void main(){gl_Position=vec4(positions[gl_VertexID],0.,1.);vTexCoord=(gl_Position.xy*.5)+.5;}";

#define SCREEN_VERT_SIZE 216

#ifdef __cplusplus
}
#endif

#endif // SCREEN_VERT_H
