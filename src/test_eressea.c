#include <platform.h>
#include <kernel/config.h>
#include <CuTest.h>
#include <stdio.h>
#include <util/log.h>


CuSuite *get_tests_suite(void);
CuSuite *get_callback_suite(void);
CuSuite *get_jsonconf_suite(void);
CuSuite *get_json_suite(void);
CuSuite *get_economy_suite(void);
CuSuite *get_laws_suite(void);
CuSuite *get_market_suite(void);
CuSuite *get_battle_suite(void);
CuSuite *get_building_suite(void);
CuSuite *get_curse_suite(void);
CuSuite *get_equipment_suite(void);
CuSuite *get_item_suite(void);
CuSuite *get_magic_suite(void);
CuSuite *get_move_suite(void);
CuSuite *get_pool_suite(void);
CuSuite *get_build_suite(void);
CuSuite *get_reports_suite(void);
CuSuite *get_ship_suite(void);
CuSuite *get_spellbook_suite(void);
CuSuite *get_spell_suite(void);
CuSuite *get_base36_suite(void);
CuSuite *get_config_suite(void);
CuSuite *get_bsdstring_suite(void);
CuSuite *get_functions_suite(void);
CuSuite *get_umlaut_suite(void);
CuSuite *get_ally_suite(void);
CuSuite *get_direction_suite(void);
CuSuite *get_skill_suite(void);
CuSuite *get_keyword_suite(void);

int RunAllTests(void)
{
  CuString *output = CuStringNew();
  CuSuite *suite = CuSuiteNew();
  int flags = log_flags;

  log_flags = LOG_FLUSH | LOG_CPERROR;
  kernel_init();

  /* self-test */
  CuSuiteAddSuite(suite, get_tests_suite());
  CuSuiteAddSuite(suite, get_callback_suite());
  CuSuiteAddSuite(suite, get_json_suite());
  CuSuiteAddSuite(suite, get_jsonconf_suite());
  CuSuiteAddSuite(suite, get_direction_suite());
  CuSuiteAddSuite(suite, get_skill_suite());
  CuSuiteAddSuite(suite, get_keyword_suite());
  /* util */
  CuSuiteAddSuite(suite, get_config_suite());
  CuSuiteAddSuite(suite, get_base36_suite());
  CuSuiteAddSuite(suite, get_bsdstring_suite());
  CuSuiteAddSuite(suite, get_functions_suite());
  CuSuiteAddSuite(suite, get_umlaut_suite());
  /* kernel */
  CuSuiteAddSuite(suite, get_build_suite());
  CuSuiteAddSuite(suite, get_pool_suite());
  CuSuiteAddSuite(suite, get_curse_suite());
  CuSuiteAddSuite(suite, get_equipment_suite());
  CuSuiteAddSuite(suite, get_item_suite());
  CuSuiteAddSuite(suite, get_magic_suite());
  CuSuiteAddSuite(suite, get_move_suite());
  CuSuiteAddSuite(suite, get_reports_suite());
  CuSuiteAddSuite(suite, get_ship_suite());
  CuSuiteAddSuite(suite, get_spellbook_suite());
  CuSuiteAddSuite(suite, get_building_suite());
  CuSuiteAddSuite(suite, get_spell_suite());
  CuSuiteAddSuite(suite, get_battle_suite());
  CuSuiteAddSuite(suite, get_ally_suite());
  /* gamecode */
  CuSuiteAddSuite(suite, get_market_suite());
  CuSuiteAddSuite(suite, get_laws_suite());
  CuSuiteAddSuite(suite, get_economy_suite());

  CuSuiteRun(suite);
  CuSuiteSummary(suite, output);
  CuSuiteDetails(suite, output);
  printf("%s\n", output->buffer);

  log_flags = flags;
  return suite->failCount;
}

int main(int argc, char ** argv) {
    log_stderr = 0;
    return RunAllTests();
}
