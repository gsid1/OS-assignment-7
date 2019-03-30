#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

//s->smoke t->tobacco p->paper m->match(in names)

pthread_mutex_t m =	PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t smoker =	PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t agent_c =	PTHREAD_COND_INITIALIZER;
pthread_cond_t s_t_c =	PTHREAD_COND_INITIALIZER;
pthread_cond_t s_m_c =	PTHREAD_COND_INITIALIZER;
pthread_cond_t s_p_c =	PTHREAD_COND_INITIALIZER;

pthread_cond_t t =	PTHREAD_COND_INITIALIZER;
pthread_cond_t p =	PTHREAD_COND_INITIALIZER;
pthread_cond_t m =	PTHREAD_COND_INITIALIZER;

int getRand(int range){
    int randNum = rand() % range;
    return randNum;
}

int have_t = 0;
int have_p = 0;
int have_m = 0;

int agent_job = 1;
int s_t_job = 0;
int s_m_job = 0;
int s_p_job = 0;

void * agent(void * arg){
    while(1) {  
       sleep(1); 
        pthread_mutex_lock(&m);
        while(agent_job == 0)
            pthread_cond_wait(&agent_c, &m);
        int randNum = getRand(3);
        if ( randNum == 0 ) {
           
            agent_job = 0;
            have_m = 1;
            have_p = 1;
            puts("Put paper and match");
            pthread_cond_signal(&paper);
            pthread_cond_signal(&match);

        }
        else if ( randNum == 1 ) {
            agent_job = 0;
            have_m = 1;
            have_t = 1;
            puts("Put tobacco and match");
            pthread_cond_signal(&paper);
            pthread_cond_signal(&match);
        }
        else if ( randNum == 2 ) {
            agent_job = 0;
            have_t = 1;
            have_p = 1;
            puts("Put paper and tobacco");
            pthread_cond_signal(&paper);
            pthread_cond_signal(&tobacco);
        }
        
        pthread_mutex_unlock(&m);
    }
    return 0;
}

void * push_paper(void * arg){
    
    while(1){
        pthread_mutex_lock(&m);
        while(have_p == 0)
            pthread_cond_wait(&paper, &m);
    
        if(have_m == 1) {
            have_m = 0;
            agent_job = 0;
            s_t_job = 1;
            puts("Call the tobacco smoker");
            pthread_cond_signal(&s_t_c);
        }
        if(have_t == 1) {
            have_t = 0;
            agent_job = 0;
            s_m_job = 1;
            puts("Call the match smoker");
            pthread_cond_signal(&s_m_c);
        }
        pthread_mutex_unlock(&m);
    }
    
    return 0 ;
}

void * push_match(void * arg){
    
    while(1) {
        pthread_mutex_lock(&m);
        while(have_m == 0)
            pthread_cond_wait(&match, &m);
    
        if(have_p == 1) {
            have_p = 0;
            agent_job = 0;
            s_t_job = 1;
            puts("Call the tobacco smoker");
            pthread_cond_signal(&s_t_c);
        }
        if(have_t == 1) {
            have_t = 0;
            agent_job = 0;
            s_p_job = 1;
            puts("Call the paper smoker");
            pthread_cond_signal(&s_p_c);
        }
        pthread_mutex_unlock(&m);
    }
    
    return 0 ;
}

void * push_tobacco(void * arg){
    while(1){
        pthread_mutex_lock(&m);
        while(have_t == 0)
            pthread_cond_wait(&tobacco, &m);
    
        if(have_m == 1) {
            have_m = 0;
            agent_job = 0;
            s_p_job = 1;
            puts("Call the paper smoker");
            pthread_cond_signal(&s_p_c);
        }
        if(have_p == 1) {
            have_t = 0;
            agent_job = 0;
            s_m_job = 1;
            puts("Call the match smoker");
            pthread_cond_signal(&s_m_c);
        }
        pthread_mutex_unlock(&m);
    }
    return 0 ;
}

void * smoker_tobacco(void * arg){
    
    while(1){
        
        pthread_mutex_lock(&smoker);
        while(s_t_job == 0)
            pthread_cond_wait(&s_t_c, &smoker);
        have_p = 0;
        have_m = 0;
        s_t_job = 0;
        agent_job = 1;
        puts("Tobacco Smoker: make cigarette...");
        pthread_mutex_unlock(&smoker);
        
        puts("Tobacco Smoker: Smoking...");
    }
    
    return 0;
}

void * smoker_paper(void * arg){
    
    while(1){
        
        pthread_mutex_lock(&smoker);
        while(s_p_job == 0)
            pthread_cond_wait(&s_p_c, &smoker);
        have_t = 0;
        have_m = 0;
        s_p_job = 0;
        agent_job = 1;
        puts("Paper Smoker: make cigarette...");
        pthread_mutex_unlock(&smoker);
        
        puts("Paper Smoker: Smoking...");
    }
    
    return 0;
}

void * smoker_match(void * arg){
    
    while(1){
        
        pthread_mutex_lock(&smoker);
        while(s_m_job == 0)
            pthread_cond_wait(&s_m_c, &smoker);
        have_p = 0;
        have_t = 0;
        s_m_job = 0;
        agent_job = 1;
        puts("Match Smoker: make cigarette...");
        pthread_mutex_unlock(&smoker);
        
        puts("Match Smoker: Smoking...");
    }
    
    return 0;
}


int main(int argc, char *argv[])
{
    pthread_t agent_t, smoker_tobacco_t, smoker_paper_t, smoker_match_t, push_tobacco_t, push_paper_t, push_match_t;

    time_t t;
    srand((unsigned) time(&t));
    if (pthread_create(&agent_t,NULL,agent,NULL) != 0) {
        exit (1);
    }
    if (pthread_create(&push_tobacco_t,NULL,push_tobacco,NULL) != 0) {
        exit (1);
    }
    if (pthread_create(&push_paper_t,NULL,push_paper,NULL) != 0) {
        exit (1);
    }
    if (pthread_create(&push_match_t,NULL,push_match,NULL) != 0) {
        exit (1);
    }
    if (pthread_create(&smoker_tobacco_t,NULL,smoker_tobacco,NULL) != 0) {
        exit (1);
    }
    if (pthread_create(&smoker_paper_t,NULL,smoker_paper,NULL) != 0) {
        exit (1);
    }
    if (pthread_create(&smoker_match_t,NULL,smoker_match,NULL) != 0) {
        exit (1);
    }
    pthread_join(agent_t, NULL);
    pthread_join(push_tobacco_t, NULL);
    pthread_join(push_paper_t, NULL);
    pthread_join(push_match_t, NULL);
    pthread_join(smoker_tobacco_t, NULL);
    pthread_join(smoker_paper_t, NULL);
    pthread_join(smoker_match_t, NULL);

}