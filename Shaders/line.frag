#version 400 core

in float texCoord;
uniform sampler1D tex;
out vec4 col;

void main()
{
    col=texture(tex,texCoord);
    col.r=1.0;
}
