#ifndef SOUNDCOMPONENT_H
#define SOUNDCOMPONENT_H
#include "innpch.h"

class SoundSource;
/**
 * The SoundComponent class
 * Contains the Sound source and functions to set it.
 */
class SoundComponent
{

public:
    SoundComponent();
    int mEntityID = 0;
    std::string mName = "Filename";


    SoundSource *source(){return mSource;}

    void setSource(SoundSource *source);

private:
    SoundSource* mSource{nullptr};

};

#endif // SOUNDCOMPONENT_H
