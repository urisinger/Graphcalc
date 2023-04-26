#version 460 core

#define GRAPHCOLOR vec3(0.176, 0.439, 0.701)

layout(location = 0) out vec4 color;

uniform sampler2D _texture;

uniform vec2 Screen_Size;
uniform vec2 zoom;
uniform vec2 offset;

in vec4 gl_FragCoord;

vec2 cameramat(vec2 pos) {
    return((pos - offset) * zoom);
}



void main(){

    vec2 pos = cameramat(2.0 * (gl_FragCoord.xy / Screen_Size) - vec2(1, 1));


    for(int i=-2;i<2;++i){
        for (int j =-2; j<2;j++){
            if(texture(_texture, gl_FragCoord.xy/Screen_Size) != texture(_texture,(gl_FragCoord.xy+vec2(i,j)) / Screen_Size)){
                color = vec4(GRAPHCOLOR,1);
                return;
            }
        }
    }

    color = vec4(1.0);
    color = mix(color, vec4(0.70), step(abs(pos.x - round(pos.x)), 0.001 * zoom.x));
    color = mix(color, vec4(0.70), step(abs(pos.y - round(pos.y)), 0.001 * zoom.y));
    color = mix(color, vec4(0.4), step(abs(pos.x), 0.002 * zoom.x));
    color = mix(color, vec4(0.4), step(abs(pos.y), 0.0025 * zoom.y));
}