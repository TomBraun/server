/* vi: set ts=2:
 +-------------------+  Christian Schlittchen <corwin@amber.kn-bremen.de>
 |                   |  Enno Rehling <enno@eressea.de>
 | Eressea PBEM host |  Katja Zedel <katze@felidae.kn-bremen.de>
 | (c) 1998 - 2003   |  Henning Peters <faroul@beyond.kn-bremen.de>
 |                   |  Ingo Wilken <Ingo.Wilken@informatik.uni-oldenburg.de>
 +-------------------+  Stefan Reich <reich@halbling.de>

 This program may not be used, modified or distributed 
 without prior permission by the authors of Eressea.
*/

#ifndef H_MOD_XMAS
#define H_MOD_XMAS
#ifdef __cplusplus
extern "C" {
#endif

  struct region;
  struct unit;

  extern struct trigger *trigger_xmasgate(struct building *b);

  extern void register_xmas(void);

#ifdef __cplusplus
}
#endif
#endif
