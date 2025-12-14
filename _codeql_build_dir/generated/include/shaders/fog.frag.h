#ifndef FOG_FRAG_H
#define FOG_FRAG_H

#ifdef __cplusplus
extern "C" {
#endif

static const char FOG_FRAG[] = "#version 330 core\n#define FOG_DISABLED 0\n#define FOG_LINEAR 1\n#define FOG_EXP2 2\n#define FOG_EXP 3\nnoperspective in vec2 vTexCoord;uniform sampler2D uTexColor;uniform sampler2D uTexDepth;uniform float uNear;uniform float uFar;uniform lowp int uFogMode;uniform vec3 uFogColor;uniform float uFogStart;uniform float uFogEnd;uniform float uFogDensity;uniform float uSkyAffect;out vec4 FragColor;float LinearizeDepth(float depth,float near,float far){return(2.*near*far)/(far+near-(2.*depth-1.)*(far-near));;}float FogFactorLinear(float dist,float start,float end){return 1.-clamp((end-dist)/(end-start),0.,1.);}float FogFactorExp2(float dist,float density){const float LOG2=-1.442695;float d=density*dist;return 1.-clamp(exp2(d*d*LOG2),0.,1.);}float FogFactorExp(float dist,float density){return 1.-clamp(exp(-density*dist),0.,1.);}float FogFactor(float dist,int mode,float density,float start,float end){if(mode==FOG_LINEAR)return FogFactorLinear(dist,start,end);if(mode==FOG_EXP2)return FogFactorExp2(dist,density);if(mode==FOG_EXP)return FogFactorExp(dist,density);return 1.;}void main(){vec3 result=texture(uTexColor,vTexCoord).rgb;float depth=texture(uTexDepth,vTexCoord).r;depth=LinearizeDepth(depth,uNear,uFar);float fogFactor=FogFactor(depth,uFogMode,uFogDensity,uFogStart,uFogEnd);fogFactor*=uSkyAffect*step(depth,uFar);result=mix(result,uFogColor,fogFactor);FragColor=vec4(result,1.);}";

#define FOG_FRAG_SIZE 1391

#ifdef __cplusplus
}
#endif

#endif // FOG_FRAG_H
