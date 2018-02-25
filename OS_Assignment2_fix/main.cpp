//Benson Tran
//CSCI 3341: Intro to Operating Systems
//Professor: Ray Hashemi
//Assignment 2
//DUE: November 28. 2017

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <ctime>
#include <vector>

using namespace std;
sem_t empty;
sem_t full;
sem_t smutex;
vector<char> buffer;
void* mainT(void*);
void* producerT(void*);
void* consumerT(void*);
char randomLetter();
void consumerProcessor(char in);

int main() {
    pthread_t mainThread;

    sem_init(&empty, 0, 3);
    sem_init(&full, 0, 0);
    sem_init(&smutex, 0, 1);

    if(pthread_create(&mainThread, NULL, mainT, NULL))
    {
        cout << "Error creating the main thread" << endl;
        return 0;
    }

    if(pthread_join(mainThread, NULL))
    {
        cout << "Error joining the main thread" << endl;
    }

    sem_destroy(&empty);
    sem_destroy(&full);
    sem_destroy(&smutex);

    return 0;
}

void * mainT(void * in)
{
    pthread_t producer, consumer;

    //Always ensures that the consumer does not try to consume an empty vector
    char letter = randomLetter();
    buffer.push_back(letter);
    cout << "Producer: " << letter << endl;

    if(pthread_create(&producer, NULL, producerT, NULL))
    {
        cout << "Error creating producer thread" << endl;
        return 0;
    }

    if(pthread_create(&consumer, NULL, consumerT, NULL))
    {
        cout << "Error creating the consumer thread" << endl;
        return 0;
    }
    if(pthread_join(producer, NULL))
    {
        cout << "Error joining producer thread" << endl;
        return 0;
    }

    if(pthread_join(consumer, NULL))
    {
        cout << "Error joining consumer thread" << endl;
        return 0;
    }
}

void * producerT(void * in)
{
    //Allows a different random number to be generated each iteration
    srand((int)time(0));

    char letter;

    while (true)
    {
        letter = randomLetter();
        sem_wait(&empty);
        sem_wait(&smutex);
        buffer.push_back(letter);
        cout << "Producer: " << letter << endl;
        sem_post(&smutex);
        sem_post(&full);

    }
}

void * consumerT(void * in)
{
    char letter;

    while(true)
    {
        sem_wait(&full);
        sem_wait(&smutex);

        letter = buffer.back();
        buffer.pop_back();
        sem_post(&smutex);
        sem_post(&empty);
        consumerProcessor(letter);
    }
}

//Generate a random letter by randomly choosing a number between 0-26 and adding 65
char randomLetter()
{
    int random = rand() % 26;
    char letter = random + 65;
    return letter;
}

//Handles the Consumer Thread logic
void consumerProcessor(char in)
{
    char c = in;

    //Odd letter case
    if (((c + 2) % 2) != 0)
    {
        cout << "Consumer: " << c << endl;
    }
    else
    {
        //Even letter case
        //Find vowels on either side of the char
        char v1, v2;

        if ((c >= 65) && (c <= 69))
        {
            v1 = 'A';
            v2 = 'E';
        }
        else
        {
            if ((c >= 70) && (c <= 73))
            {
                v1 = 'E';
                v2 = 'I';
            }
            else
            {
                if ((c >= 74) && (c <= 79))
                {
                    v1 = 'I';
                    v2 = 'O';
                }
                else
                {
                    if ((c >= 80) && (c <= 85))
                    {
                        v1 = 'O';
                        v2 = 'U';
                    }
                    else
                    {
                        if(c >= 86)
                        {
                            v1 = 'U';
                            v2 = 'A';
                        }
                    }
                }
            }
        }
        cout << "c: " << c << endl;
        cout << "v1: "  << v1 << endl;
        cout << "v2: " << v2 << endl;

        //Concatenate v1, c, and v2
        string s {v1 ,' ', c , ' ' , v2};
        cout << "S: " << s << endl;

        //Find which vowel has the largest distance
        char larger;
        int v1distance = c - v1;
        int v2distance = v2 - c;

        if(v1distance > v2distance)
        {
            larger = v1;
        }
        else
        {
            larger = v2;
        }

        cout << "v1 distance from c: " << v1distance << endl;
        cout << "v2 distance from c: " << v2distance << endl;
        cout << "Larger of the two distances: " << larger << endl;

        string t {larger, ' ', v1 ,' ', c , ' ' , v2 , ' ', larger};
        cout << "T: " << t << endl;
    }
}
