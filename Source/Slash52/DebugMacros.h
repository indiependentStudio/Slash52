// 2023, indiependent.studio. All rights reserved.

#pragma once

#define DRAW_SPHERE(Location) if (GetWorld()) DrawDebugSphere(GetWorld(), Location, 10.f, 24, FColor::Red, false, 30.f);
#define DRAW_LINE(StartLocation, EndLocation) if (GetWorld()) DrawDebugLine(World, StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f);
#define DRAW_POINT(Location) if (GetWorld()) DrawDebugPoint(World, Location, 25, FColor::Red, true, -1.f, 0);

#define DRAW_VECTOR(StartLocation, EndLocation) if (GetWorld()) \
{ \
DrawDebugLine(World, StartLocation, EndLocation, FColor::Red, true, -1.f, 0, 1.f); \
DrawDebugPoint(World, EndLocation, 25, FColor::Red, true, -1.f, 0); \
}
