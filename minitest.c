#include <stdio.h>
#include <libguile.h>




int main(int argc, char *argv[])
{
  scm_init_guile();
  scm_c_primitive_load("script.scm");
  SCM cm = scm_current_module ();

  char *func_name = "print-hello";

  SCM f = scm_module_variable(cm, scm_from_utf8_symbol(func_name));

  if(scm_is_false(f))
  {
    printf("couldn't load function %s\n", func_name);
    exit(0);
  }

  SCM func = scm_variable_ref(f);

  SCM *args = malloc(sizeof(SCM) * 2);
  args[0] = scm_from_double(20);
  args[1] = scm_from_double(10);
  SCM p = scm_call_n(func, args, 2);
  printf("%f\n", scm_to_double(p));
  free(args);
  return 0;
}
