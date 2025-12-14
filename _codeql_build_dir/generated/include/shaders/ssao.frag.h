#ifndef SSAO_FRAG_H
#define SSAO_FRAG_H

#ifdef __cplusplus
extern "C" {
#endif

static const char SSAO_FRAG[] = "#version 330 core\n#define M_PI 3.1415926535897931\n#define M_TAU 6.2831853071795862\n#define M_INV_PI .3183098861837907\nvec3 M_Rotate3D(vec3 v,vec4 q){vec3 t=2.*cross(q.xyz,v);return v+q.w*t+cross(q.xyz,t);}mat3 M_OrthonormalBasis(vec3 n){float sgn=n.z>=0.?1.:-1.;float a=-1./(sgn+n.z);float b=n.x*n.y*a;vec3 t=vec3(1.+sgn*n.x*n.x*a,sgn*b,-sgn*n.x);vec3 bt=vec3(b,sgn+n.y*n.y*a,-n.y);return mat3(t,bt,n);}vec2 M_OctahedronWrap(vec2 val){return(1.-abs(val.yx))*mix(vec2(-1.),vec2(1.),vec2(greaterThanEqual(val.xy,vec2(0.))));}vec3 M_DecodeOctahedral(vec2 encoded){encoded=encoded*2.-1.;vec3 normal;normal.z=1.-abs(encoded.x)-abs(encoded.y);normal.xy=normal.z>=0.?encoded.xy:M_OctahedronWrap(encoded.xy);return normalize(normal);}vec2 M_EncodeOctahedral(vec3 normal){normal/=abs(normal.x)+abs(normal.y)+abs(normal.z);normal.xy=normal.z>=0.?normal.xy:M_OctahedronWrap(normal.xy);normal.xy=normal.xy*.5+.5;return normal.xy;}vec3 M_NormalScale(vec3 normal,float scale){normal.xy*=scale;normal.z=sqrt(1.-clamp(dot(normal.xy,normal.xy),0.,1.));return normal;}float M_HashIGN(vec2 pos){const vec3 magic=vec3(0.06711056,0.00583715,52.9829189);return fract(magic.z*fract(dot(pos,magic.xy)));}noperspective in vec2 vTexCoord;uniform sampler2D uTexDepth;uniform sampler2D uTexNormal;uniform sampler1D uTexKernel;uniform sampler2D uTexNoise;uniform mat4 uMatInvProj;uniform mat4 uMatProj;uniform mat4 uMatView;uniform float uRadius;uniform float uBias;uniform float uIntensity;const int NOISE_TEXTURE_SIZE=4;const int KERNEL_SIZE=32;out float FragOcclusion;vec3 DepthToViewPosition(float depth){vec4 ndcPos=vec4(vTexCoord*2.-1.,depth*2.-1.,1.);vec4 viewPos=uMatInvProj*ndcPos;return viewPos.xyz/viewPos.w;}vec2 ViewPositionToScreenUV(vec3 viewPos){vec4 clipPos=uMatProj*vec4(viewPos,1.);vec3 ndc=clipPos.xyz/clipPos.w;return ndc.xy*.5+.5;}vec3 SampleKernel(int index,int kernelSize){float texCoord=(float(index)+.5)/float(kernelSize);return texture(uTexKernel,texCoord).rgb;}void main(){float depth=texture(uTexDepth,vTexCoord).r;vec3 position=DepthToViewPosition(depth);vec3 normal=M_DecodeOctahedral(texture(uTexNormal,vTexCoord).rg);normal=normalize(mat3(uMatView)*normal);vec2 noiseCoord=gl_FragCoord.xy/float(NOISE_TEXTURE_SIZE);vec3 randomVec=normalize(texture(uTexNoise,noiseCoord).xyz*2.-1.);vec3 tangent=normalize(randomVec-normal*dot(randomVec,normal));vec3 bitangent=cross(normal,tangent);mat3 TBN=mat3(tangent,bitangent,normal);float occlusion=0.;for(int i=0;i<KERNEL_SIZE;i++){vec3 samplePos=SampleKernel(i,KERNEL_SIZE);samplePos=position+(TBN*samplePos)*uRadius;vec2 offset=ViewPositionToScreenUV(samplePos);if(all(greaterThanEqual(offset,vec2(0.)))&&all(lessThanEqual(offset,vec2(1.)))){float sampleDepth=texture(uTexDepth,offset).r;vec3 sampleViewPos=DepthToViewPosition(sampleDepth);float rangeCheck=1.-smoothstep(0.,uRadius,abs(position.z-sampleViewPos.z));occlusion+=(sampleViewPos.z>=samplePos.z+uBias)?rangeCheck:0.;}}FragOcclusion=1.-(occlusion/float(KERNEL_SIZE))*uIntensity;}";

#define SSAO_FRAG_SIZE 2984

#ifdef __cplusplus
}
#endif

#endif // SSAO_FRAG_H
