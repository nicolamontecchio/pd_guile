#include <stdio.h>
#include <libguile.h>


SCM
pdguile_guile_catch (void *procedure, void *data)
{
  SCM value;
  value = scm_internal_catch (SCM_BOOL_T,
			      (scm_t_catch_body)procedure,
			      data,
			      (scm_t_catch_handler) scm_handle_by_message_noexit,
			      NULL);
  return value;
}

struct t_guile_function
{
  SCM proc;
  SCM *argv;
  size_t nargs;
};

SCM pdguile_guile_scm_call_n (void *proc)
{
  struct t_guile_function *guile_function;
  guile_function = (struct t_guile_function *)proc;
  return scm_call_n (guile_function->proc,
		     guile_function->argv, guile_function->nargs);
}


SCM pdguile_guile_exec_function (const char *function, SCM *argv, size_t nargs)
{
  SCM func, func2, value;
  struct t_guile_function guile_function;
  func = pdguile_guile_catch (scm_c_lookup, (void *)function);
  func2 = pdguile_guile_catch (scm_variable_ref, func);
  if (argv)
  {
    guile_function.proc = func2;
    guile_function.argv = argv;
    guile_function.nargs = nargs;
    value = pdguile_guile_catch (pdguile_guile_scm_call_n, &guile_function);
  }
  else
  {
    value = pdguile_guile_catch (scm_call_0, func2);
  }
  return value;
}


int main(int argc, char *argv[])
{
  scm_init_guile();
  scm_c_primitive_load("script.scm");
  SCM cm = scm_current_module ();

  char *func_name = "thiscrashes";

  SCM *args = malloc(sizeof(SCM) * 2);
  args[0] = scm_from_double(20);
  args[1] = scm_from_double(10);

  SCM p = pdguile_guile_exec_function(func_name, args, 2);

  /* printf("%f\n", scm_to_double(p)); */
  free(args);
  return 0;
}
