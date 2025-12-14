#ifndef UPSAMPLING_FRAG_H
#define UPSAMPLING_FRAG_H

#ifdef __cplusplus
extern "C" {
#endif

static const char UPSAMPLING_FRAG[] = "#version 330 core\nnoperspective in vec2 vTexCoord;uniform sampler2D uTexture;uniform vec2 uFilterRadius;layout(location=0)out vec3 FragColor;void main(){float x=uFilterRadius.x;float y=uFilterRadius.y;vec3 a=texture(uTexture,vec2(vTexCoord.x-x,vTexCoord.y+y)).rgb;vec3 b=texture(uTexture,vec2(vTexCoord.x,vTexCoord.y+y)).rgb;vec3 c=texture(uTexture,vec2(vTexCoord.x+x,vTexCoord.y+y)).rgb;vec3 d=texture(uTexture,vec2(vTexCoord.x-x,vTexCoord.y)).rgb;vec3 e=texture(uTexture,vec2(vTexCoord.x,vTexCoord.y)).rgb;vec3 f=texture(uTexture,vec2(vTexCoord.x+x,vTexCoord.y)).rgb;vec3 g=texture(uTexture,vec2(vTexCoord.x-x,vTexCoord.y-y)).rgb;vec3 h=texture(uTexture,vec2(vTexCoord.x,vTexCoord.y-y)).rgb;vec3 iT=texture(uTexture,vec2(vTexCoord.x+x,vTexCoord.y-y)).rgb;FragColor=e*4.;FragColor+=(b+d+f+h)*2.;FragColor+=(a+c+g+iT);FragColor*=1./16.;}";

#define UPSAMPLING_FRAG_SIZE 837

#ifdef __cplusplus
}
#endif

#endif // UPSAMPLING_FRAG_H
