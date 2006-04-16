#include <config.h>
#include <eressea.h>

#include "bindings.h"
#include "list.h"
#include "../gmtool_structs.h"

#include <kernel/region.h>

// lua includes
#include <lua.hpp>
#include <luabind/luabind.hpp>
#include <luabind/iterator_policy.hpp>

using namespace luabind;

region *
current_region(void)
{
  map_region * mr = cursor_region(&current_state->display, &current_state->cursor);
  return mr->r;
}

static tag *
next_tag(int hash, const state * st)
{
  while (hash!=MAXTHASH) {
    tag * t = st->selected->tags[hash];
    if (t!=NULL) return t;
    ++hash;
  }
  return NULL;
}

class selectedregion {
public:
  static tag * next(tag * node) {
    if (node->nexthash) {
      return node->nexthash;
    }
    coordinate * c = &node->coord;
    unsigned int key = ((c->x << 12) ^ c->y);
    unsigned int hash = key & (MAXTHASH-1);

    return next_tag(++hash, current_state);
  }

  static region * value(tag * node) {
    return findregion((short)node->coord.x, (short)node->coord.y);
  }
};


static eressea::list<region *, tag *, selectedregion>
selected_regions(void)
{
  return eressea::list<region *, tag *, selectedregion>(next_tag(0, current_state));
}

void
bind_gmtool(lua_State * L)
{
  module(L)[
    def("selected_regions", &selected_regions, return_stl_iterator),
    def("current_region", &current_region)
  ];
#ifdef LUABIND_NO_EXCEPTIONS
  luabind::set_error_callback(error_callback);
#endif
}
