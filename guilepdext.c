/* guile -- use GNU Guile to script stuff inside Pd. */


#include "m_pd.h"
#include <math.h>
static t_class *guile_class;


typedef struct _guile
{
  t_object x_obj;
  // other members here
} t_guile;

static void *guile_new(t_float fnonrepeat)
{
  t_guile *x = (t_guile *)pd_new(guile_class);
  outlet_new(&x->x_obj, gensym("float"));
  return (x);
}

static void guile_clear(t_guile *x)
{
  // ?
}


static void guile_free(t_guile *x)
{
  /* freebytes(x->x_vec, x->x_n * sizeof(t_elem)); */
}

void guile_setup(void)
{
  guile_class = class_new(gensym("guile"), (t_newmethod)guile_new,
			  (t_method)guile_free, sizeof(t_guile), 0, A_DEFFLOAT, 0);
  /* class_addmethod(guile_class, (t_method)guile_add, gensym("add"), A_GIMME, 0); */
  /* class_addlist(guile_class, guile_list); */
}
