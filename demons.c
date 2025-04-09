#include <stdio.h>
#include <stdlib.h>

#include <gmp.h>
#include <mpfr.h>
#include <mpf2mpfr.h>

#define MAXD 5
#define MAXT 120000000

#define PRINT_NOKILL
#define PRINT_FAIRS

#define P(d,t) probs[((d)-1)*(MAXD+1) + (((t)-1) % (MAXD+1))]


int main(void) {
    mpf_set_default_prec(200);

#ifdef PRINT_NOKILL
    int nokillmods[6] = {0, 2, 3, 12, 20, 60};
    int nokilldata[6*60];
    for (int i = 0; i < 6*60; ++i) nokilldata[i] = -1;
#endif

#ifdef PRINT_FAIRS
    int fairs[6] = {0};
#endif

    mpf_t probs[MAXD * (MAXD+1)];
    mpf_t dkill, tkill, nkill;
    for (int i = 0; i < MAXD * (MAXD+1); ++i) mpf_init(probs[i]);
    mpf_init(dkill); mpf_init(tkill); mpf_init(nkill);

    for (int t = 1; t <= MAXT; ++t) {
        if (t % 1000000 == 0) printf("at t=%d\n", t);
        for (int d = 1; d <= MAXD; ++d) {
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
            mpf_div_ui(P(d,t), P(d,t), d+t);
            if (mpf_cmp(P(d,t), nkill) < 0) {
#ifdef PRINT_NOKILL
                if (nokilldata[d*60 + (t%nokillmods[d])] == -1) {
                    nokilldata[d*60 + (t%nokillmods[d])] = t;
                    printf("nokill from d=%d t=%d (%d mod %d)\n", d, t, t%nokillmods[d], nokillmods[d]);
                }
#endif
                mpf_set(P(d,t), nkill);
            }
#ifdef PRINT_NOKILL
            else if (nokilldata[d*60 + (t%nokillmods[d])] != -1) {
                printf("nokill broken at d=%d t=%d (%d mod %d)\n", d, nokilldata[d*60 + (t%nokillmods[d])], t%nokillmods[d], nokillmods[d]);
                nokilldata[d*60 + (t%nokillmods[d])] = -1;
            }
#endif

#ifdef PRINT_FAIRS
            if (!fairs[d] && mpf_cmp_d(P(d,t), 0.5) > 0) {
                fairs[d] = 1;
                printf("P(d=%d,t=%d) = ", d, t-1);
                mpf_out_str(stdout, 10, 0, P(d,t-1));
                printf("\nP(d=%d,t=%d) = ", d, t);
                mpf_out_str(stdout, 10, 0, P(d,t));
                printf("\n");
            }
#endif

        }
    }

    return 0;
}
