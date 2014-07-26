/* guile -- use GNU Guile to script stuff inside Pd. */


#include "m_pd.h"
#include <math.h>
static t_class *guile_class;
#define DIMENSION 10

typedef struct _elem
{
    t_float e_age;
    t_float e_weight[DIMENSION];
} t_elem;

typedef struct _guile
{
    t_object x_obj;
    t_elem *x_vec;
    int x_n;
    int x_nonrepeat;
} t_guile;

static void *guile_new(t_float fnonrepeat)
{
    t_guile *x = (t_guile *)pd_new(guile_class);
    outlet_new(&x->x_obj, gensym("float"));
    x->x_vec = (t_elem *)getbytes(0);
    x->x_n = 0;
    x->x_nonrepeat = (fnonrepeat != 0);
    return (x);
}

static void guile_clear(t_guile *x)
{
    x->x_vec = (t_elem *)resizebytes(x->x_vec, x->x_n * sizeof(t_elem), 0);
    x->x_n = 0;
}

static void guile_print(t_guile *x)
{
    int j;
    for (j = 0; j < x->x_n; j++)
    {
        t_elem *e = x->x_vec + j;
        t_float *w = e->e_weight;
        post("%2d age %2d \
w %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2f",
            j, (int)(e->e_age), w[0], w[1], w[2], w[3], w[4], w[5],
                w[6], w[7], w[8], w[9]);
    }
}

static void guile_add(t_guile *x, t_symbol *s, int argc, t_atom *argv)
{
    int oldn = x->x_n, newn = oldn + 1, i;
    t_elem *e;
    t_float sum, normal;
    x->x_vec = (t_elem *)resizebytes(x->x_vec, oldn * sizeof(t_elem),
        newn * sizeof(t_elem));
    x->x_n = newn;
    e = x->x_vec + oldn;
    e->e_age = 2;

    for (i = 0, sum = 0; i < DIMENSION; i++)
    {
        t_float f = atom_getfloatarg(i, argc, argv);
        e->e_weight[i] = f;
        sum += f*f;
    }
    normal = (t_float)(sum > 0 ? 1./sqrt(sum) : 1);
    for (i = 0; i < DIMENSION; i++)
        e->e_weight[i] *= normal;
}

static void guile_list(t_guile *x, t_symbol *s, int argc, t_atom *argv)
{
    int i, j;
    t_float bestsum = 0;
    int bestindex = -1;
    t_float invec[DIMENSION];
    for (i = 0; i < DIMENSION; i++)
        invec[i] = atom_getfloatarg(i, argc, argv);
    for (j = 0; j < x->x_n; j++)
    {
        t_elem *e = x->x_vec + j;
        t_float sum;
        for (i = 0, sum = 0; i < DIMENSION; i++)
            sum += e->e_weight[i] * invec[i];
        if (x->x_nonrepeat) sum *= (t_float)(log(e->e_age));
        if (sum > bestsum)
        {
            bestsum = sum;
            sum = 1;
            bestindex = j;
        }
    }
    if (bestindex >= 0)
    {
        for (j = 0; j < x->x_n; j++)
            x->x_vec[j].e_age += 1.;
        x->x_vec[bestindex].e_age = 1;
    }
    outlet_float(x->x_obj.ob_outlet, (t_float)bestindex);
}

static void guile_free(t_guile *x)
{
    freebytes(x->x_vec, x->x_n * sizeof(t_elem));
}

void guile_setup(void)
{
    guile_class = class_new(gensym("guile"), (t_newmethod)guile_new,
        (t_method)guile_free, sizeof(t_guile), 0, A_DEFFLOAT, 0);
    class_addmethod(guile_class, (t_method)guile_add, gensym("add"), A_GIMME, 0);
    class_addmethod(guile_class, (t_method)guile_clear, gensym("clear"), 0);
    class_addmethod(guile_class, (t_method)guile_print, gensym("print"), 0);
    class_addlist(guile_class, guile_list);
}
