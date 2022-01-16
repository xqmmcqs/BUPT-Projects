#include "equ.h"

int shared_data;
int read_count;
sem_t wrt;
sem_t cs_read;
sem_t mutex_r;

void *writer(void *param)
{
    task *t = (task *)param;
    printf("[ID: %d] Created thread\n", t->p_id);
    sem_wait(&cs_read);
    sem_wait(&wrt); //申请缓冲区访问权限

    printf("[ID: %d] Entering critical section\n", t->p_id);
    shared_data = t->p_id;
    printf("[ID: %d] Set shared_data = %d\n", t->p_id, t->p_id);
    fprintf(t->file, "[ID: %d] Set shared_data = %d\n", t->p_id, t->p_id);
    sleep(t->spendtime);
    printf("[ID: %d] Exiting critical section\n", t->p_id);

    sem_post(&wrt);
    sem_post(&cs_read);
    printf("[ID: %d] Exiting\n", t->p_id);
    delete t;
    pthread_exit(NULL);
}

void *reader(void *param)
{
    task *t = (task *)param;
    printf("[ID: %d] Created thread\n", t->p_id);
    sem_wait(&cs_read); //申请排队权限
    sem_wait(&mutex_r); //互斥访问 read_count
    read_count++;
    if (read_count == 1) //如果是第一个读进程，申请获取访问权限
        sem_wait(&wrt);
    sem_post(&mutex_r);
    sem_post(&cs_read);

    printf("[ID: %d] Entering critical section\n", t->p_id);
    printf("[ID: %d] Read shared_data = %d\n", t->p_id, shared_data);
    fprintf(t->file, "[ID: %d] Read shared_data = %d\n", t->p_id, shared_data);
    sleep(t->spendtime);
    printf("[ID: %d] Exiting critical section\n", t->p_id);

    sem_wait(&mutex_r);
    read_count--;
    if (read_count == 0)
        sem_post(&wrt);
    sem_post(&mutex_r);
    printf("[ID: %d] Exiting\n", t->p_id);
    delete t;
    pthread_exit(NULL);
}

void simulate(FILE *file, const std::vector<order> &orders)
{
    int orderNum = orders.size();
    pthread_t *p = (pthread_t *)malloc(orderNum * sizeof(pthread_t));
    int *p_id = (int *)malloc(orderNum * sizeof(int));
    sem_init(&wrt, 0, 1);
    sem_init(&cs_read, 0, 1);
    sem_init(&mutex_r, 0, 1);
    //读写进程创建
    for (int i = 0; i < orderNum; i++)
    {
        task *t = new task{orders[i].spendtime, i + 1, file};
        if (orders[i].rw == 'R') //创建读进程
        {
            p_id[i] = i + 1;
            pthread_create(&p[i], NULL, reader, t);
        }
        else //创建写进程
        {
            p_id[i] = i + 1;
            pthread_create(&p[i], NULL, writer, t);
        }
    }
    //等待子进程全部结束后主进程结束
    for (int i = 0; i < orderNum; i++)
    {
        pthread_join(p[i], NULL);
    }
    sem_destroy(&wrt);
    sem_destroy(&cs_read);
    sem_destroy(&mutex_r);
}
