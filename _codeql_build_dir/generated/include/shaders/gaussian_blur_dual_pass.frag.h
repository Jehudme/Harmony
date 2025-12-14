#ifndef GAUSSIAN_BLUR_DUAL_PASS_FRAG_H
#define GAUSSIAN_BLUR_DUAL_PASS_FRAG_H

#ifdef __cplusplus
extern "C" {
#endif

static const char GAUSSIAN_BLUR_DUAL_PASS_FRAG[] = "#version 330 core\nnoperspective in vec2 vTexCoord;uniform sampler2D uTexture;uniform vec2 uTexelDir;out vec4 FragColor;const int SAMPLE_COUNT=6;const float OFFSETS[6]=float[6](-4.455269417428358,-2.4751038298192056,-.4950160492928827,1.485055021558738,3.465172537482815,5);const float WEIGHTS[6]=float[6](.14587920530480702,.19230308352110734,.21647621943673803,.20809835496561988,.17082879595769634,0.06641434081403137);void main(){vec3 result=vec3(0.);for(int i=0;i<SAMPLE_COUNT;++i){result+=texture(uTexture,vTexCoord+uTexelDir*OFFSETS[i]).rgb*WEIGHTS[i];}FragColor=vec4(result,1.);}";

#define GAUSSIAN_BLUR_DUAL_PASS_FRAG_SIZE 586

#ifdef __cplusplus
}
#endif

#endif // GAUSSIAN_BLUR_DUAL_PASS_FRAG_H
