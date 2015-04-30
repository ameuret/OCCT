// Created on: 2014-05-22
// Created by: Varvara POSKONINA
// Copyright (c) 2005-2014 OPEN CASCADE SAS
//
// This file is part of Open CASCADE Technology software library.
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 2.1 as published
// by the Free Software Foundation, with special exception defined in the file
// OCCT_LGPL_EXCEPTION.txt. Consult the file LICENSE_LGPL_21.txt included in OCCT
// distribution for complete text of the license and disclaimer of any warranty.
//
// Alternatively, this file may be used under the terms of Open CASCADE
// commercial license or contractual agreement.

#ifndef _SelectMgr_SelectingVolumeManager_HeaderFile
#define _SelectMgr_SelectingVolumeManager_HeaderFile

#include <NCollection_Handle.hxx>

#include <Graphic3d_Camera.hxx>
#include <Graphic3d_SequenceOfHClipPlane.hxx>

#include <SelectMgr_BaseFrustum.hxx>
#include <SelectMgr_RectangularFrustum.hxx>
#include <SelectMgr_TriangularFrustumSet.hxx>
#include <SelectBasics_SelectingVolumeManager.hxx>

//! This class is used to switch between active selecting volumes depending
//! on selection type chosen by the user
class SelectMgr_SelectingVolumeManager : public SelectBasics_SelectingVolumeManager
{
public:

  //! Creates instances of all available selecting volume types
  Standard_EXPORT SelectMgr_SelectingVolumeManager (Standard_Boolean theToAllocateFrustums = Standard_True);

  Standard_EXPORT virtual ~SelectMgr_SelectingVolumeManager() {};

  //! Returns a copy of active frustum transformed according to the matrix given
  Standard_EXPORT virtual SelectMgr_SelectingVolumeManager Transform (const gp_Trsf& theTrsf);

  Standard_EXPORT virtual Standard_Integer GetActiveSelectionType() const Standard_OVERRIDE;

  Standard_EXPORT void SetActiveSelectionType (const SelectionType& theType);

  //! Updates camera projection and orientation matrices in all selecting volumes
  Standard_EXPORT void SetCamera (const Handle(Graphic3d_Camera) theCamera);

  //! Updates camera projection and orientation matrices in all selecting volumes
  Standard_EXPORT void SetCamera (const Graphic3d_Mat4d& theProjection,
                                  const Graphic3d_Mat4d& theOrientation,
                                  const Standard_Boolean theIsOrthographic);

  //! Updates viewport in all selecting volumes
  Standard_EXPORT void SetViewport (const Standard_Real theX,
                                    const Standard_Real theY,
                                    const Standard_Real theWidth,
                                    const Standard_Real theHeight);

  //! Updates pixel tolerance in all selecting volumes
  Standard_EXPORT void SetPixelTolerance (const Standard_Real theTolerance);

  //! Updates window size in all selecting volumes
  Standard_EXPORT void SetWindowSize (const Standard_Integer theWidth, const Standard_Integer theHeight);


  //! Builds rectangular selecting frustum for point selection
  Standard_EXPORT void BuildSelectingVolume (const gp_Pnt2d& thePoint);

  //! Builds rectangular selecting frustum for box selection
  Standard_EXPORT void BuildSelectingVolume (const gp_Pnt2d& theMinPt,
                                             const gp_Pnt2d& theMaxPt);

  //! Builds set of triangular selecting frustums for polyline selection
  Standard_EXPORT void BuildSelectingVolume (const TColgp_Array1OfPnt2d& thePoints);


  //! SAT intersection test between defined volume and given axis-aligned box
  Standard_EXPORT virtual Standard_Boolean Overlaps (const BVH_Box<Standard_Real, 3>& theBndBox,
                                                     Standard_Real& theDepth) Standard_OVERRIDE;

  //! Returns true if selecting volume is overlapped by axis-aligned bounding box
  //! with minimum corner at point theMinPt and maximum at point theMaxPt
  Standard_EXPORT  virtual Standard_Boolean Overlaps (const SelectMgr_Vec3& theMinPt,
                                                      const SelectMgr_Vec3& theMaxPt) Standard_OVERRIDE;

  //! Intersection test between defined volume and given point
  Standard_EXPORT virtual Standard_Boolean Overlaps (const gp_Pnt& thePt,
                                                     Standard_Real& theDepth) Standard_OVERRIDE;

  //! SAT intersection test between defined volume and given ordered set of points,
  //! representing line segments. The test may be considered of interior part or
  //! boundary line defined by segments depending on given sensitivity type
  Standard_EXPORT virtual Standard_Boolean Overlaps (const Handle(TColgp_HArray1OfPnt)& theArrayOfPts,
                                                     Standard_Integer theSensType,
                                                     Standard_Real& theDepth) Standard_OVERRIDE;

  //! Checks if line segment overlaps selecting frustum
  Standard_EXPORT virtual Standard_Boolean Overlaps (const gp_Pnt& thePt1,
                                                     const gp_Pnt& thePt2,
                                                     Standard_Real& theDepth) Standard_OVERRIDE;

  //! SAT intersection test between defined volume and given triangle. The test may
  //! be considered of interior part or boundary line defined by triangle vertices
  //! depending on given sensitivity type
  Standard_EXPORT  virtual Standard_Boolean Overlaps (const gp_Pnt& thePt1,
                                                      const gp_Pnt& thePt2,
                                                      const gp_Pnt& thePt3,
                                                      Standard_Integer theSensType,
                                                      Standard_Real& theDepth) Standard_OVERRIDE;


  //! Measures distance between 3d projection of user-picked
  //! screen point and given point theCOG
  Standard_EXPORT virtual Standard_Real DistToGeometryCenter (const gp_Pnt& theCOG) Standard_OVERRIDE;

  //! Calculates the point on a view ray that was detected during the run of selection algo by given depth. Is valid for point
  //! selection only
  Standard_EXPORT virtual NCollection_Vec3<Standard_Real> DetectedPoint (const Standard_Real theDepth) const Standard_OVERRIDE;

  //! Checks if the point of sensitive in which selection was detected belongs
  //! to the region defined by clipping planes
  Standard_EXPORT virtual Standard_Boolean IsClipped (const Graphic3d_SequenceOfHClipPlane& thePlanes,
                                                      const Standard_Real& theDepth);

private:
  enum { Frustum, FrustumSet, VolumeTypesNb };       //!< Defines the amount of available selecting volumes

  NCollection_Handle<SelectMgr_BaseFrustum> mySelectingVolumes[VolumeTypesNb];      //!< Array of selecting volumes
};

#endif