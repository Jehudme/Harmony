#ifndef CUBEMAP_FROM_EQUIRECTANGULAR_FRAG_H
#define CUBEMAP_FROM_EQUIRECTANGULAR_FRAG_H

#ifdef __cplusplus
extern "C" {
#endif

static const char CUBEMAP_FROM_EQUIRECTANGULAR_FRAG[] = "#version 330 core\nin vec3 vPosition;uniform sampler2D uTexEquirectangular;out vec4 FragColor;vec2 SampleSphericalMap(vec3 v){vec2 uv=vec2(atan(v.z,v.x),asin(v.y));uv*=vec2(.1591,-.3183);uv+=.5;return uv;}void main(){vec2 uv=SampleSphericalMap(normalize(vPosition));vec3 color=texture(uTexEquirectangular,uv).rgb;FragColor=vec4(color,1.);}";

#define CUBEMAP_FROM_EQUIRECTANGULAR_FRAG_SIZE 338

#ifdef __cplusplus
}
#endif

#endif // CUBEMAP_FROM_EQUIRECTANGULAR_FRAG_H
