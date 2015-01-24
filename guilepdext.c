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

struct t_guile_function
{
  SCM proc;
  SCM *argv;
  size_t nargs;
};

SCM pdguile_catch (void *procedure, void *data)
{
  SCM value;
  value = scm_internal_catch (SCM_BOOL_T, (scm_t_catch_body)procedure, data,
			      (scm_t_catch_handler) scm_handle_by_message_noexit, NULL);
  return value;
}

SCM pdguile_scm_call_n (void *proc)
{
  struct t_guile_function *guile_function;
  guile_function = (struct t_guile_function *)proc;
  return scm_call_n (guile_function->proc,
		     guile_function->argv, guile_function->nargs);
}

SCM pdguile_exec_function (const char *function, SCM *argv, size_t nargs)
{
  SCM func, func2, value;
  struct t_guile_function guile_function;
  func = pdguile_catch (scm_c_lookup, (void *)function);
  func2 = pdguile_catch (scm_variable_ref, func);
  if (nargs > 0)
  {
    guile_function.proc = func2;
    guile_function.argv = argv;
    guile_function.nargs = nargs;
    value = pdguile_catch (pdguile_scm_call_n, &guile_function);
  }
  else
  {
    value = pdguile_catch (scm_call_0, func2);
  }
  return value;
}


static void guile_anything(t_guile *x, t_symbol *s, int argc, t_atom *argv)
{
  if(strcmp(s->s_name, "guile-reload") == 0)
    scm_c_primitive_load(x->guile_src_fullpath);
  else if(strcmp(s->s_name, "float") == 0)
  {
    // do nothing TODO FIX if statement
  }
  else
  {
    char *func_name = s->s_name;
    SCM cm = scm_current_module();
    SCM f = scm_module_variable(cm, scm_from_utf8_symbol(func_name));
    if(scm_is_false(f))
    {
      post("[guile]: can't load function %s; check your scheme source\n", func_name);
      return;
    }

    SCM * args = malloc(sizeof(SCM) * argc);
    int all_good = 1;
    for(int i = 0; i < argc; i++)
    {
      if(argv[i].a_type == A_FLOAT)
      {
	double d = (double)atom_getfloat(argv + i);
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
      SCM ret_val = pdguile_exec_function(func_name, args, argc);

      if(scm_is_false(ret_val))
      {
      	// do nothing - this avoid ending in the scm_list_p case
	post("you have some error in your code, check STDOUT\n");
      }
      else if(scm_is_number(ret_val))
      {
      	double v = scm_to_double(ret_val);
      	outlet_float(x->x_obj.ob_outlet, (t_float)v);
      }
      else if(scm_is_string(ret_val))
      {
      	char *s = scm_to_locale_string(ret_val);
      	outlet_symbol(x->x_obj.ob_outlet, gensym(s));
      }
      else if(scm_list_p(ret_val))
      {
      	int l = scm_to_int(scm_length(ret_val));
      	t_atom *out_atoms = malloc(l * sizeof(t_atom));
      	for(int i = 0; i < l; i++)
      	{
      	  SCM item_i = scm_list_ref(ret_val, scm_from_int(i));
      	  if(scm_is_number(item_i))
      	  {
      	    double v = scm_to_double(item_i);
      	    out_atoms[i].a_type = A_FLOAT;
      	    out_atoms[i].a_w.w_float = v;
      	  }
      	  else if(scm_is_string(item_i))
      	  {
      	    char *s = scm_to_locale_string(item_i);
      	    out_atoms[i].a_type = A_SYMBOL;
      	    out_atoms[i].a_w.w_symbol = gensym(s);
      	  }
      	}
	outlet_list(x->x_obj.ob_outlet, gensym("list"), l, out_atoms);
	free(out_atoms);
      }
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
