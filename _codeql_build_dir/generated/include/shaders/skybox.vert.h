#ifndef SKYBOX_VERT_H
#define SKYBOX_VERT_H

#ifdef __cplusplus
extern "C" {
#endif

static const char SKYBOX_VERT[] = "#version 330 core\n#define M_PI 3.1415926535897931\n#define M_TAU 6.2831853071795862\n#define M_INV_PI .3183098861837907\nvec3 M_Rotate3D(vec3 v,vec4 q){vec3 t=2.*cross(q.xyz,v);return v+q.w*t+cross(q.xyz,t);}mat3 M_OrthonormalBasis(vec3 n){float sgn=n.z>=0.?1.:-1.;float a=-1./(sgn+n.z);float b=n.x*n.y*a;vec3 t=vec3(1.+sgn*n.x*n.x*a,sgn*b,-sgn*n.x);vec3 bt=vec3(b,sgn+n.y*n.y*a,-n.y);return mat3(t,bt,n);}vec2 M_OctahedronWrap(vec2 val){return(1.-abs(val.yx))*mix(vec2(-1.),vec2(1.),vec2(greaterThanEqual(val.xy,vec2(0.))));}vec3 M_DecodeOctahedral(vec2 encoded){encoded=encoded*2.-1.;vec3 normal;normal.z=1.-abs(encoded.x)-abs(encoded.y);normal.xy=normal.z>=0.?encoded.xy:M_OctahedronWrap(encoded.xy);return normalize(normal);}vec2 M_EncodeOctahedral(vec3 normal){normal/=abs(normal.x)+abs(normal.y)+abs(normal.z);normal.xy=normal.z>=0.?normal.xy:M_OctahedronWrap(normal.xy);normal.xy=normal.xy*.5+.5;return normal.xy;}vec3 M_NormalScale(vec3 normal,float scale){normal.xy*=scale;normal.z=sqrt(1.-clamp(dot(normal.xy,normal.xy),0.,1.));return normal;}float M_HashIGN(vec2 pos){const vec3 magic=vec3(0.06711056,0.00583715,52.9829189);return fract(magic.z*fract(dot(pos,magic.xy)));}layout(location=0)in vec3 aPosition;uniform mat4 uMatProj;uniform mat4 uMatView;uniform vec4 uRotation;out vec3 vPosition;void main(){vPosition=M_Rotate3D(aPosition,uRotation);mat4 rotView=mat4(mat3(uMatView));gl_Position=uMatProj*rotView*vec4(aPosition,1.);}";

#define SKYBOX_VERT_SIZE 1439

#ifdef __cplusplus
}
#endif

#endif // SKYBOX_VERT_H
