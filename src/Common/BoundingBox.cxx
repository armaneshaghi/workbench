/*LICENSE_START*/ 
/* 
 *  Copyright 1995-2002 Washington University School of Medicine 
 * 
 *  http://brainmap.wustl.edu 
 * 
 *  This file is part of CARET. 
 * 
 *  CARET is free software; you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation; either version 2 of the License, or 
 *  (at your option) any later version. 
 * 
 *  CARET is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details. 
 * 
 *  You should have received a copy of the GNU General Public License 
 *  along with CARET; if not, write to the Free Software 
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 * 
 */ 

#include <limits>
#include <sstream>
#include <QtCore>
#include "BoundingBox.h"

using namespace caret;

/**
 * Constructor that should never be called.
 *
 */
BoundingBox::BoundingBox()
: CaretObject()
{
    this->initializeMembersBoundingBox();
}

/**
 * Create a bounding box from a six-dimensional array containing
 * min-X, max-X, min-Y, max-Y, min-Z, max-Z.
 * 
 * @param minMaxXYZ - array described above.
 * @throws IllegalArgumentException if the array does not contain six
 *    elements or a min value is greater than a max value.
 *
 */
BoundingBox::BoundingBox(
                   const float minMaxXYZ[])
: CaretObject()
{
    this->initializeMembersBoundingBox();
    for (int i = 0; i < 6; i++) {
        this->boundingBox[i] = minMaxXYZ[i];
    }
}

/**
 * Copy constructor.
 * @param bb 
 *     BoundingBox that is copied.
 */
BoundingBox::BoundingBox(const BoundingBox& bb)
: CaretObject(bb)
{
    this->initializeMembersBoundingBox();
    this->copyHelper(bb);
}

/**
 * Assignment operator.
 * @param bb
 *     BoundingBox that replace this bounding box.
 */
BoundingBox& 
BoundingBox::operator=(const BoundingBox& bb)
{
    if (this != &bb) {
        CaretObject::operator=(bb);
        this->copyHelper(bb);
    }
    return *this;
}

/**
 * Destructor
 */
BoundingBox::~BoundingBox()
{
}

/**
 * Initialize data members.
 */
void
BoundingBox::initializeMembersBoundingBox()
{
}

/**
 * Helps with copy constructor and assignemnt operator.
 */
void 
BoundingBox::copyHelper(const BoundingBox& bo)
{
    for (int i = 0; i < 6; i++) {
        this->boundingBox[i] = bo.boundingBox[i];
    }
}
void resetZeros();

void resetWithMaximumExtent();

void resetForUpdate();

void set(const float* points3D, const int numPoints);

void set(const float minX,
         const float maxX,
         const float minY,
         const float maxY,
         const float minZ,
         const float maxZ);

void set(const float minMaxXYZ[6]);


/**
 * Reset a new bounding box with the minimum and maximum values
 * all set to zero.
 */
