#ifndef DEPTH_CUBE_FRAG_H
#define DEPTH_CUBE_FRAG_H

#ifdef __cplusplus
extern "C" {
#endif

static const char DEPTH_CUBE_FRAG[] = "#version 330 core\nin vec3 vPosition;in vec2 vTexCoord;in float vAlpha;uniform sampler2D uTexAlbedo;uniform float uAlphaCutoff;uniform vec3 uViewPosition;uniform float uFar;void main(){float alpha=vAlpha*texture(uTexAlbedo,vTexCoord).a;if(alpha<uAlphaCutoff)discard;gl_FragDepth=length(vPosition-uViewPosition)/uFar;}";

#define DEPTH_CUBE_FRAG_SIZE 316

#ifdef __cplusplus
}
#endif

#endif // DEPTH_CUBE_FRAG_H
