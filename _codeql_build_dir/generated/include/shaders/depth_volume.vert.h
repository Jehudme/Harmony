#ifndef DEPTH_VOLUME_VERT_H
#define DEPTH_VOLUME_VERT_H

#ifdef __cplusplus
extern "C" {
#endif

static const char DEPTH_VOLUME_VERT[] = "#version 330 core\nlayout(location=0)in vec3 aPosition;uniform mat4 uMatMVP;void main(){gl_Position=uMatMVP*vec4(aPosition,1.);}";

#define DEPTH_VOLUME_VERT_SIZE 127

#ifdef __cplusplus
}
#endif

#endif // DEPTH_VOLUME_VERT_H
