#ifndef MANDELBROTOPTIONS_H
#define MANDELBROTOPTIONS_H



class COptions
{
public:
    static int granularity;
    static bool animating;
    static float animationOffset;
};

class CJuliaOptions
{
public:
    static int iIterations;
};

class CMandelbrotOptions
{
public:
    static int iIterations;
    static int iLineIterations;
};

#endif // MANDELBROTOPTIONS_H
