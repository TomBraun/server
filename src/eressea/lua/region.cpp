#include <config.h>
#include <eressea.h>
#include "list.h"

// kernel includes
#include <kernel/building.h>
#include <kernel/item.h>
#include <kernel/plane.h>
#include <kernel/region.h>
#include <kernel/ship.h>
#include <kernel/unit.h>

// lua includes
#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/iterator_policy.hpp>
#ifdef HAVE_LUABIND_B7
# include <luabind/operator.hpp>
#endif

#include <ostream>
using namespace luabind;

static eressea::list<region *>
get_regions(void) {
  return eressea::list<region *>(regions);
}

static eressea::list<unit *>
region_units(const region& r) {
  return eressea::list<unit *>(r.units);
}

static eressea::list<building *>
region_buildings(const region& r) {
  return eressea::list<building *>(r.buildings);
}

static eressea::list<ship *>
region_ships(const region& r) {
  return eressea::list<ship *>(r.ships);
}

static void
region_setname(region& r, const char * name) {
  if (r.land) rsetname((&r), name);
}

static const char *
region_getterrain(const region& r) {
  return terrain[r.terrain].name;
}

static const char *
region_getname(const region& r) {
  if (r.land) return r.land->name;
  return terrain[r.terrain].name;
}

static void
region_setinfo(region& r, const char * info) {
  set_string(&r.display, info);
}

static const char *
region_getinfo(const region& r) {
  return r.display;
}

static int 
region_plane(const region& r)
{
  if (r.planep==NULL) return 0;
  return r.planep->id;
}

static void
region_addnotice(region& r, const char * str)
{
  addmessage(&r, NULL, str, MSG_MESSAGE, ML_IMPORTANT);
}

static std::ostream& 
operator<<(std::ostream& stream, const region& r)
{
  stream << regionname(&r, NULL) << ", " << region_getterrain(r);
  return stream;
}

static bool 
operator==(const region& a, const region&b)
{
  return a.x==b.x && a.y==b.y;
}

static bool
region_getflag(const region& r, int bit)
{
  if (r.flags & (1<<bit)) return true;
  return false;
}

static void
region_setflag(region& r, int bit, bool set)
{
  if (set) r.flags |= (1<<bit);
  else r.flags &= ~(1<<bit);
}

static int
region_getresource(const region& r, const char * type)
{
	const resource_type * rtype = rt_find(type);
  if (rtype==rt_find("money")) return rmoney(&r);
  if (rtype==rt_find("peasant")) return rpeasants(&r);
	return 0;
}

static void
region_setroad(region& r, int dir, lua_Number size)
{
  rsetroad(&r, (direction_t)dir, (short)(terrain[rterrain(&r)].roadreq * size));
}

static lua_Number
region_getroad(region& r, int dir)
{
  lua_Number result = rroad(&r, (direction_t)dir);
  return terrain[rterrain(&r)].roadreq / result;
}

static region *
terraform_region(short x, short y, const char * tname)
{
  terrain_t t;

  if (tname==NULL) {
    t = NOTERRAIN;
  } else {
    for (t=0;t!=MAXTERRAINS;++t) {
      if (strcmp(terrain[t].name, tname)==0) break;
    }
    if (t==MAXTERRAINS) return NULL;
  }

  region * r = findregion(x, y);
  if (t==NOTERRAIN) {
    if (r!=NULL) {
      if (r->units!=NULL) {
        // TODO: error message
        return r; 
      }
      terraform(r, T_FIREWALL);
      // TODO: durch einen NULL-�quivalenten terraintyp ersetzen
    }
    return NULL;
  }
  if (r==NULL) r = new_region(x, y);
  terraform(r, t);
  return r;
}

static region *
region_next(const region& r, int dir)
{
  if (dir<0 || dir >=MAXDIRECTIONS) return NULL;
  return r_connect(&r, (direction_t)dir);
}

static void
region_adddirection(region& r, region &rt, const char * name, const char * info)
{
  create_special_direction(&r, &rt, -1, info, name);
  spec_direction * sd = special_direction(&r, &rt);
  sd->active = 1;
}

static void
region_remove(region& r)
{
  region ** rp = &regions;
  while (*rp) {
    if (*rp==&r) {
      while (r.units) {
        destroy_unit(r.units);
      }
      *rp = r.next;
#ifdef FAST_CONNECT
      direction_t dir;
      for (dir=0;dir!=MAXDIRECTIONS;++dir) {
        region * rn = r.connect[dir];
        if (rn) {
          direction_t reldir = reldirection(rn, &r);
          r.connect[dir] = NULL;
          rn->connect[reldir] = NULL;
        }
      }
#endif
      runhash(&r);
      break;
    }
    rp = &(*rp)->next;
  }
}

void
region_move(region& r, short x, short y)
{
  if (findregion(x,y)) {
    log_error(("Bei %d, %d gibt es schon eine Region.\n", x, y));
    return;
  }
#ifdef FAST_CONNECT
  direction_t dir;
  for (dir=0;dir!=MAXDIRECTIONS;++dir) {
    region * rn = r.connect[dir];
    if (rn!=NULL) {
      direction_t reldir = reldirection(rn, &r);
      rn->connect[reldir] = NULL;
    }
    rn = findregion(x+delta_x[dir], y+delta_y[dir]);
    if (rn!=NULL) {
      direction_t reldir = (direction_t)((dir + 3) % MAXDIRECTIONS);
      rn->connect[reldir] = &r;
    }
    r.connect[dir] = rn;
  }
#endif
  runhash(&r);
  r.x = x;
  r.y = y;
  rhash(&r);
}

void
bind_region(lua_State * L) 
{
  module(L)[
    def("regions", &get_regions, return_stl_iterator),
    def("get_region", &findregion),
    def("terraform", &terraform_region),

    class_<struct region>("region")
    .def(tostring(self))
    .def(self == region())
    .property("name", &region_getname, &region_setname)
    .property("info", &region_getinfo, &region_setinfo)
    .property("terrain", &region_getterrain)
    .def("add_notice", &region_addnotice)
    .def("add_direction", &region_adddirection)

    .def("get_flag", &region_getflag)
    .def("set_flag", &region_setflag)

    .def("remove", &region_remove)
    .def("move", &region_move)

    .def("get_road", &region_getroad)
    .def("set_road", &region_setroad)

    .def("next", &region_next)
    .def("get_resource", &region_getresource)
    .def_readonly("x", &region::x)
    .def_readonly("y", &region::y)
    .def_readwrite("age", &region::age)
    .property("plane_id", &region_plane)
    .property("units", &region_units, return_stl_iterator)
    .property("buildings", &region_buildings, return_stl_iterator)
    .property("ships", &region_ships, return_stl_iterator)
  ];
}
