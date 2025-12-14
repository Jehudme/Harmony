#ifndef DOF_FRAG_H
#define DOF_FRAG_H

#ifdef __cplusplus
extern "C" {
#endif

static const char DOF_FRAG[] = "#version 330 core\nnoperspective in vec2 vTexCoord;uniform sampler2D uTexColor;uniform sampler2D uTexDepth;uniform vec2 uTexelSize;uniform float uNear;uniform float uFar;uniform float uFocusPoint;uniform float uFocusScale;uniform float uMaxBlurSize;uniform int uDebugMode;out vec4 FragColor;const float GOLDEN_ANGLE=2.39996323;const float RAD_SCALE=.5;float LinearizeDepth(float depth){return(2.*uNear*uFar)/(uFar+uNear-(2.*depth-1.)*(uFar-uNear));;}float GetBlurSize(float depth){float coc=clamp((1./uFocusPoint-1./depth)*uFocusScale,-1.,1.);return abs(coc)*uMaxBlurSize;}void main(){vec3 color=texture(uTexColor,vTexCoord).rgb;float centerDepth=LinearizeDepth(texture(uTexDepth,vTexCoord).r);float centerSize=GetBlurSize(centerDepth);float tot=1.;float radius=RAD_SCALE;for(float ang=0.;radius<uMaxBlurSize;ang+=GOLDEN_ANGLE){vec2 tc=vTexCoord+vec2(cos(ang),sin(ang))*uTexelSize*radius;vec3 sampleColor=texture(uTexColor,tc).rgb;float sampleDepth=LinearizeDepth(texture(uTexDepth,tc).r);float sampleSize=GetBlurSize(sampleDepth);if(sampleDepth>centerDepth){sampleSize=clamp(sampleSize,0.,centerSize*2.);}float m=smoothstep(radius-.5,radius+.5,sampleSize);color+=mix(color/tot,sampleColor,m);radius+=RAD_SCALE/max(radius,0.001);tot+=1.;}FragColor=vec4(color/tot,1.);if(uDebugMode==1){float cocSigned=clamp((1./uFocusPoint-1./centerDepth)*uFocusScale,-1.,1.);float front=clamp(-cocSigned,0.,1.);float back=clamp(cocSigned,0.,1.);vec3 tint=vec3(0.,front,back);FragColor=vec4(tint,1.);}}";

#define DOF_FRAG_SIZE 1484

#ifdef __cplusplus
}
#endif

#endif // DOF_FRAG_H
