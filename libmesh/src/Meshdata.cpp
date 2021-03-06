/*  Sirikata
 *  Meshdata.cpp
 *
 *  Copyright (c) 2010, Ewen Cheslack-Postava
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

#include <sirikata/mesh/Meshdata.hpp>
#include <sirikata/mesh/Filter.hpp>

namespace Sirikata {
namespace Mesh {

SIRIKATA_MESH_EXPORT NodeIndex NullNodeIndex = -1;

Node::Node()
 : parent(NullNodeIndex),
   transform(Matrix4x4f::identity())
{
}

Node::Node(NodeIndex par, const Matrix4x4f& xform)
 : parent(par),
   transform(xform)
{
}

Node::Node(const Matrix4x4f& xform)
 : parent(NullNodeIndex),
   transform(xform)
{
}

void SubMeshGeometry::append(const SubMeshGeometry& rhs, const Matrix4x4f& xform) {
    Matrix3x3f normal_xform = xform.extract3x3().inverseTranspose();

    int32 index_offset = this->positions.size();
    bool first_append = (index_offset == 0);
    for(uint32 pi = 0; pi < rhs.positions.size(); pi++)
        this->positions.push_back(xform * rhs.positions[pi]);
    if (!first_append &&
        ((!this->normals.empty() && rhs.normals.empty()) ||
            (this->normals.empty() && !rhs.normals.empty())))
        SILOG(mesh, error, "SubMeshGeometry::append is appending SubMeshGeometries with and without normals.");
    for(uint32 ni = 0; ni < rhs.normals.size(); ni++)
        this->normals.push_back( normal_xform * rhs.normals[ni] );
    // FIXME tangents?
    if (!first_append &&
        ((!this->colors.empty() && rhs.colors.empty()) ||
            (this->colors.empty() && !rhs.colors.empty())))
        SILOG(mesh, error, "SubMeshGeometry::append is appending SubMeshGeometries with and without colors.");
    this->colors.insert(this->colors.end(), rhs.colors.begin(), rhs.colors.end());

    // FIXME: influenceStartIndex? jointindices? weights? inverseBindMatrices?

    // Either one set has 0 texUVs (and we can fill them in) or both have the
    // same size
    assert(this->texUVs.size() == 0 || rhs.texUVs.size() == 0 ||
        this->texUVs.size() == rhs.texUVs.size()
    );

    if (rhs.texUVs.size() == 0) {
        // Fill in bogus values
        for(uint32 ti = 0; ti < this->texUVs.size(); ti++) {
            uint32 stride = this->texUVs[ti].stride;
            uint32 to_insert = rhs.positions.size() * stride;
            for(uint32 uvi = 0; uvi < to_insert; uvi++)
                this->texUVs[ti].uvs.push_back(0);
        }
    }
    else {
        if (this->texUVs.size() == 0) {
            // Fill in bogus values for past entries
            for(uint32 ti = 0; ti < rhs.texUVs.size(); ti++) {
                uint32 stride = rhs.texUVs[ti].stride;
                uint32 to_insert = index_offset * stride;
                this->texUVs.push_back(TextureSet());
                this->texUVs[ti].stride = stride;
                for(uint32 uvi = 0; uvi < to_insert; uvi++)
                    this->texUVs[ti].uvs.push_back(0);
            }
        }
        // Append
        for(uint32 ti = 0; ti < rhs.texUVs.size(); ti++) {
            assert( this->texUVs[ti].stride == rhs.texUVs[ti].stride );
            this->texUVs[ti].uvs.insert(
                this->texUVs[ti].uvs.end(),
                rhs.texUVs[ti].uvs.begin(), rhs.texUVs[ti].uvs.end()
            );
        }
    }

    // Copy primitives
    // FIXME this currently assumes all the materialIDs line up, and its not
    // clear how to handle this any better
    for(uint32 pi = 0; pi < rhs.primitives.size(); pi++) {
        const Primitive& orig_prim = rhs.primitives[pi];
        this->primitives.push_back( Primitive() );
        Primitive& prim = this->primitives.back();
        prim.primitiveType = orig_prim.primitiveType;
        prim.materialId = orig_prim.materialId;

        prim.indices.resize( orig_prim.indices.size() );
        for(uint32 ii = 0; ii < orig_prim.indices.size(); ii++)
            prim.indices[ii] = orig_prim.indices[ii] + index_offset;
    }
}

void SubMeshGeometry::recomputeBounds() {
    aabb = BoundingBox3f3f::null();
    radius = 0.;
    for(uint32 pi = 0; pi < primitives.size(); pi++) {
        const Primitive& prim = primitives[pi];
        for(uint32 ii = 0; ii < prim.indices.size(); ii++) {
            aabb = (aabb == BoundingBox3f3f::null()) ? BoundingBox3f3f(positions[prim.indices[ii]], positions[prim.indices[ii]]) : aabb.merge(positions[prim.indices[ii]]);
            double l = sqrt(positions[prim.indices[ii]].lengthSquared());
            radius = std::max(radius, l);
        }
    }
}

void GeometryInstance::computeTransformedBounds(MeshdataPtr parent, const Matrix4x4f& xform, BoundingBox3f3f* bounds_out, double* radius_out) const {
    computeTransformedBounds(*parent, xform, bounds_out, radius_out);
}

void GeometryInstance::computeTransformedBounds(const Meshdata& parent, const Matrix4x4f& xform, BoundingBox3f3f* bounds_out, double* radius_out) const {
    const SubMeshGeometry& geo = parent.geometry[ geometryIndex ];

    if (bounds_out != NULL)
        *bounds_out = BoundingBox3f3f::null();
    if (radius_out != NULL)
        *radius_out = 0.;

    for(uint32 pi = 0; pi < geo.primitives.size(); pi++) {
        const SubMeshGeometry::Primitive& prim = geo.primitives[pi];
        for(uint32 ii = 0; ii < prim.indices.size(); ii++) {
            Vector3f xpos = xform * geo.positions[ prim.indices[ii] ];
            if (bounds_out != NULL)
                *bounds_out = (*bounds_out == BoundingBox3f3f::null()) ? BoundingBox3f3f(xpos, xpos) : bounds_out->merge(xpos);
            if (radius_out != NULL && (((*radius_out)*(*radius_out))<xpos.lengthSquared()))
                *radius_out = sqrt(xpos.lengthSquared());
        }
    }
}

BoundingBox3f3f GeometryInstance::computeTransformedBounds(const Meshdata& parent, const Matrix4x4f& xform) const {
    BoundingBox3f3f result;
    computeTransformedBounds(parent, xform, &result, NULL);
    return result;
}

BoundingBox3f3f GeometryInstance::computeTransformedBounds(MeshdataPtr parent, const Matrix4x4f& xform) const {
    return computeTransformedBounds(*parent, xform);
}

bool MaterialEffectInfo::Texture::operator==(const MaterialEffectInfo::Texture& rhs) const {
    return (
        uri == rhs.uri &&
        color == rhs.color &&
        texCoord == rhs.texCoord &&
        affecting == rhs.affecting &&
        samplerType == rhs.samplerType &&
        minFilter == rhs.minFilter &&
        magFilter == rhs.magFilter &&
        wrapS == rhs.wrapS &&
        wrapT == rhs.wrapT &&
        wrapU == rhs.wrapU &&
        maxMipLevel == rhs.maxMipLevel &&
        mipBias == rhs.mipBias);

}
bool MaterialEffectInfo::Texture::operator!=(const MaterialEffectInfo::Texture& rhs) const {
    return !(*this == rhs);
}

bool MaterialEffectInfo::operator==(const MaterialEffectInfo& rhs) const {
    bool basic =
        shininess == rhs.shininess &&
        reflectivity == rhs.reflectivity &&
        textures.size() == rhs.textures.size();
    if (!basic) return false;
    for(uint32 i = 0; i < textures.size(); i++) {
        if (textures[i] != rhs.textures[i]) return false;
    }
    return true;
}
bool MaterialEffectInfo::operator!=(const MaterialEffectInfo& rhs) const {
    return !(*this == rhs);
}


String Meshdata::sType("Meshdata");

Meshdata::~Meshdata() {
}

const String& Meshdata::type() const {
    return sType;
}

Matrix4x4f Meshdata::getTransform(NodeIndex index) const {
    // Just trace up the tree, multiplying in transforms
    Matrix4x4f xform(Matrix4x4f::identity());

    while(index != NullNodeIndex) {
        xform = nodes[index].transform * xform;
        index = nodes[index].parent;
    }

    return globalTransform * xform;
}

Meshdata::GeometryInstanceIterator Meshdata::getGeometryInstanceIterator() const {
    return GeometryInstanceIterator(this);
}

uint32 Meshdata::getInstancedGeometryCount() const {
    uint32 count = 0;
    Meshdata::GeometryInstanceIterator geoinst_it = getGeometryInstanceIterator();
    uint32 geoinst_idx;
    Matrix4x4f pos_xform;
    while( geoinst_it.next(&geoinst_idx, &pos_xform) )
        count++;
    return count;
}



Meshdata::JointIterator Meshdata::getJointIterator() const {
    return JointIterator(this);
}

uint32 Meshdata::getJointCount() const {
    uint32 count = 0;
    Meshdata::JointIterator joint_it = getJointIterator();
    uint32 joint_id;
    uint32 joint_idx;
    Matrix4x4f pos_xform;
    uint32 parent_id;
    std::vector<Matrix4x4f> transformList;
    while( joint_it.next(&joint_id, &joint_idx, &pos_xform, &parent_id, transformList) )
        count++;
    return count;
}



Meshdata::LightInstanceIterator Meshdata::getLightInstanceIterator() const {
    return LightInstanceIterator(this);
}

uint32 Meshdata::getInstancedLightCount() const {
    uint32 count = 0;
    Meshdata::LightInstanceIterator lightinst_it = getLightInstanceIterator();
    uint32 lightinst_idx;
    Matrix4x4f pos_xform;
    while( lightinst_it.next(&lightinst_idx, &pos_xform) )
        count++;
    return count;
}




Meshdata::GeometryInstanceIterator::GeometryInstanceIterator(const Meshdata* const mesh)
 : mMesh(mesh),
   mRoot(-1)
{
}

bool Meshdata::GeometryInstanceIterator::next(uint32* geo_idx, Matrix4x4f* xform) {
    while(true) { // Outer loop keeps us moving until we hit something to return

        // First, if we emptied out, try to handle the next root.
        if (mStack.empty()) {
            mRoot++;
            if (mRoot >= (int32)mMesh->rootNodes.size()) return false;

            NodeState st;
            st.index = mMesh->rootNodes[mRoot];
            st.step = NodeState::Nodes;
            st.currentChild = -1;
            st.transform = mMesh->globalTransform * mMesh->nodes[st.index].transform;
            mStack.push(st);
        }

        NodeState& node = mStack.top();

        if (node.step == NodeState::Nodes) {
            node.currentChild++;
            if (node.currentChild >= (int)mMesh->nodes[node.index].children.size()) {
                node.step = NodeState::InstanceNodes;
                node.currentChild = -1;
                continue;
            }

            NodeState st;
            st.index = mMesh->nodes[node.index].children[node.currentChild];
            st.step = NodeState::Nodes;
            st.currentChild = -1;
            st.transform = node.transform * mMesh->nodes[ st.index ].transform;
            mStack.push(st);
            continue;
        }

        if (node.step == NodeState::InstanceNodes) {
            node.currentChild++;
            if (node.currentChild >= (int)mMesh->nodes[node.index].instanceChildren.size()) {
                node.step = NodeState::InstanceGeometries;
                node.currentChild = -1;
                continue;
            }

            NodeState st;
            st.index = mMesh->nodes[node.index].instanceChildren[node.currentChild];
            st.step = NodeState::Nodes;
            st.currentChild = -1;
            st.transform = node.transform * mMesh->nodes[ st.index ].transform;
            mStack.push(st);
            continue;
        }

        if (node.step == NodeState::InstanceGeometries) {
            // FIXME this step is inefficient because each node doesn't have a
            // list of instance geometries. Instead, we have to iterate over all
            // instance geometries for all nodes.
            node.currentChild++;
            if (node.currentChild >= (int)mMesh->instances.size()) {
                node.step = NodeState::Done;
                continue;
            }
            // Need this check since we're using the global instances list
            if (node.index != mMesh->instances[node.currentChild].parentNode)
                continue;
            // Otherwise, just yield the information
            *geo_idx = node.currentChild;
            *xform = node.transform;

            return true;
        }

        if (node.step == NodeState::Done) {
            // We're finished with the node, just pop it so parent will continue
            // processing
            mStack.pop();
        }
    }
}




Meshdata::JointIterator::JointIterator(const Meshdata* const mesh)
 : mMesh(mesh),
   mRoot(-1),
   mNextID(1)
{
}

  bool Meshdata::JointIterator::next(uint32* joint_id, uint32* joint_idx, Matrix4x4f* xform, uint32* parent_id,
                                     std::vector<Matrix4x4f>& transformList)
  {
    while(true) { // Outer loop keeps us moving until we hit something to return

        // First, if we emptied out, try to handle the next root.
        if (mStack.empty()) {
            mRoot++;
            if (mRoot >= (int32)mMesh->rootNodes.size()) return false;

            JointNodeState st;
            st.index = mMesh->rootNodes[mRoot];
            st.step = NodeState::Init;
            st.currentChild = -1;
            st.transform = mMesh->globalTransform * mMesh->nodes[st.index].transform;

            st.joint_id = 0;
            mStack.push(st);
        }

        JointNodeState& node = mStack.top();

        if (node.step == NodeState::Init) {
            // On the first visit, check if this is a joint and yield
            // it. However, no matter what, we'll need to advance to the next
            // step
            node.step = NodeState::Nodes;
            node.currentChild = -1;

            for(uint32 i = 0; i < mMesh->joints.size(); i++) {
                if (mMesh->joints[i] == node.index) {
                    // Otherwise, just yield the information
                    // Allocate new joint id, but save the current setting first
                    // since we need it as the parent_id.
                    *parent_id = node.joint_id;
                    node.joint_id = mNextID; // allocate new id
                    mNextID++;
                    *joint_id = node.joint_id;
                    *joint_idx = i;
                    *xform = node.transform;

                    transformList = mMesh->mInstanceControllerTransformList;
                    return true;
                }
            }
        }

        if (node.step == NodeState::Nodes) {
            node.currentChild++;
            if (node.currentChild >= (int)mMesh->nodes[node.index].children.size()) {
                node.step = NodeState::InstanceNodes;
                node.currentChild = -1;
                continue;
            }

            JointNodeState st;
            st.index = mMesh->nodes[node.index].children[node.currentChild];
            st.step = NodeState::Init;
            st.currentChild = -1;

            st.transform = node.transform * mMesh->nodes[ st.index ].transform;

            st.joint_id = node.joint_id;
            mStack.push(st);
            continue;
        }

        if (node.step == NodeState::InstanceNodes) {
            node.currentChild++;
            if (node.currentChild >= (int)mMesh->nodes[node.index].instanceChildren.size()) {
                node.step = NodeState::Done;
                node.currentChild = -1;
                continue;
            }

            JointNodeState st;
            st.index = mMesh->nodes[node.index].instanceChildren[node.currentChild];
            st.step = NodeState::Init;
            st.currentChild = -1;
            st.transform = node.transform * mMesh->nodes[ st.index ].transform;

            st.joint_id = node.joint_id;
            mStack.push(st);
            continue;
        }

        if (node.step == NodeState::Done) {
            // We're finished with the node, just pop it so parent will continue
            // processing
            mStack.pop();
        }
    }
}




Meshdata::LightInstanceIterator::LightInstanceIterator(const Meshdata* const mesh)
 : mMesh(mesh),
   mRoot(-1)
{
}

bool Meshdata::LightInstanceIterator::next(uint32* light_idx, Matrix4x4f* xform) {
    while(true) { // Outer loop keeps us moving until we hit something to return

        // First, if we emptied out, try to handle the next root.
        if (mStack.empty()) {
            mRoot++;
            if (mRoot >= (int)mMesh->rootNodes.size()) return false;

            NodeState st;
            st.index = mMesh->rootNodes[mRoot];
            st.step = NodeState::Nodes;
            st.currentChild = -1;
            st.transform = mMesh->globalTransform;
            mStack.push(st);
        }

        NodeState& node = mStack.top();

        if (node.step == NodeState::Nodes) {
            node.currentChild++;
            if (node.currentChild >= (int)mMesh->nodes[node.index].children.size()) {
                node.step = NodeState::InstanceNodes;
                node.currentChild = -1;
                continue;
            }

            NodeState st;
            st.index = mMesh->nodes[node.index].children[node.currentChild];
            st.step = NodeState::Nodes;
            st.currentChild = -1;
            st.transform = node.transform * mMesh->nodes[ st.index ].transform;
            mStack.push(st);
            continue;
        }

        if (node.step == NodeState::InstanceNodes) {
            node.currentChild++;
            if (node.currentChild >= (int)mMesh->nodes[node.index].instanceChildren.size()) {
                node.step = NodeState::InstanceLights;
                node.currentChild = -1;
                continue;
            }

            NodeState st;
            st.index = mMesh->nodes[node.index].instanceChildren[node.currentChild];
            st.step = NodeState::Nodes;
            st.currentChild = -1;
            st.transform = node.transform * mMesh->nodes[ st.index ].transform;
            mStack.push(st);
            continue;
        }

        if (node.step == NodeState::InstanceLights) {
            // FIXME this step is inefficient because each node doesn't have a
            // list of instance lights. Instead, we have to iterate over all
            // instance lights for all nodes.
            node.currentChild++;
            if (node.currentChild >= (int)mMesh->lightInstances.size()) {
                node.step = NodeState::Done;
                continue;
            }
            // Need this check since we're using the global instances list
            if (node.index != mMesh->lightInstances[node.currentChild].parentNode)
                continue;
            // Otherwise, just yield the information
            *light_idx = node.currentChild;
            *xform = node.transform;
            return true;
        }

        if (node.step == NodeState::Done) {
            // We're finished with the node, just pop it so parent will continue
            // processing
            mStack.pop();
        }
    }
}


} // namespace Mesh
} // namespace Sirikata
