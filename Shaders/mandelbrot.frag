#version 400 core

noperspective in vec2 c;
out vec4 color;
uniform int iterations;
uniform sampler1D tex;
uniform int granularity;
uniform float texOffset;

vec2 sqr(vec2 c)
{
    return vec2(c.x*c.x-c.y*c.y,2*c.x*c.y);
}



void main()
{
    vec2 oldVal;
    vec2 newVal=vec2(0.0,0.0);
    int i=0;
    for(i = 0; i < iterations; i++)
    {
        oldVal=newVal;
        newVal = sqr(oldVal)+c;
        if(dot(newVal,newVal) > 4) break;
    }
    if (i<iterations)
    {
        color = texture1D(tex,float(i%granularity)/float(granularity)+texOffset);
    }else
    {
        color = vec4(0.2,0.0,0.0,0.0);
    }
}
