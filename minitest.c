#include <stdio.h>
#include <libguile.h>

int main(int argc, char *argv[])
{
  scm_init_guile();
  scm_c_primitive_load("script.scm");
  char *func_name = "print-hello2";
  SCM func = scm_variable_ref(scm_c_lookup(func_name));
  SCM *args = malloc(sizeof(SCM) * 2);
  args[0] = scm_from_double(20);
  args[1] = scm_from_double(10);
  SCM p = scm_call_n(func, args, 2);
  printf("%f\n", scm_to_double(p));
  free(args);
  return 0;
}
