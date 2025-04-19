#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <gmp.h>
#include <mpfr.h>
#include <mpf2mpfr.h>

#define MAXD 5
/* #define MAXT 120000000 */

/* #define PRINT_TABLE 2 */
#define PRINT_NOKILL
#define PRINT_UNDERKILL
/* #define PRINT_FAIRS */

#define P(d,t) probs[((d)-1)*(MAXD+1) + (((t)-1) % (MAXD+1))]


#if MAXD > 5
#undef PRINT_NOKILL
#undef PRINT_UNDERKILL
#endif

int main(void) {
    mpf_set_default_prec(200);

#ifdef PRINT_NOKILL
    int nokillmods[6] = {0, 2, 3, 12, 20, 60};
    int nokilldata[6*60];
    for (int i = 0; i < 6*60; ++i) nokilldata[i] = -1;
#endif

#ifdef PRINT_UNDERKILL
    int underkillmods[6] = {0, 2, 3, 12, 20, 30};
    int underkilldata[6*60];
    for (int i = 0; i < 6*60; ++i) underkilldata[i] = -1;
#endif

#ifdef PRINT_FAIRS
    int fairs[MAXD+1] = {0};
#endif

    mpf_t probs[MAXD * (MAXD+1)];
    mpf_t dkill, tkill, nkill;
    for (int i = 0; i < MAXD * (MAXD+1); ++i) mpf_init(probs[i]);
    mpf_init(dkill); mpf_init(tkill); mpf_init(nkill);

    struct timeval init, prev, cur;
    gettimeofday(&init, NULL); prev = init;

#ifdef MAXT
    for (unsigned int t = 1; t <= MAXT; ++t) {
#else
    for (unsigned int t = 1;; ++t) {
#endif
        if (t % 1000000 == 0) {
            gettimeofday(&cur, NULL);
            unsigned long
                initdiff = (cur.tv_sec - init.tv_sec) * 1000000 + cur.tv_usec - init.tv_usec,
                prevdiff = (cur.tv_sec - prev.tv_sec) * 1000000 + cur.tv_usec - prev.tv_usec;
            prev = cur;
            printf("at t=%d (%lu.%06lus / %lu.%06lus)\n", t,
                    prevdiff / 1000000, prevdiff % 1000000,
                    initdiff / 1000000, initdiff % 1000000);
        }
        for (int d = 1; d <= MAXD; ++d) {
            if (t - d + 1 <= d - 1) {
                mpf_set_ui(dkill, 0);
            } else {
                mpf_set_ui(dkill, 1);
                switch (d) {
                case 15: if (mpf_cmp(dkill, P(d-1,t-14)) > 0) mpf_set(dkill, P(d-1,t-14));
                case 14: if (mpf_cmp(dkill, P(d-1,t-13)) > 0) mpf_set(dkill, P(d-1,t-13));
                case 13: if (mpf_cmp(dkill, P(d-1,t-12)) > 0) mpf_set(dkill, P(d-1,t-12));
                case 12: if (mpf_cmp(dkill, P(d-1,t-11)) > 0) mpf_set(dkill, P(d-1,t-11));
                case 11: if (mpf_cmp(dkill, P(d-1,t-10)) > 0) mpf_set(dkill, P(d-1,t-10));
                case 10: if (mpf_cmp(dkill, P(d-1,t-9))  > 0) mpf_set(dkill, P(d-1,t-9));
                case 9:  if (mpf_cmp(dkill, P(d-1,t-8))  > 0) mpf_set(dkill, P(d-1,t-8));
                case 8:  if (mpf_cmp(dkill, P(d-1,t-7))  > 0) mpf_set(dkill, P(d-1,t-7));
                case 7:  if (mpf_cmp(dkill, P(d-1,t-6))  > 0) mpf_set(dkill, P(d-1,t-6));
                case 6:  if (mpf_cmp(dkill, P(d-1,t-5))  > 0) mpf_set(dkill, P(d-1,t-5));
                case 5:  if (mpf_cmp(dkill, P(d-1,t-4))  > 0) mpf_set(dkill, P(d-1,t-4));
                case 4:  if (mpf_cmp(dkill, P(d-1,t-3))  > 0) mpf_set(dkill, P(d-1,t-3));
                case 3:  if (mpf_cmp(dkill, P(d-1,t-2))  > 0) mpf_set(dkill, P(d-1,t-2));
                case 2:  if (mpf_cmp(dkill, P(d-1,t-1))  > 0) mpf_set(dkill, P(d-1,t-1));
                }
            }
            mpf_mul_ui(dkill, dkill, d);

            if (t - d - 1 <= d) {
                mpf_set_ui(tkill, 0);
            } else {
                mpf_set_ui(tkill, 1);
                switch (d) {
                case 15: if (mpf_cmp(tkill, P(d,t-16)) > 0) mpf_set(tkill, P(d,t-16));
                case 14: if (mpf_cmp(tkill, P(d,t-15)) > 0) mpf_set(tkill, P(d,t-15));
                case 13: if (mpf_cmp(tkill, P(d,t-14)) > 0) mpf_set(tkill, P(d,t-14));
                case 12: if (mpf_cmp(tkill, P(d,t-13)) > 0) mpf_set(tkill, P(d,t-13));
                case 11: if (mpf_cmp(tkill, P(d,t-12)) > 0) mpf_set(tkill, P(d,t-12));
                case 10: if (mpf_cmp(tkill, P(d,t-11)) > 0) mpf_set(tkill, P(d,t-11));
                case 9:  if (mpf_cmp(tkill, P(d,t-10)) > 0) mpf_set(tkill, P(d,t-10));
                case 8:  if (mpf_cmp(tkill, P(d,t-9))  > 0) mpf_set(tkill, P(d,t-9));
                case 7:  if (mpf_cmp(tkill, P(d,t-8))  > 0) mpf_set(tkill, P(d,t-8));
                case 6:  if (mpf_cmp(tkill, P(d,t-7))  > 0) mpf_set(tkill, P(d,t-7));
                case 5:  if (mpf_cmp(tkill, P(d,t-6))  > 0) mpf_set(tkill, P(d,t-6));
                case 4:  if (mpf_cmp(tkill, P(d,t-5))  > 0) mpf_set(tkill, P(d,t-5));
                case 3:  if (mpf_cmp(tkill, P(d,t-4))  > 0) mpf_set(tkill, P(d,t-4));
                case 2:  if (mpf_cmp(tkill, P(d,t-3))  > 0) mpf_set(tkill, P(d,t-3));
                case 1:  if (mpf_cmp(tkill, P(d,t-2))  > 0) mpf_set(tkill, P(d,t-2));
                }
            }
            mpf_mul_ui(tkill, tkill, t);

            if (t - d <= d) {
                mpf_set_ui(nkill, 0);
            } else {
                mpf_set_ui(nkill, 1);
#ifdef PRINT_UNDERKILL
                int dchoice = 0;
#endif
                switch (d) {
                case 15: if (mpf_cmp(nkill, P(d,t-15)) > 0) mpf_set(nkill, P(d,t-15));
                case 14: if (mpf_cmp(nkill, P(d,t-14)) > 0) mpf_set(nkill, P(d,t-14));
                case 13: if (mpf_cmp(nkill, P(d,t-13)) > 0) mpf_set(nkill, P(d,t-13));
                case 12: if (mpf_cmp(nkill, P(d,t-12)) > 0) mpf_set(nkill, P(d,t-12));
                case 11: if (mpf_cmp(nkill, P(d,t-11)) > 0) mpf_set(nkill, P(d,t-11));
                case 10: if (mpf_cmp(nkill, P(d,t-10)) > 0) mpf_set(nkill, P(d,t-10));
                case 9:  if (mpf_cmp(nkill, P(d,t-9))  > 0) mpf_set(nkill, P(d,t-9));
                case 8:  if (mpf_cmp(nkill, P(d,t-8))  > 0) mpf_set(nkill, P(d,t-8));
                case 7:  if (mpf_cmp(nkill, P(d,t-7))  > 0) mpf_set(nkill, P(d,t-7));
                case 6:  if (mpf_cmp(nkill, P(d,t-6))  > 0) mpf_set(nkill, P(d,t-6));
#ifdef PRINT_UNDERKILL
                case 5:  if (mpf_cmp(nkill, P(d,t-5))  > 0) { dchoice = 5; mpf_set(nkill, P(d,t-5)); }
                case 4:  if (mpf_cmp(nkill, P(d,t-4))  > 0) { dchoice = 4; mpf_set(nkill, P(d,t-4)); }
                case 3:  if (mpf_cmp(nkill, P(d,t-3))  > 0) { dchoice = 3; mpf_set(nkill, P(d,t-3)); }
                case 2:  if (mpf_cmp(nkill, P(d,t-2))  > 0) { dchoice = 2; mpf_set(nkill, P(d,t-2)); }
                case 1:  if (mpf_cmp(nkill, P(d,t-1))  > 0) { dchoice = 1; mpf_set(nkill, P(d,t-1)); }
#else
                case 5:  if (mpf_cmp(nkill, P(d,t-5))  > 0) mpf_set(nkill, P(d,t-5));
                case 4:  if (mpf_cmp(nkill, P(d,t-4))  > 0) mpf_set(nkill, P(d,t-4));
                case 3:  if (mpf_cmp(nkill, P(d,t-3))  > 0) mpf_set(nkill, P(d,t-3));
                case 2:  if (mpf_cmp(nkill, P(d,t-2))  > 0) mpf_set(nkill, P(d,t-2));
                case 1:  if (mpf_cmp(nkill, P(d,t-1))  > 0) mpf_set(nkill, P(d,t-1));
#endif
                }
#ifdef PRINT_UNDERKILL
                if (dchoice != d) {
                    if (underkilldata[d*60 + (t%underkillmods[d])] == -1) {
                        underkilldata[d*60 + (t%underkillmods[d])] = dchoice;
                        printf("underkill %d from d=%d t=%d (%d mod %d)\n", dchoice, d, t, t%underkillmods[d], underkillmods[d]);
                    } else if (underkilldata[d*60 + (t%underkillmods[d])] != dchoice) {
                        printf("underkill changed from %d to %d at d=%d t=%d (%d mod %d)\n", underkilldata[d*60 + (t%underkillmods[d])], dchoice, d, t, t%underkillmods[d], underkillmods[d]);
                        underkilldata[d*60 + (t%underkillmods[d])] = dchoice;
                    }
                } else if (underkilldata[d*60 + (t%underkillmods[d])] != -1) {
                    printf("underkill broken at d=%d t=%d (%d mod %d)\n", d, t, t%underkillmods[d], underkillmods[d]);
                    underkilldata[d*60 + (t%underkillmods[d])] = -1;
                }
#endif
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

#ifdef PRINT_TABLE
            if (d == PRINT_TABLE) {
                mpf_out_str(stdout, 10, 0, P(d,t));
                printf("\n");
            }
#endif

        }
    }

    return 0;
}
