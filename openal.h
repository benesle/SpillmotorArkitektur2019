#ifndef OPENAL_H
#define OPENAL_H


#ifdef _WIN32
#include <al.h>
#include <alc.h>
#endif

#ifdef __APPLE__
#include <OpenAL/al.h>
#include <OpenAL/alc.h>
#endif

#endif // OPENAL_H
