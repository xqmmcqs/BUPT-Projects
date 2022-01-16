#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int n, m, k;
int **a, **b, **c;
struct v
{
    int i, j;
};

void *cal_sum(void *param);

int main()
{
    scanf("%d%d%d", &n, &k, &m);
    a = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; ++i)
    {
        a[i] = (int *)malloc(k * sizeof(int));
        for (int j = 0; j < k; ++j)
            scanf("%d", &a[i][j]);
    }
    b = (int **)malloc(k * sizeof(int *));
    for (int i = 0; i < k; ++i)
    {
        b[i] = (int *)malloc(m * sizeof(int));
        for (int j = 0; j < m; ++j)
            scanf("%d", &b[i][j]);
    }
    c = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; ++i)
        c[i] = (int *)malloc(m * sizeof(int));

    pthread_t *tid = (pthread_t *)malloc(n * m * sizeof(pthread_t));
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
        {
            struct v *data = (struct v *)malloc(sizeof(struct v));
            data->i = i;
            data->j = j;
            pthread_create(&tid[i * m + j], &attr, cal_sum, data);
        }

    for (int i = 0; i < n * m; ++i)
        pthread_join(tid[i], NULL);

    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
            printf("%d ", c[i][j]);
        puts("");
    }

    pthread_attr_destroy(&attr);
    for (int i = 0; i < n; ++i)
        free(a[i]);
    free(a);
    for (int i = 0; i < k; ++i)
        free(b[i]);
    free(b);
    for (int i = 0; i < n; ++i)
        free(c[i]);
    free(c);
    free(tid);
    return 0;
}

void *cal_sum(void *param)
{
    int i = ((struct v *)param)->i, j = ((struct v *)param)->j;
    c[i][j] = 0;
    for (int l = 0; l < k; ++l)
        c[i][j] += a[i][l] * b[l][j];
    free(param);
    pthread_exit(NULL);
}