#include <stdio.h>
#include <stdlib.h>

#include <gmp.h>
#include <mpfr.h>
#include <mpf2mpfr.h>

/* #define MAXD 5 */
/* #define MAXT 1000000 */

#define MAXD 4
#define MAXT 40000000

#define P(d,t) probs[((d)-1)*MAXT+((t)-1)]

int main(void) {
    mpf_set_default_prec(200);
    int nokillmods[6] = {0, 2, 3, 12, 20, 60};
    int nokilldata[6*60];
    for (int i = 0; i < 6*60; ++i) nokilldata[i] = -1;

    mpf_t *probs = calloc(MAXD * MAXT, sizeof *probs);
    mpf_t dkill, tkill, nkill;
    mpf_init(dkill); mpf_init(tkill); mpf_init(nkill);

    for (int d = 1; d <= MAXD; ++d) {
        printf("processing d=%d\n", d);
        for (int t = 1; t <= MAXT; ++t) {
            mpf_init(P(d,t));

            if (t - d + 1 <= d - 1) {
                mpf_set_ui(dkill, 0);
            } else {
                mpf_set_ui(dkill, 1);
                switch (d) {
                case 5: if (mpf_cmp(dkill, P(d-1,t-4)) > 0) mpf_set(dkill, P(d-1,t-4));
                case 4: if (mpf_cmp(dkill, P(d-1,t-3)) > 0) mpf_set(dkill, P(d-1,t-3));
                case 3: if (mpf_cmp(dkill, P(d-1,t-2)) > 0) mpf_set(dkill, P(d-1,t-2));
                case 2: if (mpf_cmp(dkill, P(d-1,t-1)) > 0) mpf_set(dkill, P(d-1,t-1));
                }
            }
            mpf_mul_ui(dkill, dkill, d);
            mpf_div_ui(dkill, dkill, d+t);

            if (t - d - 1 <= d) {
                mpf_set_ui(tkill, 0);
            } else {
                mpf_set_ui(tkill, 1);
                switch (d) {
                case 5: if (mpf_cmp(tkill, P(d,t-6)) > 0) mpf_set(tkill, P(d,t-6));
                case 4: if (mpf_cmp(tkill, P(d,t-5)) > 0) mpf_set(tkill, P(d,t-5));
                case 3: if (mpf_cmp(tkill, P(d,t-4)) > 0) mpf_set(tkill, P(d,t-4));
                case 2: if (mpf_cmp(tkill, P(d,t-3)) > 0) mpf_set(tkill, P(d,t-3));
                case 1: if (mpf_cmp(tkill, P(d,t-2)) > 0) mpf_set(tkill, P(d,t-2));
                }
            }
            mpf_mul_ui(tkill, tkill, t);
            mpf_div_ui(tkill, tkill, d+t);

            if (t - d <= d) {
                mpf_set_ui(nkill, 0);
            } else {
                mpf_set_ui(nkill, 1);
                switch (d) {
                case 5: if (mpf_cmp(nkill, P(d,t-5)) > 0) mpf_set(nkill, P(d,t-5));
                case 4: if (mpf_cmp(nkill, P(d,t-4)) > 0) mpf_set(nkill, P(d,t-4));
                case 3: if (mpf_cmp(nkill, P(d,t-3)) > 0) mpf_set(nkill, P(d,t-3));
                case 2: if (mpf_cmp(nkill, P(d,t-2)) > 0) mpf_set(nkill, P(d,t-2));
                case 1: if (mpf_cmp(nkill, P(d,t-1)) > 0) mpf_set(nkill, P(d,t-1));
                }
            }

            mpf_add(P(d,t), dkill, tkill);
            if (mpf_cmp(P(d,t), nkill) < 0) {
                if (nokilldata[d*60 + (t%nokillmods[d])] == -1) {
                    nokilldata[d*60 + (t%nokillmods[d])] = t;
                    printf("nokill from d=%d t=%d (%d mod %d)\n", d, t, t%nokillmods[d], nokillmods[d]);
                }
                mpf_set(P(d,t), nkill);
            } else if (nokilldata[d*60 + (t%nokillmods[d])] != -1) {
                printf("nokill broken at d=%d t=%d (%d mod %d)\n", d, nokilldata[d*60 + (t%nokillmods[d])], t%nokillmods[d], nokillmods[d]);
                nokilldata[d*60 + (t%nokillmods[d])] = -1;
            }
        }
    }

    return 0;
}
