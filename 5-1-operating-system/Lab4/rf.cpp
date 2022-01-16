#include "rf.h"

int shared_data;
int read_count;
sem_t rp_wrt;
sem_t mutex;

void *writer(void *param)
{
    task *t = (task *)param;
    printf("[ID: %d] Created thread\n", t->p_id);
    sem_wait(&rp_wrt); //等待访问权限

    printf("[ID: %d] Entering critical section\n", t->p_id);
    shared_data = t->p_id;
    printf("[ID: %d] Set shared_data = %d\n", t->p_id, t->p_id);
    fprintf(t->file, "[ID: %d] Set shared_data = %d\n", t->p_id, t->p_id);
    sleep(t->spendtime);
    printf("[ID: %d] Exiting critical section\n", t->p_id);

    sem_post(&rp_wrt); //释放访问权限
    printf("[ID: %d] Exiting\n", t->p_id);
    delete t;
    pthread_exit(NULL);
}

void *reader(void *param)
{
    task *t = (task *)param;
    printf("[ID: %d] Created thread\n", t->p_id);
    sem_wait(&mutex); //互斥访问 read_count
    read_count++;
    if (read_count == 1) //如果是第一个读进程，申请获取访问权限
        sem_wait(&rp_wrt);
    sem_post(&mutex);

    printf("[ID: %d] Entering critical section\n", t->p_id);
    printf("[ID: %d] Read shared_data = %d\n", t->p_id, shared_data);
    fprintf(t->file, "[ID: %d] Read shared_data = %d\n", t->p_id, shared_data);
    sleep(t->spendtime);
    printf("[ID: %d] Exiting critical section\n", t->p_id);

    sem_wait(&mutex);
    read_count--;
    if (read_count == 0)
        sem_post(&rp_wrt);
    sem_post(&mutex);
    printf("[ID: %d] Exiting\n", t->p_id);
    delete t;
    pthread_exit(NULL);
}

void simulate(FILE *file, const std::vector<order> &orders)
{
    int orderNum = orders.size();
    pthread_t *p = (pthread_t *)malloc(orderNum * sizeof(pthread_t));
    int *p_id = (int *)malloc(orderNum * sizeof(int));
    sem_init(&rp_wrt, 0, 1);
    sem_init(&mutex, 0, 1);
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
    sem_destroy(&rp_wrt);
    sem_destroy(&mutex);
}
