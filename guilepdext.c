/* guile -- use GNU Guile to script stuff inside Pd. */
#include <stdio.h>
#include "m_pd.h"
#include <libguile.h>

static t_class *guile_class;

typedef struct _guile
{
  t_object x_obj;
  // other members here

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
  char *script_filename = atom_getsymbol(argv + 0)->s_name;
  char script_fullpath[2048];
  sprintf(script_fullpath, "%s/%s", path_dir, script_filename);
  post("[guile]: loading scheme source from %s", script_fullpath);
  if(access(script_fullpath, F_OK) == -1)
  {
    post("[guile]: scheme source file does not exist :(");
    return NULL;
  }

  return (x);
}

static void guile_free(t_guile *x)
{
  // destructor
}


static void guile_sayhi(t_guile *x)
{
  post("hi from pd");
  /* scm_call_0(x->func); */
}


void guile_setup(void)
{

  guile_class = class_new(gensym("guile"), (t_newmethod)guile_new,
			  (t_method)guile_free, sizeof(t_guile), 0, A_GIMME, 0);


  /* scm_call_0( func ); */


  /* class_addmethod(guile_class, (t_method)guile_add, gensym("add"), A_GIMME, 0); */
  /* class_addlist(guile_class, guile_list); */
  class_addmethod(guile_class, (t_method)guile_sayhi, gensym("sayhi"), 0);

}
