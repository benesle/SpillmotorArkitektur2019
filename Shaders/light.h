#ifndef LIGHT_H
#define LIGHT_H

#include "visualobject.h"

//Main steps to illuminate the world:

//First activate at least one light source.
//For each polygon we need to determine the amount of light that it is able to reflect and to transmit to the eye of our observer.
//In the rendering phase we paint the polygons according to their illumination value.


class Light : public VisualObject
{
public:
    Light();
    virtual void init() override;
    virtual void draw() override;

//    void glLightfv( GLenum light, GLenum pname, const GLfloat *params );

    //It is mainly light that has been reflected several times.
    //A polygon is illuminated by the ambient environment, it doesn't matter what orientation or position it has in the space.


    GLfloat mAmbientStrenght{0.3f};
    gsl::Vector3D mAmbientColor{0.3f, 0.3f, 0.3f};


//    is the light that originates from one direction,
//    it considers the angle that the polygon has with regards to the light source.
//    The more perpendicular the polygon is to the light ray the brighter it will be.
//    The position of the observer is not used for this and the polygon is  uniformly illuminated.



    GLfloat mLightStrenght{0.7f};
    gsl::Vector3D mLightColor{0.3f, 0.3f, 0.3f};



    //takes into account the degree of inclination
    //of the polygon and also the observer's position.
    //Specular light comes from a direction and is
    //reflected by the polygon according to its inclination.


    GLfloat mSpecularStrenght{0.3f};
    GLint mSpecularExponent{4};
};

#endif // LIGHT_H
