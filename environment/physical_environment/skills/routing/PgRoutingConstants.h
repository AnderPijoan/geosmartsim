#ifndef PGROUTINGCONSTANTS_H
#define PGROUTINGCONSTANTS_H

#include <QObject>

class PgRoutingConstants : public QObject
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

    static PgRoutingConstants::road_types roadClassToRoadType(int road_class){
        switch (road_class){
        case 101: return PgRoutingConstants::h_motorway;
         case 102: return PgRoutingConstants::h_motorway_link;
         case 103: return PgRoutingConstants::h_motorway_junction;
        case 104: return  PgRoutingConstants::h_trunk;
         case 105: return  PgRoutingConstants::h_trunk_link ;
         case 106: return PgRoutingConstants::h_primary;
         case 107: return PgRoutingConstants::h_primary_link;
        case 108: return  PgRoutingConstants::h_secondary;
         case 109: return PgRoutingConstants::h_secondary_link;
         case 110: return PgRoutingConstants::h_tertiary;
         case 111: return PgRoutingConstants::h_tertiary_link;
         case 112: return PgRoutingConstants::h_residential;
         case 113: return PgRoutingConstants::h_living_street;
         case 114: return PgRoutingConstants::h_service;
         case 115: return PgRoutingConstants::h_track;
         case 116: return PgRoutingConstants::h_path;
         case 117: return PgRoutingConstants::h_cycleway;
         case 118: return PgRoutingConstants::h_bridleway;
         case 119: return PgRoutingConstants::h_pedestrian;
         case 120: return PgRoutingConstants::h_footway;
         case 121: return PgRoutingConstants::h_steps;
         case 198: return PgRoutingConstants::h_unclassified;
        case 199: return  PgRoutingConstants::h_road;
        // JUNCTION
         case 401: return PgRoutingConstants::j_roundabout;
        // CYCLEWAY
         case 201: return PgRoutingConstants::c_lane;
         case 202: return PgRoutingConstants::c_track;
         case 203: return PgRoutingConstants::c_opposite_lane;
         case 204: return PgRoutingConstants::c_opposite;
        // TRACKTYPE
         case 301: return PgRoutingConstants::t_grade1;
        case 302: return  PgRoutingConstants::t_grade2;
        case 303: return  PgRoutingConstants::t_grade3;
         case 304: return PgRoutingConstants::t_grade4;
         case 305: return PgRoutingConstants::t_grade5;
        // RAILWAY
         case 501: return PgRoutingConstants::r_funicular;
        case 502: return  PgRoutingConstants::r_light_rail;
         case 503: return PgRoutingConstants::r_narrow_gauge;
         case 504: return PgRoutingConstants::r_preserverd;
         case 505: return PgRoutingConstants::r_rail;
         case 506: return PgRoutingConstants::r_subway;
         case 507: return PgRoutingConstants::r_tram;
        }
        return PgRoutingConstants::h_unclassified;
    }

};

#endif // PGROUTINGCONSTANTS_H
