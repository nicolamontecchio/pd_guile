/* guile -- use GNU Guile to script stuff inside Pd. */
#include "m_pd.h"
#include <math.h>
#include <libguile.h>

static t_class *guile_class;

typedef struct _guile
{
  t_object x_obj;
  // other members here
  /* SCM func; */

} t_guile;

static void *guile_new(t_float fnonrepeat)
{
  t_guile *x = (t_guile *)pd_new(guile_class);
  outlet_new(&x->x_obj, gensym("float"));
  scm_init_guile();
  /* scm_c_primitive_load( "/Users/echonest/repos/pd_guile/script.scm" ); */
  /* x->func = scm_variable_ref( scm_c_lookup( "print-hello" ) ); */
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
			  (t_method)guile_free, sizeof(t_guile), 0, A_DEFFLOAT, 0);


  /* scm_call_0( func ); */


  /* class_addmethod(guile_class, (t_method)guile_add, gensym("add"), A_GIMME, 0); */
  /* class_addlist(guile_class, guile_list); */
  class_addmethod(guile_class, (t_method)guile_sayhi, gensym("sayhi"), 0);

}
