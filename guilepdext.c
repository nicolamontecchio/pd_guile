/* guile -- use GNU Guile to script stuff inside Pd. */
#include <stdio.h>
#include <string.h>
#include "m_pd.h"
#include <libguile.h>

static t_class *guile_class;

typedef struct _guile
{
  t_object x_obj;
  char guile_src_fullpath[2048];
} t_guile;

static void *guile_new(t_symbol *s, int argc, t_atom *argv)
{
  if(argc != 1)
  {
    post("[guile] must be instantiated with exactly one argument (the script file name)");
    return NULL;
  }
  t_guile *x = (t_guile *)pd_new(guile_class);
  outlet_new(&x->x_obj, gensym("float"));
  scm_init_guile();
  char *path_dir = canvas_getcurrentdir()->s_name;
  char *guile_src_filename = atom_getsymbol(argv + 0)->s_name;
  sprintf(x->guile_src_fullpath, "%s/%s", path_dir, guile_src_filename);
  post("[guile]: loading scheme source from %s", x->guile_src_fullpath);
  if(access(x->guile_src_fullpath, F_OK) == -1)
  {
    post("[guile]: scheme source file does not exist :(");
    return NULL;
  }
  scm_c_primitive_load(x->guile_src_fullpath);
  return (x);
}


static void guile_anything(t_guile *x, t_symbol *s, int argc, t_atom *argv)
{
  if(strcmp(s->s_name, "guile-reload") == 0)
    scm_c_primitive_load(x->guile_src_fullpath);
  else
  {
    char *func_name = s->s_name;
    // TODO scm_c_lookup crashes PD if func_name is incorrect; find out how to wrap exceptions
    SCM func = scm_variable_ref(scm_c_lookup(func_name));

    SCM * args = malloc(sizeof(SCM) * argc);
    int all_good = 1;
    for(int i = 0; i < argc; i++)
    {
      if(argv[i].a_type == A_FLOAT)
      {
	double d = (double)atom_getfloat(argv + i);
	printf("%f\n", d);
	args[i] = scm_from_double(d);
      }
      else if (argv[i].a_type == A_SYMBOL)
      {
	char *s = atom_getsymbol(argv + i)->s_name;
	args[i] = scm_from_locale_string(s);
      }
      else
      {
	post("[guile]: error - data type not supported for list item %d", i);
	all_good = 0;
      }
    }
    if(all_good)
    {
      SCM ret_val = scm_call_n(func, args, argc);
      if(scm_is_number(ret_val))
      {
	double v = scm_to_double(ret_val);
	outlet_float(x->x_obj.ob_outlet, (t_float)v);
      }
      else if(scm_is_string(ret_val))
      {
	char *s = scm_to_locale_string(ret_val);
	outlet_symbol(x->x_obj.ob_outlet, gensym(s));
      }
      // TODO finish with string and list return type

    }
    free(args);
  }
}

static void guile_free(t_guile *x)
{
  // TODO destructor

  /* x->x_obj
   */


}


void guile_setup(void)
{

  guile_class = class_new(gensym("guile"), (t_newmethod)guile_new,
			  (t_method)guile_free, sizeof(t_guile), 0, A_GIMME, 0);
  class_addanything(guile_class, (t_method)guile_anything);
}
