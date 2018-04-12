#version 400 core

in vec2 posAttr;
uniform vec2 mandelPos;
uniform float fScale;
uniform int granularity;
in int gl_VertexID;
out float texCoord;
uniform float texOffset;

void main()
{
    vec4 posAttr1=vec4(posAttr.x/fScale,posAttr.y/fScale,0.5,1.0);
    vec4 pos1=vec4(mandelPos.x/fScale,mandelPos.y/fScale,0.5,1.0);
   // gl_Position = vec4(posAttr.x/fScale,posAttr.y/fScale,0.5,1.0);
     gl_Position = vec4(posAttr.x/fScale-mandelPos.x/fScale,posAttr.y/fScale-mandelPos.y/fScale,0.5,1.0);
     texCoord=float(gl_VertexID)/float(granularity)+texOffset;
//    gl_Position = posAttr1-pos1;
}
