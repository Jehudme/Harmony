#ifndef DECAL_FRAG_H
#define DECAL_FRAG_H

#ifdef __cplusplus
extern "C" {
#endif

static const char DECAL_FRAG[] = "#version 330 core\nin mat4 vFinalMatModel;flat in vec3 vEmission;in vec4 vColor;in vec4 vClipPos;uniform sampler2D uTexAlbedo;uniform sampler2D uTexNormal;uniform sampler2D uTexEmission;uniform sampler2D uTexORM;uniform sampler2D uTexDepth;uniform mat4 uMatInvView;uniform mat4 uMatNormal;uniform mat4 uMatVP;uniform mat4 uMatInvProj;uniform mat4 uMatProj;uniform float uAlphaCutoff;uniform float uNormalScale;uniform float uOcclusion;uniform float uRoughness;uniform float uMetalness;uniform vec2 uTexCoordOffset;uniform vec2 uTexCoordScale;layout(location=0)out vec4 FragAlbedo;layout(location=1)out vec4 FragEmission;layout(location=2)out vec2 FragNormal;layout(location=3)out vec3 FragORM;vec3 DepthToViewPosition(vec2 texCoord,float depth){vec4 ndcPos=vec4(texCoord*2.-1.,depth*2.-1.,1.);vec4 viewPos=uMatInvProj*ndcPos;return viewPos.xyz/viewPos.w;}void main(){vec2 screenPos=vClipPos.xy/vClipPos.w;vec2 fragTexCoord=screenPos*.5+.5;float textureDepth=texture(uTexDepth,fragTexCoord).r;vec3 positionViewSpace=DepthToViewPosition(fragTexCoord,textureDepth);vec4 positionModelSpace=inverse(vFinalMatModel)*uMatInvView*vec4(positionViewSpace,1.);if(abs(positionModelSpace.x)>.5||abs(positionModelSpace.y)>.5||abs(positionModelSpace.z)>.5){discard;}vec2 decalTexCoord=uTexCoordOffset+(positionModelSpace.xz+.5)*uTexCoordScale;vec4 albedo=vColor*texture(uTexAlbedo,decalTexCoord);if(albedo.a<uAlphaCutoff)discard;FragAlbedo=albedo;FragEmission=vec4(vEmission,1.)*texture(uTexEmission,decalTexCoord);vec3 orm=texture(uTexORM,decalTexCoord).xyz;FragORM.x=uOcclusion*orm.x;FragORM.y=uRoughness*orm.y;FragORM.z=uMetalness*orm.z;}";

#define DECAL_FRAG_SIZE 1625

#ifdef __cplusplus
}
#endif

#endif // DECAL_FRAG_H
