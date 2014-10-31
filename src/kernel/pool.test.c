#include <platform.h>
#include <kernel/types.h>

#include "ally.h"
#include "pool.h"
#include "magic.h"
#include "unit.h"
#include "item.h"
#include "faction.h"
#include "region.h"
#include "skill.h"

#include <CuTest.h>
#include <tests.h>
#include <assert.h>
#include <limits.h>

void test_reservation(CuTest *tc) {
    unit *u;
    faction *f;
    region *r;
    struct resource_type *rtype;
    test_cleanup();
    test_create_world();
    rtype = rt_get_or_create("money");
    it_get_or_create(rtype);
    f = test_create_faction(0);
    r = findregion(0, 0);
    assert(r && f && rtype && rtype->itype);
    u = test_create_unit(f, r);
    assert(u);
    i_change(&u->items, rtype->itype, 100);
    CuAssertIntEquals(tc, 100, get_resource(u, rtype));
    CuAssertIntEquals(tc, 0, get_reservation(u, rtype));
    CuAssertIntEquals(tc, 50, change_reservation(u, rtype, 50));
    CuAssertIntEquals(tc, 100, change_reservation(u, rtype, 50));
    CuAssertIntEquals(tc, 50, set_resvalue(u, rtype, 50));
    CuAssertIntEquals(tc, 100, get_resource(u, rtype));
    CuAssertIntEquals(tc, 200, change_resource(u, rtype, 100));
    CuAssertIntEquals(tc, 200, get_resource(u, rtype));
    CuAssertIntEquals(tc, 200, i_get(u->items, rtype->itype));

    test_cleanup();
}

void test_pool(CuTest *tc) {
    unit *u1, *u2, *u3;
    faction *f;
    region *r;
    struct resource_type *rtype;
    ally *al;

    test_cleanup();
    test_create_world();
    rtype = rt_get_or_create("money");
    it_get_or_create(rtype);
    f = test_create_faction(0);
    r = findregion(0, 0);
    assert(r && f && rtype && rtype->itype);
    u1 = test_create_unit(f, r);
    u2 = test_create_unit(f, r);
    u3 = test_create_unit(test_create_faction(0), r);
    assert(u1 && u2);
    i_change(&u1->items, rtype->itype, 100);
    set_resvalue(u1, rtype, 50);
    i_change(&u2->items, rtype->itype, 200);
    set_resvalue(u2, rtype, 100);
    i_change(&u3->items, rtype->itype, 400);
    set_resvalue(u3, rtype, 200);

    CuAssertIntEquals(tc, 50, get_pooled(u1, rtype, GET_SLACK, 40));
    CuAssertIntEquals(tc, 50, get_pooled(u1, rtype, GET_SLACK, INT_MAX));
    CuAssertIntEquals(tc, 100, get_pooled(u1, rtype, GET_SLACK | GET_RESERVE, INT_MAX));
    CuAssertIntEquals(tc, 150, get_pooled(u1, rtype, GET_SLACK | GET_POOLED_SLACK, INT_MAX));
    CuAssertIntEquals(tc, 100, get_pooled(u1, rtype, GET_POOLED_SLACK, INT_MAX));
    CuAssertIntEquals(tc, 200, get_pooled(u1, rtype, GET_POOLED_SLACK | GET_POOLED_RESERVE, INT_MAX));

    al = ally_add(&u3->faction->allies, f);
    al->status = HELP_GUARD;
    CuAssertIntEquals(tc, 0, get_pooled(u1, rtype, GET_ALLIED_SLACK | GET_ALLIED_RESERVE, INT_MAX));
    al->status = HELP_MONEY;
    CuAssertIntEquals(tc, 200, get_pooled(u1, rtype, GET_ALLIED_SLACK, INT_MAX));
    CuAssertIntEquals(tc, 200, get_pooled(u1, rtype, GET_ALLIED_RESERVE, INT_MAX));
    CuAssertIntEquals(tc, 400, get_pooled(u1, rtype, GET_ALLIED_SLACK | GET_ALLIED_RESERVE, INT_MAX));

    CuAssertIntEquals(tc, 100, get_pooled(u1, rtype, GET_ALL, 50));
    CuAssertIntEquals(tc, 300, get_pooled(u1, rtype, GET_ALL, 150));
    CuAssertIntEquals(tc, 300, get_pooled(u1, rtype, GET_ALL, INT_MAX));
}

