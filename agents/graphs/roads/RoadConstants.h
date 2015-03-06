#ifndef ROADCONSTANTS_H
#define ROADCONSTANTS_H

#include <QObject>

class RoadConstants : public QObject
{
    Q_OBJECT
public:

    enum road_types         {
        // HIGHWAY
        h_motorway = 101,
        h_motorway_link = 102,
        h_motorway_junction = 103,
        h_trunk = 104,
        h_trunk_link = 105,
        h_primary = 106,
        h_primary_link = 107,
        h_secondary = 108,
        h_secondary_link = 109,
        h_tertiary = 110,
        h_tertiary_link = 111,
        h_residential = 112,
        h_living_street = 113,
        h_service = 114,
        h_track = 115,
        h_path = 116,
        h_cycleway = 117,
        h_bridleway = 118,
        h_pedestrian = 119,
        h_footway = 120,
        h_steps = 121,
        h_unclassified = 198,
        h_road = 199,
        // JUNCTION
        j_roundabout = 401,
        // CYCLEWAY
        c_lane = 201,
        c_track = 202,
        c_opposite_lane = 203,
        c_opposite = 204,
        // TRACKTYPE
        t_grade1 = 301,
        t_grade2 = 302,
        t_grade3 = 303,
        t_grade4 = 304,
        t_grade5 = 305,
        // RAILWAY
        r_funicular = 501,
        r_light_rail = 502,
        r_narrow_gauge = 503,
        r_preserverd = 504,
        r_rail = 505,
        r_subway = 506,
        r_tram = 507,
    };
};


#endif // ROADCONSTANTS_H

