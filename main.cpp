#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#include "Thread.h"
#include "Mutex.h"
#include "Socket.h"
#include "Debug.h"

// Thread callback variable counter.
static unsigned int counter = 0;

// Thread callback args.
struct thread_args
{
	unsigned int id;
	unsigned int font_color;
	unsigned int counter_limit;
};

// Generetes a randomic number between two integers.
int rand_interval(int min, int max)
{
	return ((rand() % (max - min + 1)) + min);
}

// Thread callback function.
void *task(void *arg)
{
	struct thread_args *ta = (struct thread_args *) arg;
	bool loop_continue = true;
	unsigned int rand_num = 0;

	Debug::print((Debug::Colors) ta->font_color, "Printing msg from %s function running on thread id %u...\n", __FUNCTION__, ta->id);

	while(loop_continue)
	{
		rand_num = rand_interval(100000, 900000);
		usleep(rand_num);

		Mutex::lock();

		if(counter <= ta->counter_limit)
		{
			Debug::print((Debug::Colors) ta->font_color, "Thread id: %u -> counter %04u -> rand: %06u\n", ta->id, counter, rand_num);
			counter++;
		}
		else
		{
			Debug::print((Debug::Colors) ta->font_color, "Stopping thread id %u...\n", ta->id);
			loop_continue = false;
		}

		Mutex::unlock();
	}

	return (void *) strdup("Returning msg from task.");
}

void threadTest()
{
	Thread *task1;
	Thread *task2;
	Thread *task3;
	struct thread_args ta1;
	struct thread_args ta2;
	struct thread_args ta3;
	bool start1;
	bool start2;
	bool start3;

	Debug::print(Debug::CL_NORMAL, "Starting thread tests...\n");

	ta1 = {1, (unsigned int) Debug::CL_MAGENTA, 10};
	ta2 = {2, (unsigned int) Debug::CL_BLUE,    15};
	ta3 = {3, (unsigned int) Debug::CL_GREEN,   15};

	task1 = new Thread();
	task2 = new Thread();
	task3 = new Thread();

	if(!Mutex::init())
	{
		Debug::print(Debug::CL_NORMAL, "Mutex init error!\n");
	}

	start1 = task1->create(task, (void *) &ta1);
	start2 = task2->create(task, (void *) &ta2);
	start3 = task3->create(task, (void *) &ta3);

	Debug::print(Debug::CL_NORMAL, "Thread id %u %s%s\n", ta1.id, start1 ? "started" : "error", "!");
	Debug::print(Debug::CL_NORMAL, "Thread id %u %s%s\n", ta2.id, start2 ? "started" : "error", "!");
	Debug::print(Debug::CL_NORMAL, "Thread id %u %s%s\n", ta3.id, start3 ? "started" : "error", "!");

	if(start1 && task1->join() && !task1->wasCanceled())
	{
		Debug::print(Debug::CL_NORMAL, "Thread id %u joined -> retval %s\n", ta1.id, (const char *) task1->getRetval());
		free(task1->getRetval());
	}

	if(start2 && task2->join() && !task2->wasCanceled())
	{
		Debug::print(Debug::CL_NORMAL, "Thread id %u joined -> retval %s\n", ta2.id, (const char *) task2->getRetval());
		free(task2->getRetval());
	}

	if(start3 && task3->join() && !task3->wasCanceled())
	{
		Debug::print(Debug::CL_NORMAL, "Thread id %u joined -> retval %s\n", ta3.id, (const char *) task3->getRetval());
		free(task3->getRetval());
	}

	Debug::print(Debug::CL_NORMAL, "Done!\n");

	if(task1)
	{
		delete task1;
	}

	if(task2)
	{
		delete task2;
	}

	if(task3)
	{
		delete task3;
	}
}

#define HOST "127.0.0.1"
#define PORT 8888

void socketTest()
{
	Socket *sock;
	int clientfd;
	const size_t timeout_sec = 10;
	string toSend;
	unsigned char toReceive[128];

	Debug::print(Debug::CL_NORMAL, "Starting socket tests...\n");

	sock = new Socket(HOST, (uint16_t) PORT);
	toSend = "Hello, I'm the socket server!";

	if(sock->isOpened())
	{
		Debug::print(Debug::CL_NORMAL, "Start listening...\n");
		clientfd = sock->listenConnections(timeout_sec);
		if(clientfd > 0)
		{
			Debug::print(Debug::CL_NORMAL, "New client connection...\n");

			// Sending data...
			if(sock->sendData(clientfd, (const unsigned char *) toSend.c_str(), toSend.length()))
			{
				Debug::print(Debug::CL_NORMAL, "Data sent\n");
			}

			// Sending new line...
			if(sock->sendData(clientfd, (const unsigned char *) "\n", 1))
			{
				Debug::print(Debug::CL_NORMAL, "Newline sent\n");
			}

			// Receiving response...
			if(sock->receiveData(clientfd, toReceive, sizeof(toReceive), timeout_sec, true))
			{
				Debug::print(Debug::CL_NORMAL, "Socket client response: [%s]\n", toReceive);
			}

			Debug::print(Debug::CL_NORMAL, "Closing client connection...\n");
			sock->closeClientConnection(clientfd);
		}
	}

	Debug::print(Debug::CL_NORMAL, "Done!\n");

	delete sock;
}

int main(int argc, char *argv[])
{
	threadTest();
	socketTest();
	return 0;
}
