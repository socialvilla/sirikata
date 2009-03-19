/*  Sirikata Graphical Object Host
 *  Entity.hpp
 *
 *  Copyright (c) 2009, Patrick Reiter Horn
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 *  * Neither the name of Sirikata nor the names of its contributors may
 *    be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
 * IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef SIRIKATA_GRAPHICS_CAMERA_HPP__
#define SIRIKATA_GRAPHICS_CAMERA_HPP__
#include "Entity.hpp"
#include <oh/CameraListener.hpp>
#include <OgreMovableObject.h>
#include <OgreRenderable.h>
#include <OgreRenderTarget.h>

namespace Sirikata {
namespace Graphics {

class Camera : public Entity, public CameraListener {
    Ogre::Camera *mCamera;
    Ogre::RenderTarget *mRenderTarget;
    Ogre::Viewport *mViewport;
public:
    virtual void attach (const String&renderTargetName,
                 const uint32 width,
                 const uint32 height){
        detatch();
        mRenderTarget = mScene->createRenderTarget(renderTargetName,
                                                  width,
                                                  height);
        mViewport= mRenderTarget->addViewport(mCamera);
    }
    virtual void detatch() {
        if (mViewport&&mRenderTarget) {
            mRenderTarget->removeViewport(mViewport->getZOrder());
/*
            unsigned int numViewports=sm->getNumViewports();
            for (unsigned int i=0;i<numViewports;++i){
                if (sm->getViewport(i)==mViewport) {
                    sm->removeViewport(i);
                    break;
                }
            }
*/
        }else {
            assert(!mViewport);
        }
        if (mRenderTarget) {
            mScene->destroyRenderTarget(mRenderTarget->getName());
            mRenderTarget=NULL;
        }
    }
    Camera(OgreSystem *scene,
           const UUID &id,
           String cameraName=String())
      : Entity(scene,
               id,
               scene->getSceneManager()->hasCamera(cameraName=id.readableHexData())?scene->getSceneManager()->getCamera(cameraName):scene->getSceneManager()->createCamera(cameraName)),mRenderTarget(NULL),mViewport(NULL) {
        }

    virtual ~Camera() {
        mSceneNode->detachAllObjects();
        mScene->getSceneManager()->destroyCamera(mId.readableHexData());
    }
    virtual Ogre::Viewport* getViewport();
};

}
}

#endif
