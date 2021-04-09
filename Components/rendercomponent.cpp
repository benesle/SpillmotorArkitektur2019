#include "rendercomponent.h"
#include "innpch.h"
#include "materialcomponent.h"
#include "meshcomponent.h"
#include "transformcomponent.h"
#include "collisioncomponent.h"



RenderComponent::RenderComponent(MaterialComponent * mMat, MeshComponent* mMesh, TransformComponent* mTrans, CollisionComponent* mCol)
{
    mMaterialComp = mMat;
    mMeshComp = mMesh;
    mTransComp = mTrans;
    mCollisComp = mCol;
}

void RenderComponent::draw()
{
    if(mMeshComp->isRendering == true)
    {
        initializeOpenGLFunctions();


        glUseProgram(mMaterialComp->returnMaterial().mShader->getProgram());
        glBindVertexArray( mMeshComp->mVAO );
        mMaterialComp->returnMaterial().mShader->transmitUniformData(&mTransComp->modelMatrix(), &mMaterialComp->returnMaterial());
        glDrawArrays(GL_TRIANGLES, 0, mMeshComp->mVerticesSize());//mVertices.size());
    }
}

void RenderComponent::drawCollisionComponent()
{
    if(mCollisComp != nullptr)
    {
        if(mCollisComp->isVisible == true)
        {
            initializeOpenGLFunctions();
            glUseProgram(mMaterialComp->returnMaterial().mShader->getProgram());
            glBindVertexArray( mCollisComp->mVAO );
            mMaterialComp->returnMaterial().mShader->transmitUniformData(&mTransComp->modelMatrix(), &mMaterialComp->returnMaterial());
            glDrawArrays(GL_LINES, 0, mCollisComp->mVerticesSize());//mVertices.size());
        }
    }
}
