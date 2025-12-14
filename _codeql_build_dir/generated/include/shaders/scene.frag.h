#ifndef SCENE_FRAG_H
#define SCENE_FRAG_H

#ifdef __cplusplus
extern "C" {
#endif

static const char SCENE_FRAG[] = "#version 330 core\nnoperspective in vec2 vTexCoord;uniform sampler2D uTexAlbedo;uniform sampler2D uTexEmission;uniform sampler2D uTexDiffuse;uniform sampler2D uTexSpecular;uniform sampler2D uTexSSAO;uniform float uSSAOPower;uniform float uSSAOLightAffect;layout(location=0)out vec3 FragColor;void main(){vec3 albedo=texture(uTexAlbedo,vTexCoord).rgb;vec3 emission=texture(uTexEmission,vTexCoord).rgb;vec3 diffuse=texture(uTexDiffuse,vTexCoord).rgb;vec3 specular=texture(uTexSpecular,vTexCoord).rgb;float ssao=mix(1.,texture(uTexSSAO,vTexCoord).r,uSSAOLightAffect);if(uSSAOPower!=1.){ssao=pow(ssao,uSSAOPower);}diffuse*=ssao;FragColor=(albedo*diffuse)+specular+emission;}";

#define SCENE_FRAG_SIZE 669

#ifdef __cplusplus
}
#endif

#endif // SCENE_FRAG_H