void test_pool_use(CuTest *tc) {
    unit *u1, *u2, *u3;
    faction *f;
    region *r;
    struct resource_type *rtype;
    ally *al;

    test_cleanup();
    test_create_world();
    rtype = rt_get_or_create("money");
    it_get_or_create(rtype);
    f = test_create_faction(0);
    r = findregion(0, 0);
    assert(r && f && rtype && rtype->itype);
    u1 = test_create_unit(f, r);
    u2 = test_create_unit(f, r);
    u3 = test_create_unit(test_create_faction(0), r);
    assert(u1 && u2);
    i_change(&u1->items, rtype->itype, 100);
    set_resvalue(u1, rtype, 50);
    i_change(&u2->items, rtype->itype, 200);
    set_resvalue(u2, rtype, 100);
    i_change(&u3->items, rtype->itype, 400);
    set_resvalue(u3, rtype, 200);
    al = ally_add(&u3->faction->allies, f);
    al->status = HELP_MONEY;

    CuAssertIntEquals(tc, 10, use_pooled(u1, rtype, GET_SLACK, 10));
    CuAssertIntEquals(tc, 40, use_pooled(u1, rtype, GET_SLACK, 50));
    CuAssertIntEquals(tc, 50, i_get(u1->items, rtype->itype));

    CuAssertIntEquals(tc, 50, get_reservation(u1, rtype));
    CuAssertIntEquals(tc, 10, use_pooled(u1, rtype, GET_RESERVE, 10));
    CuAssertIntEquals(tc, 40, i_get(u1->items, rtype->itype));
    CuAssertIntEquals(tc, 40, get_reservation(u1, rtype));
    CuAssertIntEquals(tc, 40, use_pooled(u1, rtype, GET_RESERVE, 50));

    CuAssertIntEquals(tc, 10, use_pooled(u1, rtype, GET_POOLED_SLACK, 10));
    CuAssertIntEquals(tc, 90, use_pooled(u1, rtype, GET_POOLED_SLACK, 100));
    CuAssertIntEquals(tc, 100, i_get(u2->items, rtype->itype));
    CuAssertIntEquals(tc, 10, use_pooled(u1, rtype, GET_POOLED_RESERVE, 10));
    CuAssertIntEquals(tc, 90, get_reservation(u2, rtype));
    CuAssertIntEquals(tc, 90, use_pooled(u1, rtype, GET_POOLED_RESERVE, 100));
    CuAssertIntEquals(tc, 0, i_get(u2->items, rtype->itype));
}

void test_change_resource(CuTest * tc)
{
    struct unit * u;
    struct faction * f;
    struct region * r;
    const char * names[] = { "money", "aura", "permaura", "horse", "hp", 0 };
    int i;

    test_cleanup();
    test_create_world();
    enable_skill(SK_MAGIC, true);

    r = findregion(0, 0);
    f = test_create_faction(0);
    u = test_create_unit(f, r);
    CuAssertPtrNotNull(tc, u);
    set_level(u, SK_MAGIC, 5);
    create_mage(u, M_DRAIG);

    for (i = 0; names[i]; ++i) {
        const struct resource_type *rtype = rt_find(names[i]);
        int have = get_resource(u, rtype);
        CuAssertIntEquals(tc, have + 1, change_resource(u, rtype, 1));
        CuAssertIntEquals(tc, have + 1, get_resource(u, rtype));
    }
}

CuSuite *get_pool_suite(void)
{
    CuSuite *suite = CuSuiteNew();
    SUITE_ADD_TEST(suite, test_reservation);
    SUITE_ADD_TEST(suite, test_pool);
    SUITE_ADD_TEST(suite, test_pool_use);
    SUITE_ADD_TEST(suite, test_change_resource);
    return suite;
}
