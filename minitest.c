#include <stdio.h>
#include <libguile.h>



SCM my_handler_proc (void *handler_data,
                     SCM key,
                     SCM parameters)
{
  printf("something went wrong\n");
  return SCM_BOOL_F;
}


// from weechat:
// https://github.com/weechat/weechat/blob/master/src/plugins/guile/weechat-guile.c
SCM
weechat_guile_catch (void *procedure, void *data)
{
  SCM value;
  value = scm_internal_catch (SCM_BOOL_T,
			      (scm_t_catch_body)procedure,
			      data,
			      (scm_t_catch_handler) scm_handle_by_message_noexit,
			      NULL);
  return value;
}


int main(int argc, char *argv[])
{
  scm_init_guile();
  scm_c_primitive_load("script.scm");
  SCM cm = scm_current_module ();

  /* char *func_name = "thiscrashes"; */
  char *func_name = "flululu";

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

  /* SCM p = scm_call_n(func, args, 2); */

  SCM p = scm_c_catch(SCM_BOOL_T,
		      func, args,
		      my_handler_proc, 0,
		      NULL, NULL);

  printf("%f\n", scm_to_double(p));
  free(args);
  return 0;
}
