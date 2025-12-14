#ifndef SKYBOX_FRAG_H
#define SKYBOX_FRAG_H

#ifdef __cplusplus
extern "C" {
#endif

static const char SKYBOX_FRAG[] = "#version 330 core\nin vec3 vPosition;uniform samplerCube uCubeSky;uniform float uSkyIntensity;layout(location=0)out vec3 FragColor;void main(){FragColor=texture(uCubeSky,vPosition).rgb*uSkyIntensity;}";

#define SKYBOX_FRAG_SIZE 199

#ifdef __cplusplus
}
#endif

#endif // SKYBOX_FRAG_H
