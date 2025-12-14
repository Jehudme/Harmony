#ifndef DOWNSAMPLING_FRAG_H
#define DOWNSAMPLING_FRAG_H

#ifdef __cplusplus
extern "C" {
#endif

static const char DOWNSAMPLING_FRAG[] = "#version 330 core\nnoperspective in vec2 vTexCoord;uniform sampler2D uTexture;uniform vec2 uTexelSize;uniform vec4 uPrefilter;uniform int uMipLevel;layout(location=0)out vec3 FragColor;vec3 LinearToSRGB(vec3 color){return max(vec3(1.055)*pow(color,vec3(.416666667))-vec3(0.055),vec3(0.));}float sRGBToLuma(vec3 col){return dot(col,vec3(.299,.587,.114));}float KarisAverage(vec3 col){float luma=sRGBToLuma(LinearToSRGB(col))*0.25f;return 1.f/(1.f+luma);}vec3 Prefilter(vec3 col){float brightness=max(col.r,max(col.g,col.b));float soft=brightness-uPrefilter.y;soft=clamp(soft,0,uPrefilter.z);soft=soft*soft*uPrefilter.w;float contribution=max(soft,brightness-uPrefilter.x);contribution/=max(brightness,0.00001);return col*contribution;}void main(){float x=uTexelSize.x;float y=uTexelSize.y;vec3 a=texture(uTexture,vec2(vTexCoord.x-2*x,vTexCoord.y+2*y)).rgb;vec3 b=texture(uTexture,vec2(vTexCoord.x,vTexCoord.y+2*y)).rgb;vec3 c=texture(uTexture,vec2(vTexCoord.x+2*x,vTexCoord.y+2*y)).rgb;vec3 d=texture(uTexture,vec2(vTexCoord.x-2*x,vTexCoord.y)).rgb;vec3 e=texture(uTexture,vec2(vTexCoord.x,vTexCoord.y)).rgb;vec3 f=texture(uTexture,vec2(vTexCoord.x+2*x,vTexCoord.y)).rgb;vec3 g=texture(uTexture,vec2(vTexCoord.x-2*x,vTexCoord.y-2*y)).rgb;vec3 h=texture(uTexture,vec2(vTexCoord.x,vTexCoord.y-2*y)).rgb;vec3 i=texture(uTexture,vec2(vTexCoord.x+2*x,vTexCoord.y-2*y)).rgb;vec3 j=texture(uTexture,vec2(vTexCoord.x-x,vTexCoord.y+y)).rgb;vec3 k=texture(uTexture,vec2(vTexCoord.x+x,vTexCoord.y+y)).rgb;vec3 l=texture(uTexture,vec2(vTexCoord.x-x,vTexCoord.y-y)).rgb;vec3 m=texture(uTexture,vec2(vTexCoord.x+x,vTexCoord.y-y)).rgb;vec3 groups[5];if(uMipLevel==0){groups[0]=(a+b+d+e)*(.125/4.);groups[1]=(b+c+e+f)*(.125/4.);groups[2]=(d+e+g+h)*(.125/4.);groups[3]=(e+f+h+i)*(.125/4.);groups[4]=(j+k+l+m)*(.5/4.);groups[0]*=KarisAverage(groups[0]);groups[1]*=KarisAverage(groups[1]);groups[2]*=KarisAverage(groups[2]);groups[3]*=KarisAverage(groups[3]);groups[4]*=KarisAverage(groups[4]);FragColor=groups[0]+groups[1]+groups[2]+groups[3]+groups[4];FragColor=max(FragColor,0.0001);FragColor=Prefilter(FragColor);}else{FragColor=e*.125;FragColor+=(a+c+g+i)*0.03125;FragColor+=(b+d+f+h)*0.0625;FragColor+=(j+k+l+m)*.125;}}";

#define DOWNSAMPLING_FRAG_SIZE 2204

#ifdef __cplusplus
}
#endif

#endif // DOWNSAMPLING_FRAG_H
