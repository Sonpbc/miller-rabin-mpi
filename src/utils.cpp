#include "../headers/utils.h"


Testrun get_testrun_params() {

  Testrun testrun;
  testrun.iterations = 1;
  const char* testrun_env = getenv("TESTRUN");
  testrun.running = (testrun_env != NULL && (*testrun_env) == 'y');
  if (testrun.running) {
    const char* testiterations_env = getenv("TESTITERATIONS");
    if (testiterations_env != NULL) {
      testrun.iterations = atoi(testiterations_env);
    }
  }
  return testrun;
}
