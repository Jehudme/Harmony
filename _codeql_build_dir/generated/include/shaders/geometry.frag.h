#ifndef GEOMETRY_FRAG_H
#define GEOMETRY_FRAG_H

#ifdef __cplusplus
extern "C" {
#endif

static const char GEOMETRY_FRAG[] = "#version 330 core\n#define M_PI 3.1415926535897931\n#define M_TAU 6.2831853071795862\n#define M_INV_PI .3183098861837907\nvec3 M_Rotate3D(vec3 v,vec4 q){vec3 t=2.*cross(q.xyz,v);return v+q.w*t+cross(q.xyz,t);}mat3 M_OrthonormalBasis(vec3 n){float sgn=n.z>=0.?1.:-1.;float a=-1./(sgn+n.z);float b=n.x*n.y*a;vec3 t=vec3(1.+sgn*n.x*n.x*a,sgn*b,-sgn*n.x);vec3 bt=vec3(b,sgn+n.y*n.y*a,-n.y);return mat3(t,bt,n);}vec2 M_OctahedronWrap(vec2 val){return(1.-abs(val.yx))*mix(vec2(-1.),vec2(1.),vec2(greaterThanEqual(val.xy,vec2(0.))));}vec3 M_DecodeOctahedral(vec2 encoded){encoded=encoded*2.-1.;vec3 normal;normal.z=1.-abs(encoded.x)-abs(encoded.y);normal.xy=normal.z>=0.?encoded.xy:M_OctahedronWrap(encoded.xy);return normalize(normal);}vec2 M_EncodeOctahedral(vec3 normal){normal/=abs(normal.x)+abs(normal.y)+abs(normal.z);normal.xy=normal.z>=0.?normal.xy:M_OctahedronWrap(normal.xy);normal.xy=normal.xy*.5+.5;return normal.xy;}vec3 M_NormalScale(vec3 normal,float scale){normal.xy*=scale;normal.z=sqrt(1.-clamp(dot(normal.xy,normal.xy),0.,1.));return normal;}float M_HashIGN(vec2 pos){const vec3 magic=vec3(0.06711056,0.00583715,52.9829189);return fract(magic.z*fract(dot(pos,magic.xy)));}flat in vec3 vEmission;in vec2 vTexCoord;in vec4 vColor;in mat3 vTBN;uniform sampler2D uTexAlbedo;uniform sampler2D uTexNormal;uniform sampler2D uTexEmission;uniform sampler2D uTexORM;uniform float uAlphaCutoff;uniform float uNormalScale;uniform float uOcclusion;uniform float uRoughness;uniform float uMetalness;layout(location=0)out vec3 FragAlbedo;layout(location=1)out vec3 FragEmission;layout(location=2)out vec2 FragNormal;layout(location=3)out vec3 FragORM;void main(){vec4 albedo=vColor*texture(uTexAlbedo,vTexCoord);if(albedo.a<uAlphaCutoff)discard;vec3 N=normalize(vTBN*M_NormalScale(texture(uTexNormal,vTexCoord).rgb*2.-1.,uNormalScale));if(!gl_FrontFacing)N=-N;FragAlbedo=albedo.rgb;FragEmission=vEmission*texture(uTexEmission,vTexCoord).rgb;FragNormal=M_EncodeOctahedral(N);vec3 orm=texture(uTexORM,vTexCoord).rgb;FragORM.r=uOcclusion*orm.x;FragORM.g=uRoughness*orm.y;FragORM.b=uMetalness*orm.z;}";

#define GEOMETRY_FRAG_SIZE 2089

#ifdef __cplusplus
}
#endif

#endif // GEOMETRY_FRAG_H
