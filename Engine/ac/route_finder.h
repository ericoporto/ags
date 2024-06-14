//=============================================================================
//
// Adventure Game Studio (AGS)
//
// Copyright (C) 1999-2011 Chris Jones and 2011-2024 various contributors
// The full list of copyright holders can be found in the Copyright.txt
// file, which is part of this source code distribution.
//
// The AGS source code is provided under the Artistic License 2.0.
// A copy of this license can be found in the file License.txt and at
// https://opensource.org/license/artistic-2-0/
//
//=============================================================================
#ifndef __AGS_EN_AC__ROUTEFINDER_H
#define __AGS_EN_AC__ROUTEFINDER_H

#include <memory>
#include <vector>
#include "util/geometry.h"

struct MoveList;

namespace AGS
{

namespace Common { class Bitmap; }

namespace Engine
{

// IRouteFinder: a basic pathfinding interface.
// FIXME: separate finding a path and calculating MoveLists! The latter must not depend on a pathfinder impl.
class IRouteFinder 
{
public:
    virtual ~IRouteFinder() = default;

    // Traces a straight line between two points, returns if it's fully passable;
    // optionally assigns last found passable position.
    virtual bool CanSeeFrom(int srcx, int srcy, int dstx, int dsty, int *lastcx = nullptr, int *lastcy = nullptr) = 0;
    // Search for a route between (srcx,y) and (destx,y), and returns a vector of Points.
    // exact_dest - tells to fail if the destination is inside the wall and cannot be reached;
    //              otherwise pathfinder will try to find the closest possible end point.
    // ignore_walls - tells to ignore impassable areas (builds a straight line path).
    virtual bool FindRoute(std::vector<Point> &path, int srcx, int srcy, int dstx, int dsty,
        bool exact_dest = false, bool ignore_walls = false) = 0;
};

// MaskRouteFinder: a mask-based RouteFinder.
// Works with a 8-bit mask, where each color index represents certain walkable area,
// while index 0 represents a wall (impassable).
class MaskRouteFinder : public IRouteFinder
{
public:
    // Assign a walkable mask, and an optional coordinate scale factor which will be used
    // to convert (divide) input coordinates, and resulting path back (multiply).
    // Note that this may make routefinder to generate additional data, taking more time.
    virtual void SetWalkableArea(const AGS::Common::Bitmap *walkablearea, uint32_t coord_scale = 1) = 0;
};


//
// Various additional pathfinding functions and helpers.
// Manages converting navigation paths into MoveLists.
namespace Pathfinding
{
    // Creates a default engine's MaskRouteFinder implementation
    std::unique_ptr<MaskRouteFinder> CreateDefaultMaskPathfinder();

    // Find route using a provided IRouteFinder, and calculate the MoveList using move speeds
    bool FindRoute(MoveList &mls, IRouteFinder *finder, int srcx, int srcy, int dstx, int dsty,
        int move_speed_x, int move_speed_y, bool exact_dest, bool ignore_walls);
    // Calculate the MoveList from the given navigation path and move speeds.
    bool CalculateMoveList(MoveList &mls, const std::vector<Point> path, int move_speed_x, int move_speed_y);
    // Append a waypoint to the move list, skip pathfinding
    bool AddWaypointDirect(MoveList &mls, int x, int y, int move_speed_x, int move_speed_y);
    // Recalculates MoveList's step speeds
    void RecalculateMoveSpeeds(MoveList &mls, int old_speed_x, int old_speed_y, int new_speed_x, int new_speed_y);
}

} // namespace Engine
} // namespace AGS

#endif // __AGS_EN_AC__ROUTEFINDER_H