void
BoundingBox::resetZeros()
{
    this->set(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
}

/**
 * Reset a bounding box with the minimum values initialized to
 * the minimum float value and the maximum values initialized to the
 * maximum float value.  Use one of the setMax() or setMinY() methods
 * to update this bounding box.
 */
void
BoundingBox::resetWithMaximumExtent()
{
    const float f = std::numeric_limits<float>::max();
    this->set(-f, f, -f, f, -f, f);
}

/**
 * Reset a  bounding box with the minimum values initialized to
 * the maximum float value and the maximum values initialized to the
 * minimum float value.  Use the update(float[]) method to update
 * the bounding box.
 */
void
BoundingBox::resetForUpdate()
{
    const float f = std::numeric_limits<float>::max();
    this->set(f, -f, f, -f, f, -f);
}

/**
 * Set bounding box using the array of points.
 * @param points3D
 *     Array of three dimensional points.
 */
void
BoundingBox::set(const float* points3D, const int64_t numPoints)
{
    for (int64_t i = 0; i < numPoints; i++) {
        this->update(&points3D[i*3]);
    }
}

/**
 * Set a new bounding box.
 * @param minX  Minimum X-coordinate for bounding box.
 * @param maxX  Maximum X-coordinate for bounding box.
 * @param minY  Minimum Y-coordinate for bounding box.
 * @param maxY  Maximum Y-coordinate for bounding box.
 * @param minZ  Minimum Z-coordinate for bounding box.
 * @param maxZ  Maximum Z-coordinate for bounding box.
 */
void
BoundingBox::set(
                   const float minX,
                   const float maxX,
                   const float minY,
                   const float maxY,
                   const float minZ,
                   const float maxZ)
{
    this->boundingBox[0] = minX;
    this->boundingBox[1] = maxX;
    this->boundingBox[2] = minY;
    this->boundingBox[3] = maxY;
    this->boundingBox[4] = minZ;
    this->boundingBox[5] = maxZ;
}

/**
 * Set a bounding box from a six-dimensional array containing
 * min-X, max-X, min-Y, max-Y, min-Z, max-Z.
 *
 * @param minMaxXYZ - array described above.
 */
void
BoundingBox::set(const float minMaxXYZ[6])
{
    this->boundingBox[0] = minMaxXYZ[0];
    this->boundingBox[1] = minMaxXYZ[1];
    this->boundingBox[2] = minMaxXYZ[2];
    this->boundingBox[3] = minMaxXYZ[3];
    this->boundingBox[4] = minMaxXYZ[4];
    this->boundingBox[5] = minMaxXYZ[5];
}

/**
 * Update the bounding box with the XYZ value passed in.  The bound box
 * must have been created with newInstanceForUpdate() or properly
 * initialized by the user.
 *
 * @param xyz - Three dimensional array containing XYZ.
 * @throws IllegalArgumentException if array does not have three elements.
 *
 */
void
BoundingBox::update(const float xyz[3])
{
    if (xyz[0] < this->boundingBox[0]) this->boundingBox[0] = xyz[0];
    if (xyz[0] > this->boundingBox[1]) this->boundingBox[1] = xyz[0];
    if (xyz[1] < this->boundingBox[2]) this->boundingBox[2] = xyz[1];
    if (xyz[1] > this->boundingBox[3]) this->boundingBox[3] = xyz[1];
    if (xyz[2] < this->boundingBox[4]) this->boundingBox[4] = xyz[2];
    if (xyz[2] > this->boundingBox[5]) this->boundingBox[5] = xyz[2];
}

/**
 * Get the bounds in an array.
 * @return  Array of six containing minX, maxX, minY, maxY, minZ, maxZ.
 *
 */
const float*
BoundingBox::getBounds() const
{
    return this->boundingBox;
}

/**
 * Get the X-Coordinate difference.
 * @return X-Coordinate difference.
 *
 */
float
BoundingBox::getDifferenceX() const
{
    return (this->boundingBox[1] - this->boundingBox[0]);
}

/**
 * Get the Y-Coordinate difference.
 * @return Y-Coordinate difference.
 *
 */
float
BoundingBox::getDifferenceY() const
{
    return (this->boundingBox[3] - this->boundingBox[2]);
}

/**
 * Get the Z-Coordinate difference.
 * @return Z-Coordinate difference.
 *
 */
float
BoundingBox::getDifferenceZ() const
{
    return (this->boundingBox[5] - this->boundingBox[4]);
}

/**
 * Get the X minimum value.
 * @return Its value.
 *
 */
float
BoundingBox::getMinX() const
{
    return this->boundingBox[0];
}

/**
 * Get the X maximum value.
 * @return Its value.
 *
 */
float
BoundingBox::getMaxX() const
{
    return this->boundingBox[1];
}

/**
 * Get the Y minimum value.
 * @return Its value.
 *
 */
float
BoundingBox::getMinY() const
{
    return this->boundingBox[2];
}

/**
 * Get the Y maximum value.
 * @return Its value.
 *
 */
float
BoundingBox::getMaxY() const
{
    return this->boundingBox[3];
}

/**
 * Get the Z minimum value.
 * @return Its value.
 *
 */
float
BoundingBox::getMinZ() const
{
    return this->boundingBox[4];
}

/**
 * Get the Z maximum value.
 * @return Its value.
 *
 */
float
BoundingBox::getMaxZ() const
{
    return this->boundingBox[5];
}

/**
 * Get the minimum XYZ of the bounding box.
 * @return Minimum XYZ of the bounding box.
 * in an array that the caller MUST delete[];
 */
float*
BoundingBox::getMinXYZ() const
{
    float* f = new float[3];
    f[0] = this->boundingBox[0];
    f[1] = this->boundingBox[2];
    f[2] = this->boundingBox[4];
    return f;
}

/**
 * Get the maximum XYZ of the bounding box.
 * @return Maximum XYZ of the bounding box
 * in an array that the caller MUST delete[];
 */
float*
BoundingBox::getMaxXYZ() const
{
    float* f = new float[3];
    f[0] = this->boundingBox[1];
    f[1] = this->boundingBox[3];
    f[2] = this->boundingBox[5];
    return f;
}

/**
 * Set the minimum X value.
 * @param value - new value.
 *
 */
void
BoundingBox::setMinX(const float value)
{
    this->boundingBox[0] = value;
}

/**
 * Set the maximum X value.
 * @param value - new value.
 *
 */
void
BoundingBox::setMaxX(const float value)
{
    this->boundingBox[1] = value;
}

/**
 * Set the minimum Y value.
 * @param value - new value.
 *
 */
void
BoundingBox::setMinY(const float value)
{
    this->boundingBox[2] = value;
}

/**
 * Set the maximum Y value.
 * @param value - new value.
 *
 */
void
BoundingBox::setMaxY(const float value)
{
    this->boundingBox[3] = value;
}

/**
 * Set the minimum Z value.
 * @param value - new value.
 *
 */
void
BoundingBox::setMinZ(const float value)
{
    this->boundingBox[4] = value;
}

/**
 * Set the maximum Z value.
 * @param value - new value.
 *
 */
void
BoundingBox::setMaxZ(const float value)
{
    this->boundingBox[5] = value;
}

/**
 * Is the coordinate within the bounding box?
 * @param xyz - The coordinate.
 * @return  True if coordinate is within the bounding box, else false.
 *
 */
bool
BoundingBox::isCoordinateWithinBoundingBox(const float xyz[]) const
{
    if (xyz[0] < this->boundingBox[0]) return false;
    if (xyz[0] > this->boundingBox[1]) return false;
    if (xyz[1] < this->boundingBox[2]) return false;
    if (xyz[1] > this->boundingBox[3]) return false;
    if (xyz[2] < this->boundingBox[4]) return false;
    if (xyz[2] > this->boundingBox[5]) return false;
    return true;
}

/**
 * Get String representation of bounding box.
 * @return String containing bounding box.
 *
 */
AString
BoundingBox::toString() const
{
    std::stringstream str;
    str << "BoundingBox=["
    << this->boundingBox[0]
    << ","
    << this->boundingBox[1]
    << ","
    << this->boundingBox[2]
    << ","
    << this->boundingBox[3]
    << ","
    << this->boundingBox[4]
    << ","
    << this->boundingBox[5]
    << "]";
    AString s = AString::fromStdString(str.str());
    return s;
}

