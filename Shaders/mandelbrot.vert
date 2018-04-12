#version 400 core

in vec4 posAttr;
in vec2 fracCoord;
noperspective out vec2 c;


void main()
{
    c = fracCoord;
    gl_Position = posAttr;
}
