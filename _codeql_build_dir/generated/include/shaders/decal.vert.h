#ifndef DECAL_VERT_H
#define DECAL_VERT_H

#ifdef __cplusplus
extern "C" {
#endif

static const char DECAL_VERT[] = "#version 330 core\nlayout(location=0)in vec3 aPosition;layout(location=1)in vec2 aTexCoord;layout(location=2)in vec3 aNormal;layout(location=3)in vec4 aColor;layout(location=4)in vec4 aTangent;layout(location=10)in mat4 iMatModel;uniform mat4 uMatNormal;uniform mat4 uMatModel;uniform mat4 uMatVP;uniform vec4 uAlbedoColor;uniform float uEmissionEnergy;uniform vec3 uEmissionColor;uniform bool uInstancing;out mat4 vFinalMatModel;flat out vec3 vEmission;out vec4 vColor;out vec4 vClipPos;void main(){mat4 matModel=uMatModel;mat3 matNormal=mat3(uMatNormal);if(uInstancing){matModel=transpose(iMatModel)*matModel;matNormal=mat3(transpose(inverse(iMatModel)))*matNormal;}vFinalMatModel=matModel;vColor=aColor*uAlbedoColor;vEmission=uEmissionColor*uEmissionEnergy;vec3 position=vec3(matModel*vec4(aPosition,1.));gl_Position=uMatVP*vec4(position,1.);vClipPos=gl_Position;}";

#define DECAL_VERT_SIZE 866

#ifdef __cplusplus
}
#endif

#endif // DECAL_VERT_H
