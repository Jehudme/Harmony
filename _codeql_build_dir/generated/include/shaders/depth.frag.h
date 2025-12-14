#ifndef DEPTH_FRAG_H
#define DEPTH_FRAG_H

#ifdef __cplusplus
extern "C" {
#endif

static const char DEPTH_FRAG[] = "#version 330 core\nin vec2 vTexCoord;in float vAlpha;uniform sampler2D uTexAlbedo;uniform float uAlphaCutoff;void main(){float alpha=vAlpha*texture(uTexAlbedo,vTexCoord).a;if(alpha<uAlphaCutoff)discard;}";

#define DEPTH_FRAG_SIZE 202

#ifdef __cplusplus
}
#endif

#endif // DEPTH_FRAG_H
